#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "util.h"

class KernelEv;

class IVTEntry {
public:
	IVTEntry(IVTNo numEntry, InterP userRoutine);
	~IVTEntry();
	static IVTEntry* get(IVTNo numEntry);
	InterP getOldInterrupt();
	int hasEvent();
	void signalEvent();
	void put(KernelEv *kev);
	void remove(KernelEv *kev);
	
private:
	IVTNo numEntry;
	InterP userRoutine;
	InterP oldRoutine;
	static IVTEntry* entries[256];
	KernelEv *kev;
	
};

#endif