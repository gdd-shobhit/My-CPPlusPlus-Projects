#pragma once
#include <iostream>
using namespace std;

template <class T>
class OgStack
{
private:
	// variables for the stack where stackSize is basically the capacity and stackData is the data and stackTop is like an index of the top most value on stack
	T* stackData;
	int stackSize;
	int stackTop;

public:

	OgStack(){

		stackSize = 1;
		stackData = new T[stackSize];
		stackTop = -1;
	};

	// Constructor Copy
	OgStack(const OgStack& other) {
		stackSize = other.stackSize;
		stackData = new T[stackSize];
		stackTop = other.stackTop;
		for (int i = 0; i < GetSize(); i++) {
			stackData[i] = other.stackData[i];
		}
	};

	~OgStack() {
		delete[] stackData;
		stackData = nullptr;
		stackSize = 0;
	}

	// Assignment operator
	OgStack& operator=(const OgStack& other) {

		if (this == &other) {
			return *this;
		}

		if (stackData != nullptr) {
			delete[] stackData;
			stackData = nullptr;
		}

		stackSize = other.stackSize;
		stackData = new T[stackSize];
		stackTop = other.stackTop;
		for (int i = 0; i < GetSize(); i++) {
			stackData[i] = other.stackData[i];
		}
		return *this;
	};

	void Push(T value);
	T Pop();
	void Print();
	int GetSize();
	bool isEmpty();
};

template<class T>
inline void OgStack<T>::Push(T value)
{

	// for empty stack
	if (isEmpty()) {
		stackTop++;
		stackData[stackTop] = value;
		return;
	}

	// stack which is full
	if (stackTop==stackSize-1) {
		
		// Doubling the size in temp stack
		T* temp = new T[stackSize*2];
		for (int i = 0; i < GetSize(); i++) {
			temp[i] = stackData[i];			
		}
		stackTop++;
		delete[] stackData;
		stackData = temp;
		stackData[stackTop] = value;
		stackSize *= 2;
		temp = nullptr;
	}
	// normal pushing
	else {
		stackData[stackTop+1]=value;
		stackTop++;
	}
}

template<class T>
inline T OgStack<T>::Pop()
{
	if (isEmpty()) {
		// for returning null in case of empty stack
		return NULL;
	}
	T valueToPop;
	valueToPop = stackData[stackTop];
	stackData[stackTop] == NULL;
	stackTop--;
	
	return valueToPop;
}

template<class T>
inline void OgStack<T>::Print()
{
	for (int i = 0; i < GetSize(); i++) {
		cout << "Stack Data at "<< i << " : " <<stackData[i] << endl;
	}
}

template<class T>
inline int OgStack<T>::GetSize()
{
	return stackTop + 1;
}

template<class T>
inline bool OgStack<T>::isEmpty()
{
	if (stackTop == -1) {
		return true;
	}

	return false;
}
