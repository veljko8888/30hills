#ifndef _SYSSTR_H_
#define _SYSSTR_H_

#include <dos.h>

typedef int ID;

struct SysCallData {
	int function;
	void* result;
	unsigned long data[4];
};

#endif