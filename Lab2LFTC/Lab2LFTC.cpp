// Lab2LFTC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

vector<string> elems;
vector<string> states;
map<string, int> alphabet;
vector<string> initial_states;
vector<string> final_states;
map<string, map<string, vector<string>>> transitions;

//Init q0 : q1, / -q2, 1, 2, 3, 4, 5, 6, 7, 8, 9;
//Inter q1 : q2, 1, 2, 3, 4, 5, 6, 7, 8, 9;
//Fin q2 : q2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;

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

void clearStorage() {
	elems.clear(); states.clear(); alphabet.clear(); initial_states.clear(); final_states.clear(); transitions.clear();
}

void classifyTokens() {
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
			if(i<elems.size())
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
				initial_states.push_back(prev);
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
			else if (i > 0 && (elems[i - 1] == "-" || elems[i-1] == ":")) {
				lastState = current;
			}
			else if(current == "-"){
				to.emplace(lastState, tr);
				tr.clear();
			}
		}
		
		i++;
	}
}

void displayMenu() {

	cout << "Available commands:\n";
	cout << "\t0 - read from keyboard\n";
	cout << "\t1 - read from file\n";
	cout << "\t2 - display states\n";
	cout << "\t3 - display alphabet\n";
	cout << "\t4 - display transitions\n";
	cout << "\t5 - display final states\n";
	cout << "\t6 - check if sequence is accepted by the FSM\n";
	cout << "\t7 - get longest prefix accepted by FSM\n";
	cout << "\t-1 - exit\n";
}

void readFromFile(string file) {
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

void readFromKeyboard() {
	string line;
	while (!line._Equal("end")) {
		
		string delimiter = " ";
		size_t pos = 0;
		string token;
		while ((pos = line.find(delimiter)) != string::npos) {
			token = line.substr(0, pos);
			elems.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
		if(!line.empty())
			elems.push_back(line);
		cin >> line;
	}
}

void displayStates() {
	cout << "states:\n";
	for (string s : states) {
		cout << s << '\n';
	}
}

void displayInitialStates() {
	cout << "initial states:\n";
	for (string s : initial_states) {
		cout << s << '\n';
	}
}

void displayFinalStates() {
	cout << "final states:\n";
	for (string s : final_states) {
		cout << s << '\n';
	}
}

void displayAlphabet() {
	cout << "alphabet:\n";
	for (auto s : alphabet) {
		cout << s.first << '\n';
	}
}

void displayTransitions() {

	cout << "transitions:\n";
	for (auto t : transitions) {
		string from = t.first;
		auto to_tr = t.second;
		for (auto tr : to_tr) {
			cout << from << " ---";
			string to = tr.first;
			for (int i = 0; i < tr.second.size(); i++) {
				string a = tr.second[i];
				cout << a;
				if (i != tr.second.size() - 1) {
					cout << ", ";
				}
			}
			cout << "--> ";
			cout << to << '\n';
		}
	}
}

pair<bool, string> checkIsAccepted(string sequence) {
	bool isAccepted = true;

	string currentState = initial_states[0];
	string current;
	current = sequence[0];
	string next;
	next = sequence[0];
	string prefix;
	for (int i = -1; i < sequence.size() - 1, isAccepted == true;i++) {

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

		current = sequence[i + 2];
	}

	return pair<bool,string>((isAccepted || prefix.size() == sequence.size()), prefix);
}

int main()
{
	displayMenu();

	string file = "integer_literal_fsm_description.txt";
	//string file = "af_description.txt";

	int cmd = 0;
	bool wasRead = false;

	string sequence = "123431253445433";
	
	bool isAccepted = false;
	string prefix;
	
	//readFromFile(file);
	//classifyTokens();

	while (cmd != -1) {
		cout << "Enter a command: ";

		cin >> cmd;

		switch (cmd)
		{

		case 0:
			clearStorage();
			cout << "Enter the FSM description, followed by *enter* \"end\" *enter*\n";
			readFromKeyboard();
			wasRead = true;
			classifyTokens();
			break;
		case 1:
			clearStorage();
			readFromFile(file);
			wasRead = true;
			classifyTokens();
			break;
		case 2:
			displayStates();
			break;
		case 3:
			displayAlphabet();
			break;
		case 4:
			displayTransitions();
			break;
		case 5:
			displayFinalStates();
			break;
		case 6:

			cout << "enter sequence:\n";
			cin >> sequence;

			isAccepted = checkIsAccepted(sequence).first;
			if (isAccepted)
				cout << "Sequence " << sequence << " is accepted by the FSM\n";
			else
				cout << "Sequence " << sequence << " is not accepted by the FSM\n";
			break;
		case 7:
			cout << "enter sequence:\n";
			cin >> sequence;
			prefix = checkIsAccepted(sequence).second;
			cout << "Longest prefix is: " << prefix << '\n';
			break;
		default:
			if (cmd != -1)
				cout << "Wrong input!\n";
			break;
		}
	}

}
