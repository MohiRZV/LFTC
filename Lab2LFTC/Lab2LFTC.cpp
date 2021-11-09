// Lab2LFTC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "FIP.cpp"
#include "LT.cpp"
using namespace std;

vector<FIP*> fip;
LT* ts = new LT();

map<string, int> availableAtoms;
//method to create the available atoms table as a map
void generateAtomsTable() {

	map<string, int> fip;
	fip.insert({ "ID",      0 });
	fip.insert({ "CONST",   1 });
	fip.insert({ "int", 2 });
	fip.insert({ "double",  3 });
	fip.insert({ "circle",  4 });
	fip.insert({ ":", 5 });
	fip.insert({ ",", 6 });
	fip.insert({ ";", 7 });
	fip.insert({ "{", 8 });
	fip.insert({ "}", 9 });
	fip.insert({ "(", 10 });
	fip.insert({ ")", 11 });
	fip.insert({ ">>",  12 });
	fip.insert({ "<<",  13 });
	fip.insert({ "+",   14 });
	fip.insert({ "-",   15 });
	fip.insert({ "*",   16 });
	fip.insert({ "++",  17 });
	fip.insert({ "--",  18 });
	fip.insert({ "<",   19 });
	fip.insert({ ">",   20 });
	fip.insert({ "!=",  21 });
	fip.insert({ "!",   22 });
	fip.insert({ "<=",  23 });
	fip.insert({ "=",   24 });
	fip.insert({ "main",    25 });
	fip.insert({ "return",  26 });
	fip.insert({ "if",      27 });
	fip.insert({ "while",   28 });
	fip.insert({ "cin",     29 });
	fip.insert({ "cout",    30 });
	fip.insert({ "for",     31 });
	availableAtoms = fip;
}

vector<string> readSourceAsString(string file) {
	vector<string> source;
	ifstream fin(file);
	string line;
	while (getline(fin, line)) {

		source.push_back(line);
	}
	return source;
}

enum StateType {
	INIT,
	INTER,
	FIN,
	NONE
};

StateType getStateTypeFromString(string type) {
	if (type._Equal("Init"))
		return INIT;
	if (type._Equal("Inter"))
		return INTER;
	if (type._Equal("Fin"))
		return FIN;
	return NONE;
}

// parse the FSM structure and create it
void classifyTokens(string &initial_state, map<string, map<string, vector<string>>> &transitions, vector<string> &elems, map<string, int> &alphabet, vector<string> &states, vector<string> &final_states) {
	StateType currentStateType = NONE;
	int i = 2;
	string currentFrom = elems[1];
	vector<string> tr;
	map<string, vector<string>> to;
	string lastState;
	while(i<elems.size()) {
		string current = elems[i];
		
		// if the production rule terminator is met
		if (current._Equal(";")) {
			currentStateType = NONE;
			to.emplace(lastState, tr);
			tr.clear();
			transitions.emplace(currentFrom, to);
			to.clear();
			i += 2;
			if(i< elems.size())
				currentFrom = elems[i];
			continue;
		}

		// if the : separator is met, the previous element is a state
		if (current._Equal(":")) {
			string prev = elems[i - 1];
			// get the type of the current state
			StateType cs = getStateTypeFromString(elems[i-2]);
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
			else if (i > 0 && (elems[i - 1] == "~" || elems[i-1] == ":")) {
				lastState = current;
			}
			else if(current == "~"){
				to.emplace(lastState, tr);
				tr.clear();
			}
		}
		
		i++;
	}
}
vector<string> elemsAtoms, elemsOperators, elemsLiterals;
vector<string> statesAtoms, statesOperators, statesLiterals;
map<string, int> alphabetAtoms, alphabetOperators, alphabetLiterals;
string initial_stateAtoms, initial_stateOperators, initial_stateLiterals;

vector<string> final_statesAtoms, final_statesOperators, final_statesLiterals;
map<string, map<string, vector<string>>> transitionsAtoms, transitionsOperators, transitionsLiterals;;
void classifyAtoms() {

}

bool isFinalState(string state, vector<string> final_states) {
	for (int i = 0; i < final_states.size(); i++)
		if (final_states[i]._Equal(state))
			return true;

	return false;
}

// read FSM structure
void readFSMStructureFromFile(string file, vector<string> &elems) {
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
	fin.close();
}

void initStateMachines() {
	string fileAtoms = "atom_fsm_description.txt";
	string fileLiterals = "literal_fsm_description.txt";
	string fileOperators = "operators.txt";

	readFSMStructureFromFile(fileLiterals, elemsLiterals);
	readFSMStructureFromFile(fileAtoms, elemsAtoms);
	readFSMStructureFromFile(fileOperators, elemsOperators);

	classifyTokens(initial_stateAtoms, transitionsAtoms, elemsAtoms, alphabetAtoms, statesAtoms, final_statesAtoms);
	classifyTokens(initial_stateLiterals, transitionsLiterals, elemsLiterals, alphabetLiterals, statesLiterals, final_statesLiterals);
	classifyTokens(initial_stateOperators, transitionsOperators, elemsOperators, alphabetOperators, statesOperators, final_statesOperators);
}

