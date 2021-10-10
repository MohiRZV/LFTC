//FIP - Forma Interna a Programului
//data class for one FIP entry
#pragma once
#include <string>

class FIP {
	int codeAtom;
	int codeTS;

public:
	FIP(int codeAtom, int codeTS = -1) : codeAtom{ codeAtom }, codeTS{ codeTS }{

	}

	std::string toString() {
		std::string s = "";
		s.append("codeAtom= ").append(std::to_string(codeAtom)).append(" , codeTS= ").append(std::to_string(codeTS));
		return s;
	}
};