/*
LT - Lexicografic Table
an array of TS objects that stores its elements in lexicografic order
*/
#pragma once
#include <string>
#include "TS.cpp"

class LT {
	//the maximum capacity
	int capacity;
	//the actual size
	int size;
	//the current code - generated incrementally
	int code = 0;

	//the array
	TS** table;

	//doubles the capacity of the table
	void resize() {
		int newCap = capacity * 2;
		TS** tmp = new TS * [newCap];
		for (int i = 0; i < size; i++) {
			tmp[i] = table[i];
		}

		capacity = newCap;
		table = tmp;
	}

	void init(int cap) {
		capacity = cap;
		table = new TS * [cap];
	}

public:
	/*
		constructor
		param: cap - the initial capacity
	*/
	LT(int cap) {
		init(cap);
	}

	LT() {
		init(7);
	}

	/*
		adds the given element in the table
		returns the codeTS of the element that was inserted
	*/
	int add(std::string element) {
		if (find(element) != -1)
			return -2;
		if (size >= 0.75 * capacity)
			resize();
		//insertion sort lexicografic
		//add element on the last position
		table[size++] = new TS(element, code++);

		int pos = size - 1;
		//as long as the inserted element is smaller lexicographically then the current one
		while (pos > 0 && table[pos]->getSymbol().compare(table[pos - 1]->getSymbol()) < 0) {
			//interchange them
			TS* tmp = table[pos];
			table[pos] = table[pos - 1];
			table[pos - 1] = tmp;
			table[pos]->setCode(pos);
			table[pos - 1]->setCode(pos - 1);
			pos--;
		}

		return pos + 1;
	}

	//returns the actual size of the table
	int getSize() {
		return size;
	}

	//returns the entry on position @index from the table
	TS* get(int index) {
		return table[index];
	}

	//returns  the position the desired element is found on
	//or -1 if it is not in the table
	int find(std::string element) {
		//binary search
		int right = size - 1, left = 0;
		while (left <= right) {
			int middle = (left + right) / 2;
			if (table[middle]->getSymbol()._Equal(element))
				return middle;
			if (table[middle]->getSymbol().compare(element) < 0)
				left = middle + 1;
			else
				right = middle - 1;
		}
		return -1;
	}

	/*
		returns a reference to the LT as a std::string*
		THIS REFERENCE SHOULD NOT BE STORED
		AND MUST BE USED ONLY FOR READ OPERATIONS
	*/
	TS** getLTasVector() {
		return table;
	}


};
