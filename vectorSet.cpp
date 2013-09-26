#include<list>
#include<vector>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<sstream>
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

void vectorSet::set_vSet(string file){
  ifstream iFile;
  istringstream iStream;
  string tempLine;
  sVector temp_sVec;
  int size_A,size_S;
  iFile.open(file.c_str());
  if(!iFile){
    cerr<<"couldn't open the file!"<<endl;
    return;
  }
  getline(iFile,tempLine);  
  iStream.clear();
  iStream.str(tempLine);
  iStream>>size_S;
  iStream>>size_A;
  temp_sVec.resize(size_S);
  this->vSet.resize(size_A);  
  for(auto itr=this->vSet.begin();itr!=this->vSet.end();){
    getline(iFile,tempLine);
    if(tempLine.empty()) continue;
    iStream.clear();
    iStream.str(tempLine);
    for(int j=0;j<size_S;j++){
      iStream>>temp_sVec[j];
    }
    itr->sVec = temp_sVec;
    ++itr;    
  } 
}

void vectorSet::set_index(int index_a, int index_z){
  for(auto itr=vSet.begin();itr!=vSet.end();++itr){
    itr->index_a = index_a;
    itr->index_z = index_z;
  }
}

void vectorSet::sort_unique(){
  list<sNode>::iterator itr;
  vSet.sort(comp);
  itr = unique(vSet.begin(),vSet.end(),pred);
  vSet.resize(distance(vSet.begin(),itr));
}

void print_vSet(const vectorSet &v){
  for(auto itr=v.vSet.begin();itr!=v.vSet.end();++itr){
    cout<<itr->index_a<<"  "<<itr->index_z<<endl;
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

float weak_distance(const vectorSet &x, const vectorSet &y){
  float maxx, miny, maxs;
 
  if(x.vSet.begin()->sVec.size() != y.vSet.begin()->sVec.size()) {
    cerr << "Cannot compare lists with different size vectors" << endl;
    return INF;
  }
 
  maxx=-1.0*INF;
  for(auto itr=x.vSet.begin();itr!=x.vSet.end();++itr){
    miny=INF;
    for(auto itr1=y.vSet.begin();itr1!=y.vSet.end();++itr1){
      maxs=-1.0*INF;
      for(int i=0;i<itr->sVec.size();i++) {
        float d=fabs(itr->sVec[i]-itr1->sVec[i]);
        if(maxs<d) maxs=d;
      }
      if(miny>maxs) miny=maxs;
    }
    if(maxx<miny) maxx=miny;
  }
  return maxx;
}
