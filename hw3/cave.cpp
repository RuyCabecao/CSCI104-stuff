#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "No input file" << std::endl;
        return 0;
    }

    std::ifstream ifile(argv[1]);
    std::stringstream ss;
    std::string line;


	std::map<char, int> caveMap;
    caveMap['N'] = 0; //I implemented maps for this code,
    caveMap['E'] = 0; //mapping every letter to an int
    caveMap['S'] = 0; //representing how many times each
    caveMap['W'] = 0; //direction occurs
    
    getline(ifile, line); //reads first line
    while (!ifile.fail()) { //loops until end of file

        if (line[0] == 'N') {
            caveMap['N']++; //increments North map
        }
        
        else if (line[0] == 'E') {
            caveMap['E']++; //increments East map
        }

        else if (line[0] == 'S') {
            caveMap['S']++; //increments South map
        }

        else if (line[0] == 'W') {
            caveMap['W']++; //increments West map
        }
	
        getline(ifile, line); //takes in next line
    }
    
    int nsval = 0; //int that hold vertical distance
    int ewval = 0; //int that hold horizontal distance
    int dist = 0;  //int that holds total distance

    //following lines calculate horiz and vert distances.
    
    //Since North and South cancel each other out and
    //East and West cancel each other out, the total
    //distance can be found my finding the difference
    //between inputs.

    if (caveMap['N'] > caveMap['S']) {
        nsval = caveMap['N'] - caveMap['S'];
    }
    else if (caveMap['N'] < caveMap['S']) {
        nsval = caveMap['S'] - caveMap['N'];
    } 
    if (caveMap['E'] > caveMap['W']) {
        ewval = caveMap['E'] - caveMap['W'];
    }
    else if (caveMap['W'] < caveMap['E']) {
        ewval = caveMap['W'] - caveMap['E'];
    }

    dist = nsval + ewval; //calculates total
        
    std::cout << dist << std::endl; //prints total

    return 0;
}
