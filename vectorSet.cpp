#include<list>
#include<vector>
#include<iostream>
#include"vectorSet.h"

using namespace std;

void vectorSet::set_vSet(list<sVector> vSet){
  this->vSet.resize(vSet.size());
  list<sNode>::iterator itr;
  itr = this->vSet.begin();  
  for(auto itr2=vSet.begin();itr2!=vSet.end();++itr2){
    /* avoid copy */
    itr->sVec.v = itr2->v;
    ++itr;
  }
}

void vectorSet::set_index(int index_a, int index_z){
  for(auto itr=vSet.begin();itr!=vSet.end();++itr){
    itr->index_a = index_a;
    itr->index_z = index_z;
  }
}

void vectorSet::vSet_unique(){
  if(vSet.size()<=1) return;
  for(auto itr=vSet.begin();itr!=vSet.end();++itr){
    auto itr2=itr;
    ++itr2;
    for(;itr2!=vSet.end();){
      if(itr->sVec.v==itr2->sVec.v){
        /* accuracy */
        itr2 = vSet.erase(itr2);
      }
      else{
        ++itr2;        
      }
    }
    if(itr==vSet.end()) break;    
  }  
}
