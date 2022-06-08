#ifndef PAIR_H
#define PAIR_H
#include <string>
#include "Set.h"
#include "HashMap.h"
using namespace std;

template<typename T1, typename T2>
struct Pair {
	T1 first;
	T2 second;
	// Construct a pair from two values.
	Pair(const T1& x, const T2& y) : first(x), second(y) {}

	// Construct a default pair.
	Pair() : first(T1()), second(T2()) {}

	// Construct a default pair.
	Pair(const T1& x) : first(x), second(T2()) {}

	// Construct an assignable pair
	template<typename Other_T1, typename Other_T2>
	Pair(const Pair<Other_T1, Other_T2>& other) {
		first = other.first;
		second = other.second;
	}

	friend ostream& operator<<(ostream& os, const Pair& pair) {			//friend insertion to string operation for pairs of any types
		return os << pair.first << "->" << pair.second;
	}
};

#endif // PAIR_H