bool nextState(string &currentState, string current, string &prefix, map<string, map<string, vector<string>>> transitions) {
	// move to the next state
	auto neighbours = transitions[currentState];

	bool found = false;
	// try going to the neighbours
	for (auto neighbour : neighbours) {
		auto trans = neighbour.second;

		// search the possible transitions for the current neighbour
		for (auto t : trans) {
			// if we can go to this state - DETERMINISTIC
			if (t._Equal(current)) {
				found = true;
				currentState = neighbour.first;
				break;
			}
		}

	}

	// if no possible transitions were found
	if (!found) {
		return false;
	}
	else {
		prefix.append(current);
		return true;
	}
}
string errors;
void saveIfFinalWord(bool &isAccepted, string &currentState, vector<string> final_states, string &prefix, string initial_state, int stateType, int line) {
	// if the sequence generated so far is no longer accepted by neither of  the FSMs
	if (!isAccepted) {
		// the word build so far is final
		// determine in which FSM it was accepted last ( with the longest length )


		// to be accepted is has to end in a final state
		bool isFinalStateCheck = isFinalState(currentState, final_states);
		// if it is in a final state => it is valid
		if (isFinalStateCheck) {
			//cout << prefix << '\n';
			auto atom = availableAtoms.find(prefix);
			int codeAtom = 0;
			if (stateType == 2)
				codeAtom = 1;
			if (atom != availableAtoms.end())
				codeAtom = atom->second;
			fip.push_back(new FIP(prefix, codeAtom));

			if (codeAtom == 0 || codeAtom == 1) {
				int code = ts->find(prefix);
				if (code == -1) {
					ts->add(prefix);
				}
			}
			if (codeAtom == 0 && prefix.length() > 8) {
				errors.append("Identifier: ").append(prefix).append(" , on line ").append(to_string(line)).append(" is too long\n");
			}
		}
		else {
			if(!prefix.empty())
				errors.append("Error at: ").append(prefix).append(" , on line ").append(to_string(line)).append("\n");
		}

		// reinit states
		currentState = initial_state;
		
		prefix.clear();
	}
}

void run(string source, int line) {
	
	bool isAcceptedAtom = true;
	bool isAcceptedOperator = true;
	bool isAcceptedLiteral = true;

	// current state
	string currentStateAtoms = initial_stateAtoms;
	string currentStateOperators = initial_stateOperators;
	string currentStateLiterals = initial_stateLiterals;
	// current char
	string current;
	current = source[0];
	string prefixAtoms;
	string prefixOperators;
	string prefixLiterals;
	//cout << source << '\n';
	// given a source file as a string
	// go char by char with it through FSMs
	for (int pos = 0; pos < source.size(); pos++) {

		current = source[pos];

		// different FSM for literals and identifiers

		isAcceptedAtom = nextState(currentStateAtoms, current, prefixAtoms, transitionsAtoms);
		isAcceptedOperator = nextState(currentStateOperators, current, prefixOperators, transitionsOperators);
		isAcceptedLiteral = nextState(currentStateLiterals, current, prefixLiterals, transitionsLiterals);

		saveIfFinalWord(isAcceptedAtom, currentStateAtoms, final_statesAtoms, prefixAtoms, initial_stateAtoms, 0, line);
		saveIfFinalWord(isAcceptedOperator, currentStateOperators, final_statesOperators, prefixOperators, initial_stateOperators, 1, line);
		saveIfFinalWord(isAcceptedLiteral, currentStateLiterals, final_statesLiterals, prefixLiterals, initial_stateLiterals, 2, line);

		if (current._Equal(";")) {
			fip.push_back(new FIP(";", availableAtoms.find(";")->second));
		}
	}
	// check separately if on last position
	isAcceptedAtom = false;
	saveIfFinalWord(isAcceptedAtom, currentStateAtoms, final_statesAtoms, prefixAtoms, initial_stateAtoms, 0, line);
	isAcceptedOperator = false;
	saveIfFinalWord(isAcceptedOperator, currentStateOperators, final_statesOperators, prefixOperators, initial_stateOperators, 1, line);
	isAcceptedLiteral = false;
	saveIfFinalWord(isAcceptedLiteral, currentStateLiterals, final_statesLiterals, prefixLiterals, initial_stateLiterals, 2, line);
}

