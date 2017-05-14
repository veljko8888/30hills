#include "ivtentry.h"
#include "util.h"
#include <dos.h>
#include "kerev.h"

IVTEntry* IVTEntry::entries[256];

IVTEntry::IVTEntry(IVTNo numEntry, InterP userRoutine) {
	lock;
	oldRoutine = getvect(numEntry);
	setvect(numEntry, userRoutine);
	this->numEntry = numEntry;
	entries[numEntry] = this;
	kev = 0;
	unlock;
}

IVTEntry::~IVTEntry() {
	lock;
	entries[numEntry] = 0;
	setvect(numEntry, oldRoutine);
	// prvo 20h je kod koji znaci "tastaturo, obradjen prekid"
	// drugo 20h je adresa kontrolnog (valjda) registra tastature
	// na koju saljemo kod da je obradjen prekid
	// ovo moze da se desi kad se pritisne esc, pa zavrsi u javnom
	// testu nit za tastaturu, pa dok se ne zavrsi sve i unisti
	// ivtentry objekat nakon main, prekid na tastaturi nece biti
	// obradjen, prestanu da rade prekidi kad se drugi put uzastopno
	// u istom konzolnom prozoru pokrene test
	if (oldRoutine)
		oldRoutine();
	unlock;
}

IVTEntry* IVTEntry::get(IVTNo numEntry) {
	return entries[numEntry];
}

InterP IVTEntry::getOldInterrupt() {
	return oldRoutine;
}

int IVTEntry::hasEvent() {
	return kev != 0;
}

void IVTEntry::signalEvent() {
	if (hasEvent())
		kev->callSignal();
}

void IVTEntry::put(KernelEv *kev) {
	if (this->kev == 0)
		this->kev = kev;
}

void IVTEntry::remove(KernelEv *kev) {
	if (this->kev == kev)
		this->kev = 0;
}

