#include "event.h"
#include "kerev.h"
#include "util.h"

// syscall
// #include "syscall.h"


Event::Event(IVTNo ivtNo) {
	lock;
	myImpl = new KernelEv(this, ivtNo);
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 13;	// KernelEv::KernelEv();
	// data.result = &id;
	// data.data[0] = (unsigned long)this;
	// data.data[1] = (unsigned long)ivtNo;
	
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

Event::~Event() {
	lock;
	delete myImpl;
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 14;	// KernelEv::~KernelEv();
	// data.result = &id;
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

void Event::wait() {
	lock;
	myImpl->wait();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 15;	// KernelEv::wait();
	// data.result = &id;
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

void Event::signal() {
	lock;
	myImpl->signal();
	unlock;
	
	// // syscall
	// SysCallData data;
	// data.function = 16;	// KernelEv::signal();
	// data.result = &id;
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

