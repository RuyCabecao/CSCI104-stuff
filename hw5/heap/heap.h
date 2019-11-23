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

   private:
      // whatever you need to naturally store things.
      // You may also add helper functions here.
      
      
      std::vector< std::pair <T,int> > pairvec;
      //stores "node" as pairs
      std::vector<int> ordervec;
      //stores where nodes are in heap and their add order
      
      int dval = 2;
      int addcount = 0;
  };

template <typename T>
MinHeap<T>::MinHeap(int d) {
  if (d >= 2) 
    dval = d; //sets d value
}

template <typename T>
int MinHeap<T>::add (T item, int priority) {

  std::pair<T, int> addpair; 
  addpair = std::make_pair (item, priority);
  //creates Item, int pair
  
  pairvec.push_back(addpair);
  ordervec.push_back(addcount);
  //adds pair to pairvector and adds
  //add order to the auxiliary vector
  
  TrickleUp(pairvec.size()-1);
  //trickles up the added node
  
  addcount++;
  return addcount;
}

template <typename T>
const T & MinHeap<T>::peek () const {
  if (!(pairvec.empty())) return pairvec[0].first;
  //returns root

  else {
    throw std::runtime_error("Peeked empty heap");
  } //error handling
}

template <typename T>
void MinHeap<T>::remove () {
  int del1 = 0;

  if (pairvec.empty()) {
    return; //can't remove from empty vector
  }

  for (unsigned int i = 0; i < ordervec.size(); i++) {
    if (ordervec[i] == 0) {
      del1 = i; //finds when current root was added
    }
  }

  if (pairvec.size() == 1) { //edge case check.
    ordervec[del1] = -1; 
    pairvec.pop_back(); //removes from heap vector.
    return;
  }

  Swap(0, pairvec.size()-1); //swaps last node to root
  Swapord(0, pairvec.size()-1); //swaps order vector
  
  ordervec[del1] = -1; //sets deleted node to -1
  
  pairvec.pop_back(); //removes from heap vector
  TrickleDown(0);

}


template <typename T>
void MinHeap<T>::update (int nth, int priority) {
  int temppos = 0;
  int size = ordervec.size();
  
  if (nth < size && nth > -1 && ordervec[nth] != -1) {
    //checks if node is valid, i.e. not a deleted node
    temppos = ordervec[nth];
    //gets index value of nth added
    if (priority < pairvec[temppos].second) {
      pairvec[temppos].second = priority;
      TrickleUp(temppos);
      //trickles up if priority is less
    }
    else if (priority > pairvec[temppos].second) {
      pairvec[temppos].second = priority;
      TrickleDown(temppos);
      //trickles down if priority is greater
    }
  }
}


template <typename T>
bool MinHeap<T>::isEmpty() {
  return pairvec.empty(); 
}//if the node vector is empty, then the heap is empty



template <typename T>
void MinHeap<T>::TrickleUp(int pos) {

  if (pairvec.size() == 1)
    return; //nothing to do if only root

  if (pos > 0 && pairvec[pos].second < pairvec[(pos-1)/dval].second) {

    Swap(pos, (pos-1)/dval);
    Swapord(pos, (pos-1)/dval);
    pos = ((pos-1)/dval);
    TrickleUp(pos);
    //sends the node up if it has lower priority

  }

  else if (pos > 0 && pairvec[pos].second == pairvec[(pos-1)/dval].second) {
    if (pairvec[pos].first < pairvec[(pos-1)/dval].first) {
         
      Swap(pos, (pos-1)/dval);
      Swapord(pos, (pos-1)/dval);
      pos = ((pos-1)/dval);
      TrickleUp(pos);
      //tiebreaker scenario

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
  int size = pairvec.size(); //defines varialbes to not get warnings

  if ((dval*pos)+1 < size) {
    //checks if range is valid
    for (int i = (dval*pos)+1; i < (dval*pos)+dval+1; i++) {
      //loops through every child
      if (i < size) {
        //checks if range is valid
        
        if (pairvec[i].second < minp) {
          minp = pairvec[i].second; //sets new minimum
          pind = i; //sets new minimum index
        }

        else if (pairvec[i].second == minp) {
          if (pairvec[i].first < pairvec[pind].first) {
            minp = pairvec[i].second;
            pind = i; //tie breaker scenario
          }
        }
      }
    }
  }

  if (minp != pairvec[pos].second) {
    Swap(pos, pind); //swaps values downwards
    Swapord(pos, pind);
    TrickleDown(pind); //recurisve call to trickle
  }

}


template <typename T>
void MinHeap<T>::Swap(int child, int father) {
  std::pair<T, int> temp;
  //swaps both elements of the pair with a standard swap 
  //algorithm, nothing too fancy.
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
    //finds index of child node in order vector
    else if (ordervec[i] == father) pi = i;
    //finds index of parent node in order vector
  }
  temp = ordervec[pi];

  ordervec[pi] = ordervec[ci];

  ordervec[ci] = temp;
  //swaps values with standard swap algorithm
}