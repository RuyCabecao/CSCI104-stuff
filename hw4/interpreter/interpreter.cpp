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
    //map that will hold line numbers at proper indices 
    //i.e. if lines go in 10s, I can still count them by 1
    //in order to use them together with my command vector
    vector<Command*> commandvec; 
    //vector that will hold all commands
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
            //if case
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

            commandvec.push_back(ifcom);
            linemap[ifcom->getLine()]=linecount;
        }
        
        else if (ss[foundany] == 'L') {
            //just a LetVar
            int i = 0;
            ss.erase(foundany, 3); //erases command from string
            while (isblank(ss[i]) || isalpha(ss[i])) i++;
            if (ss[i] != '[') { //checks for presence of array
                
                LetVar* letvarc = LetParse(ss, linen);
                commandvec.push_back(letvarc);
                linemap[letvarc->getLine()]=linecount;

            }
            else {
                //LetVar with array  case
                LetVarArray* letvararray = LetParseArr(ss, linen);
                commandvec.push_back(letvararray);
                linemap[letvararray->getLine()]=linecount;

            }
        }

        else if (ss[foundany] == 'G' && ss[foundany+2] == 'T') {
            //goto command case

            ss.erase(foundany, 4); //erases command from string

            Goto* gotoc = GotoParse(linen, ss); 

            commandvec.push_back(gotoc);
            linemap[gotoc->getLine()]=linecount;
        }
         
        else if (ss[foundany] == 'G' && ss[foundany+2] == 'S') {
            //gosub command case

            ss.erase(foundany, 5); //erases command from string
            
            GoSub* gosub = GosubParse(linen, ss); 

            commandvec.push_back(gosub);
            linemap[gosub->getLine()]=linecount;

        }

        else if (ss[foundany] == 'P') {
            //print command case

            ss.erase(foundany, 5); //erases command from string
            NumericExpression* PNumEx = leftrightrec(ss);

            Print* print = new Print(PNumEx, linen);

            commandvec.push_back(print);
            linemap[print->getLine()]=linecount;



        }

        else if (ss[foundany] == 'R') {
            //return command case

            ss.erase(foundany, 6); //erases command from string

            ReturnC* cretrun = new ReturnC(linen);
            commandvec.push_back(cretrun);
            linemap[cretrun->getLine()]=linecount;
            
        }


        else if (ss[foundany] == 'E') {
            //end command case

            ss.erase(foundany, 3); //erases command from string

            EndC* cend = new EndC(linen);
            commandvec.push_back(cend);
            linemap[cend->getLine()]=linecount;

        }
        linecount++;
    }

    Interpret(commandvec, linemap); //does all the heavylifting
                                //(calculates, stores variables, etc.)
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


        while(!checkbrack.empty()) {     //if array is found, iterate
            if (name[j] == '[') {        //through its values to store it
                checkbrack.push(name[j]);//as numerical expression
            }
            else if(name[j] == ']') {          
                if (checkbrack.top() == '[') {
                    checkbrack.pop();//checks if entire index was copied
                }                    //allows for arrays within arrays
            }
            arrnumex << name[j];
            j++;
        }
        string ssintostrf = arrnumex.str();

        string ssintostr = ssintostrf.substr(1,j-tempj-1);
        //strings that hold index contents

        NumericExpression* varnumex = leftrightrec(ssintostr);
        //recursive call to parse variable numeric expressions

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
    stack<char> parencheck; //changed my implementation
    size_t pos = 0;         //to use stacks like I did with
    char oper = ' ';        //the variable array
    

    if (ss2[pos] == '-' || isdigit(ss2[pos])) {
        return parseConst(ss2);
    }   //checks for negatives and constants

    else if (isalpha(ss2[pos]) || ss2[pos+1] == '[') {
        return parseVar(ss2);
    }   //checks for variables or arrays

    pos++;
    while (true) {
        if (ss2[pos] == '(') parencheck.push(ss2[pos]);
        else if (ss2[pos] == ')') {
            if (parencheck.top() == '(') {
                parencheck.pop();
            }//matches opening with closing parens
        }

        else if (ss2[pos] == '[' || ss2[pos] == ']'){
            if (ss2[pos] == '[') brcount++;
            else if (ss2[pos] == ']') brcount--;
        }   //matches opening with closing brackets

        else if (isdigit(ss2[pos]) || isalpha(ss2[pos])) numcount++; 
        //ensures that at least a var or constant was read

        else if ((ss2[pos] == '*' || ss2[pos] == '/' || ss2[pos] == '+' 
        || ss2[pos] == '-') && parencheck.empty() && numcount != 0 && brcount == 0) {
            oper = ss2[pos];
            break;
        }//breaks loop when outermost operator is found

        pos++;
    }

    string lscopy = ss2.substr(1, pos-1);
    string rscopy = ss2.substr(pos+1, ss2.length()-pos-2);
    //copies left and right numericexpressions

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

    jlinecopy = jline.substr(linebeg, lineend - linebeg+1);//get jump line value

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
                            //but creates a Gosub command
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
    
    while (isdigit(ss[i])) {//finds beginning of jump line
        i--;                    
    }                           

    while (i < ss.length()) {//copies jump line
        linenumb.push_back(ss[i]);
        i++;
    }

    linenumb = cleanws(linenumb);
    ss.erase(ss.size()-linenumb.size(), linenumb.size());
    ss.erase(findthen, ss.size()-findthen);
    //cleans ws and erases THEN and jump line for easier parsing

    string leftarg = "";
    string rightarg = "";
    string rightside = "";
    
    char boolop = ss[findbool]; //stores operator

    leftarg = ss.substr(findfirst, findbool-findfirst);
    rightside = ss.substr(findbool+1, ss.length()-findbool-1 );
    //gets left and right operations


    NumericExpression* LeftEx = leftrightrec(leftarg);
    NumericExpression* RightEx = leftrightrec(rightside);
    //recursive call to parse Numeric Expressions

    if (boolop == '<') { //checks for evert bool operator
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
    //parses variables and values

    size_t findVar = ss.find(parsedvar->format());
    string novar = ss.substr(0, ss.size());

    novar.erase(findVar, (parsedvar->format()).length());
    //deletes variable part of command

    NumericExpression* letvarnumex = leftrightrec(novar);
    //parses value part of command

    LetVar* letvarc = new LetVar(parsedvar, letvarnumex, line);
    //creates letvar command

    return letvarc;

}

