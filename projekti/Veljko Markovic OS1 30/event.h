// File: event.h
#ifndef _EVENT_H_
#define _EVENT_H_

#include "ivtentry.h"

#define PREPAREENTRY(numEntry, oldCall)	\
void interrupt interrupt##numEntry(...);	\
IVTEntry entry##numEntry(numEntry, interrupt##numEntry);	\
void interrupt interrupt##numEntry(...) {	\
	if (oldCall && entry##numEntry.getOldInterrupt()) \
		(entry##numEntry.getOldInterrupt()) ();	\
	\
	if (entry##numEntry.hasEvent())	\
		entry##numEntry.signalEvent();	\
	\
	dispatch();	\
}

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
	
private:
	KernelEv* myImpl;
	// ID id;

};

#endif

