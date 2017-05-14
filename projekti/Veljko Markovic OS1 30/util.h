// File: util.h
#ifndef _util_h_
#define _util_h_

//#ifndef BCC_BLOCK_IGNORE
#define lock asm pushf; asm cli;
#define unlock asm popf;
//#else
//#define lock
//#define unlock
//#endif

typedef void interrupt (*InterP) (...);
typedef unsigned char IVTNo;
typedef int ID;

class TimerList;
class PCB;
class SysModeList;

extern SysModeList *pcbList;
extern TimerList *timerList;
extern PCB *running;
extern PCB *mainPCB;
extern PCB *idlePCB;
extern int timerCountdown;

#endif
