//TS - tabela de simboluri
//data class for one TS entry
#pragma once
#include <string>

class TS {
	std::string symbol;
	int code;

public:
	TS(std::string symbol, int code) : symbol{ symbol }, code{ code }{

	}
	
	std::string toString() {
		std::string s = "";
		s.append("symbol= ").append(symbol).append(" , code= ").append(std::to_string(code));
		return s;
	}

	void setCode(int code) {
		this->code = code;
	}

	std::string getSymbol() {
		return symbol;
	}
};