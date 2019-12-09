#include "splay.h"
#include <iostream>

using namespace std;

int main() {
    SplayTree<double, double> splay;
    pair<double, double> pairins;
    pairins.first = 9;
    pairins.second = 9;
    pair<double, double> pairins2;
    pairins2.first = 4;
    pairins2.second = 4;
    pair<double, double> pairins3;
    pairins3.first = 6;
    pairins3.second = 6;
    pair<double, double> pairins4;
    pairins4.first = 5;
    pairins4.second = 5;
    pair<double, double> pairins5;
    pairins5.first = 11;
    pairins5.second = 11;
    pair<double, double> pairins6;
    pairins6.first = 10;
    pairins6.second = 10;
    pair<double, double> pairins7;
    pairins7.first = 3;
    pairins7.second = 3;
    pair<double, double> pairins8;
    pairins8.first = 7;
    pairins8.second = 7;
    pair<double, double> pairins9;
    pairins9.first = 13;
    pairins9.second = 13;
    splay.print();


    splay.insert(pairins4);
    splay.print();
    splay.insert(pairins);
    splay.print();
    splay.insert(pairins2);
    splay.print();
    splay.insert(pairins6);
    splay.print();
    splay.insert(pairins3);
    splay.print();
    splay.insert(pairins5);
    splay.print();
    splay.insert(pairins7);
    splay.print();
    splay.remove(6);
    splay.print();
    

    return 0;
}