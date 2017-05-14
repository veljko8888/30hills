#include "kersem.h"
#include "lists.h"
#include "schedule.h"
#include "pcb.h"
#include "util.h"


// ID KernelSem::tID = 0;


KernelSem::KernelSem(int init) {
	v = init;
	blockedPCBs = new PCBList();
	
	// mID = ++tID;
}

KernelSem::~KernelSem() {
	while (v < 0) {
		signal();
	}
	
	delete blockedPCBs;
}

void KernelSem::wait(Time maxTimeToWait) {
	v--;
	if (v < 0) {
		running->status = PCB_BLOCKED;
		blockedPCBs->putLast(running);
		if (maxTimeToWait != 0)
			timerList->put(running, maxTimeToWait);
		running->sem = this;
		switchContext();
	}
}

void KernelSem::signal() {
	v++;
	if (v <= 0) {
		PCB *pcb = blockedPCBs->takeFirst();
		timerList->getRemove(pcb);
		Scheduler::put(pcb);
		pcb->status = PCB_READY;
	}
}

int KernelSem::val() const {
	return v;
}

