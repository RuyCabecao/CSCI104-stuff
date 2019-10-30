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

Constant::Constant(string value) : value(value) {

}

Constant::~Constant() {

}

string Constant::format() const {
    return this->value;

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

VarNum::VarNum(string varName, int content) : varName(varName), content(content){


}

VarNum::~VarNum() {


}

string VarNum::format() const {
    return this->varName;

}