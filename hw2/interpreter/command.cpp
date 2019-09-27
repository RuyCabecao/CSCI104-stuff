#include "command.h"


using namespace std;

Print::Print(NumericExpression* target, LineNum line) : target(target), line(line) {


}

Print::~Print() {
    if (target) {
        delete this->target;
    }
}

string Print::format() const{
    return this->line.format() + " PRINT " + this->target->format();

}

//////////////////////////////////////////

LetVar::LetVar(NumericExpression* var, NumericExpression* NExp, LineNum line) : var(var), line(line) {


}

LetVar::~LetVar() {
    if (var) {
        delete this->var;
    }
    if (NExp) {
        delete this->NExp;
    }

}

string LetVar::format() const{
    return this->line.format() + " LET " + this->var->format() + this->NExp->format();

}

//////////////////////////////////////////

LetVarArray::LetVarArray(NumericExpression* var, NumericExpression* ind, NumericExpression* NExp, LineNum line) : var(var), ind(ind), NExp(NExp), line(line) {


}

LetVarArray::~LetVarArray() {
    if (var) {
        delete this->var;
    }
    if (NExp) {
        delete this->NExp;
    }
    if (ind) {
        delete this->ind;
    }
}

string LetVarArray::format() const{
    return this->line.format() + " LET " + this->var->format() + "[" + this->ind->format() + "] " + this->NExp->format();

}

//////////////////////////////////////////

Goto::Goto(LineNum line, LineNum jLine) : line(line), jLine(jLine) {


}

Goto::~Goto() {


}

string Goto::format() const{
    return this->line.format() + " GOTO " + this->jLine.format();

}

//////////////////////////////////////////

IfC::IfC(BooleanExpression* cond, LineNum jLine, LineNum line) : cond(cond), jLine(jLine), line(line) {


}

IfC::~IfC() {
    if (cond) {
        delete this->cond;
    }

}

string IfC::format() const{
    return this->line.format() + " IF [" + this->cond->format() + "] THEN <" + this->jLine.format() + ">"; 

}

//////////////////////////////////////////

GoSub::GoSub(LineNum line, LineNum jLine) : line(line), jLine(jLine){


}

GoSub::~GoSub() {


}

string GoSub::format() const{
    return this->line.format() + " GOSUB " + "<" + this->jLine.format() + ">";

}

//////////////////////////////////////////

ReturnC::ReturnC(LineNum line) : line(line) {


}

ReturnC::~ReturnC() {


}

string ReturnC::format() const{
    return this->line.format() + " RETURN";

}

//////////////////////////////////////////

EndC::EndC(LineNum line) : line(line) {


}

EndC::~EndC() {


}

string EndC::format() const{
    return this->line.format() + " END";

}

/////////////////////////////////////////

LineNum::LineNum(int line) : line(line) {


}

LineNum::~LineNum() {


}

string LineNum::format() const{
    stringstream out;
    out << line;
    return  out.str();

}