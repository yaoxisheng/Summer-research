#include<vector>
#include<list>
#include<iostream>
#include"pomdp.h"

using namespace std;

int main(int argc, char* argv[]){
  pomdp p("foo.pomdp");
  vectorSet s;
  vector<float> tempVec(9,2.0);
  list<vector<float> > tempList(3,tempVec);
  s.set_vSet(tempList);
  //p.show_parameter();
  s = update_vectorSet(s,1.0,p.get_T_Matrix(),p.get_r_Matrix(),p.get_valid_Matrix());
  cout<<s.vSet.size()<<endl;
  //print_vSet(s);
}


