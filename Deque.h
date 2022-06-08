#ifndef DEQUE_H
#define DEQUE_H
#include <string>
#include <utility>
#define DEFAULT_CAPACITY 25
using namespace std;

template <typename T>
class Deque {
private:
	T* data;
	size_t capacity;
	size_t num_items;
	size_t front_index;
	size_t rear_index;

public:
	Deque() : capacity(DEFAULT_CAPACITY), num_items(0), front_index(0), rear_index(DEFAULT_CAPACITY - 1), data(new T[DEFAULT_CAPACITY]) {}
	~Deque() { delete[] data; }

	void reallocate() {			//reallocating size to make the circular array bigger when needed
		size_t new_capacity = 2 * capacity;
		T* new_data = new T[new_capacity];
		size_t old = front_index;
		for (size_t i = 0; i < num_items; ++i) {
			new_data[i] = data[old];
			old = (old + 1) % capacity;
		}
		front_index = 0;
		rear_index = num_items - 1;
		capacity = new_capacity;
		T* tmp = data;
		data = new_data;
		delete[] tmp;
	}
	/** Insert item at rear of deque */
	void push_back(const T& value) {
		if (num_items == capacity) {
			reallocate();
		}
		num_items++;
		rear_index = (rear_index + 1) % capacity;
		data[rear_index] = value;
		return;
	}
	/** Remove the rear item of the deque */
	void pop_back(void) {
		num_items--;
		rear_index = (rear_index - 1) % capacity;
		return;
	}
	/** Return the rear item of the deque (Do not remove) */
	T& back(void) {
		return data[rear_index];
	}
	/** Return the number of items in the deque */
	size_t size(void) {
		return num_items;
	}
};
#endif

