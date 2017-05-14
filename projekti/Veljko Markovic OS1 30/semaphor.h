// File: semaphor.h
#ifndef _semaphor_h_
#define _semaphor_h_

class KernelSem;
typedef unsigned int Time;
// typedef int ID;

class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const; // Returns the current value of the semaphore

private:
	KernelSem* myImpl;
	// ID id;

};

#endif