#include "semaphor.h"
#include "kersem.h"
#include "util.h"
#include "pcb.h"


Semaphore::Semaphore(int init) {
	lock;
	myImpl = new KernelSem(init);
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 8;	// KernelSem::KernelSem();
	// data.result = &id;
	// data.data[0] = (unsigned long)init;
	
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

Semaphore::~Semaphore() {
	lock;
	delete myImpl;
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 9;	// KernelSem::~KernelSem();
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

int Semaphore::wait(Time maxTimeToWait) {
	lock;
	myImpl->wait(maxTimeToWait);
	unlock;
	return running->val;
	
	// // syscall
	// SysCallData data;
	// data.function = 10;	// KernelSem::wait();
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

void Semaphore::signal() {
	lock;
	myImpl->signal();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 11;	// KernelSem::signal();
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

int Semaphore::val() const {
	lock;
	int v = myImpl->val();
	unlock;
	return v;
	
	// // syscall
	// int v;
	
	// SysCallData data;
	// data.function = 12;	// KernelSem::val();
	// data.result = &v;
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
	
	// return v;
}

