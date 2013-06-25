#include<vector>
#include<list>
#include<iostream>
#include"pomdp.h"
#include"purge.h"

using namespace std;

int main(int argc, char* argv[]){
  pomdp p("pomdp1c");
  vectorSet F;
  list<sVector> tempList;
  vector<sVector> tempVec;
  tempVec = p.get_r_Matrix();
  tempList.assign(tempVec.begin(),tempVec.end());
  F.set_vSet(tempList);
  int i=0;
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
    itr->index_a=i;
    itr->index_z=-1;
    i++;
  }
  cout<<F.vSet.size()<<endl;
  print_vSet(F);
  cout<<endl;
  cout << "Purge before start of iterations" << endl;
  //cout<<"size before purge:"<<F.vSet.size()<<endl;
  purge(F);
  //cout<<"size after purge:"<<F.vSet.size()<<endl;
  cout<<F.vSet.size()<<endl;
  print_vSet(F);
  cout<<endl;

  //p.show_parameter();
  for(int i=0;i<10;i++){
    cout << endl << "Iteration = " << i << endl << endl;
    cout << "List size is " << F.vSet.size()<<endl;
    //print_vSet(F);
    cout<<endl;
    F = update_vectorSet(F,0.99,p.get_T_Matrix(),p.get_r_Matrix(),p.get_valid_Matrix());
    cout<<F.vSet.size()<<endl;
    print_vSet(F);
    cout<<endl;
  }  
}


