#include "heap.h"

using namespace std;

int main () {
    int d = 2;

    MinHeap<int> heap(d);
    
    for (int i = 10; i > 5; i--) {
        heap.add(i,i);
        heap.PrintHeap();
        heap.PrintOrder();
        cout << heap.peek() << endl;
        cout << "------------------------" << endl;
    }
    cout << "done with loop1 " << endl;
    heap.update(0,1);
    heap.PrintHeap();
    heap.PrintOrder();
    cout << "------------------------" << endl;

    cout << "is it empty? " << heap.isEmpty() << endl;
    for (int i = 10; i > 6; i--) {
        heap.remove();
        heap.PrintHeap();
        heap.PrintOrder();
        cout << heap.peek() << endl;
        cout << "------------------------" << endl;
    }

    cout << "is it empty? " << heap.isEmpty() << endl;

    heap.remove();
    heap.PrintHeap();
    heap.PrintOrder();
    cout << "------------------------" << endl;

    cout << "is it empty? " << heap.isEmpty() << endl;
    cout << heap.peek() << endl;
    
    

    return 0;
}