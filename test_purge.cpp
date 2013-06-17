#include<iostream>
#include<vector>
#include<list>
#include"purge.h"

using namespace std;

int main(int argc, char* argv[]){
  sVector temp_sVec,temp_sVec2;
  temp_sVec.assign(3,1.5);
  temp_sVec2.assign(3,2.5);
  list<sVector> temp_list(2,temp_sVec),temp_list2(2,temp_sVec2),temp_list3;
  temp_list3.push_back(temp_sVec);
  temp_list3.push_back(temp_sVec2);
  vectorSet F1,F2,F3;
  F1.set_vSet(temp_list);
  F2.set_vSet(temp_list2);
  F3.set_vSet(temp_list3);
  //purge(F3);
  F3 = purge2(F3);
  cout<<"F3:";
  print_vSet(F3);
}


