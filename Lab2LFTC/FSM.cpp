#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
using namespace std;

class FSM {
	enum StateType {
		INIT,
		INTER,
		FIN,
		NONE
	};
	// file where the FSM description is found
	string file;

	vector<string> elems;
	vector<string> states;
	map<string, int> alphabet;
	string initial_state;
	vector<string> final_states;
	map<string, map<string, vector<string>>> transitions;

	FSM(string file) {
		this->file = file;
		readFSMStructureFromFile(file);
		classifyTokens();
	}

	bool isFinalState(string state) {
		for (int i = 0; i < final_states.size(); i++)
			if (final_states[i]._Equal(state))
				return true;

		return false;
	}
	// parse the FSM structure and create it
	void classifyTokens() {
		StateType currentStateType = NONE;
		int i = 2;
		string currentFrom = elems[1];
		vector<string> tr;
		map<string, vector<string>> to;
		string lastState;
		while (i < elems.size()) {
			string current = elems[i];

			// if the production rule terminator is met
			if (current._Equal(";")) {
				currentStateType = NONE;
				to.emplace(lastState, tr);
				tr.clear();
				transitions.emplace(currentFrom, to);
				to.clear();
				i += 2;
				if (i < elems.size())
					currentFrom = elems[i];
				continue;
			}

			// if the : separator is met, the previous element is a state
			if (current._Equal(":")) {
				string prev = elems[i - 1];
				// get the type of the current state
				StateType cs = getStateTypeFromString(elems[i - 2]);
				switch (cs)
				{
					// if previous was INIT state type identifier, this is the initial state
				case INIT:
					initial_state = prev;
					states.push_back(prev);
					break;
					// if previous was INTER state type identifier, this is an intermediary state
				case INTER:
					states.push_back(prev);
					break;
					// if previous was FIN state type identifier, this is the final state
				case FIN:
					final_states.push_back(prev);
					states.push_back(prev);
					break;
				case NONE:
					break;
				default:
					break;
				}

			}
			else {
				// if previous was transition separator, this is a transition
				if (i > 0 && elems[i - 1] == ",") {
					tr.push_back(current);
					alphabet.emplace(current, 0);
				}
				else if (i > 0 && (elems[i - 1] == "~" || elems[i - 1] == ":")) {
					lastState = current;
				}
				else if (current == "~") {
					to.emplace(lastState, tr);
					tr.clear();
				}
			}

			i++;
		}
	}

	StateType getStateTypeFromString(string type) {
		if (type._Equal("Init"))
			return INIT;
		if (type._Equal("Inter"))
			return INTER;
		if (type._Equal("Fin"))
			return FIN;
		return NONE;
	}

	pair<bool, string> checkIsAccepted(string sequence) {
		bool isAccepted = true;

		string currentState = initial_state;
		string current;
		current = sequence[0];
		string next;
		next = sequence[0];
		string prefix;
		string longestSeq;
		for (int i = -1; i < sequence.size() - 1, isAccepted == true; i++) {

			next = sequence[i + 1];
			auto neighbours = transitions[currentState];

			bool found = false;
			// try going to the neighbours
			for (auto neighbour : neighbours) {
				auto trans = neighbour.second;

				// search the possible transitions for the current neighbour
				for (auto t : trans) {
					// if we can go to this state
					if (t._Equal(next)) {
						found = true;
						currentState = neighbour.first;
						break;
					}
				}

			}

			if (!found) {
				isAccepted = false;
				break;
			}

			prefix.append(current);

			if (isFinalState(currentState)) {
				longestSeq = prefix;
			}

			current = sequence[i + 2];
		}
		// to be accepted is has to end in a final state
		bool isFinalStateCheck = isFinalState(currentState);

		return pair<bool, string>(((isAccepted || prefix.size() == sequence.size()) && isFinalStateCheck), longestSeq);
	}

	// read FSM structure
	void readFSMStructureFromFile(string file) {
		ifstream fin(file);
		string line;
		while (getline(fin, line)) {
			string delimiter = " ";
			size_t pos = 0;
			string token;
			while ((pos = line.find(delimiter)) != string::npos) {
				token = line.substr(0, pos);
				elems.push_back(token);
				line.erase(0, pos + delimiter.length());
			}
			elems.push_back(line);
		}
	}
};