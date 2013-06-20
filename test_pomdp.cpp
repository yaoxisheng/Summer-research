#include<vector>
#include<list>
#include<iostream>
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
  //p.show_parameter();
  for(int i=0;i<1;i++){
    //print_vSet(F);
    //cout<<endl;
    F = update_vectorSet(F,1.0,p.get_T_Matrix(),p.get_r_Matrix(),p.get_valid_Matrix());
    //cout<<F.vSet.size()<<endl;
    //print_vSet(F);
    //cout<<endl;
  }  
}


