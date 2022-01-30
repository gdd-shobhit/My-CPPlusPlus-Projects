#include <iostream>
template <class T>
class PriorityQueue
{
	public: 

	int front = -1;
	int sizeOfArray = 1;
	T* list;

	// the 3 important things
	PriorityQueue() {

		sizeOfArray = 1;
		front = -1;
		list = new T[sizeOfArray];
	};
	PriorityQueue(int initialSize) {
		if (initialSize < 0)
			initialSize = 1;
		sizeOfArray = initialSize;
		front = -1;
		list = new T[sizeOfArray];

	}

	PriorityQueue(PriorityQueue<T> const& other) {
		sizeOfArray = other.sizeOfArray;
		front = other.front;
		list = new T[sizeOfArray];

		for (int i = 0; i < GetSize(); i++) {
			list[i] = other.list[i];
		}

	};

	PriorityQueue<T>& operator=(PriorityQueue<T> const& other) {
		if (this == &other) {
			return *this;
		}

		if (list != nullptr) {
			delete[] list;
			list = nullptr;
		}

		front = other.front;	
		sizeOfArray = other.sizeOfArray;
		list = new T[sizeOfArray];

		for (int i = 0; i < GetSize(); i ++ ) {
			list[i] = other.list[i];
		}
		
		return *this;
	};

	~PriorityQueue() {
		delete[] list;
		sizeOfArray = 0;
		front = -1;
		list = nullptr;
	};

	// Functions of Queue

	void Push(T data) {
		// checks if the arr is full and if yes, doubles the capacity
		if (GetSize() == sizeOfArray)
			IncreaseCapacity();
		Sort(data);
	};

	void Pop() {
		list[0] = NULL;
		for (int i = 0; i < GetSize(); i++) {
			list[i] = list[i + 1];
		}
		front--;
	};

	void Print() {
		for (int i = 0; i < GetSize(); i++) {
			std::cout << list[i];
			std::cout << "\n";
		}
	};

	int GetSize() {
		return front + 1;
	};

	bool isEmpty() {
		return front == -1;
	};

	void Sort(T data) {
		// place is the variable where you wanna insert the data passed in
		int place = 0;

		if (isEmpty()) {
			list[0] = data;
			front++;
			return;
		}

		for (int i = GetSize(); i > 0; i--) {
			if (data > list[i - 1]) {
				place = i;
				break;
			}			
		}

		for (int i = GetSize(); i >= place ; i--) {
			list[i] = list[i-1];
		}

		list[place] = data;
		front++;
	};

	//void IncreaseCapacity() {
	//	PriorityQueue<T>* temp = new PriorityQueue<T>(*this);
	//	temp->sizeOfArray *= 2;
	//	operator=(*temp);
	//	
	//};

	void IncreaseCapacity() {

		T* tempList = new T[sizeOfArray * 2];
		for (int i = 0; i < GetSize(); i++) {
			tempList[i] = list[i];
		}
		sizeOfArray *= 2;
		delete[] list;
		list = tempList;
		tempList = nullptr;
	};


};

