#include "util.h"
#include "pcb.h"
#include "lists.h"
#include "thread.h"
#include <dos.h>

// syscall
#include "syscall.h"



TimerList *timerList = 0;
SysModeList *pcbList = 0;
PCB *running = 0;
PCB *mainPCB = 0;
PCB *idlePCB = 0;
int timerCountdown = 0;

int userMain(int argc, char *argv[]);
void tick();

class MainThread : public Thread {
public:
	MainThread(int argc, char **argv) {
		this->argc = argc;
		this->argv = argv;
		// Warning zato sto nije inicijalizovana varijabla u konstruktoru.
		// Iako se koristi tako da ce prvo da se inicijalizuje, kompajler to ne zna.
		// Ova inicijalizacija je samo da skloni warning.
		this->val = -1;
	}
	
	virtual ~MainThread() { waitToComplete(); }
	virtual void run() { val = userMain(argc, argv); }
	int getVal() { return val; }
	Thread* clone() const { return new MainThread(argc, argv); }
	
private:
	int argc;
	char **argv;
	int val;
	
};

class IdleThread : public Thread {
public:
	virtual void run() { while(1); }
};

InterP oldTimerP = 0;

void interrupt timer(...) {
	// 1) Zovi stari tajmer.
	(*oldTimerP)();
	
	// 2) Zovi tick() za javni test.
	tick();

	// 3) Meri vreme za uspavane niti.
	timerList->tick();
	
	// 4) Ako treba, meri vreme za running nit.
	if (running->slice > 0) {
		// Treba da se meri vreme.
		timerCountdown--;

		if (timerCountdown <= 0)
			dispatch();
	}
}



int main(int argc, char *argv[]) {
	// enableSysCalls();
	
	timerList = new TimerList();
	
	pcbList = new SysModeList();
	
	mainPCB = new PCB(250, 0, 0);
	running = mainPCB;

	IdleThread *idleThread = new IdleThread();
	idleThread->start();
	
	MainThread *mainThread = new MainThread(argc, argv);
	mainThread->start();
	
	lock;
//#ifndef BCC_BLOCK_IGNORE
	oldTimerP = getvect(0x8);
	setvect(0x8, timer);
//#endif
	unlock;
	
	mainThread->waitToComplete();
	
	lock;
//#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, oldTimerP);
//#endif
	unlock;
	
	int val = mainThread->getVal();
	
	delete mainThread;

	delete idleThread;

	delete mainPCB;
	
	delete pcbList;

	delete timerList;
	
	// disableSysCalls();

	return val;
}

