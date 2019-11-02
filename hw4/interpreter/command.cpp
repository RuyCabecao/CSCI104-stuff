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

int Print::getLine() const{
    return this->line.getLine();
}

int Print::getVal(map<string,map<int,int> >& valmap) const {
    int value = this->target->getVal(valmap);
    return value; //recursive call to compute Numeric Expression
}

string Print::getCommName() const{
    return "PRINT";
}


//////////////////////////////////////////

LetVar::LetVar(NumericExpression* var, NumericExpression* NExp, LineNum line) : var(var), NExp(NExp) , line(line) {


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
    return this->line.format() + " LET " + this->var->format() + " " + this->NExp->format();

}

int LetVar::getLine() const{
    return this->line.getLine();
}

int LetVar::getVal(map<string,map<int,int> >& valmap) const {
    int value = this->NExp->getVal(valmap);
    //recursively calculates value var will be set to

    map<int,int> tempmap;
    //initializes a temporary map of index to value

    tempmap = valmap[var->getName()];
    tempmap[696969696] = value;
    //copies what was already in valmap and adds
    //value at non-array index

    //index is that long number so that  I didn't
    //have to create an entire new map for 
    //non-arrays

    valmap[var->getName()] = tempmap;
    //copies tempmap with all correct values back to
    //valmap

    return value;
}

string LetVar::getCommName() const{
    return "LET";
}




//////////////////////////////////////////

LetVarArray::LetVarArray(NumericExpression* vararr, NumericExpression* NExp, LineNum line) : vararr(vararr), NExp(NExp), line(line) {


}

LetVarArray::~LetVarArray() {
    if (vararr) {
        delete this->vararr;
    }
    if (NExp) {
        delete this->NExp;
    }
}

string LetVarArray::format() const{
    return this->line.format() + " LET " + this->vararr->format() + " " + this->NExp->format();

}

int LetVarArray::getLine() const{
    return this->line.getLine();

}

int LetVarArray::getVal(map<string,map<int,int> >& valmap) const {
    int access = this->vararr->getIndex(valmap);
    int value = this->NExp->getVal(valmap);
    map<int,int> tempmap;
    //does same as LetVar, except gets actual index 
    //from recursive getVal call

    tempmap = valmap[vararr->getName()];
    tempmap[access]=value;
    //copies what was already in valmap and adds
    //value at given index

    valmap[vararr->getName()]=tempmap;
    //copies tempmap with all correct values back to
    //valmap

    return value;
}

string LetVarArray::getCommName() const{
    return "LETARR";
}

//////////////////////////////////////////

Goto::Goto(LineNum line, LineNum jLine) : line(line), jLine(jLine) {


}

Goto::~Goto() {


}

string Goto::format() const{
    return this->line.format() + " GOTO " + "<" + this->jLine.format() + ">";

}

int Goto::getLine() const{
    return this->line.getLine();
}

int Goto::getJline() const{
    return this->jLine.getLine();
} 

string Goto::getCommName() const{
    return "GOTO";
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

int IfC::getLine() const{
    return this->line.getLine();
}

int IfC::getJline() const{
    return this->jLine.getLine();
} 

int IfC::getVal(map<string,map<int,int> >& valmap) const {
    int value = this->cond->getVal(valmap);
    return value; //recursive call to compute Numeric Expression
}

string IfC::getCommName() const{
    return "IF";
}

//////////////////////////////////////////

GoSub::GoSub(LineNum line, LineNum jLine) : line(line), jLine(jLine){


}

GoSub::~GoSub() {


}

string GoSub::format() const{
    return this->line.format() + " GOSUB " + "<" + this->jLine.format() + ">";

}

int GoSub::getLine() const{
    return this->line.getLine();
}

int GoSub::getJline() const{
    return this->jLine.getLine();
} 

string GoSub::getCommName() const{
    return "GOSUB";
}


//////////////////////////////////////////

ReturnC::ReturnC(LineNum line) : line(line) {


}

ReturnC::~ReturnC() {


}

string ReturnC::format() const{
    return this->line.format() + " RETURN";

}

int ReturnC::getLine() const{
    return this->line.getLine();
}

string ReturnC::getCommName() const{
    return "RETURN";
}

//////////////////////////////////////////

EndC::EndC(LineNum line) : line(line) {


}

EndC::~EndC() {


}

string EndC::format() const{
    return this->line.format() + " END";
}

int EndC::getLine() const{
    return this->line.getLine();
}

string EndC::getCommName() const{
    return "END";
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

int LineNum::getLine() const{
    return line;
}

string LineNum::getCommName() const{
    return "LINE";
}