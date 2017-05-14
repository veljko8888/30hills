// File: pcb.h
#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
class KernelSem;

// syscall
// #include "syscall.h"

class PCBList;

const int PCB_NEW = 1;
const int PCB_READY = 2;
const int PCB_DONE = 3;
const int PCB_BLOCKED = 4;
const int PCB_SLEEPING = 5;

class PCB {
public:
	void start();
	void waitToComplete();
	virtual ~PCB();
	static void sleep(Time timeToSleep);
	PCB(StackSize stackSize, Time timeSlice, Thread *t);
	static void exitThread();
	Thread* getThreadById(ID id);
	static void waitForForkChildren();
	static void interrupt fork();
	
	int val;
	ID mID;
	KernelSem *sem;
	ID forkVal;
	
	friend void interrupt timer(...);
	friend void interrupt switchContext();
	friend class KernelSem;
	friend class KernelEv;
	friend class TimerList;
	// friend void execcall(SysCallData *data);

private:
	Thread *t;
	StackSize size;
	unsigned int curSS, curSP;
	unsigned char *stack;
	Time slice;
	int status;
	PCBList *blockedPCBs;
	static ID tID;
	PCB *parent;
	int childNum;
	int childWaiting;
	int forked;
	
};

void interrupt switchContext();

#endif
