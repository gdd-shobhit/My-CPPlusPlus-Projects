#define _CRTDBG_MAP_ALLOC
#include "Main.h"
#include "Alberto.h"
#include "PriorityQueue.h"
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

void wrapper();

int main() {

	wrapper();
	cout << _CrtDumpMemoryLeaks();
}

void wrapper() {
	cout << "Last line of code tells if there is memory leak or not. 1 for true and 0 for false" << "\n";
	PriorityQueue<int>* queue = new PriorityQueue<int>();

	queue->Push(32);
	queue->Push(46);
	queue->Push(36);
	// Printing all the numbers;
	queue->Print();
	cout << "Queue capacity right now: " << queue->sizeOfArray << "\n";
	cout << "Queue count right now: " << queue->GetSize() << "\n";
	queue->Push(3212);
	queue->Push(40);
	queue->Push(2);
	queue->Pop();
	queue->Print();

	// Alberto class
	cout << "Alberto Queue :---- >" << "\n";
	cout << "\n";
	AlbertoClass alObject1 = AlbertoClass(24);
	AlbertoClass alObject2 = AlbertoClass(12);
	AlbertoClass alObject3 = AlbertoClass(32);
	AlbertoClass alObject4 = AlbertoClass(3);
	alObject1.SetAge(24);
	alObject2.SetAge(12);
	alObject3.SetAge(32);
	alObject4.SetAge(3);

	// making the albertoQueue
	PriorityQueue<AlbertoClass>* albertoQueue = new PriorityQueue<AlbertoClass>();
	albertoQueue->Push(alObject1);
	albertoQueue->Push(alObject2);
	albertoQueue->Push(alObject3);
	albertoQueue->Push(alObject4);
	albertoQueue->Print();
	cout << "\n";
	albertoQueue->Pop();
	albertoQueue->Print();
	cout << "\n";

	// cleaning memory
	delete albertoQueue;
	albertoQueue = nullptr;	
	delete queue;
	queue = nullptr;
}