#include<list>
#include<vector>
#include<iostream>
#include"vectorSet.h"

using namespace std;

void vectorSet::set_vSet(list<sVector> sVec_list){
  this->vSet.resize(sVec_list.size());
  list<sNode>::iterator itr;
  itr = this->vSet.begin();  
  for(auto itr2=sVec_list.begin();itr2!=sVec_list.end();++itr2){
    /* copy constructor of vector avoids copying automatically */
    itr->sVec = *itr2;
    ++itr;
  }
}

void vectorSet::set_index(int index_a, int index_z){
  for(auto itr=vSet.begin();itr!=vSet.end();++itr){
    itr->index_a = index_a;
    itr->index_z = index_z;
  }
}

/*
void vectorSet::vSet_unique(){
  if(vSet.size()<=1) return;
  for(auto itr=vSet.begin();itr!=vSet.end();++itr){
    auto itr2=itr;
    ++itr2;
    for(;itr2!=vSet.end();){
      if(isEqual(itr->sVec,itr2->sVec,0.000001)){              
        itr2 = vSet.erase(itr2);
      }
      else{
        ++itr2;        
      }
    }
    if(itr==vSet.end()) break;    
  }
}
*/

void print_vSet(const vectorSet &v){
  for(auto itr=v.vSet.begin();itr!=v.vSet.end();++itr){
    for(int i=0;i<itr->sVec.size();i++){
      cout<<itr->sVec[i]<<" ";
    }
    cout<<endl;
  }
}

bool check_existence(const vectorSet &v, const sVector &sVec, float epsilon){
  for(auto itr=v.vSet.begin();itr!=v.vSet.end();++itr){
    if(isEqual(itr->sVec,sVec,epsilon)) return true;
  }
  return false;
}
