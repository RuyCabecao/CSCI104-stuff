#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>
#include <iostream>

class NumericExpression {
public:
    NumericExpression() {}
    virtual ~NumericExpression() {}
    virtual std::string format() const = 0;
};

class Addition : public NumericExpression {
public:
    Addition(NumericExpression* left, NumericExpression* right);
    ~Addition();

    std::string format() const;

private:
    NumericExpression* left;
    NumericExpression* right;
};

class Subtraction : public NumericExpression {
public:
    Subtraction(NumericExpression* left, NumericExpression* right);
    ~Subtraction();

    std::string format() const;

private:
    NumericExpression* left;
    NumericExpression* right;
};


class Multiplication : public NumericExpression {
public:
    Multiplication(NumericExpression* left, NumericExpression* right);
    ~Multiplication();

    std::string format() const;
    
private:
    NumericExpression* left;
    NumericExpression* right;
};

class Division : public NumericExpression {
public:
    Division(NumericExpression* left, NumericExpression* right);
    ~Division();

    std::string format() const;
    
private:
    NumericExpression* left;
    NumericExpression* right;
};


class Constant : public NumericExpression {
public:
    Constant(std::string value);
    ~Constant();

    std::string format() const;
    
private:
    std::string value;

};

class VarArray : public NumericExpression {
public:
    VarArray(NumericExpression* index, std::string varArrName);
    ~VarArray();

    std::string format() const;
    
private:
    NumericExpression* index;
    std::string varArrName;

};

class VarNum : public NumericExpression {
public:
    VarNum(std::string varName, int content);
    ~VarNum();

    std::string format() const;
    
private:
    std::string varName;
    int content;

};

#endif
