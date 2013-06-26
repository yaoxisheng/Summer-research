#include<iostream>
#include<vector>
#include<list>
#include"purge.h"
#include"pomdp.h"

using namespace std;

int main(int argc, char* argv[]){
  pomdp p("pomdp1");
  vectorSet F;
  list<sVector> tempList;
  vector<sVector> tempVec;
  int i=0; 
  tempVec = p.get_r_Matrix();
  tempList.assign(tempVec.begin(),tempVec.end());
  F.set_vSet(tempList);
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
    itr->index_a=i;
    itr->index_z=-1;
    i++;
  }
  cout<<"before purge:"<<endl;
  print_vSet(F);
  cout<<endl;
  purge(F);
  //F = purge2(F);
  cout<<"after purge:"<<endl;
  print_vSet(F);
}


