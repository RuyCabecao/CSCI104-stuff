#include "avlbst.h"
#include <iostream>

using namespace std;

int main() {
    AVLTree<double, double> avl;
    pair<double, double> pairins;
    pairins.first = 6;
    pairins.second = 6;
    pair<double, double> pairins2;
    pairins2.first = 3;
    pairins2.second = 3;
    pair<double, double> pairins3;
    pairins3.first = 7;
    pairins3.second = 7;
    pair<double, double> pairins4;
    pairins4.first = 1;
    pairins4.second = 1;
    pair<double, double> pairins5;
    pairins5.first = 5;
    pairins5.second = 5;
    pair<double, double> pairins6;
    pairins6.first = 4;
    pairins6.second = 4;
    pair<double, double> pairins7;
    pairins7.first = 4;
    pairins7.second = 10;
    pair<double, double> pairins8;
    pairins8.first = 8;
    pairins8.second = 8;
    pair<double, double> pairins9;
    pairins9.first = 3;
    pairins9.second = 3;
    avl.print();


    avl.insert(pairins);
    avl.print();
    avl.insert(pairins2);
    avl.print();
    avl.insert(pairins3);
    avl.print();
    avl.insert(pairins4);
    avl.print();
    avl.insert(pairins5);
    avl.print();
    avl.insert(pairins6);
    avl.print();
    //avl.insert(pairins7);
    avl.remove(6);
    avl.print();
//    avl.remove(7);
//    avl.print();
    /*avl.insert(pairins8);
    avl.print();
    avl.insert(pairins9);
    avl.print();*/


    avl.print();
    return 0;
}