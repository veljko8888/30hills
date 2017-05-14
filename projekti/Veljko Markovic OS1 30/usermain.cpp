#include "thread.h"
#include "semaphor.h"
#include "event.h"

#include <iostream.h>
#include <stdlib.h>

#define CALL_OLD 1
PREPAREENTRY(9, CALL_OLD);

Semaphore mutex(1);

void tick() {}


class Znak : public Thread {
public:
	Znak(char c, int n) : Thread(), c(c), n(n) {}
	virtual ~Znak() { waitToComplete(); }
	
	virtual void run() {
		for (int i = 0; i < n; i++) {
//		for (unsigned long i = 0; i < 100000000UL; i++) {	// ovo za semafore test, iskljuci cekanje
			mutex.wait();
			cout << c;
			mutex.signal();
			
//			for (int j = 0; j < 10000; j++)
//				for (int k = 0; k < 20000; k++);
			
			Thread::sleep(3 + rand() % 3);
			
//			dispatch();
		}
		
		mutex.wait();
		cout << c << " zavrsio" << endl;
		mutex.signal();
	}
	
private:
	char c;
	int n;
	
};


class Key : public Thread {
public:
	Key(int n) : Thread(), n(n) {}
	virtual ~Key() { waitToComplete(); }
	
	virtual void run();
	
private:
	int n;
	
};

void Key::run() {
	Event event9(9);
	
	for (int i = 0; i < n; i++) {
		mutex.wait();
		cout << endl << "ceka " << i << endl;
		mutex.signal();
		
		event9.wait();
		
		mutex.wait();
		cout << endl << "docekao " << i << endl;
		mutex.signal();
	}
	
	mutex.wait();
	cout << endl << "zavrsio" << endl;
	mutex.signal();
}


int userMain(int argc, char *argv[]) {
	mutex.wait();
	cout << endl << "userMain pocetak" << endl;
	mutex.signal();
	
	Znak *a = new Znak('a', 10);
	Znak *b = new Znak('b', 15);
	Znak *c = new Znak('c', 20);
	
	Key *k = new Key(100);
	
	a->start();
	b->start();
	c->start();
	
	k->start();
	
	delete a;
	delete b;
	delete c;
	
	delete k;
	
	mutex.wait();
	cout << endl << "userMain kraj" << endl;
	mutex.signal();
	
	return 0;
}

