#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include "command.h"

class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();

    void write(std::ostream& out);
    Constant parseConst(std::string v);
    NumericExpression* parseVar(std::string n);  

    std::string cleanws(std::string dirtystring);
    NumericExpression* leftrightrec(std::string);
    
    Goto* GotoParse(LineNum line, std::string);
    GoSub* GosubParse(LineNum line, std::string);
    BooleanExpression* IfParse(LineNum line, std::string);
    LetVar* LetParse(std::string ss, LineNum line);
    LetVarArray* LetParseArr(std::string ss, LineNum line);



private:
    void parse(std::istream& in);

};


#endif
