#include "arithmetic.h"

using namespace std;

Addition::Addition(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Addition::~Addition() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Addition::format() const {
    return "(" + this->left->format() + " + " + this->right->format() + ")";
}

int Addition::getVal(map<string,map<int,int> > valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    return leftex + rightex;
}

Subtraction::Subtraction(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Subtraction::~Subtraction() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Subtraction::format() const {
    return "(" + this->left->format() + " - " + this->right->format() + ")";
}

int Subtraction::getVal(map<string,map<int,int> > valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    return leftex - rightex;
}


Multiplication::Multiplication(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Multiplication::~Multiplication() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Multiplication::format() const {
    return "(" + this->left->format() + " * " + this->right->format() + ")";
}

int Multiplication::getVal(map<string,map<int,int> > valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    return leftex * rightex;
}

Division::Division(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Division::~Division() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Division::format() const {
    return "(" + this->left->format() + " / " + this->right->format() + ")";
}

int Division::getVal(map<string,map<int,int> > valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    return leftex / rightex;
}

Constant::Constant(string value) : value(value) {

}

Constant::~Constant() {

}

string Constant::format() const {
    return this->value;

}

int Constant::getVal(map<string,map<int,int> > valmap) const {
    return stoi(value);
}

VarArray::VarArray(NumericExpression* index, string varArrName) : index(index), varArrName(varArrName) {

}

VarArray::~VarArray() {
    if (index) {
        delete this->index;
    }
}

string VarArray::format() const {
    return this->varArrName + "[" + this->index->format() + "]";
}

int VarArray::getVal(map<string,map<int,int> > valmap) const {
    
    
    int access = this->index->getVal(valmap);

    return valmap[varArrName][access];
}

VarNum::VarNum(string varName, int content) : varName(varName), content(content){


}

VarNum::~VarNum() {


}

string VarNum::format() const {
    return this->varName;

}

int VarNum::getVal(map<string,map<int,int> > valmap) const {

    return valmap[varName][696969696];
    
}