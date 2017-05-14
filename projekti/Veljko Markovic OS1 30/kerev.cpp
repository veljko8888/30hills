#include "kerev.h"
#include "util.h"
#include "pcb.h"
#include "schedule.h"
#include "event.h"

// ID KernelEv::tID = 0;


KernelEv::KernelEv(Event *myEvent, IVTNo ivtNo) {
	// mID = ++tID;
	
	this->myEvent = myEvent;
	this->ivtNo = ivtNo;
	this->parent = running;
	this->blocked = 0;
	
	IVTEntry *entry = IVTEntry::get(ivtNo);
	entry->put(this);
}

KernelEv::~KernelEv() {
	IVTEntry *entry = IVTEntry::get(ivtNo);
	entry->remove(this);
	
	if (blocked != 0) {
		// ako je vlasnik blokiran, odblokiraj ga
		blocked->status = PCB_READY;
		Scheduler::put(blocked);
		blocked = 0;
	}
}

void KernelEv::wait() {
	if (parent != running)
		return;
	
	if (blocked == 0) {
		blocked = running;
		blocked->status = PCB_BLOCKED;
		switchContext();
	}
}

void KernelEv::signal() {
	if (blocked != 0) {
		blocked->status = PCB_READY;
		Scheduler::put(blocked);
		blocked = 0;
	}
}

void KernelEv::callSignal() {
	myEvent->signal();
}

