#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

class TheRani {
public:
    TheRani(char* inputp, char* outputp);
    ~TheRani();

    // Call execute and handles exceptions
    void main();




private:
    int experiment_count;       // You will need to track the number of experiments
    int* subject_counts;        // For each, the number of subjects
    string** subject_history = NULL;   // And for each subject, their history

    ifstream input;             // Input file stream
    ofstream output;            // Output file stream

    int totalSubj;

    // Called in the main method
    void execute(const string& line);

    void start(int subjNum);

    void add();

    void query(int x, int n);

    void move(int x, int y, int m, int n);

    void delater();

    int starter;

    int lNum;

    // Possible helper: deallocate all current members
};

TheRani::TheRani(char* inputp, char* outputp){
    input.open(inputp); //open input
    
    if(input.fail()) {
        output << "Error: input file doesn't exist" << endl;
        return; //checks for file
    }

    output.open(outputp); //opens output
    lNum = 0; //line counter
    starter = 0; //program start checker
}

TheRani::~TheRani() { //deleter
    for (int d = 0; d < experiment_count; d++)  {
        delete[] subject_history[d];
    }
    delete[] subject_history;
    delete[] subject_counts;
}

void TheRani::delater() { //deleter I can call
    for (int d = 0; d < experiment_count; d++)  {
        delete[] subject_history[d];
    }
    delete[] subject_history;
    delete[] subject_counts;
}

void TheRani::start(int subjNum) {
	if (starter == 1) { //deletes previous stuff if any
        delater();
    }
    
    starter = 1; //signals  there has been start

    experiment_count = 1;
    totalSubj = subjNum;
    

    subject_history = new string*[experiment_count];
    subject_counts = new int[1];

    subject_history[0] = new string[subjNum];


    for (int i = 0; i <  subjNum; i++) {
        subject_history[0][i] = ""; //initialise array 
    }

    subject_counts[0] = subjNum;

}

void TheRani::add() {
    experiment_count++;
  
    int* tempCount = subject_counts;
    int* copyCount;
    copyCount = new int[experiment_count];

    
    string** tPointer = subject_history;   //creation of pointer and substitute
    string** tArray;                       //arrays for changing  size, adding
    tArray = new string*[experiment_count];//subjects, etc.

    for (int j = 0; j < experiment_count; j++) {//initializing new row
        copyCount[j] = 0;
    }

    for (int j = 0; j < experiment_count-1; j++) {//copying prev contents
        copyCount[j] = subject_counts[j];
    }

    for (int m = 0; m < experiment_count; m++) {
        tArray[m] = new string[copyCount[m]]; //initializing new layers
        for (int n = 0; n <  copyCount[m]; n++) {
            tArray[m][n] = "";
        }
    }


    for (int m = 0; m < experiment_count-1; m++) {
        for (int n = 0; n <  subject_counts[m]; n++) {
            tArray[m][n] = subject_history[m][n]; //copyin prev contents
        }
    }
             
   

    subject_history = tArray; //sets new arrays to global
    subject_counts = copyCount; 

    for (int d = 0; d < experiment_count-1; d++)  {//deletes stuff
        delete[] tPointer[d];
    }

    delete[] tempCount;
    delete[] tPointer;



}

void TheRani::move(int x,int y, int n, int m) {

    int dif = m-n+1; //calculate number of  subjects
                     //being moved
    stringstream ss;

    int* tempCount = subject_counts;
    int* copyCount;
    copyCount = new int[experiment_count];


    for (int i = 0; i < experiment_count; i++) {
        copyCount[i] = subject_counts[i];
    }



    string** tPointer = subject_history;
    string** tArray;
    tArray = new string*[experiment_count];

    if (m < subject_counts[x] && y != x) {
        copyCount[y] = subject_counts[y] + dif;//update array dimensions
        copyCount[x] = subject_counts[x] - dif;
        for (int i = 0; i < experiment_count; i++) {
            tArray[i] = new string[copyCount[i]];
            for (int j = 0; j < copyCount[i]; j++) {    
                if (j < subject_counts[i]) { //copy previous contents 
                    tArray[i][j] = subject_history[i][j];
                }
                else {
                    tArray[i][j] = ""; //initialise empty space
                }
            }
        }


    

        for (int i = 0; i<n; i++){
            tArray[x][i] = subject_history[x][i]; //copies first values
        }    

        for (int i = m+1; i < subject_counts[x] ; i++) {//copies last values
            tArray[x][i-dif] = subject_history[x][i];
        }   //copying like this removes subjects that 
            //are transferred to other experiment
    
        int inc = n; //variable to only copy things user wants to move from prev experiment
        
        for (int i = copyCount[y] - dif; i < copyCount[y]; i++) { //copies values to end of//
                                                                  //destination experiment//
            tArray[y][i] = subject_history[x][inc];
            if (y != 0) {
                ss << tArray[y][i] << " " << y; //writes new experiment into subject history
                tArray[y][i] = ss.str();        //if experiment is not the subj pool
                ss.str(""); //cleanses stringstream
            
            }
            inc++; //moves indices from n to m in origin experiment
        }
    
    }

    else if (y == x && subject_counts[x] != 0) { //subjects moving to same experiment case
        for (int i = 0; i < experiment_count; i++) {
            tArray[i] = new string[subject_counts[i]];
            for (int j = 0; j < subject_counts[i]; j++){
                    tArray[i][j] = "";
            }
        }

        for (int i = 0; i<n; i++){
            tArray[x][i] = subject_history[x][i]; //values before n stay the same
        }    

        for (int i = subject_counts[x]-1; i > m ; i--) {//moves values after
            tArray[x][i-dif] = subject_history[x][i];   //m to lower indices
        }

        for (int i = subject_counts[x] - dif; i < subject_counts[x]; i++) {
            tArray[x][i] = subject_history[x][i];
            if (y != 0) {
                ss << tArray[x][i] << " " << y; //writes new experiment into subject history
                tArray[y][i] = ss.str();        // //if experiment is not the subj pool
                ss.str("");
            }
        }

        for (int i = 0; i < experiment_count; i++) {
            if (i != x) {
                for (int j = 0; j < subject_counts[i]; j++) {
                    tArray[i][j] = subject_history[i][j]; //copies rest of previous array     
                }
            }
        }
    }

    subject_history = tArray;
    subject_counts = copyCount;

    for (int d = 0; d < experiment_count; d++)  {
        delete[] tPointer[d];
    }
    delete[] tPointer;
    delete[] tempCount;

}


