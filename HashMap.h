#ifndef HASH_MAP_H
#define HASH_MAP_H
#include "HashMapInterface.h"
#include "Pair.h"
#include "Set.h"
#include <cmath>
#include <string>
using namespace std;

#define DEFAULT_MAP_HASH_TABLE_SIZE	31
#define HASH_CONSTANT				3
#define LOAD_THRESHOLD				75

template <typename K, typename V>
class HashMap : public HashMapInterface<K, V> {			
private:
	size_t capacity;
	Pair<K, V>* myPairs;

public:
	HashMap() : capacity(DEFAULT_MAP_HASH_TABLE_SIZE), myPairs(new Pair<K, V>[DEFAULT_MAP_HASH_TABLE_SIZE]) {}
	~HashMap() { clear(); }

	virtual V& operator[](const K& key) {
		if (((100 * size()) / max_size()) > LOAD_THRESHOLD) {		//check for load threshold
			rehash();
		}
		long long hashIndex = 0;
		long long tempIndex = 0;
		int j = 1;
		for (int i = 0; i < key.length(); ++i) {
			hashIndex += key[i] * (pow(HASH_CONSTANT, (key.length() - (i + 1))));		//calculate the hash index
		}
		hashIndex = hashIndex % capacity;
		tempIndex = hashIndex;
		static_cast<size_t>(hashIndex);
		while (1) {
			if (myPairs[tempIndex].first.length() > 0) {		//check for collissions
				if (myPairs[tempIndex].first == key) break;
				tempIndex = hashIndex + pow(j++, 2);		//do quadratic probing 
				tempIndex %= capacity;
			}
			else {
				myPairs[tempIndex].first = key;
				break;
			}
		}
		return myPairs[tempIndex].second;
	}

	void rehash() {					//rehash function for when load threshold is exceeded
		size_t oldCapacity = capacity;
		capacity *= 2;
		Pair<K, V>* newPairs = new Pair<K, V>[capacity];
		for (int i = 0; i < oldCapacity; ++i) {
			if (myPairs[i].first != "") {
				long long hashIndex = calcHash(myPairs[i].first);
				hashIndex = hashIndex % capacity;
				long long tempIndex = hashIndex;
				int j = 1;
				while (1) {
					if (newPairs[tempIndex].first.length() > 0) {			//check for collissions
						if (newPairs[tempIndex].first == myPairs[i].first) break;
						tempIndex = hashIndex + pow(j++, 2);
						tempIndex %= capacity;					//do quadratic probing
					}
					else {
						newPairs[tempIndex].first = myPairs[i].first;
						newPairs[tempIndex].second = myPairs[i].second;
						break;
					}
				}
			}
		}
		delete[] myPairs;
		myPairs = newPairs;
		return;
	}

	long long calcHash(string key) {
		long long hashIndex = 0;
		for (int i = 0; i < key.length(); ++i) {
			hashIndex += key[i] * (pow(HASH_CONSTANT, (key.length() - (i + 1))));		//calculate the hash index
		}
		return hashIndex;
	}

	virtual size_t count(const K& key) {			//return a 1 if the key is found, and a 0 if not
		size_t one = 1;
		size_t zero = 0;
		bool count = false;
		for (size_t i = 0; i < capacity; ++i) {
			if (myPairs[i].first == key) {
				count = true;
			}
		}
		if (count == true) {
			return one;
		}
		else {
			return zero;
		}
	}

	virtual void clear() {
		delete[] myPairs;			//delete the dynamically allocated array to avoid memory leaks
		return;
	}

	virtual size_t size() const {		//return the size of the array by counting up every instance of a hash with adequate data
		size_t count = 0;
		for (size_t i = 0; i < capacity; ++i) {
			if (myPairs[i].first != "") {
				++count;
			}
		}
		return count;
	}

	virtual size_t max_size() const {		//return the capacity of the current hash map
		return capacity;
	}

	virtual string toString() const {		//to string which calls the pair friend insertion operator as well
		stringstream ss;
		for (size_t i = 0; i < capacity; ++i) {
			if (myPairs[i].first != "") {
				ss << endl << "  [" << i << ":" << myPairs[i] << "]";
			}
		}
		return ss.str();
	}

	friend ostream& operator<<(ostream& os, HashMap<K, V>& map) {		//freind insertion that calls to string
		return os << map.toString();
	}
};
#endif // HASH_MAP_H

