#include "interpreter.h"

using namespace std;

Interpreter::Interpreter(std::istream& in) {
    this->parse(in);

}

Interpreter::~Interpreter() {

}

void Interpreter::parse(std::istream& in) {
    int linecount = 0;
    string line;
    map<int,int> linemap;
    vector<Command*> commandvec;
    //stack<LineNum> Jlinestack;
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
            //cout << ifcom->format() << endl; 

            commandvec.push_back(ifcom);
            linemap[ifcom->getLine()]=linecount;

            //delete ifcom;
        }
        
        else if (ss[foundany] == 'L') {
            //just a LetVar
            int i = 0;
            ss.erase(foundany, 3); //erases command from string
            while (isblank(ss[i]) || isalpha(ss[i])) i++;
            if (ss[i] != '[') { //checks for presence of array
                
                LetVar* letvarc = LetParse(ss, linen);
                //cout << letvarc->format() << endl;
                commandvec.push_back(letvarc);
                linemap[letvarc->getLine()]=linecount;

                //delete letvarc;
            }
            else {
                //LetVar with array  case
                LetVarArray* letvararray = LetParseArr(ss, linen);
                //cout << letvararray->format() << endl;
                commandvec.push_back(letvararray);
                linemap[letvararray->getLine()]=linecount;

                //delete letvararray;

            }
        }

        else if (ss[foundany] == 'G' && ss[foundany+2] == 'T') {
            //goto command case

            ss.erase(foundany, 4); //erases command from string

            Goto* gotoc = GotoParse(linen, ss); 
            //cout << gotoc->format() << endl;
            commandvec.push_back(gotoc);
            linemap[gotoc->getLine()]=linecount;
            //Jlinestack.push(gotoc->getJline());

            //delete gotoc;
        }
         
        else if (ss[foundany] == 'G' && ss[foundany+2] == 'S') {
            //gosub command case

            ss.erase(foundany, 5); //erases command from string
            
            GoSub* gosub = GosubParse(linen, ss); 
            //cout << gosub->format() << endl;
            commandvec.push_back(gosub);
            linemap[gosub->getLine()]=linecount;
            //Jlinestack.push(gosub->getJline());

            //delete gosub;
        }

        else if (ss[foundany] == 'P') {
            //print command case

            ss.erase(foundany, 5); //erases command from string
            NumericExpression* PNumEx = leftrightrec(ss);

            Print* print = new Print(PNumEx, linen);
            //cout << print->format() << endl;
            commandvec.push_back(print);
            linemap[print->getLine()]=linecount;

            //delete print;


        }

        else if (ss[foundany] == 'R') {
            //return command case

            ss.erase(foundany, 6); //erases command from string

            ReturnC* cretrun = new ReturnC(linen);
            //cout << cretrun->format() << endl;
            commandvec.push_back(cretrun);
            linemap[cretrun->getLine()]=linecount;

            //delete cretrun;
            
        }


        else if (ss[foundany] == 'E') {
            //end command case

            ss.erase(foundany, 3); //erases command from string

            EndC* cend = new EndC(linen);
            //cout << cend->format() << endl;
            commandvec.push_back(cend);
            linemap[cend->getLine()]=linecount;

            //delete cend;
        }
        linecount++;
    }

    Interpret(commandvec, linemap);
    
}

void Interpreter::write(std::ostream& out) {

}

NumericExpression* Interpreter::parseVar(string name) {
    unsigned int j = 0;
    string varfind = "";
    stringstream buffer ("");
    stringstream arrnumex("");

    size_t isvar = name.find_first_not_of("1234567890+/-* \t()[]");
    size_t lastbr = name.find_last_of("]");
    unsigned int variter = isvar; //unsigned int iterator     

    while (isalpha(name[variter])) {
        buffer << name[variter];
        variter++;//copies variable name
    }

    while (isblank(name[variter])) variter++;

    if (name[variter] != '[' || lastbr == string::npos) {
        VarNum* variable = new VarNum(buffer.str(), 0);

        return variable;
    }

    else if (name[variter] == '[') {
        j = variter+1;
        int tempj = j;
        stack<char> checkbrack;
        checkbrack.push(name[variter]);
        arrnumex << name[variter];


        while(!checkbrack.empty()) {    //if array is found, iterate
            if (name[j] == '[') {
                checkbrack.push(name[j]);
            }//through its values to store it
            else if(name[j] == ']') {          // as numerical expression
                if (checkbrack.top() == '[') {
                    checkbrack.pop();
                }
            }
            arrnumex << name[j];
            j++;
        }
         string ssintostrf = arrnumex.str();

        string ssintostr = ssintostrf.substr(1,j-tempj-1);


        NumericExpression* varnumex = leftrightrec(ssintostr);
        VarArray* vararray = new VarArray(varnumex, buffer.str());
        return vararray;

    }
    return 0;
}

