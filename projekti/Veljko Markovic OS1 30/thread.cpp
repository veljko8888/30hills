#include "thread.h"
#include "pcb.h"
#include "util.h"

// syscall
// #include "sysstr.h"
// #include "syscall.h"

//#include <iostream.h>

void Thread::start() {
	lock;
	myPCB->start();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 2;	// PCB::start();
	// data.data[0] = (unsigned long)id;
	
	// unsigned int segd;
	// unsigned int offd;
	// segd = FP_SEG(&data);
	// offd = FP_OFF(&data);
	
	// asm {
		// mov ax, segd;
		// mov bx, offd;
	// }
	// lock;
	// syscall();
	// unlock;
}

void Thread::waitToComplete() {
	lock;
	myPCB->waitToComplete();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 4;	// PCB::waitToComplete();
	// data.data[0] = (unsigned long)id;
	
	// unsigned int segd;
	// unsigned int offd;
	// segd = FP_SEG(&data);
	// offd = FP_OFF(&data);
	
	// asm {
		// mov ax, segd;
		// mov bx, offd;
	// }
	// lock;
	// syscall();
	// unlock;
}

Thread::~Thread() {
	waitToComplete();
	lock;
	delete myPCB;
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 3;	// PCB::~PCB();
	// data.data[0] = (unsigned long)id;
	
	// unsigned int segd;
	// unsigned int offd;
	// segd = FP_SEG(&data);
	// offd = FP_OFF(&data);
	
	// asm {
		// mov ax, segd;
		// mov bx, offd;
	// }
	// lock;
	// syscall();
	// unlock;
}

// void Thread::sleep(Time timeToSleep) {
	// lock;
	// PCB::sleep(timeToSleep);
	// unlock;
	
	// // // syscall
	// // SysCallData data;
	// // data.function = 5;	// PCB::sleep();
	// // data.data[0] = (unsigned long)timeToSleep;
	
	// // unsigned int segd;
	// // unsigned int offd;
	// // segd = FP_SEG(&data);
	// // offd = FP_OFF(&data);
	
	// // asm {
		// // mov ax, segd;
		// // mov bx, offd;
	// // }
	// // lock;
	// // syscall();
	// // unlock;
// }

ID Thread::getId() {
	return myPCB->mID;
}

ID Thread::getRunningId() {
	return running->mID;
}

Thread* Thread::getThreadById(ID id) {
	lock;
	Thread* t = myPCB->getThreadById(id);
	unlock;
	return t;
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 1;	// PCB::PCB();
	// data.result = &id;
	// data.data[0] = (unsigned long)stackSize;
	// data.data[1] = (unsigned long)timeSlice;
	// data.data[2] = (unsigned long)this;
	
	// unsigned int segd;
	// unsigned int offd;
	// segd = FP_SEG(&data);
	// offd = FP_OFF(&data);
	
	// asm {
		// mov ax, segd;
		// mov bx, offd;
	// }
	// lock;
	// syscall();
	// unlock;
}

void Thread::wrapper(Thread *t) {
	t->run();
	
	lock;
	PCB::exitThread();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 6;	// PCB::exitThread();
	
	// unsigned int segd;
	// unsigned int offd;
	// segd = FP_SEG(&data);
	// offd = FP_OFF(&data);
	
	// asm {
		// mov ax, segd;
		// mov bx, offd;
	// }
	// lock;
	// syscall();
	// unlock;
}

void dispatch() {
	lock;
	switchContext();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 7;	// switchContext();
	
	// unsigned int segd;
	// unsigned int offd;
	// segd = FP_SEG(&data);
	// offd = FP_OFF(&data);
	
	// asm {
		// mov ax, segd;
		// mov bx, offd;
	// }
	// lock;
	// syscall();
	// unlock;
}

Thread* Thread::clone() const {
	return 0;
}

void Thread::exit() {
	lock;
	PCB::exitThread();
	unlock;
}

void Thread::waitForForkChildren() {
	lock;
	PCB::waitForForkChildren();
	unlock;
}

ID Thread::fork() {
	lock;
	PCB::fork();
	unlock;
	return running->forkVal;
}




















