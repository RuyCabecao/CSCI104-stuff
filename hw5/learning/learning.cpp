#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void subsets(double work[], double learn[], double workload, int numclasses, 
    int start, double worksum,double learnsum, 
    vector <double> &wvec, vector <double> &lvec) {

//This implementation uses a recursive subset sum to find all possible learn 
//and work combinations.
//This helper function feeds into two vectors that store respecitve learn and
//work sums in the same indices

    if (start > numclasses) {
        lvec.push_back(learnsum); //pushes amount learned into vector
        wvec.push_back(worksum); //pushes amount of work into vector
        return;
    }
    
    subsets(work, learn, workload, numclasses, start+1,  
    worksum+work[start], learnsum+learn[start], wvec, lvec);
    //recursive call incrementing worksum and learnsum

    subsets(work, learn, workload, numclasses, start+1,
    worksum, learnsum, wvec, lvec);
    //recursive call not incrementing 

}

/*###################################################################*/

void subsetmap(double work[], double learn[], double workload, 
int numclasses, int start, double worksum, double learnsum) {
    
    vector <double> lvec; //defines vectors
    vector <double> wvec;
    double max = 0; //double that will store maximum possible learn
   
    subsets(work, learn, workload, numclasses, 0, 0, 0, wvec, lvec);//fills vectors
    
    for (unsigned int i = 0; i < wvec.size(); i++) {
        if (wvec[i] <= workload && lvec[i] > max){
            max = lvec[i]; //finds maximum learn that doesn't exceed workload
        }   
    }
    
    cout << max << endl; //prints maximum learn

}

/*###################################################################*/

int main(int argc, char* argv[]) { 
    
    if (argc < 2) {
        cerr << "Error: please provide an input file." << endl;
        return 1; //checks for # of arguments
    }

    else if (argc > 2) {
        cerr << "Error: too many arguments." << endl;
        return 1; //checks for # of arguments
    }

    ifstream input(argv[1]);

    if (input.fail()) {
        cerr << "Error: file " << argv[1] << " cannot be opened." << endl;
        return 1; //checks if file exists;
    }

    string line = "";
    int numclasses = 0;
    double workload = 0;
    int counter = 0;
    //defines some vars

    getline(input, line);
    stringstream ss(line);
    ss >> numclasses >> workload;
    //gets number of classes and max workload

    string classes[numclasses];
    double learn[numclasses];
    double workloadarr[numclasses];
    //creates arrays to store all values


    while(getline(input, line)) {

        stringstream ss2(line);
        ss2 >> classes[counter] >> workloadarr[counter] >> learn[counter];
        counter++;
        //stores all values in arrays
    }

    subsetmap(workloadarr, learn, workload, numclasses, 0, 0, 0);
    //calls recursion

    return 0;
}