#include<iostream>
#include<vector>
#include<list>
#include"purge.h"
#include"pomdp.h"

using namespace std;

int main(int argc, char* argv[]){
  pomdp p("foo.pomdp");
  vectorSet F;
  list<sVector> tempList;
  vector<sVector> tempVec; 
  tempVec = p.get_r_Matrix();
  tempList.assign(tempVec.begin(),tempVec.end());
  F.set_vSet(tempList);  
  purge(F);
  //F = purge2(F);
  print_vSet(F);
}


