#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include <string>
#include <iostream>
#include "arithmetic.h"

class BooleanExpression {
public:
    BooleanExpression() {}
    virtual ~BooleanExpression() {}
    virtual std::string format() const = 0;
};


class Greater : public BooleanExpression {
public:
    Greater(NumericExpression* left, NumericExpression* right);
    ~Greater();

    virtual std::string format() const;

private:
    NumericExpression* left;
    NumericExpression* right;

};

class Less : public BooleanExpression {
public:
    Less(NumericExpression* left, NumericExpression* right);
    ~Less();

    virtual std::string format() const;

private:
    NumericExpression* left;
    NumericExpression* right;

};

class Equals : public BooleanExpression {
public:
    Equals(NumericExpression* left, NumericExpression* right);
    ~Equals();

    virtual std::string format() const;

private:
    NumericExpression* left;
    NumericExpression* right;

};

#endif