void TheRani::query(int x, int n) {
    output << subject_history[x][n]; //prints query to ouput file
    output << endl;

}

void TheRani::main() {
    string line;
    while (getline(input, line)) {
        lNum++; //increments line count
        try {
            this->execute(line);
        } catch(exception& e) {
            this->output << "Error on line ?: " << e.what() << endl;
        }
    }
}

void TheRani::execute(const string& line) {
    string command;
    stringstream stream(line);  // Initialize the stream with the line
    stream >> command;          // Read the first word, which is the command

    if (starter == 0 && command != "START") {
        output <<"Error on line" << lNum << ": no subjects yet" << endl;
        return; //error check in case user tries any command before START
    }


    if (command == "START") {  
        int subject_pool_count;
        stream >> subject_pool_count;
        if (subject_pool_count < 0) {
            throw out_of_range("argument out of range");
        }

        string errtest;
        stream >> errtest; //take line as string (array of chars)
        for (unsigned int i = 0; i < errtest.size(); i++) {
            if (isdigit(errtest[i]) == false || errtest[i] != '.') {
                output << "Error on line "<< lNum 
                << ": expected integer argument" << endl;
                return; //check every char for not ints
                        
                    //had to use unsigned int in loop
                    //because compiler was giving errors
            }

            if (errtest[i] == '.') {
                for (unsigned int j = i+1; j < errtest.size(); j++) {
                    if (errtest[j] != 0) {
                        output << "Error on line "<< lNum 
                        << ": expected integer argument" << endl;
                        return; //check if input is a float that can be interpreted as an int
                    }           //that can be interpreted as an int i.e. 3.0000

                }

            }


        }

        start(subject_pool_count);
    }

    else if (command == "ADD") {
        add();
    }

    else if (command == "MOVE") {
        int x1, y1, n1, m1;

        stream >> x1;
        stream >> y1;
        stream >> n1;
        stream >> m1;

        if (stream.fail()) {
            output << "Error on line " << lNum << ": too few arguments" << endl; 
            return;
        }

        if (x1 > experiment_count || x1 < 0 || y1 > experiment_count || y1 < 0 || 
        m1 > subject_counts[x1] || m1 < 0 || n1 > subject_counts[x1] || n1 < 0) 
        {
            output << "Error on line " << lNum << ": argument out of range" << endl;
            return; //checks for all possible incorrect inputs
        }

        if (n1 > m1) {
            output << "Error on line " << lNum << 
            ": invalid range of subjects to move" << endl;
            return; //pretty obvious what this check does
        }

        string errtest1;
        stream >> errtest1; //same error checking as in START
        for (unsigned int i = 0; i < errtest1.size(); i++) {
            if (isdigit(errtest1[i]) == false || errtest1[i] != '.') {
                output << "Error on line "<< lNum 
                << ": expected integer argument" << endl;
                return;
            }

            if (errtest1[i] == '.') {
                for (unsigned int j = i+1; j < errtest1.size(); j++) {
                    if (errtest1[j] != 0) {
                        output << "Error on line "<< lNum 
                        << ": expected integer argument" << endl;
                        return;
                    }

                }

            }


        }

        move(x1, y1, n1, m1);
        

    }

    else if (command == "QUERY") {
        int x2, n2; 

        stream >> x2;
        stream >> n2;

        if (stream.fail()) {
            output << "Error on line " << lNum << ": too few arguments" << endl; 
            return; //checks if argument # is correct
        }

        string errtest2;
        stream >> errtest2; //same checking as MOVE and START
        for (unsigned int i = 0; i < errtest2.size(); i++) {
            if (isdigit(errtest2[i]) == false || errtest2[i] != '.') {
                output << "Error on line "<< lNum 
                << ": expected integer argument" << endl;
                return;

            }

            if (errtest2[i] == '.') {
                for (unsigned int j = i+1; j < errtest2.size(); j++) {
                    if (errtest2[j] != 0) {
                        output << "Error on line "<< lNum 
                        << ": expected integer argument" << endl;
                        return;
                    }

                }

            }


        }

        query(x2, n2);
    }

    else {
    	output << "command doesn't exist" << endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Please provide an input and output file!" << endl;
        return 1;
    }

    TheRani tr(argv[1], argv[2]);
    tr.main();
    return 0;
}
