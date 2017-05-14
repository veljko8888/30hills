#ifndef _KERSEM_H_
#define _KERSEM_H_

class PCBList;
typedef unsigned int Time;

// syscall
// #include "syscall.h"

class KernelSem {
public:
	KernelSem(int init);
	virtual ~KernelSem();
	
	void wait(Time maxTimeToWait);
	void signal();
	
	int val() const;
	
private:
	// friend void execcall(SysCallData *data);
	friend class TimerList;
	
	// static ID tID;
	// ID mID;
	
	int v;
	PCBList *blockedPCBs;
	
};

#endif