#include "lists.h"
#include "pcb.h"
#include "kersem.h"
#include "schedule.h"




//**** PCBList ****//
PCBList::PCBList() {
	first = last = 0;
	sz = 0;
}

PCBList::~PCBList() {
	while (first) {
		Elem *old = first;
		first = first->next;
		delete old;
	}
}

void PCBList::putLast(PCB *pcb) {
	if (pcb == 0)
		return;
	
	Elem *elem = new Elem();
	elem->pcb = pcb;
	elem->next = 0;
	
	if (first == 0)
		first = elem;
	else
		last->next = elem;
	last = elem;
	
	sz++;
}

PCB* PCBList::takeFirst() {
	if (sz == 0)
		return 0;
	
	sz--;
	
	Elem *retElem = first;
	PCB *retPCB = first->pcb;
	
	first = first->next;
	delete retElem;
	
	return retPCB;
}

int PCBList::size() {
	return sz;
}

PCB* PCBList::getRemove(PCB *pcb) {
	Elem *prev = 0, *cur = first;
	while (1) {
		if (cur != 0 && cur->pcb == pcb) {
			if (prev)
				prev->next = cur->next;
			else
				first = cur->next;
			PCB *pcb = cur->pcb;
			delete cur;
			return pcb;
		}
		else {
			if (cur == 0)
				break;
			
			prev = cur;
			cur = cur->next;
		}
	}
	
	return 0;
}




//**** TimerList ****//
TimerList::TimerList() {
	first = 0;
	sz = 0;
}

TimerList::~TimerList() {
	while (sz > 0) {
		Elem *old = first;
		first = first->next;
		
		old->pcb->status = PCB_READY;
		Scheduler::put(old->pcb);
		delete old;
		sz--;
	}
}

void TimerList::put(PCB *pcb, Time timeToSleep) {
	Elem *prev = 0, *cur = first;
	while (1) {
		if (cur != 0 && cur->timeToSleep <= timeToSleep) {
			timeToSleep -= cur->timeToSleep;
			prev = cur;
			cur = cur->next;
		}
		else {
			if (cur != 0)
				cur->timeToSleep -= timeToSleep;
			
			Elem *elem = new Elem();
			elem->pcb = pcb;
			elem->timeToSleep = timeToSleep;
			elem->next = cur;
			
			if (prev != 0)
				prev->next = elem;
			else
				first = elem;
			
			sz++;
			
			return;
		}
	}
}

void TimerList::tick() {
	if (sz > 0) {
		if (first->timeToSleep > 0)
			first->timeToSleep--;
		
		while (sz > 0 && first->timeToSleep == 0) {
			Elem *elem = first;
			first->pcb->sem->blockedPCBs->getRemove(first->pcb);
			first->pcb->sem->v++;
			first->pcb->status = PCB_READY;
			Scheduler::put(first->pcb);
			first = first->next;
			delete elem;
			
			sz--;
		}
	}
}

int TimerList::size() {
	return sz;
}

PCB* TimerList::getRemove(PCB *pcb) {
	Elem *prev = 0, *cur = first;
	while (1) {
		if (cur != 0 && cur->pcb == pcb) {
			if (cur->next)
				cur->next->timeToSleep += cur->timeToSleep;
			if (prev)
				prev->next = cur->next;
			else
				first = cur->next;
			PCB *pcb = cur->pcb;
			delete cur;
			return pcb;
		}
		else {
			if (cur == 0)
				break;
			
			prev = cur;
			cur = cur->next;
		}
	}
	
	return 0;
}




//**** SysModeList ****//
SysModeList::SysModeList() {
	first = last = 0;
	sz = 0;
}

SysModeList::~SysModeList() {
	while (first) {
		Elem *old = first;
		first = first->next;
		delete old;
	}
}

void SysModeList::put(void *data, ID id) {
	Elem *elem = new Elem();
	elem->data = data;
	elem->id = id;
	elem->next = 0;
	
	if (first == 0)
		first = elem;
	else
		last->next = elem;
	last = elem;
	
	sz++;
}

void* SysModeList::getRemove(ID id) {
	Elem *prev = 0, *cur = first;
	
	while (cur && cur->id != id) {
		prev = cur;
		cur = cur->next;
	}
	
	if (cur) {
		if (prev != 0)
			prev->next = cur->next;
		else
			first = cur->next;
		
		if (cur == last)
			last = prev;
		
		cur->next = 0;
		
		void *data = cur->data;
		delete cur;
		
		sz--;
		
		return data;
	}
	else {
		return 0;
	}
}

void* SysModeList::getNoRemove(ID id) {
	Elem *cur = first;
	
	while (cur && cur->id != id) {
		cur = cur->next;
	}
	
	if (cur != 0)
		return cur->data;
	else
		return 0;
}

