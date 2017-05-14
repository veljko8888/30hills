#ifndef _KEREV_H_
#define _KEREV_H_

#include "util.h"
// #include "sysstr.h"

// typedef int ID;

class Event;
class PCB;

class KernelEv {
public:
	KernelEv(Event *myEvent, IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
	void callSignal();
	
	// friend void execcall(SysCallData *data);
	
private:
	IVTNo ivtNo;
	Event *myEvent;
	PCB *parent;
	PCB *blocked;
	
	// static ID tID;
	// ID mID;
	
};

#endif