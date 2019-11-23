#include <vector>
#include <utility>
#include <iostream>
#include<exception>
#include<stdexcept>

template <typename T>
  class MinHeap {
     public:
       MinHeap (int d);
       /* Constructor that builds a d-ary Min Heap
          This should work for any d >= 2,
          but doesn't have to do anything for smaller d.*/

       int add (T item, int priority);
         /* adds the item to the heap, with the given priority. 
            multiple identical items can be in the heap simultaneously. 
            Returns the number of times add has been called prior to this
            call (for use with the update function).*/

       const T & peek () const;
         /* returns the element with smallest priority.  
            If two elements have the same priority, use operator< on the 
            T data, and return the one with smaller data.*/

       void remove ();
         /* removes the element with smallest priority, with the same tie-breaker
            as peek. */

       void update (int nth, int priority);
         /* finds the nth item (where nth is 0-based) that has ever been added 
            to the heap (the node that was created on the nth call to add), 
            and updates its priority accordingly. */

       bool isEmpty ();
         /* returns true iff there are no elements on the heap. */



       void TrickleUp (int pos);

       void TrickleDown (int pos);

       void Swap (int child, int father);

       void Swapord (int child, int father);

       void PrintOrder();

       void PrintHeap();

   private:
      // whatever you need to naturally store things.
      // You may also add helper functions here.
      
      
      std::vector< std::pair <T,int> > pairvec;
      std::vector<int> ordervec;
      
      int dval = 2;
      int addcount = 0;
      int offset = 0;
  };

template <typename T>
MinHeap<T>::MinHeap(int d) {
  if (d >= 2) 
    dval = d;
}

template <typename T>
int MinHeap<T>::add (T item, int priority) {
  

  std::pair<T, int> addpair; 
  addpair = std::make_pair (item, priority);
  
  pairvec.push_back(addpair);
  ordervec.push_back(addcount);
  
  TrickleUp(pairvec.size()-1);
  
  addcount++;
  return addcount;
}

template <typename T>
const T & MinHeap<T>::peek () const {
  if (!(pairvec.empty())) return pairvec[0].first;
  else {
    throw std::runtime_error("Peeked empty heap");
  }
}

template <typename T>
void MinHeap<T>::remove () {
  int del1 = 0;

  if (pairvec.empty()) {
    return;
  }

  for (unsigned int i = 0; i < ordervec.size(); i++) {
    if (ordervec[i] == 0) {
      del1 = i;
    }
  }

  if (pairvec.size() == 1) {
    ordervec[del1] = -1;
    pairvec.pop_back();
    return;
  }

  Swap(0, pairvec.size()-1);
  Swapord(0, pairvec.size()-1);
  
  ordervec[del1] = -1;
  
  pairvec.pop_back();
  TrickleDown(0);

}


template <typename T>
void MinHeap<T>::update (int nth, int priority) {
  int temppos = 0;
  int size = ordervec.size();
  
  if (nth < size && nth > -1 && ordervec[nth] != -1) {
    temppos = ordervec[nth];
    if (priority < pairvec[temppos].second) {
      pairvec[temppos].second = priority;
      TrickleUp(temppos);
    }
    else if (priority > pairvec[temppos].second) {
      pairvec[temppos].second = priority;
      TrickleDown(temppos);
    }
  }
}


template <typename T>
bool MinHeap<T>::isEmpty() {
  return pairvec.empty();
}



template <typename T>
void MinHeap<T>::TrickleUp(int pos) {
  if (pairvec.size() == 1)
    return;


  if (pos > 0 && pairvec[pos].second < pairvec[(pos-1)/dval].second) {

    Swap(pos, (pos-1)/dval);
    Swapord(pos, (pos-1)/dval);
    pos = ((pos-1)/dval);
    TrickleUp(pos);

  }
  else if (pos > 0 && pairvec[pos].second == pairvec[(pos-1)/dval].second) {
    if (pairvec[pos].first < pairvec[(pos-1)/dval].first) {
         
      Swap(pos, (pos-1)/dval);
      Swapord(pos, (pos-1)/dval);
      pos = ((pos-1)/dval);
      TrickleUp(pos);

    }
    else
      return;
  }
  else 
    return;
}


template <typename T>
void MinHeap<T>::TrickleDown(int pos) {

  int minp = pairvec[pos].second;
  int pind = 0;
  int size = pairvec.size();

  if ((dval*pos)+1 < size) {
    for (int i = (dval*pos)+1; i < (dval*pos)+dval+1; i++) {
      if (i < size) {
        
        if (pairvec[i].second < minp) {
          minp = pairvec[i].second;
          pind = i;
        }

        else if (pairvec[i].second == minp) {
          if (pairvec[i].first < pairvec[pind].first) {
            minp = pairvec[i].second;
            pind = i;
          }
        }
      }
    }
  }

  if (minp != pairvec[pos].second) {
    Swap(pos, pind);
    Swapord(pos, pind);
    TrickleDown(pind);
  }

}


template <typename T>
void MinHeap<T>::Swap(int child, int father) {
  std::pair<T, int> temp;
  
  temp.first = pairvec[father].first;
  temp.second = pairvec[father].second;

  pairvec[father].first = pairvec[child].first;
  pairvec[father].second = pairvec[child].second;

  pairvec[child].first = temp.first;
  pairvec[child].second = temp.second;
}

template <typename T>
void MinHeap<T>::Swapord(int child, int father) {
  int temp = 0;
  int ci = 0;
  int pi = 0;
  
  for (unsigned int i = 0; i < ordervec.size(); i++) {
    if (ordervec[i] == child) ci = i;
    else if (ordervec[i] == father) pi = i;
  }
  temp = ordervec[pi];

  ordervec[pi] = ordervec[ci];

  ordervec[ci] = temp;
}


template <typename T>
void MinHeap<T>::PrintOrder() {
  for (unsigned int i = 0; i < ordervec.size(); i++) {
    std::cout << ordervec[i] << " ";
  }
  std::cout << std::endl;
}


template <typename T>
void MinHeap<T>::PrintHeap() {
  for (unsigned int i = 0; i < pairvec.size(); i++) {
    std::cout << pairvec[i].first << " ";
  }
  std::cout << std::endl;
}