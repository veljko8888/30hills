#ifndef _LISTS_H_
#define _LISTS_H_

class PCB;
typedef int ID;
typedef unsigned int Time;

class PCBList {
public:
	PCBList();
	virtual ~PCBList();
	
	void putLast(PCB *pcb);
	PCB* takeFirst();
	int size();
	PCB* getRemove(PCB *pcb);
	
private:
	struct Elem {
		PCB *pcb;
		Elem *next;
	};
	
	Elem *first, *last;
	int sz;
	
};

class TimerList {
public:
	TimerList();
	virtual ~TimerList();

	void put(PCB *pcb, Time timeToSleep);
	void tick();
	int size();
	PCB* getRemove(PCB *pcb);
	
private:
	struct Elem {
		PCB *pcb;
		Time timeToSleep;
		Elem *next;
	};

	Elem *first;
	int sz;
	
};


// syscall
class SysModeList {
public:
	SysModeList();
	~SysModeList();
	
	void put(void *data, ID id);
	void* getRemove(ID id);
	void* getNoRemove(ID id);
	
private:
	struct Elem {
		void *data;
		ID id;
		Elem *next;
	};
	
	Elem *first, *last;
	int sz;
	
};

#endif