NumericExpression* Interpreter::parseConst(string value) {
    Constant* constantVal = new Constant(value);
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
    int brcount = 0;
    int numcount = 0;
    string ss2 = cleanws(ss);
    stack<char> parencheck; //changed my implementation to use stacks
    size_t pos = 0;
    char oper = ' ';
    

    if (ss2[pos] == '-' || isdigit(ss2[pos])) {
        return parseConst(ss2);
    }

    else if (isalpha(ss2[pos]) || ss2[pos+1] == '[') {
        return parseVar(ss2);
    }

    pos++;
    while (true) {
        if (ss2[pos] == '(') parencheck.push(ss2[pos]);
        else if (ss2[pos] == ')') {
            if (parencheck.top() == '(') {
                parencheck.pop();
            }
        }

        else if (ss2[pos] == '[' || ss2[pos] == ']'){
            if (ss2[pos] == '[') brcount++;
            else if (ss2[pos] == ']') brcount--;
        }

        else if (isdigit(ss2[pos]) || isalpha(ss2[pos])) numcount++;  

        else if ((ss2[pos] == '*' || ss2[pos] == '/' || ss2[pos] == '+' 
        || ss2[pos] == '-') && parencheck.empty() && numcount != 0 && brcount == 0) {
            oper = ss2[pos];
            break;
        }
        pos++;
    }

    string lscopy = ss2.substr(1, pos-1);
    string rscopy = ss2.substr(pos+1, ss2.length()-pos-2);

    NumericExpression* LeftEx = leftrightrec(lscopy); //recursive call of left
    NumericExpression* RightEx = leftrightrec(rscopy);//and right


    if (oper == '+') {  //if elses  to check for all possible operators
        Addition* add = new Addition(LeftEx, RightEx);
        return add;
    }

    else if (oper == '-') {
        Subtraction* subt = new Subtraction(LeftEx, RightEx);
        return subt;
    }

    else if (oper == '/') {
        Division* divs = new Division(LeftEx, RightEx);
        return divs;
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

    size_t linebeg = jline.find_first_of("1234567890-");//find indices
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
                                //this function does the same as goto
                                //just with different format
    size_t linebeg = jline.find_first_of("1234567890-");
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

    //novar = cleanws(novar);

    NumericExpression* letvarnumex = leftrightrec(novar);


    LetVar* letvarc = new LetVar(parsedvar, letvarnumex, line);


    return letvarc;

}

LetVarArray* Interpreter::LetParseArr(string ss, LineNum line) {
    int i = 0;
    
    NumericExpression* parsedvar =  parseVar(ss);

    string novar = ss.substr(0, ss.size());


    while(novar[i] != '[')  i++;

    int brackcount = 1;
    i++;

    while(brackcount > 0) {    //if array is found, iterate
        if (novar[i] == '[') {
            brackcount++;
        }//through its values to store it
        else if(novar[i] == ']') {          // as numerical expression
            brackcount--;
        }
        i++;
    }
    novar.erase(0, i);

    NumericExpression* letvarnumex = leftrightrec(novar);

    LetVarArray* letvararr = new LetVarArray(parsedvar, letvarnumex, line);

    return letvararr;

}

void Interpreter::Interpret(vector<Command*> commandvec, map<int,int> linemap) {
    bool ended = false;
    map<string,map<int,int> > valmap;
    stack<int> returnstack;
    unsigned int i = 0;


    while(i<commandvec.size()) {
        //cout << "iteration " << i << endl;

        if (commandvec[i]->getCommName() == "PRINT") {
            if (commandvec[i]->getVal(valmap) == 2147483646) break;
            cout << commandvec[i]->getVal(valmap) << endl;
        }

        else if (commandvec[i]->getCommName() == "LET") {
            commandvec[i]->getVal(valmap);
        }

        else if (commandvec[i]->getCommName() == "LETARR") {
            commandvec[i]->getVal(valmap);  
        }

        else if (commandvec[i]->getCommName() == "GOTO") {
            if (linemap.count(commandvec[i]->getJline()) > 0) {
                i = commandvec[i]->getJline();
                i = linemap[i];
                continue;
            }
            else {
                cout << "Error in line " << commandvec[i]->getLine() <<": ";
                cout << "GOTO to non-existent line ";
                cout << commandvec[i]->getJline() << "." << endl;
                break;
            }
        }

        else if (commandvec[i]->getCommName() == "IF") {
            
            if (commandvec[i]->getVal(valmap) == 0) {
                i++;
                continue;
            }

            else if (linemap.count(commandvec[i]->getJline()) == 0){
                cout << "Error in line " << commandvec[i]->getLine() <<": ";
                cout << "IF jump to non-existent line ";
                cout << commandvec[i]->getJline() << "." << endl;
                break;
            }
            
            else if (commandvec[i]->getVal(valmap) == 1 && 
            linemap.count(commandvec[i]->getJline()) > 0) {
                i = commandvec[i]->getJline();
                i = linemap[i];
                continue;
            }

        }

        else if (commandvec[i]->getCommName() == "GOSUB") {
            if (linemap.count(commandvec[i]->getJline()) == 0){
                cout << "Error in line " << commandvec[i]->getLine() <<": ";
                cout << "GOSUB to non-existent line ";
                cout << commandvec[i]->getJline() << "." << endl;
                break;
            }

            else {
                returnstack.push(commandvec[i]->getLine());
                i = commandvec[i]->getJline();
                i = linemap[i];
                continue;
            }



        }

        else if (commandvec[i]->getCommName() == "RETURN") {
            if (returnstack.empty()) {
                cout << "Error in line " << commandvec[i]->getLine() <<": ";
                cout << "No matching GOSUB for RETURN." << endl;
                break;
            }
            else {
                i = returnstack.top();
                returnstack.pop();
                i = linemap[i];
            }
        }

        else if (commandvec[i]->getCommName() == "END") {
            break;
        }

        i++;

    }

    
    
    int j = 0;
    ended = false;
    while(!ended) {
        Command* tempc = commandvec.back();
        commandvec.pop_back();
        delete tempc;
        if (commandvec.empty()) ended = true;
        j++;
    }
}

