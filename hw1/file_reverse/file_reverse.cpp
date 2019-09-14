#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    int auf = 0;
    stringstream ss;
    string ye;
    string yes;
    ifstream ifile(argv[1]);
    //ifile.open(argv[1]);

    getline(ifile, yes);
    ss << yes;
    ss >> auf;
   
    getline(ifile, ye);
    
    //cout << "print" << ye;
    //cout << auf << endl;
    //cout << ye << endl;
    
    char *lett = new char[auf];
    //cout << auf << endl;

    for (int i = 0; i < auf; i++){
        //cout << "storing" << ye[i]  << endl;
        lett[i] = ye[i];
        //cout << lett[i] << "stored" << endl;
        //cout << "done" << endl;
    }

    for (int j = auf-1; j > -1; j--){
        //cout << auf << endl;
        cout << lett[j];
    }
    cout << endl;

    delete[] lett;

return 0;
}