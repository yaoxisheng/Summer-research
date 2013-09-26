#include<iostream>
#include<vector>
#include<list>
#include"purge.h"
#include"pomdp.h"

using namespace std;

static float lp_epsilon = 1e-6;
static int cores = 1;

int main(int argc, char* argv[]){
  int i=0;
  vectorSet F;
  F.set_vSet("testcase1");
  F.sort_unique();
  
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
    itr->index_a=i;
    itr->index_z=-1;
    i++;
  }
  cout<<"before purge:"<<endl;
  print_vSet(F);
  cout<<endl;
  
  F = purge(F,cores,lp_epsilon);
  F.sort_unique();
  
  cout<<"after purge:"<<endl;
  print_vSet(F);
}


