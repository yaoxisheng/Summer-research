#include<iostream>
#include"vectorSet.h"
#include"time_measure.h"

using namespace std;

vector<float> getVec();
void getVec2(vector<float> &v);
vector<float> *getVec3();

sVector get_sVec();
void get_sVec2(sVector &sVec);
sVector *get_sVec3();

vectorSet get_vSet();
void get_vSet2(vectorSet &temp_vSet);
vectorSet *get_vSet3();

int main(int argc, char* argv[]){
  Timer t;  
  t.timer();
  for(int i=0;i<10000;i++){
    /*
    vector<float> v,*v2;
    v = getVec();
    getVec2(v);
    v2 = getVec3();
    delete v2; */
    
    /*
    sVector sVec,*sVec2;
    sVec = get_sVec();
    get_sVec2(sVec);
    sVec2 = get_sVec3();
    delete sVec2;
    */    
    
    /*
    vectorSet vSet,*vSet2;
    vSet = get_vSet();
    get_vSet2(vSet);
    vSet2 = get_vSet3();
    delete vSet2;  */
  }
  t.stopTime();
  cout<<t.getTime()<<endl;
  return 0;
}

vector<float> getVec(){
  vector<float> v;
  v.assign(100000,2.000);
  return v;
}

void getVec2(vector<float> &v){
  v.assign(100000,2.000);  
}

vector<float> *getVec3(){
  vector<float> *v = new vector<float>(100000);
  v->assign(100000,2.000);
  return v;
}

sVector get_sVec(){
  sVector sVec;
  sVec.v.assign(100000,2.000);
  return sVec;
}

void get_sVec2(sVector &sVec){
  sVec.v.assign(100000,2.000);  
}

sVector *get_sVec3(){
  sVector *sVec = new sVector;
  sVec->v.assign(100000,2.000);
  return sVec; 
}

vectorSet get_vSet(){
  vectorSet temp_vSet;
  sVector temp_sVec;  
  temp_sVec.v.assign(10000,2.000);
  list<sVector> temp_list(30,temp_sVec);
  temp_vSet.set_vSet(temp_list);
  return temp_vSet;
}

void get_vSet2(vectorSet &temp_vSet){
  sVector temp_sVec;  
  temp_sVec.v.assign(10000,2.000);
  list<sVector> temp_list(30,temp_sVec);
  temp_vSet.set_vSet(temp_list);  
}

vectorSet *get_vSet3(){
  vectorSet *temp_vSet = new vectorSet;
  sVector temp_sVec;  
  temp_sVec.v.assign(10000,2.000);
  list<sVector> temp_list(30,temp_sVec);
  temp_vSet->set_vSet(temp_list);
  return temp_vSet;
}
