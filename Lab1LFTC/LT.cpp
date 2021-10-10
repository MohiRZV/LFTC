/*
LT - Lexicografic Table
an array of strings that stores its elements in lexicografic order
*/
#pragma once
#include <string>

class LT {
	//the maximum capacity
	int capacity;
	//the actual size
	int size;

	//the array
	std::string* table;

	//doubles the capacity of the table
	void resize() {
		int newCap = capacity * 2;
		std::string* tmp = new std::string[newCap];
		for (int i = 0; i < size; i++) {
			tmp[i] = table[i];
		}

		capacity = newCap;
		table = tmp;
	}

	void init(int cap) {
		capacity = cap;
		table = new std::string[cap];
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

	//adds the given element in the table
	void add(std::string element) {
		//insertion sort lexicografic - TODO
		if (size >= 0.75 * capacity)
			resize();
		table[size++] = element;
	}

	//returns the actual size of the table
	int getSize() {
		return size;
	}

	//returns the entry on position @index from the table
	std::string get(int index) {
		return table[index];
	}

	//returns  the position the desired element is found on
	//or -1 if it is not in the table
	int find(std::string element) {
		//binary search - TODO
		return 0;
	}

	/*
		returns a reference to the LT as a std::string*
		THIS REFERENCE SHOULD NOT BE STORED
		AND MUST BE USED ONLY FOR READ OPERATIONS
	*/
	std::string* getLTasVector() {
		return table;
	}


};
