#include <iostream>

void permutationhelper(std::string in, int start, int length) {
    
    //Helper function because making the function with 
    //a single parameter was too hard.


    if (start == length) { //checks if string is long enough to print
        std::cout << in << std::endl;
        return;
    }

    std::string copys = in; //copy string to swap values

    for (int i = start; i < length; i++) {
        in[start] = copys[i]; //swaps values in order to get proper
        in[i] = copys[start]; //recursion calls, moving the letters
                              //around to cover all possibilities

        permutationhelper(in, start+1, length); //calls recursion on next
                                                //letter of the string
        in[start] = copys[start];
        in[i] = copys[i]; //resets string to preserve functionality
                          //of recursive calls

    }

}


void permutations(std::string in) {
    permutationhelper(in, 0, in.length());
    //calls helper after defining length
}