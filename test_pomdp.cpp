#include<vector>
#include<list>
#include<iostream>
#include"pomdp.h"
#include"purge.h"
#include"time_measure.h"

using namespace std;

static float lp_epsilon=1e-6;
static int cores=1;

int main(int argc, char* argv[]){
  Timer t;  
  t.timer();
  
  vectorSet F;
  list<sVector> tempList;
  vector<sVector> tempVec;
  int i=0;

  if(argc!=3) {
    cerr << "Usage: test_pomdp #iterations pomdp_file_name" << endl;
    return 0;
  }

  int maxiter=atoi(argv[1]);
  char *fname=argv[2];

  pomdp p(fname);
  tempVec = p.get_r_Matrix();
  tempList.assign(tempVec.begin(),tempVec.end());
  F.set_vSet(tempList);

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
  F.sort_unique();
  F = purge(F,cores,lp_epsilon); 
  //cout<<"size after purge:"<<F.vSet.size()<<endl;

  cout << endl << "Epoch = " << 1 << endl << endl;
  cout << "Final list size is " << F.vSet.size()<<endl;  
  F.sort_unique();
  print_vSet(F);
  cout<<endl;

  //p.show_parameter();
  for(int i=2;i<=maxiter;i++){
    cout << endl << "Starting Epoch = " << i << endl << endl;
    F = update_vectorSet(F,0.99,p.get_T_Matrix(),p.get_r_Matrix(),p.get_valid_Matrix());

    cout << endl << "Epoch = " << i << endl << endl;
    cout << "Final list size is " << F.vSet.size()<<endl;
    F.sort_unique();    
    print_vSet(F);
    cout<<endl;
  }  

  //cout << "Final list" << endl;
  //print_vSet(F);
  
  t.stopTime();
  cout<<"total time:"<<t.getTime()<<endl;
}


