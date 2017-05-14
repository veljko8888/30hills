#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "sysstr.h"
#include "lists.h"

typedef unsigned char IVTNo;

class SysModeList;

extern SysModeList *sysModePCBList;
extern SysModeList *sysModeKSemList;
extern SysModeList *sysModeKEvList;

void enableSysCalls();
void disableSysCalls();
void interrupt syscall(...);

extern int sysCallActive;
extern int somebodyWantsDispatch;

#endif