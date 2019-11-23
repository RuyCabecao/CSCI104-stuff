#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void subsets(double work[], double learn[], double workload, int numclasses, 
    int start, double worksum,double learnsum, 
    std::vector <double> &wvec, std::vector <double> &lvec) {

//This implementation uses a recursive subset sum to find all possible learn 
//and work combinations.
//This helper function feeds into two vectors that store respecitve learn and
//work sums in the same indices

    if (start == numclasses) {
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
    
    std::vector <double> lvec; //defines vectors
    std::vector <double> wvec;
    double max = 0; //double that will store maximum possible learn
   
    subsets(work, learn, workload, numclasses, 0, 0, 0, wvec, lvec);//fills vectors
    
    for (unsigned int i = 0; i < wvec.size(); i++) {
        if (wvec[i] <= workload && lvec[i] > max){
            max = lvec[i]; //finds maximum learn that doesn't exceed workload
        }   
    }
    
    std::cout << max << std::endl; //prints maximum learn

}

/*###################################################################*/

int main(int argc, char* argv[]) { 
    
    if (argc < 2) {
        std::cerr << "Error: please provide an input file." << std::endl;
        return 1; //checks for # of arguments
    }

    else if (argc > 2) {
        std::cerr << "Error: too many arguments." << std::endl;
        return 1; //checks for # of arguments
    }

    std::ifstream input(argv[1]);

    if (input.fail()) {
        std::cerr << "Error: file " << argv[1] << " cannot be opened." << std::endl;
        return 1; //checks if file exists;
    }

    std::string line = "";
    int numclasses = 0;
    double workload = 0;
    int counter = 0;
    //defines some vars

    getline(input, line);
    std::stringstream ss(line);
    ss >> numclasses >> workload;
    //gets number of classes and max workload

    std::string classes[numclasses];
    double learn[numclasses];
    double workloadarr[numclasses];
    //creates arrays to store values


    while(getline(input, line)) {

        std::stringstream ss2(line);
        ss2 >> classes[counter] >> workloadarr[counter] >> learn[counter];
        counter++;
        //stores all values in arrays
    }

    subsetmap(workloadarr, learn, workload, numclasses, 0, 0, 0);
    //calls recursion
    
    return 0;
}