#include "interpreter.h"

using namespace std;

Interpreter::Interpreter(std::istream& in) {
    this->parse(in);

}

Interpreter::~Interpreter() {

}

void Interpreter::parse(std::istream& in) {
    string line;
    while (getline(in, line)) {
        size_t line_number;
        stringstream stream(line);
        stream >> line_number;
        LineNum linen(line_number);
        string ss = stream.str();
        
        size_t linenumend = ss.find_first_not_of("1234567890");
        ss.erase(0,linenumend);


        string copystr = "";

        size_t foundany = ss.find_first_not_of(" \t");

        if (ss[foundany] == 'I') {
            //if
            cout << "inside if" << endl;
            ss.erase(foundany, 2);

        }
        
        else if (ss[foundany] == 'L') {
            size_t findarray = ss.find_first_of("[");
            ss.erase(foundany, 3);
            if (findarray == string::npos) {
                cout << "no array" << endl;
                
            }
            else {
                //array
                cout << "array" << endl;

            }
        }

        else if (ss[foundany] == 'G' && ss[foundany+2] == 'T') {
            //goto
            //cout << "inside goto" << endl;
            ss.erase(foundany, 4);

            Goto* gotoc = GotoParse(linen, ss); 
            
            cout << gotoc->format() << endl;
            delete gotoc;
        }
         
        else if (ss[foundany] == 'G' && ss[foundany+2] == 'S') {
            //gosub
            //cout << "inside gosub" << endl;
            ss.erase(foundany, 5);
            
            GoSub* gosub = GosubParse(linen, ss); 
            
            cout << gosub->format() << endl;
            delete gosub;
        }

        else if (ss[foundany] == 'P') {
            //print
            //cout << "inside print" << endl;
            ss.erase(foundany, 5);

            NumericExpression* PNumEx = leftrightrec(ss);
            //cout << "Left: " << PNumEx->format() << endl;

            Print* print = new Print(PNumEx, linen);
            cout << print->format() << endl;
            delete print;


        }

        else if (ss[foundany] == 'R') {
            //return
            //cout << "inside return" << endl;
            ss.erase(foundany, 6);

            ReturnC* cretrun = new ReturnC(linen);
            cout << cretrun->format() << endl;
            delete cretrun;
            
        }


        else if (ss[foundany] == 'E') {
            //end
            //cout << "inside end" << endl;
            ss.erase(foundany, 3);

            EndC* cend = new EndC(linen);
            cout << cend->format() << endl;
            delete cend;
        }
        

    }
}

void Interpreter::write(std::ostream& out) {

}

VarNum Interpreter::parseVar(string name) {
    VarNum variable = VarNum(name, 0);
    return variable;

}


Constant Interpreter::parseConst(string value) {
    Constant constantVal = Constant(value);
    return constantVal;

}

string Interpreter::cleanws(string dirtystring) {
    for (unsigned int i = 0; i < dirtystring.length(); i++) {
        if (dirtystring[i] == '\t' || dirtystring[i] == ' ') {
            dirtystring.erase(i,1);
            i--;
        }
    }

    return dirtystring;
}


    
NumericExpression* Interpreter::leftrightrec(string ss) {
    char oper = ' ';
    
    size_t isnum = ss.find_first_not_of("1234567890");
    //cout << ss << endl;
    if (isnum == string::npos) {
        Constant* digit = new Constant(ss);
        return digit;
    }
    
    size_t isvar = ss.find_first_of("1234567890+/-*");
    if (isvar == string::npos) {
        VarNum* variable = new VarNum(ss, 0);
        return variable;
    }

    size_t isvararrayo = ss.find_first_of("[");
    if (isvararrayo != string::npos) {}
    
    size_t foundopar = ss.find_first_of("(");
    size_t foundcpar = ss.find_last_of(")");
    size_t foundop = ss.find_first_of("+-*/");
    if (foundopar == string::npos || 
        foundcpar == string::npos || 
        foundop == string::npos) {
        cerr << "invalid" << endl; 
    }


    string copystr = ss.substr(foundopar+1, foundcpar-foundopar-1);
            
    copystr = cleanws(copystr);
            
    //cout <<" a" << copystr << endl;
    
    int oparcount = 0;
    int cparcount = 0;
    int validop = 0;
    int iter = 0;

    while (validop == 0) {
        if (copystr[iter] == '(') oparcount++;

        else if (copystr[iter] == ')') cparcount++;
    
        if (oparcount == cparcount) {
            validop = 1;
            break;
        }
        iter++;
    }
    //cout << "opar "<< oparcount << endl;
    //cout << "cpar " <<cparcount << endl; 
    //cout << "iter "<< iter << endl;
    //cout << " past while " << endl;
    string copystropl = "";
    string copystropr = "";
    size_t opfind =  copystr.find_first_of("+-*/");
    
    if (oparcount == 0 && opfind != string::npos) {
        oper = copystr[opfind];
        copystropl = copystr.substr(0, opfind);
        copystropr = copystr.substr(opfind+1, copystr.length()-opfind-1);

    }

    else if (oparcount != 0) {
        oper = copystr[iter+1];
        copystropl = copystr.substr(0, iter+1);
        copystropr = copystr.substr(iter+2, copystr.length()-iter-1);
    }
    
    NumericExpression* LeftEx = leftrightrec(copystropl);
    NumericExpression* RightEx = leftrightrec(copystropr);

    if (oper == '+') {
        Addition* add = new Addition(LeftEx, RightEx);
        return add;
    }

    else if (oper == '-') {
        Subtraction* subt = new Subtraction(LeftEx, RightEx);
        return subt;
    }

    else if (oper == '/') {
        Division* div = new Division(LeftEx, RightEx);
        return div;
    }

    else if (oper == '*') {
        Multiplication* mult = new Multiplication(LeftEx, RightEx);
        return mult;
    }
    return 0;
}


Goto* Interpreter::GotoParse(LineNum line, string jline) {
    string jlinecopy = "";
    int jlineint = 0;

    
    size_t linebeg = jline.find_first_of("1234567890");
    size_t lineend = jline.find_last_of("1234567890");

    jlinecopy = jline.substr(linebeg, lineend - linebeg+1);

    stringstream lineconv(jlinecopy);
    lineconv >> jlineint;

    LineNum jlineclass(jlineint);

    Goto* gotoc = new Goto(line, jlineclass);
    return gotoc;

}

GoSub* Interpreter::GosubParse(LineNum line, string jline) {
    string jlinecopy = "";
    int jlineint = 0;

    
    size_t linebeg = jline.find_first_of("1234567890");
    size_t lineend = jline.find_last_of("1234567890");

    jlinecopy = jline.substr(linebeg, lineend - linebeg+1);

    stringstream lineconv(jlinecopy);
    lineconv >> jlineint;

    LineNum jlineclass(jlineint);

    GoSub* gosubC = new GoSub(line, jlineclass);
    return gosubC;

}