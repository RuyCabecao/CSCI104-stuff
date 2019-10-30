NumericExpression* Interpreter::ParseConst(string ss)  {
    size_t constFinder = ss.find_first_of("1234567890-");
    if (constFinder != string::npos) {

    }

}

NumericExpression* Interpreter::leftrightrec(string ss) {
    stack<char> parencheck;
    size_t pos = find_first_not_of(" \t");
    bool getout = true;
    char oper = ' ';
    
    if (ss[pos] == '-' || isdigit(ss[pos]) {
        return parseConst(ss);
    }

    else if (isalpha(ss[pos])) {
        return parseVar(ss);
    }

    pos++;

    while (getout) {
        if (ss[pos] == '(') parencheck.push(ss[pos]);
        else if (ss[pos] == ')') {
            if (parencheck.top() == '(') {
                parencheck.pop();
            }
        }

        else if ((ss[pos] == '*' || ss[pos] == '/' || ss[pos] == '+' 
        || ss[pos] == '-') && parencheck.empty()) {
            getout = false;
            oper = ss[pos];
        }
        pos++;
    }

    string lscopy = ss.substr(0, pos-1);
    string rscopy = ss.substr(pos, string::npos);

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

}