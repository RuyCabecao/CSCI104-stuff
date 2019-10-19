#include <stack>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "No input file" << std::endl;
        return 0;
    }

    std::ifstream ifile(argv[1]);
    std::string line;
    std::stack<char> stick;
    
    while (getline(ifile, line)) { //loops until end of file
        if(line == "" || line == " ") continue;
        if (stick.empty() && (line[0] == 'N' || line[0] == 'S' || 
        line[0] == 'W' || line[0] == 'E')) {
            stick.push(line[0]); //add directions to stack
                                 //if there's nothing there
        }
        else if (line[0] == 'N') {
            if (stick.top() == 'S')
                stick.pop(); //if South right after North,
            else             //they cancel out
                stick.push(line[0]);
                //else, add direction to stack
        }
        else if (line[0] == 'E') {
            if (stick.top() == 'W')
                stick.pop(); //if West right after East,
            else             //they cancel out
                stick.push(line[0]);
                //else, add direction to stack
        }
        else if (line[0] == 'S') {
            if (stick.top() == 'N')
                stick.pop(); //if North right after South,
            else             //they cancel out
                stick.push(line[0]);
                //else, add direction to stack
        }
        else if (line[0] == 'W') {
            if (stick.top() == 'E')
                stick.pop(); //if East right after West,
            else             //they cancel out
                stick.push(line[0]);
                //else, add direction to stack
        }
    }
    if (stick.empty())
        std::cout << "0" << std::endl;
    else 
        std::cout << stick.size() << std::endl;
    return 0;
}
