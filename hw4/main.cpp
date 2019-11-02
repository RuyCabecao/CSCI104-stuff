#include <iostream>
#include <fstream>
#include "interpreter/interpreter.h"
#include "interpreter/command.h"
#include "interpreter/boolean.h"
#include "interpreter/arithmetic.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide an input file." << endl;
        return 1;
    }

    ifstream input(argv[1]);

    if (input.fail()) {
        cerr << "File " << argv[1] << " cannot be opened." << endl;
        return 1;
    }

    Interpreter interpreter(input);

    return 0;
}
