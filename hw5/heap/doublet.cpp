#include "heap.h"
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
    MinHeap<string> heap(2);

    int fval = 0;
    int hval = 0;
    int gval = 0;
    int pri = 0;
    string currword = "";
    string tempword = "";
    int temph = 0;

    makeUpper(startword);
    makeUpper(endword);

    map<string,int> distmap;
    set<string>::iterator iter;

    iter = wordset.find(startword);
    if (iter == wordset.end()) {
        cout << "Start word not in dictionary" << endl;
        return;
    }
    currword = *iter;

    hval = geth(startword, endword);
    //cout << startword << " is " << hval << " letters away from " << endword << endl;

    fval = gval + hval;
    pri = fval * (currword.length() + 1) + hval;

   /* cout << "f is: " << fval << endl;
    cout << "g is: " << gval << endl;
    cout << "h is: " << hval << endl;
    cout << "word length + 1 is: " << currword.length()+1 << endl;
    cout << "priority is: " << pri << endl;*/

    heap.add(currword, pri);
    distmap[currword] = gval;
    //gval++;
    
    int tcount = 0;
    int found = 0;

   // heap.PrintHeap();

    while (!heap.isEmpty()) {
        
        currword = heap.peek();
        heap.remove();
        cout << "distance from beggining: " << distmap[currword];
        cout << " || distance from end: " << geth(currword, endword);
        cout << " || fval: " << distmap[currword]+geth(currword, endword) << endl;
        cout << "word at top was: " << currword << endl;
        if (currword==endword) {
            found = 1;
            break;
        }

        for (unsigned int i = 0; i < currword.length(); i++) {
            tempword = currword;
            for (char j = 'A'; j  < 'Z'+1; j++) {
                tempword[i] = j;
                if ((distmap.find(tempword) == distmap.end() 
                || distmap[tempword] > distmap[currword] + geth(currword, tempword))
                && wordset.find(tempword) != wordset.end()) {
                    cout << "checking: " << tempword << endl;

                    distmap[tempword] = distmap[currword]+1;
                    temph = geth(currword, tempword);

                    if (temph == 1) {
                    
                        hval = geth(tempword, endword);
                        fval = distmap[tempword] + hval;
                        pri = fval * (currword.length() + 1) + hval;
                        heap.add(tempword, pri);
                        cout << "Added " << tempword <<" with prio: " << pri << endl;
                    }
                    //else if (temph + ){

                    //}
                }
            }
        }
        tcount++;
        heap.PrintHeap();
        cout <<  tcount << endl;
        cout << endl;
    }

    if (found == 1) {
        cout << distmap[currword] << endl << tcount << endl; 
    }

    if (found == 0) {
        cout << "No transformation" << endl << tcount << endl;
    }

    //


   /* int d[n]; //distances from the start node u
    int p[n]; //predecessors
    int c[n][n]; //edge costs
    void Dijkstra (int u) {
        PriorityQueue<int> pq(); //How should we implement this?
        d[u] = 0;
        pq.add(u, d[u]);
        while(!pq.isEmpty()) {
            int v = pq.peek();
            pq.remove();
            for all nodes outgoing edges (v,w) from v {
                if (w hasn’t been visited || d[v] + c[v][w] < d[w]) {
                d[w] = d[v] + c[v][w];
                p[w] = v;
                    if (this is w’s first visit) {
                        pq.add(w, d[w]);
                    }
                    else pq.update(w, d[w]);
                }
            }
        }       
    }*/
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

    while(getline(input, line)) {
        stringstream ss(line);
        ss >> tempword;
        if (tempword.length() != startword.length()) continue;
        makeUpper(tempword);
        wordset.insert(tempword);
        //cout << tempword << endl;
    }

    Search(wordset, startword, endword);

    return 0;
}