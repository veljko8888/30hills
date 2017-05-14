#include "syscall.h"

#include "pcb.h"
#include "kersem.h"
#include "kerev.h"

#include <iostream.h>


// SysModeList *sysModePCBList = 0;
// SysModeList *sysModeKSemList = 0;
// SysModeList *sysModeKEvList = 0;

// int sysCallActive = 0;
// int somebodyWantsDispatch = 0;


// static SysCallData *data = 0;
// static unsigned int dataSeg;
// static unsigned int dataOff;

// static unsigned int userss, usersp;
// static unsigned int systemss, systemsp;
// static unsigned char *systemStack = 0;


// void enableSysCalls() {
	// sysModePCBList = new SysModeList();
	// sysModeKSemList = new SysModeList();
	// sysModeKEvList = new SysModeList();
	
	// systemStack = new unsigned char[5000];
	// systemss = FP_SEG(systemStack + 5000);
	// systemsp = FP_OFF(systemStack + 5000);
// }

// void disableSysCalls() {
	// mozda ostaviti ovaj stek za fork
	// delete[] systemStack;
	
	// delete sysModeKEvList;
	// delete sysModeKSemList;
	// delete sysModePCBList;
// }



// void execcall(SysCallData *data) {
	// switch(data->function) {
	// case 1: {
		// // PCB::PCB();
		// PCB *pcb = new PCB((StackSize)data->data[0], (Time)data->data[1], (Thread*)data->data[2]);
		// sysModePCBList->put(pcb, pcb->mID);
		// *((ID*)data->result) = pcb->mID;
	// } break;
	// case 2: {
		// // PCB::start();
		// PCB *pcb = (PCB*)sysModePCBList->getNoRemove((ID)data->data[0]);
		// if (pcb)
			// pcb->start();
	// } break;
	// case 3: {
		// // PCB::~PCB();
		// PCB *pcb = (PCB*)sysModePCBList->getRemove((ID)data->data[0]);
		// if (pcb)
			// delete pcb;
	// } break;
	// case 4: {
		// // PCB::waitToComplete();
		// PCB *pcb = (PCB*)sysModePCBList->getNoRemove((ID)data->data[0]);
		// if (pcb)
			// pcb->waitToComplete();
	// } break;
	// case 5: {
		// // PCB::sleep();
		// PCB::sleep((Time)data->data[0]);
	// } break;
	// case 6: {
		// // PCB::exitThread();
		// PCB::exitThread();
	// } break;
	// case 7: {
		// // switchContext();
		// switchContext();
	// } break;
	// case 8: {
		// // KernelSem::KernelSem();
		// KernelSem *kernelSem = new KernelSem((int)data->data[0]);
		// sysModeKSemList->put(kernelSem, kernelSem->mID);
		// *((ID*)data->result) = kernelSem->mID;
	// } break;
	// case 9: {
		// // KernelSem::~KernelSem();
		// KernelSem *kernelSem = (KernelSem*)sysModeKSemList->getRemove((ID)data->data[0]);
		// if (kernelSem)
			// delete kernelSem;
	// } break;
	// case 10: {
		// // KernelSem::wait();
		// KernelSem *kernelSem = (KernelSem*)sysModeKSemList->getNoRemove((ID)data->data[0]);
		// if (kernelSem)
			// kernelSem->wait();
	// } break;
	// case 11: {
		// // KernelSem::signal();
		// KernelSem *kernelSem = (KernelSem*)sysModeKSemList->getNoRemove((ID)data->data[0]);
		// if (kernelSem)
			// kernelSem->signal();
	// } break;
	// case 12: {
		// // KernelSem::val();
		// KernelSem *kernelSem = (KernelSem*)sysModeKSemList->getNoRemove((ID)data->data[0]);
		// if (kernelSem) {
			// *((int*)data->result) = kernelSem->val();
		// }
	// } break;
	// case 13: {
		// // KernelEv::KernelEv();
		// KernelEv *kernelEv = new KernelEv((Event*)data->data[0], (IVTNo)data->data[1]);
		// sysModeKEvList->put(kernelEv, kernelEv->mID);
		// *((ID*)data->result) = kernelEv->mID;
	// } break;
	// case 14: {
		// // KernelEv::~KernelEv();
		// KernelEv *kernelEv = (KernelEv*)sysModeKEvList->getRemove((ID)data->data[0]);
		// if (kernelEv)
			// delete kernelEv;
	// } break;
	// case 15: {
		// // KernelEv::wait();
		// KernelEv *kernelEv = (KernelEv*)sysModeKEvList->getNoRemove((ID)data->data[0]);
		// if (kernelEv)
			// kernelEv->wait();
	// } break;
	// case 16: {
		// // KernelEv::signal();
		// KernelEv *kernelEv = (KernelEv*)sysModeKEvList->getNoRemove((ID)data->data[0]);
		// if (kernelEv)
			// kernelEv->signal();
	// } break;
	// }
// }

// void interrupt syscall(...) {
	// asm {
		// mov dataSeg, ax;
		// mov dataOff, bx;
		
		// mov userss, ss;
		// mov usersp, sp;
		// mov ss, systemss;
		// mov sp, systemsp;
	// }
	
	// data = (SysCallData*)MK_FP(dataSeg, dataOff);
	
	// sysCallActive = 1;
// //	asm sti;
	// execcall(data);
// //	asm cli;
	// sysCallActive = 0;
	
	// asm {
		// mov ss, userss;
		// mov sp, usersp;
	// }
	
	// if (somebodyWantsDispatch) {
		// somebodyWantsDispatch = 0;
		// switchContext();
	// }
	
// }

