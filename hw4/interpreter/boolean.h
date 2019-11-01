#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include "arithmetic.h"

class BooleanExpression {
public:
    BooleanExpression() {}
    virtual ~BooleanExpression() {}
    virtual std::string format() const = 0;
    virtual int getVal(std::map<std::string,std::map<int,int> >& valmap) const = 0;
};


class Greater : public BooleanExpression {
public:
    Greater(NumericExpression* left, NumericExpression* right);
    ~Greater();

    virtual std::string format() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;

private:
    NumericExpression* left;
    NumericExpression* right;

};

class Less : public BooleanExpression {
public:
    Less(NumericExpression* left, NumericExpression* right);
    ~Less();

    virtual std::string format() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;

private:
    NumericExpression* left;
    NumericExpression* right;

};

class Equals : public BooleanExpression {
public:
    Equals(NumericExpression* left, NumericExpression* right);
    ~Equals();

    virtual std::string format() const;
    int getVal(std::map<std::string,std::map<int,int> >& valmap) const;

private:
    NumericExpression* left;
    NumericExpression* right;

};

#endif