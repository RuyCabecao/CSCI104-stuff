#include "company.hpp"

CompanyTracker::CompanyTracker (int n)
  // initializes the tracker with n students and their 1-person companies
{
  numCompanies = n;
  companies = new Company* [numCompanies];
  for (int i = 0; i < numCompanies; ++i)
      companies[i] = new Company ();
}

CompanyTracker::~CompanyTracker ()
  // deallocates all dynamically allocated memory
{
  // your implementation goes here
  for (int i = 0; i < numCompanies; i++)  {
    delete companies[i]; //deletes inner pointers
  }

  for (unsigned int i = 0; i < newComps.size(); i++) {
    delete newComps[i]; //deletes pointers in vector
  }

  delete[] companies; //deletes outer pointers
}

void CompanyTracker::merge (int i, int j)
  /* Merges the largest companies containing students i and j,
     according to the rules described above.
     That is, it generates a new Company object which will
     become the parent company of the largest companies currently
     containing students i and j.
     If i and j already belong to the same company (or are the same),
     merge doesn't do anything.
     If either i or j are out of range, merge doesn't do anything. */
 
{
  // your implementation goes here
  if (i < 0 || i >= numCompanies || j < 0 || j >= numCompanies 
  || i == j) { //checks base cases
    return;
  }
    
    Company* parenti = FindParent(i); //finds parent companies
    Company* parentj = FindParent(j); //for i and j

    if (parenti == parentj) {
      return; //more base case checking
    }

    Company* newCompany = new Company(parenti, parentj);
    newComps.push_back(newCompany); //if passed all base checks
                                    //add company to vector
    parenti->parent = newCompany; 
    parentj->parent = newCompany; //set merger to parent of
}                                 //child companies

void CompanyTracker::split (int i)
  /* Splits the largest company that student i belongs to,
     according to the rules described above.
     That is, it deletes that Company object, and makes sure that
     the two subcompanies have no parent afterwards.
     If i's largest company is a 1-person company, split doesn't do anything.
     If i is out of range, split doesn't do anything. */
{
  // your implementation goes here
  if (i < 0 || i >= numCompanies) {
    return; //checks base cases
  }
  
  Company* father = FindParent(i);

  if (father == companies[i]) {
    return; //checks another base case
  }

  father->merge1->parent = nullptr; //removes parent
  father->merge2->parent = nullptr;

  for (unsigned int i = 0; i < newComps.size(); i++) {
    if(newComps[i] == father) {
      newComps.erase(newComps.begin() + i);
    } //erases father company from merger vector
  }
  
  delete father; //deletes father from memory

}

bool CompanyTracker::inSameCompany (int i, int j)
  /* Returns whether students i and j are currently in the same company.
     Returns true if i==j.
     Returns false if i or j (or both) is out of range. */
{
  // your implementation goes here
  if (i < 0 || i >= numCompanies || j < 0 || j >= numCompanies) { 
    return false; //checks base cases
  }

  if (i==j) {
    return true;
  } //nothing to check if comparing subject to itself

  Company* parenti = FindParent(i);
  Company* parentj = FindParent(j);

  if (parenti == parentj) {
    return true; 
  } //if they're in the same, true
  else {
    return false;
  } //else false
}

Company* CompanyTracker::FindParent(int i) {
  Company* top = companies[i];

  while(top->parent != nullptr)
    top = top->parent;//looks for biggest company

  return top; //returns said company
}