LetVarArray* Interpreter::LetParseArr(string ss, LineNum line) {
    int i = 0;
    int brackcount = 1;
    
    NumericExpression* parsedvar =  parseVar(ss);
    //parses variable part

    string novar = ss.substr(0, ss.size());

    while(novar[i] != '[')  i++;
    //finds opening bracket

    i++;

    while(brackcount > 0) {    //if array is found, iterate
        if (novar[i] == '[') { //through to store index as
            brackcount++;      //numerical expression
        }
        else if(novar[i] == ']') {          
            brackcount--;
        }
        i++;
    }
    novar.erase(0, i);
    //gets value part of Let

    NumericExpression* letvarnumex = leftrightrec(novar);

    LetVarArray* letvararr = new LetVarArray(parsedvar, letvarnumex, line);

    return letvararr;

}

void Interpreter::Interpret(vector<Command*> commandvec, map<int,int> linemap) {
    map<string,map<int,int> > valmap;
    //map from var or vararray name to map that holds indices and values
    stack<int> returnstack;
    //stack that holds return lines
    unsigned int i = 0;
    //iterator
    bool ended = false;
    //bool used to break from while

    try{ //exception handling for divide by 0 error
        while(i<commandvec.size()) {
            //if and else ifs that handle interpreting 
            //every command's values through recursive calls
            //and errors
            if (commandvec[i]->getCommName() == "PRINT") {
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
                    i = linemap[i]; //gets line from line map to
                    continue;       //allow for different count of
                                    //line values
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
                    i = linemap[i];//same as GOTO
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
                    i = linemap[i];//same as GOTO
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
                    //gets jline from jline stack and cross checks
                    //it with line map
                }
            }

            else if (commandvec[i]->getCommName() == "END") {
                break;
            }

            i++;

        }
    }
    catch (string &except) { //catches divide by 0 error
        cout << "Error in line " << commandvec[i]->getLine() <<": ";
        cout << except;
    }
    
    int j = 0;
    ended = false;
    while(!ended) { //deletes all pointers and empties command vector
        Command* tempc = commandvec.back();
        commandvec.pop_back();
        delete tempc;
        if (commandvec.empty()) ended = true;
        j++;
    }
}

