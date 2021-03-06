#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <sstream>
#include "arithmetic.h"
#include "boolean.h"


class Command {
public:
    Command() {}
    virtual ~Command() {}
    virtual std::string format() const = 0;
    virtual int getLine() const = 0;
    virtual int getJline() const {return 0;};
    virtual int getVal(std::map<std::string,std::map<int,int> >& valmap) const {
        return 0;
    };
    virtual std::string getCommName() const = 0;
};

class LineNum : public Command{
public:    
    LineNum(int line);
    ~LineNum();

    std::string format() const;
    int getLine() const;
    std::string getCommName() const;

private:
    int line;

};

class Print : public Command {
public:
    Print(NumericExpression* target, LineNum line);
    ~Print();
    std::string format() const;
    int getLine() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;
    std::string getCommName() const;

private:
    NumericExpression* target;
    LineNum line;

};

class LetVar : public Command {
public:
    LetVar(NumericExpression* var, NumericExpression* NExp, LineNum line);
    ~LetVar();
    std::string format() const;
    int getLine() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;
    std::string getCommName() const;

private:
    NumericExpression* var;
    NumericExpression* NExp;
    LineNum line;

};

class LetVarArray : public Command {
public:
    LetVarArray(NumericExpression* vararr, NumericExpression* NExp, LineNum line);
    ~LetVarArray();
    std::string format() const;
    int getLine() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;
    std::string getCommName() const;

private:
    NumericExpression* vararr;
    NumericExpression* NExp;
    LineNum line;

};

class Goto : public Command {
public:
    Goto(LineNum line, LineNum jLine);
    ~Goto();
    std::string format() const;
    int getLine() const;
    int getJline() const;
    std::string getCommName() const;

private:
    LineNum line;
    LineNum jLine;

};

class IfC : public Command {
public:
    IfC(BooleanExpression* cond, LineNum jLine, LineNum line);
    ~IfC();
    std::string format() const;
    int getLine() const;
    int getJline() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;
    std::string getCommName() const;

private:
    BooleanExpression* cond;
    LineNum jLine;
    LineNum line;  

};

class GoSub : public Command {
public:
    GoSub(LineNum line, LineNum jLine);
    ~GoSub();
    std::string format() const;
    int getLine() const;
    int getJline() const;
    std::string getCommName() const;

private:
    LineNum line;
    LineNum jLine; 
};

class ReturnC : public Command {
public:
    ReturnC(LineNum line);
    ~ReturnC();
    std::string format() const;
    int getLine() const;
    std::string getCommName() const;

private:
    LineNum line;

};

class EndC : public Command {
public:
    EndC(LineNum line);
    ~EndC();
    std::string format() const;
    int getLine() const;
    std::string getCommName() const;

private:
    LineNum line;

};

#endif
