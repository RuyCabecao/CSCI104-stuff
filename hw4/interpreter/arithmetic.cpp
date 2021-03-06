#include "arithmetic.h"
#include <string>
#include <sstream>

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

int Addition::getVal(map<string,map<int,int> >& valmap) const {
    int leftex = this->left->getVal(valmap); 
    int rightex = this->right->getVal(valmap);
    return leftex + rightex;
    //recursive call to get value of both sides, then add and return
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

int Subtraction::getVal(map<string,map<int,int> >& valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    return leftex - rightex;
    //recursive call to get value of both sides, subtract, and return
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

int Multiplication::getVal(map<string,map<int,int> >& valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    return leftex * rightex;
    //recursive call to get value of both sides, multiply, and return
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

int Division::getVal(map<string,map<int,int> >& valmap) const {
    int leftex = this->left->getVal(valmap);
    int rightex = this->right->getVal(valmap);
    
    if (rightex != 0) return leftex / rightex;
    else {
        std::stringstream  ss;
        ss << "Division by 0: " << this->left->format() << " = " << leftex;
        ss << ", " << this->right->format() << " = "<< rightex << "." << endl;
        std::string except  = ss.str();
        throw(except);//throws exception when dividing by 0
    }
    //recursive call to get value of both sides, divide, and return
}

Constant::Constant(string value) : value(value) {

}

Constant::~Constant() {

}

string Constant::format() const {
    return this->value;

}

int Constant::getVal(map<string,map<int,int> >& valmap) const {
    return stoi(value); //returns value as int
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

int VarArray::getVal(map<string,map<int,int> >& valmap) const {
    if(valmap.count(varArrName) == 0) {
        valmap[varArrName][index->getVal(valmap)] = 0;
        //if variable hasn't been called before, value = 0
    }

    return valmap[varArrName][index->getVal(valmap)];
    //if not, just return the value already existent
}
string VarArray::getName() const {
    return this->varArrName;

}

int VarArray::getIndex(map<string,map<int,int> >& valmap) const {
    return this->index->getVal(valmap);
}

VarNum::VarNum(string varName, int content) : varName(varName), content(content){


}

VarNum::~VarNum() {


}

string VarNum::format() const {
    return this->varName;

}

int VarNum::getVal(map<string,map<int,int> >& valmap) const {
    
    if(valmap.count(varName) == 0) {
        valmap[varName][696969696] = 0;
        //if variable hasn't been called before, value = 0
    }

    return valmap[varName][696969696];
    //if not, just return the value already existent
}

string VarNum::getName() const {
    return this->varName;

}

int VarNum::getIndex(map<string,map<int,int> >& valmap) const {
    return 696969696;
    //hacky way I explained earlier of saving all variables
    //on same map
}