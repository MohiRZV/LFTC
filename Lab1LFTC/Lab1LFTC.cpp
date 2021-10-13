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
    fip.insert({ "integer", 2 });
    fip.insert({ "double",  3 });
    fip.insert({ "circle",  4 });
    fip.insert({ ":", 5 });
    fip.insert({ ",", 6 });
    fip.insert({ ";", 7 });
    fip.insert({ "{", 8 });
    fip.insert({ "}", 9 });
    fip.insert({ "(", 10 });
    fip.insert({ "->",  11 });
    fip.insert({ "<-",  12 });
    fip.insert({ "+",   13 });
    fip.insert({ "-",   14 });
    fip.insert({ "*",   15 });
    fip.insert({ "++",  16 });
    fip.insert({ "--",  17 });
    fip.insert({ "<",   18 });
    fip.insert({ ">",   19 });
    fip.insert({ "!=",  20 });
    fip.insert({ "!",  31 });
    fip.insert({ "<=",  21 });
    fip.insert({ "=",   22 });
    fip.insert({ "birth",       23 });
    fip.insert({ "die.",        24 });
    fip.insert({ "supposing",   25 });
    fip.insert({ "thereupon",   26 });
    fip.insert({ "gather",      27 });
    fip.insert({ "dash",        28 });
    fip.insert({ "purge",       29 });
    fip.insert({ "given",       30 });

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
        //remove spaces and tabs
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        source.append(line);
    }
    //std::cout << source;
    return source;
}

int checkIsAtom(std::string atomWannabe, std::map<std::string, int> atoms) {
    std::map<std::string, int>::iterator it = atoms.find(atomWannabe);
    if (it != atoms.end()) {
        return it->second;
    }
    return -1;
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

void breakDownCharByChar() {
    std::map<std::string, int> atoms = getAtomsTable();
    std::vector<FIP*> fip;
    LT* ts = new LT();
    std::string source = readSourceAsString("source.txt");
    std::string currentAtom;
    int i = 0;
    while(i<source.size())
    {
        bool isIDorCONST = true;
        //if the atom built so far is in the atoms list
        int code = checkIsAtom(currentAtom, atoms);
        if (code != -1) {
            //add it to the fip
            fip.push_back(new FIP(currentAtom, code));
            isIDorCONST = false;
        }

        //this deals with the current character [and the next one]
        //if the atom built so far is an operator
        //check if it is part of a composite operator
        std::string current;
        current.push_back(source[i]);
        //if the current char is an operator, check if it is part of a composite operator
        if (isSeparator(current, atoms)) {
            std::string composite;
            composite.push_back(source[i]);
            composite.push_back(source[i + 1]);
            if (isSeparator(composite, atoms)) {
                current.assign(composite);
                i++;
            }
            
            if (isIDorCONST) {
                //if control reached here, currentAtom is an ID or a CONST
                code = ts->find(currentAtom);
                if (code == -1) {
                    int codeTS = ts->add(currentAtom);
                    //add the codeTS to correspoding FIP entry4
                    if (isNumeric(currentAtom))
                        fip.push_back(new FIP(currentAtom, 1, codeTS));
                    else  //TODO check if it is a valid variable name, or throw exception
                        fip.push_back(new FIP(currentAtom, 0, codeTS));
                    currentAtom.clear();
                }
            }
            fip.push_back(new FIP(current, checkIsAtom(current, atoms)));
            currentAtom.clear();
            i++;
        }

        currentAtom.push_back(source[i]);
        i++;
    }
    
    std::cout << "FIP\n";
    for (auto i : fip) {
        std::cout << i->toString() << '\n';
    }
    std::cout << "TS\n" << "size: " << ts->getSize() << '\n';
    for (int i = 0; i < ts->getSize(); i++) {
        std::cout << ts->get(i)->toString() << '\n';
    }
}

void breakDown() {
    std::map<std::string, int> atoms = getAtomsTable();
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
            int code = ts->find(i);
            if (code == -1) {
                int codeTS = ts->add(i);
                //add the codeTS to correspoding FIP entry
                if(isNumeric(i))
                    fip.push_back(new FIP(i, 1, codeTS));
                else
                    fip.push_back(new FIP(i, 0, codeTS));
            }
          
        }
    }
    std::cout << "FIP\n";
    for (auto i : fip) {
        std::cout << i->toString() << '\n';
    }
    std::cout << "TS\n";
    for (int i = 0; i < ts->getSize();i++) {
        std::cout << ts->get(i)->toString() << '\n';
    }
    /*std::cout << "Atoms:\n";
    for (auto i : fip) {
        std::cout << i->atom << '\n';
    }*/
}

int main()
{
    //breakDownCharByChar();
    breakDown();
}
