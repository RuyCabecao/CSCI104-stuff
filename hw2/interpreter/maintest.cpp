#include <iostream>
#include <fstream>
#include "interpreter.h"
#include "arithmetic.h"
#include "boolean.h"
#include "command.h"


using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide an input file." << endl;
        return 1;
    }

    ifstream input(argv[1]);
    Interpreter interpreter(input);
    //interpreter.write(cout);
    return 0;
}
