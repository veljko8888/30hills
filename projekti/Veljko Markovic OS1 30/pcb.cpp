#include "pcb.h"
#include "schedule.h"
#include <dos.h>
#include "lists.h"
#include "util.h"

// syscall
// #include "syscall.h"

#include <iostream.h>

unsigned int tss, tsp, isUserMainFinished = 0;
PCB* prevRunning = 0;
Thread* prevRunningThread = 0;

ID PCB::tID = 0;

void PCB::start() {
	if (status == PCB_NEW) {
		status = PCB_READY;
		if (this != idlePCB)
			Scheduler::put(this);
	}
}

void PCB::waitToComplete() {
	if (status != PCB_DONE && status != PCB_NEW && this != idlePCB) {
		running->status = PCB_BLOCKED;
		blockedPCBs->putLast(running);
		switchContext();
	}
}

PCB::~PCB() {
	pcbList->getRemove(mID);
	delete[] stack;
	delete blockedPCBs;
}

void PCB::sleep(Time timeToSleep) {
	if (timeToSleep == 0)
		return;
	else {
		running->status = PCB_SLEEPING;
		timerList->put(running, timeToSleep);
		switchContext();
	}
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *t) {
	this->t = t;
	size = stackSize;
	stack = 0;
	curSS = curSP = 0;
	if (size > 65536UL)
		size = 65536UL;
	
	// Stack inicijalizacija.
	stack = new unsigned char[size];
	unsigned int *top = (unsigned int*)(stack + size);
//#ifndef BCC_BLOCK_IGNORE
	top[-1] = FP_SEG(t);
	top[-2] = FP_OFF(t);
//#endif
	// nebitni su zato sto se nikad nece vracati iz Thread::wrapper funkcije,
	// pa mogu da ostanu random
	//top[-3] je nebitan
	//top[-4] je nebitan
	top[-5] = 0x200;	// svi flagovi 0, samo interrupt flag 1, dozvoljeni prekidi
//#ifndef BCC_BLOCK_IGNORE
	top[-6] = FP_SEG(Thread::wrapper);
	top[-7] = FP_OFF(Thread::wrapper);
	// nije bitan sadrzaj ostalih, osim bp, koji pokazuje na top[-5]
	top[-16] = 0;
	
	curSS = FP_SEG(&top[-16]);
	curSP = FP_OFF(&top[-16]);
//#endif
	// Kraj stek inicijalizacije.
	
	slice = timeSlice;
	status = PCB_NEW;
	blockedPCBs = new PCBList();
	
	mID = ++tID;
	if (mID == 2)
		idlePCB = this;
	
	pcbList->put(this, mID);
	
	childWaiting = 0;
	parent = 0;
	childNum = 0;
	forked = 0;
}

void PCB::exitThread() {
	if (running->forked) {
		running->parent->childNum--;
		if (running->parent->childNum == 0 && running->parent->childWaiting) {
			running->parent->childWaiting = 0;
			running->parent->status = PCB_READY;
			Scheduler::put(running->parent);
		}
	}
	
	while (running->blockedPCBs->size() > 0) {
		PCB *pcb = running->blockedPCBs->takeFirst();
		pcb->status = PCB_READY;
		Scheduler::put(pcb);
	}
	
	running->status = PCB_DONE;
	if (running->mID == 3)
		isUserMainFinished = 1;
	
	switchContext();
}

Thread* PCB::getThreadById(ID id) {
	PCB* pcb = (PCB*)pcbList->getNoRemove(id);
	if (pcb)
		return pcb->t;
	else
		return 0;
}

void interrupt switchContext() {
	// push:
	// psw, cs, ip
	// ax, bx, cx, dx, ds, es, si, di, bp
	
//#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tss, ss;
		mov tsp, sp;
	}
//#endif
	running->curSS = tss;
	running->curSP = tsp;
	
	// if (sysCallActive) {
		// somebodyWantsDispatch = 1;
	// }
	// else {
		if (running->status == PCB_READY && running != idlePCB)
			Scheduler::put(running);
		prevRunning = running;
		prevRunningThread = prevRunning->t;
		running = Scheduler::get();
		if (running == 0) {
			if (!isUserMainFinished)
				running = idlePCB;
			else
				running = mainPCB;
		}
		
		timerCountdown = running->slice;
	// }
	
	tss = running->curSS;
	tsp = running->curSP;
//#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
	}
//#endif
	
	if (prevRunning->forked != 0 && prevRunning->status == PCB_DONE) {
		delete prevRunningThread;
	}

	// pop:
	// bp, di, si, es, ds, dx, cx, bx, ax
	// ip, cs, psw
}

void PCB::waitForForkChildren() {
	if (running->childNum) {
		running->childWaiting = 1;
		running->status = PCB_BLOCKED;
		dispatch();
	}
}

unsigned* getPtr(unsigned tss, unsigned tsp) {
	return (unsigned*)(MK_FP(tss, tsp));
}

void interrupt PCB::fork() {
	Thread* t = running->t->clone();
	if (t == 0) {
		running->forkVal = -1;
		return;
	}
	else {
		PCB* p = t->myPCB;
		p->parent = running;
		running->childNum++;
		
		p->forked = 1;
		
		p->status = PCB_READY;
		Scheduler::put(p);
		
		p->forkVal = 0;
		running->forkVal = p->mID;
		
		for (StackSize i = 0; i < running->size; i++)
			p->stack[i] = running->stack[i];
		
		unsigned diffVal = FP_OFF(p->stack) - FP_OFF(running->stack);
		unsigned tbp;
		asm {
			mov tbp, bp;
		}
		tbp += diffVal;
		
		p->curSS = FP_SEG(p->stack);
		p->curSP = tbp;
		
		while (*getPtr(p->curSS, tbp) != 0) {
			tbp = *getPtr(p->curSS, tbp) += diffVal;
		}
	}
}








































/*

PSW = program status word
CS = code segment
IP = instruction pointer

// opsti registri
AX = address register
BX = buffer register
CX = counter register
DX = data register

// segmentni registri
DS = data segment
ES = extra segment

// registri za indeksiranje nizova (npr. pri kopiranju niza)
SI = source index
DI = destination index

// "baza" funkcije odakle se sa offsetom na SS:BP dohvataju
// parametri (na + pomjeraj) i lokalne varijable (na - pomjeraj)
BP = base pointer


// ne stavlja se na stek, mora posebno da se cuva u PCB
SS = stack segment
SP = stack pointer

CS : IP je kao PC, sastavljen iz dva registra
SS : SP je kao slozeni stack pointer, kao vrh steka

*/

