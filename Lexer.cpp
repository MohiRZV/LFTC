#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>
#include "FIP.cpp"
#include "LT.cpp"
#include "TS.cpp"

using namespace std;

map<string, int> keywords;
map<string, int> operators;

//method to create the available atoms table as a map

void generateAtomsTables() {

    //fip.insert({ "ID",      0 });
    //fip.insert({ "CONST",   1 });
    keywords.insert({ "int", 2 });
    keywords.insert({ "double",  3 });
    keywords.insert({ "circle",  4 });
    operators.insert({ ":", 5 });
    operators.insert({ ",", 6 });
    operators.insert({ ";", 7 });
    operators.insert({ "{", 8 });
    operators.insert({ "}", 9 });
    operators.insert({ "(", 10 });
    operators.insert({ ")", 11 });
    operators.insert({ ">>",  12 });
    operators.insert({ "<<",  13 });
    operators.insert({ "+",   14 });
    operators.insert({ "-",   15 });
    operators.insert({ "*",   16 });
    operators.insert({ "++",  17 });
    operators.insert({ "--",  18 });
    operators.insert({ "<",   19 });
    operators.insert({ ">",   20 });
    operators.insert({ "!=",  21 });
    operators.insert({ "!",  22 });
    operators.insert({ "<=",  23 });
    operators.insert({ "=",   24 });
    keywords.insert({ "main",       25 });
    keywords.insert({ "return",     26 });
    keywords.insert({ "if",         27 });
    keywords.insert({ "while",      28 });
    keywords.insert({ "cin",        29 });
    keywords.insert({ "cout",       30 });
    keywords.insert({ "for",        31 });

}
//transition states enum for the Finite State Machine
   
enum T_STATES
{
    ILLEGAL = 0,
    INTEGER,
    REAL,
    OPERATOR,
    ATOM,
    UNKNOWN,
    SPACE,
    KEYWORD, 
    VAR_NAME
};

/* FSM Table
    The zero represents a place holder, so the row in the array starts on
    row 1 instead of 0.

    
                            integer,  real,     operator,  atom,     unknown,  space,   keyword */
int stateTable[][8] = { {0, INTEGER,  REAL,     OPERATOR,  ATOM,     UNKNOWN,  SPACE,   KEYWORD},
/* STATE 1 */    {INTEGER,  INTEGER,  REAL,     ILLEGAL,   ILLEGAL,  ILLEGAL,  ILLEGAL, ILLEGAL},
/* STATE 2 */    {REAL,     REAL,     UNKNOWN,  ILLEGAL,   ILLEGAL,  ILLEGAL,  ILLEGAL, ILLEGAL},
/* STATE 3 */    {OPERATOR, ILLEGAL,  ILLEGAL,  OPERATOR,  ILLEGAL,  ILLEGAL,  ILLEGAL, ILLEGAL},
/* STATE 4 */    {ATOM,     ATOM,     ILLEGAL,  ILLEGAL,   ATOM,     ILLEGAL,  ILLEGAL, KEYWORD},
/* STATE 5 */    {UNKNOWN,  UNKNOWN,  UNKNOWN,  UNKNOWN,   UNKNOWN,  UNKNOWN,  ILLEGAL, ILLEGAL},
/* STATE 6 */    {SPACE,    ILLEGAL,  ILLEGAL,  ILLEGAL,   ILLEGAL,  ILLEGAL,  ILLEGAL, ILLEGAL},
/* STATE 7 */    {KEYWORD,  ILLEGAL,  ILLEGAL,  ILLEGAL,   KEYWORD,  ILLEGAL,  ILLEGAL, ILLEGAL} };


// token information
struct TokenType
{
    // the actual token
    string token;
    // the token type
    int lexeme;
    // the token type name
    string name;
};

// function prototypes
vector<TokenType> Lexer(string expression);
int FSM_column(char currentChar);
string getName(int lexeme);

string errors;
vector<FIP*> fip;
LT* ts;

void addTokensToTables(vector<TokenType> tokens, int line) {
    for (auto token : tokens) {
        string err;
        switch (token.lexeme) {
        case INTEGER:
            ts->add(token.token);
            fip.push_back(new FIP(token.token, 1, -1));
            break;
        case REAL:
            ts->add(token.token);
            fip.push_back(new FIP(token.token, 1, -1));
            break;
        case VAR_NAME:
            ts->add(token.token);
            fip.push_back(new FIP(token.token, 0, -1));
            break;
        case KEYWORD:
            fip.push_back(new FIP(token.token, keywords.find(token.token)->second, -1));
            break;
        case ILLEGAL:
            err.append("Something is wrong with the variable: ").append(token.token).append(" on line: ").append(to_string(line)).append("\n");
            errors.append(err);
            break;
        default:
            fip.push_back(new FIP(token.token, operators.find(token.token)->second, -1));
            break;
        }
    }
}

