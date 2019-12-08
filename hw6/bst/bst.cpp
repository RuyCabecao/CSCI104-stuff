#include "bst.h"
#include <iostream>

using namespace std;

int main() {
    BinarySearchTree<double, double> bst;
    pair<double, double> pairins;
    pairins.first = 3;
    pairins.second = 3;
    pair<double, double> pairins2;
    pairins2.first = 6;
    pairins2.second = 6;
    pair<double, double> pairins3;
    pairins3.first = 2;
    pairins3.second = 2;
    pair<double, double> pairins4;
    pairins4.first =5;
    pairins4.second = 5;
    pair<double, double> pairins5;
    pairins5.first =1;
    pairins5.second = 1;
    pair<double, double> pairins6;
    pairins6.first =0;
    pairins6.second = 0;
    bst.print();
    bst.insert(pairins);
    bst.insert(pairins2);
    bst.insert(pairins3);
    bst.insert(pairins4);
    bst.insert(pairins5);
    bst.insert(pairins6);
    pairins2.first = 8;
    pairins2.second = 8;
    bst.insert(pairins2);
    pairins2.first = 7;
    pairins2.second = 7;
    bst.insert(pairins2);
    bst.print();
  /*  pairins2.first = 5;
    pairins2.second = 10;
    bst.insert(pairins2);*/
    bst.print();
    bst.remove(3);
    bst.print();
    bst.clear();
    bst.print();
    return 0;

}