#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "command.h"
#include <vector>
#include <stack>

class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();

    void write(std::ostream& out);
    NumericExpression* parseConst(std::string v);
    NumericExpression* parseVar(std::string n);  
    

    std::string cleanws(std::string dirtystring);
    NumericExpression* leftrightrec(std::string);
    

    Goto* GotoParse(LineNum line, std::string);
    GoSub* GosubParse(LineNum line, std::string);
    BooleanExpression* IfParse(LineNum line, std::string);
    LetVar* LetParse(std::string ss, LineNum line);
    LetVarArray* LetParseArr(std::string ss, LineNum line);

    void Interpret(std::vector<Command*> cvec, std::map<int,int> linemap);


private:
    void parse(std::istream& in);

};


#endif