int main()
{
    ifstream fin;
    string fileName = "source.txt";
    string expression = "";
    vector<TokenType> tokens;

    ts = new LT();

    // create atoms list
    generateAtomsTables();

    fin.open(fileName.c_str());

    if (fin.fail())
    {
        cout << "\n** ERROR - the file \"" << fileName << "\" cannot be found!\n\n";
        exit(1);
    }

    int line = 0;
    // parse the file line by line
    while (getline(fin, expression))
    {
        // separate the tokens in the current line
        tokens = Lexer(expression);

        addTokensToTables(tokens, line);
        // display the tokens
       /* for (unsigned x = 0; x < tokens.size(); ++x)
        {
            cout << tokens[x].name << "  \t"
                << tokens[x].token << endl;
            
        }*/
        line++;
    }

    fin.close();

    if (errors.empty()) {
        for (auto i : fip) {
            // update the TS code for each FIP entry (when an item is added to the TS, the codes change so they are consistent with the position)
            i->setCodeTS(ts->find(i->atom));
        }
        // display FIP
        cout << "FIP:\n";
        for (auto i : fip) {
            cout << i->toString() << '\n';
        }
        // display TS
        cout << "TS:\n";
        for (int i = 0; i < ts->getSize();i++) {
            cout << ts->get(i)->toString() << '\n';
        }
    }
    else {
        cout << errors;
    }

    return 0;
}// end of main

// function to check if the given token is among the available operators
bool isOperator(string op) {
    return operators.find(op) != operators.end();
}

// function to check if the given token is among the available keywords
bool isKeyword(string token) {
    if (keywords.find(token) != keywords.end())
        return true;
    return false;
}

// function to check if the given variable name is correct 
bool isVariableNameCorrect(string name) {
    if (std::regex_match(name, std::regex("[a-zA-Z][[:digit:]]*[a-zA-Z]*")) && name.length() <= 8)
        return true;
    return false;
}

// function to differentiate between the different types of tokens that share the same state
int cathegorizeLexeme(string token, int state, vector<TokenType> tokens) {
    switch (state)
    {
    case ATOM:
        if (isKeyword(token))// check if token is keyword
            return KEYWORD;
        else {// if it is not a keyword
            if (isVariableNameCorrect(token)) {// it is either a variable
                return VAR_NAME;
            }
            else {// or syntax error
                return ILLEGAL;
            }
        }
    default:
        return state;
    }
}

/**
* Parses the "expression" string 
* @param expression - A string containing a line of the source code
* @return - Returns a vector containing the tokens found in the string
*/
vector<TokenType> Lexer(string expression)
{
    TokenType token;
    vector<TokenType> tokens;
    char currentChar = ' ';
    int col = ILLEGAL;
    int currentState = ILLEGAL;
    int prevState = ILLEGAL;
    string currentToken = "";

    // parse the expression by using the FSM, building atoms char by char
    for (unsigned x = 0; x < expression.length();)
    {
        currentChar = expression[x];

        // get the column number for the current character
        col = FSM_column(currentChar);

        // change the state based on the current char
        currentState = stateTable[currentState][col];

        // if the ILLEGAL state is reached, then the currently built token is either an atom, or a syntax error took place
        if (currentState == ILLEGAL)
        {
            if (prevState != SPACE) // ignore whitespace
            {
                // set the token details
                token.token = currentToken;
                // in case the current token state matches multiple types of atoms, cathegorize it
                token.lexeme = cathegorizeLexeme(currentToken, prevState, tokens);
                token.name = getName(token.lexeme);
                tokens.push_back(token);
            }
            // clear the currentToken
            currentToken = "";
        }
        else
        {
            // add the currentChar to the token that is being built
            currentToken += currentChar;
            // skip the whitespace
            ++x;
        }
        prevState = currentState;

    }

    // in case the last token is valid, save it (there might be no separator at the end of the line/received expression)
    if (currentState != SPACE && currentToken != "")// ignore whitespace
    {
        token.token = currentToken;
        token.lexeme = currentState;
        token.name = getName(token.lexeme);
        tokens.push_back(token);
    }
    return tokens;
}// end of Lexer

/**
* Determines the state of the character being examined.
* @param currentChar - a char
* @return - Returns the state of the type of character being examined.
*/
int FSM_column(char currentChar)
{
    // check for whitespace
    if (isspace(currentChar))
    {
        return SPACE;
    }

    // check for integers
    else if (isdigit(currentChar))
    {
        return INTEGER;
    }

    // check for real numbers
    else if (currentChar == '.')
    {
        return REAL;
    }
    // check for characters
    else if (isalpha(currentChar))
    {
        return ATOM;
    }

    // check for operators
    else if (isOperator(string(1, currentChar)))
    {
        return OPERATOR;
    }
    return UNKNOWN;
}

/**
* Returns the name of the token type as a string
*/
string getName(int tokenType)
{
    switch (tokenType)
    {
    case INTEGER:
        return "INTEGER";
        break;
    case REAL:
        return "REAL    ";
        break;
    case OPERATOR:
        return "OPERATOR";
        break;
    case ATOM:
        return "ATOM    ";
        break;
    case UNKNOWN:
        return "UNKNOWN";
        break;
    case SPACE:
        return "SPACE";
        break;
    case KEYWORD:
        return "KEYWORD";
        break;
    case VAR_NAME:
        return "VAR_NAME";
        break;
    default:
        return "ERROR   ";
        break;
    }
}
