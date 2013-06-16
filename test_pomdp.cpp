#include<vector>
#include<list>
#include<iostream>
#include"pomdp.h"

using namespace std;

int main(int argc, char* argv[]){
  pomdp p("foo.pomdp");  
  vectorSet s;  
  sVector temp_sVec,temp_sVec2,temp_sVec3;
  temp_sVec.assign(9,1.0);
  temp_sVec2.assign(9,2.0);
  temp_sVec3.assign(9,3.0);
  list<sVector> tempList;
  tempList.push_back(temp_sVec);
  tempList.push_back(temp_sVec2);
  tempList.push_back(temp_sVec3);
  s.set_vSet(tempList);
  //p.show_parameter();
  s = update_vectorSet(s,1.0,p.get_T_Matrix(),p.get_r_Matrix(),p.get_valid_Matrix());
  //cout<<s.vSet.size()<<endl;
  //print_vSet(s);
}


