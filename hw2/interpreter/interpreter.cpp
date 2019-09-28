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
        LineNum linen(line_number); //store line number as
        string ss = stream.str();   //line number class
        
        size_t linenumend = ss.find_first_not_of("1234567890");
        ss.erase(0,linenumend); //delete line number
                                //so it won't get in the way

        string copystr = "";    //initialize copy string

        size_t foundany = ss.find_first_not_of(" \t"); //take in first
                                                       //non-ws value
        if (ss[foundany] == 'I') {                     //to read commands
            //if cas
            ss.erase(foundany, 2); //erases command from string
            
            string linenumb = "";
            size_t findjline = ss.find_last_of("1234567890");
            unsigned int i = findjline;

            while (isdigit(ss[i])) {
                i--;
            }

            while (i < ss.length()) {       //read the last letters
                linenumb.push_back(ss[i]);  //to store jLine value
                i++;                        //to pass to IF command
            }
            
            linenumb = cleanws(linenumb);   //removes whitespace
            

            stringstream conv(linenumb);   
            int lineval = 0;                //converts line int
            conv >> lineval;                //to string to pass
            LineNum linej(lineval);

            BooleanExpression* ifbool = IfParse(linen, ss);
            IfC* ifcom = new IfC(ifbool, linej, linen);
            cout << ifcom->format() << endl; 

            delete ifcom;
        }
        
        else if (ss[foundany] == 'L') {
            //just a LetVar
            size_t findarray = ss.find_first_of("[");
            ss.erase(foundany, 3); //erases command from string
            if (findarray == string::npos) { //checks for presence of array
                
                LetVar* letvarc = LetParse(ss, linen);
                cout << letvarc->format() << endl;

                delete letvarc;
            }
            else {
                //LetVar with array  case

                LetVarArray* letvararray = LetParseArr(ss, linen);
                cout << letvararray->format() << endl;

                delete letvararray;

            }
        }

        else if (ss[foundany] == 'G' && ss[foundany+2] == 'T') {
            //goto command case

            ss.erase(foundany, 4); //erases command from string

            Goto* gotoc = GotoParse(linen, ss); 
            
            cout << gotoc->format() << endl;
            delete gotoc;
        }
         
        else if (ss[foundany] == 'G' && ss[foundany+2] == 'S') {
            //gosub command case

            ss.erase(foundany, 5); //erases command from string
            
            GoSub* gosub = GosubParse(linen, ss); 
            
            cout << gosub->format() << endl;
            delete gosub;
        }

        else if (ss[foundany] == 'P') {
            //print command case

            ss.erase(foundany, 5); //erases command from string

            NumericExpression* PNumEx = leftrightrec(ss);

            Print* print = new Print(PNumEx, linen);
            cout << print->format() << endl;
            delete print;


        }

        else if (ss[foundany] == 'R') {
            //return command case

            ss.erase(foundany, 6); //erases command from string

            ReturnC* cretrun = new ReturnC(linen);
            cout << cretrun->format() << endl;
            delete cretrun;
            
        }


        else if (ss[foundany] == 'E') {
            //end command case

            ss.erase(foundany, 3); //erases command from string

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
    size_t isarr = name.find_first_of("[");// these find the start of the variable
    size_t lastbr = name.find_last_of("]");// and the  array if they exist
    unsigned int variter = isvar; //unsigned int iterator     

    while (isalpha(name[variter])) {
        buffer << name[variter];
        variter++;//copies variable name
    }

    if (isarr == string::npos) {
        VarNum* variable = new VarNum(buffer.str(), 0);
        return variable;
    }
    else if (name[isarr] == '[') {
        j = isarr+1;
        while(j != lastbr) {    //if array is found, iterate
            arrnumex << name[j];//through its values to store it
            j++;                // as numerical expression
        }

        NumericExpression* varnumex = leftrightrec(arrnumex.str());

        VarArray* vararray = new VarArray(varnumex, buffer.str());
        return vararray;

    }
    return 0;
}

Constant Interpreter::parseConst(string value) {
    Constant constantVal = Constant(value);
    return constantVal; //parses constant

}

string Interpreter::cleanws(string dirtystring) {
    for (unsigned int i = 0; i < dirtystring.length(); i++) {
        if (dirtystring[i] == '\t' || dirtystring[i] == ' ') {
            dirtystring.erase(i,1); //removes all whitespace from
            i--;                    //input string to make 
        }                           //processing easier
    }

    return dirtystring;
}
    
NumericExpression* Interpreter::leftrightrec(string ss) {
    char oper = ' ';
    
    size_t isnum = ss.find_first_not_of("1234567890");
    if (isnum == string::npos) {
        Constant* digit = new Constant(ss); //checks if only constant 
        return digit;                       //is left
    }
    
    size_t isvar = ss.find_first_of("1234567890+/-*");
    if (isvar == string::npos) {      
        VarNum* variable = new VarNum(ss, 0);
        return variable;                   //checks if only variable
    }                                      //left
    
    size_t foundopar = ss.find_first_of("(");
    size_t foundcpar = ss.find_last_of(")"); //finds  parentheses


    string copystr = ss.substr(foundopar+1, foundcpar-foundopar-1);
            
    copystr = cleanws(copystr);  //creates string of inside of paren
            
    
    int oparcount = 0;
    int cparcount = 0;
    int validop = 0;
    int iter = 0;

    while (validop == 0) { //finds outermost operator
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
    size_t opfind =  copystr.find_first_of("+-*/"); //find outermost operator index
    
    if (oparcount == 0 && opfind != string::npos) {
        oper = copystr[opfind];                     //case that no parentheses are found
        copystropl = copystr.substr(0, opfind);     //and no operator found
        copystropr = copystr.substr(opfind+1, copystr.length()-opfind-1);

    }

    else if (oparcount != 0) {                      //case that there are parentheses
        oper = copystr[iter+1];
        copystropl = copystr.substr(0, iter+1);
        copystropr = copystr.substr(iter+2, copystr.length()-iter-1);
    }
    
    size_t isvararr = ss.find_first_of("[");        //sees if array and creates 
    if (isvararr != string::npos) {                 //NExp accordingly
        NumericExpression* variarr = parseVar(ss);
        return variarr;
    }

    NumericExpression* LeftEx = leftrightrec(copystropl); //recursive call of left
    NumericExpression* RightEx = leftrightrec(copystropr);//and right

    if (oper == '+') {  //if elses  to check for all possible operators
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

    
    size_t linebeg = jline.find_first_of("1234567890");//find indices
    size_t lineend = jline.find_last_of("1234567890");//of line value

    jlinecopy = jline.substr(linebeg, lineend - linebeg+1);//get just line value

    stringstream lineconv(jlinecopy);
    lineconv >> jlineint;

    LineNum jlineclass(jlineint);

    Goto* gotoc = new Goto(line, jlineclass);
    return gotoc;

}

GoSub* Interpreter::GosubParse(LineNum line, string jline) {
    string jlinecopy = "";
    int jlineint = 0;
                                //this function deos the same as goto
                                //just with different format
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
                                //running out of timefor comments
    while (isdigit(ss[i])) {    //this function parses the bools
        i--;                    // and makes recursive calls to
    }                           // NExps

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

    size_t findArrayEnd = ss.find_last_of("]");
    string novar = ss.substr(0, ss.size());

    novar.erase(0, findArrayEnd+1);

    novar = cleanws(novar);

    NumericExpression* letvarnumex = leftrightrec(novar);


    LetVarArray* letvararr = new LetVarArray(parsedvar, letvarnumex, line);

    return letvararr;

}
