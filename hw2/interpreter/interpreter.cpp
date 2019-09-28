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
            ss.erase(foundany, 2);
            
            string linenumb = "";
            size_t findjline = ss.find_last_of("1234567890");
            unsigned int i = findjline;

            while (isdigit(ss[i])) {
                i--;
            }

            while (i < ss.length()) {
                linenumb.push_back(ss[i]);
                i++;
            }
            
            
            linenumb = cleanws(linenumb);
            

            stringstream conv(linenumb);
            int lineval = 0;
            conv >> lineval;
            LineNum linej(lineval);

            BooleanExpression* ifbool = IfParse(linen, ss);
            IfC* ifcom = new IfC(ifbool, linej, linen);
            cout << ifcom->format() << endl;

            delete ifcom;

        }
        
        else if (ss[foundany] == 'L') {
            size_t findarray = ss.find_first_of("[");
            ss.erase(foundany, 3);
            if (findarray == string::npos) {
                
                LetVar* letvarc = LetParse(ss, linen);
                cout << letvarc->format() << endl;

                delete letvarc;
            }
            else {
                //array
                cout << "array" << endl;

                LetVarArray* letvararray = LetParseArr(ss, linen);
                cout << letvararray->format() << endl;

                delete letvararray;

            }
        }

        else if (ss[foundany] == 'G' && ss[foundany+2] == 'T') {
            //goto

            ss.erase(foundany, 4);

            Goto* gotoc = GotoParse(linen, ss); 
            
            cout << gotoc->format() << endl;
            delete gotoc;
        }
         
        else if (ss[foundany] == 'G' && ss[foundany+2] == 'S') {
            //gosub

            ss.erase(foundany, 5);
            
            GoSub* gosub = GosubParse(linen, ss); 
            
            cout << gosub->format() << endl;
            delete gosub;
        }

        else if (ss[foundany] == 'P') {
            //print

            ss.erase(foundany, 5);

            NumericExpression* PNumEx = leftrightrec(ss);

            Print* print = new Print(PNumEx, linen);
            cout << print->format() << endl;
            delete print;


        }

        else if (ss[foundany] == 'R') {
            //return

            ss.erase(foundany, 6);

            ReturnC* cretrun = new ReturnC(linen);
            cout << cretrun->format() << endl;
            delete cretrun;
            
        }


        else if (ss[foundany] == 'E') {
            //end

            ss.erase(foundany, 3);

            EndC* cend = new EndC(linen);
            cout << cend->format() << endl;
            delete cend;
        }
        

    }
}

void Interpreter::write(std::ostream& out) {

}

NumericExpression* Interpreter::parseVar(string name) {
    unsigned int j = 0;
    string varfind = "";
    stringstream buffer ("");
    stringstream arrnumex("");

    size_t isvar = name.find_first_not_of("1234567890+/-* \t()[]");
    size_t isarr = name.find_first_of("[");
    size_t lastbr = name.find_last_of("]");
    unsigned int variter = isvar;

    while (isalpha(name[variter])) {
        buffer << name[variter];
        variter++;
    }

    if (isarr == string::npos) {
        VarNum* variable = new VarNum(buffer.str(), 0);
        return variable;
    }
    else if (name[isarr] == '[') {
        j = isarr+1;
        while(j != lastbr) {
            arrnumex << name[j];
            j++;
        }

        NumericExpression* varnumex = leftrightrec(arrnumex.str());

        VarArray* vararray = new VarArray(varnumex, buffer.str());
        return vararray;

    }
    return 0;
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
    if (isnum == string::npos) {
        Constant* digit = new Constant(ss);
        return digit;
    }
    
    size_t isvar = ss.find_first_of("1234567890+/-*");
    if (isvar == string::npos) {
        VarNum* variable = new VarNum(ss, 0);
        return variable;
    }
    
    size_t foundopar = ss.find_first_of("(");
    size_t foundcpar = ss.find_last_of(")");


    string copystr = ss.substr(foundopar+1, foundcpar-foundopar-1);
            
    copystr = cleanws(copystr);
            
    
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
    size_t isvararr = ss.find_first_of("[");
    if (isvararr != string::npos) {
        NumericExpression* variarr = parseVar(ss);
        return variarr;
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

BooleanExpression* Interpreter::IfParse(LineNum line, string ss) {
    size_t findfirst = ss.find_first_not_of(" \t");
    size_t findbool = ss.find_first_of("<>=");
    size_t findjline = ss.find_last_of("1234567890");
    size_t findthen = ss.find("THEN");
    string linenumb = "";
    unsigned int i = findjline;

    while (isdigit(ss[i])) {
        i--;
    }

    while (i < ss.length()) {
        linenumb.push_back(ss[i]);
        i++;
    }

    linenumb = cleanws(linenumb);
    ss.erase(ss.size()-linenumb.size(), linenumb.size());
    ss.erase(findthen, ss.size()-findthen);


    string leftarg = "";
    string rightarg = "";
    string rightside = "";
    
    char boolop = ss[findbool];

    leftarg = ss.substr(findfirst, findbool-findfirst);
    rightside = ss.substr(findbool+1, ss.length()-findbool-1 );

    
    
    leftarg = cleanws(leftarg);
    rightside = cleanws(rightside);



    NumericExpression* LeftEx = leftrightrec(leftarg);
    NumericExpression* RightEx = leftrightrec(rightside);

    if (boolop == '<') {
        Less* less = new Less(LeftEx, RightEx);
        return less;
    }

    else if (boolop == '=') {
        Equals* equal = new Equals(LeftEx, RightEx);
        return equal;
    }

    else if (boolop == '>') {
        Greater* greater = new Greater(LeftEx, RightEx);
        return greater;
    }

    return 0;
    //return NULL;

}

LetVar* Interpreter::LetParse(string ss, LineNum line) {
 
    NumericExpression* parsedvar =  parseVar(ss);

    size_t findVar = ss.find(parsedvar->format());
    string novar = ss.substr(0, ss.size());

    novar.erase(findVar, (parsedvar->format()).length());

    novar = cleanws(novar);

    NumericExpression* letvarnumex = leftrightrec(novar);


    LetVar* letvarc = new LetVar(parsedvar, letvarnumex, line);


    return letvarc;

}

LetVarArray* Interpreter::LetParseArr(string ss, LineNum line) {
    
    NumericExpression* parsedvar =  parseVar(ss);
    cout << ss<<  endl;
    size_t findArrayEnd = ss.find_last_of("]");
    string novar = ss.substr(0, ss.size());

    cout << findArrayEnd << endl;
    cout  << parsedvar->format() << endl;

    novar.erase(findArrayEnd, ss.length());

    novar = cleanws(novar);

    NumericExpression* letvarnumex = leftrightrec(novar);


    LetVarArray* letvararr = new LetVarArray(parsedvar, letvarnumex, line);


    return letvararr;

}
