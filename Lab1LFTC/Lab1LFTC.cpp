// Lab1LFTC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <algorithm> 
#include "FIP.cpp"
#include "LT.cpp"

//method to create the available atoms table as a map
std::map<std::string, int> getAtomsTable() {
    std::map<std::string, int> fip;

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

    return fip;
}

std::vector<std::string> split(const std::string& s, std::string rgx_str = " ") {


    std::vector<std::string> elems;

    std::regex rgx(rgx_str);

    std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
    std::sregex_token_iterator end;

    while (iter != end) {
        elems.push_back(*iter);
        ++iter;
    }

    return elems;

}

//reads the source code file line by line and returns each element, separated by spaces
std::vector<std::string> readFromFile(std::string file) {
    std::vector<std::string> elements;
    std::ifstream fin(file);
    std::string line;
    std::string delimiter = " ";
    while (std::getline(fin, line)) {
        for (auto e : split(line, " ")) {
            bool whiteSpacesOnly = std::all_of(e.begin(), e.end(), isspace);
            if(!whiteSpacesOnly)
                elements.push_back(e);
        }
    }
    return elements;
}

std::string readSourceAsString(std::string file) {
    std::string source;
    std::ifstream fin(file);
    std::string line;
    while (std::getline(fin, line)) {

        source.append(line);
    }
    return source;
}

int checkIsAtom(std::string atomWannabe, std::map<std::string, int> atoms) {
    std::map<std::string, int>::iterator it = atoms.find(atomWannabe);
    if (it != atoms.end()) {
        return it->second;
    }
    return -1;
}

bool isSpace(std::string s) {
    if (s.compare(" ") == 0 || s.compare("\t") == 0 || s.empty()) {
        return true;
    }
    return false;
}

bool isSeparator(std::string s, std::map<std::string, int> atoms) {
    std::map<std::string, int>::iterator it = atoms.find(s);
    if (it != atoms.end()) {
        return true;
    }
    return false;
}


bool isNumeric(std::string atom) {
    if (std::regex_match(atom, std::regex("[[:digit:]]*\.?[[:digit:]]+")))
        return true;
    return false;
}

bool isVariableNameCorrect(std::string name) {
    if (std::regex_match(name, std::regex("[a-zA-Z][[:digit:]]*[a-zA-Z]*")) && name.length() <= 8)
        return true;
    return false;
}



void breakDown() {
    std::map<std::string, int> atoms = getAtomsTable();
    std::string errors;
    std::vector<FIP*> fip;
    LT* ts = new LT();
    std::vector<std::string> elements = readFromFile("source.txt");
    for (auto i : elements) {
        std::map<std::string, int>::iterator it = atoms.find(i);
        bool isNotSymbolOrConstant = (it != atoms.end());
        //if the element is not a symbol or constant, it has no TS code
        //== it is a keyword or separator
        if (isNotSymbolOrConstant) {
            fip.push_back(new FIP(it->first, it->second));
        }
        else {
            //if the current symbol/constant is not already added to the TS
            
            //add the codeTS to correspoding FIP entry
            if (isNumeric(i)) {//is float/double
                int code = ts->find(i);
                int codeTS = -1;
                if (code == -1) {
                    codeTS = ts->add(i);
                }
                fip.push_back(new FIP(i, 1, codeTS));
            }
            else {//is variable name
                if (isVariableNameCorrect(i)) {
                    int code = ts->find(i);
                    int codeTS = -1;
                    if (code == -1) {
                        codeTS = ts->add(i);
                    }
                    fip.push_back(new FIP(i, 0, codeTS));
                }
                else {
                    std::string err;
                    err.append("Variable ").append(i).append(" contains illegal characters or is too long!\n");
                    errors.append(err);
                }
            }
          
        }
    }

    if (errors.empty()) {
        std::cout << "FIP\n";
        for (auto i : fip) {
            //update the TS code for each FIP entry (when an item is added to the TS, the codes change so they are consistent with the position)
            i->setCodeTS(ts->find(i->atom));
            std::cout << i->toString() << '\n';
        }
        std::cout << "TS\n";
        for (int i = 0; i < ts->getSize(); i++) {
            std::cout << ts->get(i)->toString() << '\n';
        }
        /*std::cout << "Atoms:\n";
        for (auto i : fip) {
            std::cout << i->atom << '\n';
        }*/
    }
    else {
        std::cout << "Compilation FAILED! There were syntax errors:\n";
        std::cout << errors;
    }
}

int main()
{
    //breakDownCharByChar();
    breakDown();
}