//void menuApp();
int main()
{
	//menuApp();
	initStateMachines();
	generateAtomsTable();
	//read the source file as a string
	vector<string> source = readSourceAsString("source.txt");

	for (int i = 0; i < source.size(); i++)
		run(source[i], i);

	if (errors.empty()) {
		cout << "FIP\n";
		for (auto i : fip) {
			//update the TS code for each FIP entry (when an item is added to the TS, the codes change so they are consistent with the position)
			i->setCodeTS(ts->find(i->atom));
			cout << i->toString() << '\n';
		}
		cout << "TS\n";
		for (int i = 0; i < ts->getSize(); i++) {
			cout << ts->get(i)->toString() << '\n';
		}
	}
	else {
		cout << "Compilation FAILED! There were syntax errors:\n";
		cout << errors;
	}

}

//void displayMenu() {
//
//	cout << "Available commands:\n";
//	cout << "\t0 - read from keyboard\n";
//	cout << "\t1 - read from file\n";
//	cout << "\t2 - display states\n";
//	cout << "\t3 - display alphabet\n";
//	cout << "\t4 - display transitions\n";
//	cout << "\t5 - display final states\n";
//	cout << "\t6 - check if sequence is accepted by the FSM\n";
//	cout << "\t7 - get longest prefix accepted by FSM\n";
//	cout << "\t-1 - exit\n";
//}
//
//void clearStorage() {
//	elemsAtoms.clear(); states.clear(); alphabet.clear(); final_states.clear(); transitions.clear();
//}
//
//void readFromKeyboard() {
//	string line;
//	while (!line._Equal("end")) {
//
//		string delimiter = " ";
//		size_t pos = 0;
//		string token;
//		while ((pos = line.find(delimiter)) != string::npos) {
//			token = line.substr(0, pos);
//			elemsAtoms.push_back(token);
//			line.erase(0, pos + delimiter.length());
//		}
//		if (!line.empty())
//			elemsAtoms.push_back(line);
//		cin >> line;
//	}
//}
//
//void displayStates() {
//	cout << "states:\n";
//	for (string s : states) {
//		cout << s << '\n';
//	}
//}
//
//void displayInitialStates() {
//	cout << "initial state:\n";
//	cout << initial_state;
//}
//
//void displayFinalStates() {
//	cout << "final states:\n";
//	for (string s : final_states) {
//		cout << s << '\n';
//	}
//}
//
//void displayAlphabet() {
//	cout << "alphabet:\n";
//	for (auto s : alphabet) {
//		cout << s.first << '\n';
//	}
//}
//
//void displayTransitions() {
//
//	cout << "transitions:\n";
//	for (auto t : transitions) {
//		string from = t.first;
//		auto to_tr = t.second;
//		for (auto tr : to_tr) {
//			cout << from << " ---";
//			string to = tr.first;
//			for (int i = 0; i < tr.second.size(); i++) {
//				string a = tr.second[i];
//				cout << a;
//				if (i != tr.second.size() - 1) {
//					cout << ", ";
//				}
//			}
//			cout << "--> ";
//			cout << to << '\n';
//		}
//	}
//}

//void menuApp() {
//	displayMenu();
//
//	//string file = "integer_literal_fsm_description.txt";
//	//string file = "literal_fsm_description.txt";
//	string file = "atom_fsm_description.txt";
//	//string file = "activitate.txt";
//	//string file = "operators.txt";
//	//string file = "identifier_keyword.txt";
//	//string file = "af_description.txt";
//
//	int cmd = 0;
//	bool wasRead = false;
//
//	string sequence = "123431253445433";
//
//	bool isAccepted = false;
//	string prefix;
//
//	readFSMStructureFromFile(file);
//	classifyTokens();
//
//	while (cmd != -1) {
//		cout << "Enter a command: ";
//
//		cin >> cmd;
//
//		switch (cmd)
//		{
//
//		case 0:
//			clearStorage();
//			cout << "Enter the FSM description, followed by *enter* \"end\" *enter*\n";
//			readFromKeyboard();
//			wasRead = true;
//			classifyTokens();
//			break;
//		case 1:
//			clearStorage();
//			readFSMStructureFromFile(file);
//			wasRead = true;
//			classifyTokens();
//			break;
//		case 2:
//			displayStates();
//			break;
//		case 3:
//			displayAlphabet();
//			break;
//		case 4:
//			displayTransitions();
//			break;
//		case 5:
//			displayFinalStates();
//			break;
//		case 6:
//
//			cout << "enter sequence:\n";
//			cin >> sequence;
//
//			isAccepted = checkIsAccepted(sequence).first;
//			if (isAccepted)
//				cout << "Sequence " << sequence << " is accepted by the FSM\n";
//			else
//				cout << "Sequence " << sequence << " is not accepted by the FSM\n";
//			break;
//		case 7:
//			cout << "enter sequence:\n";
//			cin >> sequence;
//			prefix = checkIsAccepted(sequence).second;
//			cout << "Longest prefix is: " << prefix << '\n';
//			break;
//		default:
//			if (cmd != -1)
//				cout << "Wrong input!\n";
//			break;
//		}
//	}
//
//}