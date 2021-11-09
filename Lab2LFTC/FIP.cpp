//FIP - Forma Interna a Programului
//data class for one FIP entry
#pragma once
#include <string>

class FIP {
	
	int codeAtom;
	int codeTS;

public:
	std::string atom;

	FIP(std::string atom, int codeAtom, int codeTS = -1) : atom{ atom }, codeAtom { codeAtom }, codeTS{ codeTS }{

	}

	bool operator==(FIP& other) const {
		return atom==other.atom && codeAtom==other.codeAtom && codeTS==other.codeTS;
	}

	void setCodeTS(int code) {
		codeTS = code;
	}

	std::string toString() {
		std::string s = "";
		s.append("atom= ").append(atom).append(" ");
		s.append("codeAtom= ").append(std::to_string(codeAtom)).append(" , codeTS= ").append(std::to_string(codeTS));
		return s;
	}
};