#include "heap/heap.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>

using namespace std;

int geth(string startword, string endword) {
    int hval = 0;
    for (unsigned int i = 0; i < startword.length(); i++) {
        if (startword[i] != endword[i]) {
            hval++; //gets heuristic value by counting how many letters
        }           //have to change to become the target word
    }
    return hval;
}

void makeUpper(string& word) {//makes all words completely uppercase
    for (unsigned int i = 0; i < word.length(); i++) {
        if (!isupper(word[i])) {
            word[i] = toupper(word[i]);
        }
    }
}

void Search(set <string> wordset, string startword, string endword){
    MinHeap<string> heap(2); //initalizes a binary heap

    int fval = 0; //defines temp variables, variables needed for
    int hval = 0; //calculations, and strings to store words.
    int gval = 0;
    int pri = 0;
    string currword = "";
    string tempword = "";
    int temph = 0;

    makeUpper(startword); //makes starting word and target
    makeUpper(endword);   //upper case

    map<string,int> distmap;
    set<string>::iterator iter;

    iter = wordset.find(startword); //checks if start word
    if (iter == wordset.end()) {    //was in file
        cout << "Start word not in dictionary" << endl;
        return;
    }

    currword = *iter;

    hval = geth(startword, endword); //calculates h, f, and g
    fval = gval + hval;              //values and priority
    pri = fval * (currword.length() + 1) + hval;

    heap.add(currword, pri); //adds starting node do heap
    distmap[currword] = gval;
    
    int tcount = 0; //defines int to count transformations.
    int found = 0; //int to break if last word was found

    //A* implementation
    while (!heap.isEmpty()) {
        
        currword = heap.peek();
        heap.remove();

        if (currword==endword) {
            found = 1;
            break;
        }

        for (unsigned int i = 0; i < currword.length(); i++) {
            //for loop to run through letters of the word
            tempword = currword;

            for (char j = 'A'; j  < 'Z'+1; j++) {
            //checks for possible variations of the current word
                tempword[i] = j;

                if ((distmap.find(tempword) == distmap.end() 
                || distmap[tempword] > distmap[currword] + geth(currword, tempword))
                && wordset.find(tempword) != wordset.end()) {
                //checks if word has been added to the map or if a shorter path to the
                //word has been found and if the word is part of the dictionary.

                    distmap[tempword] = distmap[currword]+1; //sets g-value for word
                    temph = geth(currword, tempword); //gets h value for next if check 

                    if (temph == 1) {
                    //checks if new word is 1 step away from original word
                        hval = geth(tempword, endword); //gets hval for new word
                        fval = distmap[tempword] + hval; //gets fval
                        pri = fval * (currword.length() + 1) + hval; //gets priority
                        heap.add(tempword, pri); //adds to heap

                    }
                }
            }
        }
        tcount++; //increments transformation count
    }

    if (found == 1) {
        cout << distmap[currword] << endl << tcount << endl; 
    }

    else if (found == 0) {
        cout << "No transformation" << endl << tcount << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Error: too few arguments." << endl;
        return 1; //checks for # of arguments
    }

    else if (argc > 4) {
        cerr << "Error: too many arguments." << endl;
        return 1; //checks for # of arguments
    }

    ifstream input(argv[3]);

    if (input.fail()) {
        cerr << "Error: file " << argv[3] << " cannot be opened." << endl;
        return 1; //checks if file exists;
    }

    string line = "";
    string tempword = "";
    int numwords = 0;
    string startword = argv[1];
    string endword = argv[2];
    set<string> wordset;

    getline(input, line);
    stringstream ss(line);
    ss >> numwords;

    while(getline(input, line)) { //gets words from file
        stringstream ss(line);
        ss >> tempword;

        if (tempword.length() != startword.length()) continue; 
        //inserts words in set if they are the right length
        makeUpper(tempword);
        wordset.insert(tempword);
    }

    Search(wordset, startword, endword);
    //calls A*

    return 0;
}