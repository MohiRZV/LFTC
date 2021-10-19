//#pragma once
//#include <iostream>
//#include <map>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <regex>
//#include <algorithm> 
//#include "FIP.cpp"
//#include "LT.cpp"
//#include "Lab1LFTC.cpp"
//
//void breakDownCharByChar() {
//    std::map<std::string, int> atoms = getAtomsTable();
//    std::vector<FIP*> fip;
//    LT* ts = new LT();
//    std::string source = readSourceAsString("source.txt");
//    std::string currentAtom;
//    int i = 0;
//    while (i < source.size())
//    {
//        bool isIDorCONST = true;
//        //if the atom built so far is in the atoms list
//        int code = checkIsAtom(currentAtom, atoms);
//        if (code != -1) {
//            //add it to the fip
//            fip.push_back(new FIP(currentAtom, code));
//            isIDorCONST = false;
//        }
//
//        //this deals with the current character [and the next one]
//        std::string current;
//        current.push_back(source[i]);
//        if (isSpace(current) && !isSpace(std::string(1, source[i - 1]))) {
//            if (isIDorCONST) {
//                //if control reached here, currentAtom is an ID or a CONST
//                code = ts->find(currentAtom);
//                if (code == -1) {
//                    int codeTS = ts->add(currentAtom);
//                    //add the codeTS to correspoding FIP entry4
//                    if (isNumeric(currentAtom))
//                        fip.push_back(new FIP(currentAtom, 1, codeTS));
//                    else  //TODO check if it is a valid variable name, or throw exception
//                        fip.push_back(new FIP(currentAtom, 0, codeTS));
//                }
//            }
//            currentAtom.clear();
//        }
//        //if the current char is an operator, check if it is part of a composite operator
//        else if (isSeparator(current, atoms)) {
//            std::string composite;
//            composite.push_back(source[i]);
//            composite.push_back(source[i + 1]);
//            if (isSeparator(composite, atoms)) {
//                current.assign(composite);
//                i++;
//            }
//
//            if (isIDorCONST) {
//                //if control reached here, currentAtom is an ID or a CONST
//                code = ts->find(currentAtom);
//                //TODO add each atom appearance to the FIP, but with the same TS code
//                if (code == -1) {
//                    int codeTS = ts->add(currentAtom);
//                    //add the codeTS to correspoding FIP entry
//                    if (isNumeric(currentAtom))
//                        fip.push_back(new FIP(currentAtom, 1, codeTS));
//                    else  //TODO check if it is a valid variable name, or throw exception
//                        fip.push_back(new FIP(currentAtom, 0, codeTS));
//                }
//            }
//
//            fip.push_back(new FIP(current, checkIsAtom(current, atoms)));
//            currentAtom.clear();
//            i++;
//        }
//
//        currentAtom.push_back(source[i]);
//
//        i++;
//    }
//
//    std::cout << "FIP\n";
//    for (auto i : fip) {
//        i->setCodeTS(ts->find(i->atom));
//        std::cout << i->toString() << '\n';
//    }
//    std::cout << "TS\n" << "size: " << ts->getSize() << '\n';
//    for (int i = 0; i < ts->getSize(); i++) {
//        std::cout << ts->get(i)->toString() << '\n';
//    }
//}