#include "boolean.h"

using namespace std;

Greater::Greater(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Greater::~Greater() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Greater::format() const {
    return this->right->format() + " < " + this->left->format();
}

int Greater::getVal(std::map<std::string,std::map<int,int> >& valmap) const {
    if (this->left->getVal(valmap) > this->right->getVal(valmap)) return 1;
    else return 0;
}

Less::Less(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Less::~Less() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Less::format() const {
    return this->left->format() + " < " + this->right->format();
}

int Less::getVal(std::map<std::string,std::map<int,int> >& valmap) const {
    if (this->left->getVal(valmap) < this->right->getVal(valmap)) return 1;
    else return 0;
}

Equals::Equals(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

Equals::~Equals() {
    if (left) {
        delete this->left;
    }
    if (right) {
        delete this->right;
    }
}

string Equals::format() const {
    return this->left->format() + " = " + this->right->format();
}

int Equals::getVal(std::map<std::string,std::map<int,int> >& valmap) const {
    if (this->left->getVal(valmap) == this->right->getVal(valmap)) return 1;
    else return 0;
}