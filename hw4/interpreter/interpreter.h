#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "command.h"

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


    void Interpret(vector<Command*> commandvec);


private:
    void parse(std::istream& in);

};


#endif
