#include<vector>
#include<list>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include"pomdp.h"

using namespace std;

errorPomdp e;

vector<vector<vectorSet> > projection(const vectorSet &B, const vector<vector<sMatrix> > &T_Matrix,
                                      const vector<sVector> &r_Matrix, float gamma);
vectorSet projection_list(const vectorSet &B, const sMatrix &sMat, const sVector &sVec,
                          float gamma, int size_Z);
vector<float> projection_vector(const vector<float> &b, const sMatrix &sMat, const sVector &sVec,
                                float gamma, int size_Z);                                
vector<vectorSet> cross_sum(vector<vector<vectorSet> > &B_p);
vectorSet cross_sum_list(vector<vectorSet> &B_p_a);
vectorSet cross(const vectorSet &A, const vectorSet &B);
vectorSet vSet_unoin(vector<vectorSet> &B_c);

void pomdp::readfile(string file){
  ifstream iFile;
  istringstream iStream;
  string tempLine;  
  iFile.open(file.c_str());
  if(!iFile){
    throw e;
  }
  getline(iFile,tempLine);  
  iStream.clear();
  iStream.str(tempLine);
  iStream>>size_S;
  iStream>>size_A;
  iStream>>size_Z;
  getline(iFile,tempLine);  
  T_Matrix.resize(size_A);
  for(int i=0;i<size_A;i++){
    T_Matrix[i].resize(size_Z);
    for(int j=0;j<size_Z;j++){
      T_Matrix[i][j].sMat.resize(size_S);
      for(int k=0;k<size_S;k++){
        T_Matrix[i][j].sMat[k].resize(size_S);
        getline(iFile,tempLine);        
        iStream.clear();     
        iStream.str(tempLine);
        for(int l=0;l<size_S;l++){
          iStream>>T_Matrix[i][j].sMat[k][l];
        }
      }
      getline(iFile,tempLine);      
    }    
    getline(iFile,tempLine);    
  }
  getline(iFile,tempLine);
  getline(iFile,tempLine);
  r_Matrix.resize(size_A);
  for(int i=0;i<size_A;i++){
    r_Matrix[i].sVec.resize(size_S);
    getline(iFile,tempLine);
    iStream.clear();     
    iStream.str(tempLine);
    for(int j=0;j<size_S;j++){
      iStream>>r_Matrix[i].sVec[j];
    }
  }
  iFile.close();
}

void pomdp::update_vectorSet(vectorSet B, float gamma){
  vector<vector<vectorSet> > B_p;
  vector<vectorSet> B_c;
  B_p = projection(B,T_Matrix,r_Matrix,gamma);
  //cout<<B_p.size()<<endl;
  B_c = cross_sum(B_p);
  //cout<<B_c.size()<<endl;
  this->B = vSet_unoin(B_c);
}

vector<vector<vectorSet> > projection(const vectorSet &B, const vector<vector<sMatrix> > &T_Matrix,
                                      const vector<sVector> &r_Matrix, float gamma){
  vector<vector<vectorSet> > B_p;
  B_p.resize(T_Matrix.size());
  for(int i=0;i<T_Matrix.size();i++){
    B_p[i].resize(T_Matrix[i].size());
    for(int j=0;j<T_Matrix[i].size();j++){
      B_p[i][j] = projection_list(B,T_Matrix[i][j],r_Matrix[i],gamma,T_Matrix[i].size());
      B_p[i][j].index_a = i;
      B_p[i][j].index_z = j;
    }
  }
  return B_p;
}

vectorSet projection_list(const vectorSet &B, const sMatrix &sMat, const sVector &sVec,
                          float gamma, int size_Z){
  vectorSet vSet_a_z;
  list<vector<float> >::iterator itr_vSet;
  list<vector<float> >::const_iterator itr_vSet2; 
  vSet_a_z.vSet.resize(B.vSet.size());
  itr_vSet = vSet_a_z.vSet.begin();
  itr_vSet2 = B.vSet.begin();  
  for(int i=0;i<B.vSet.size();i++){
    *itr_vSet = projection_vector(*itr_vSet2,sMat,sVec,gamma,size_Z);
    itr_vSet++;
    itr_vSet2++;    
  }
  return vSet_a_z;
}

vector<float> projection_vector(const vector<float> &b, const sMatrix &sMat, const sVector &sVec,
                                float gamma, int size_Z){
  vector<float> tempVec;
  float tempSum;
  int size_S;
  size_S = b.size();
  tempVec.resize(size_S);
  for(int i=0;i<size_S;i++){
    tempSum = 0;
    for(int j=0;j<size_S;j++){
      tempSum += sMat.sMat[i][j]*b[j];
    }
    tempVec[i] = sVec.sVec[i]/size_Z + gamma*tempSum;
  }
  return tempVec;
}

vector<vectorSet> cross_sum(vector<vector<vectorSet> > &B_p){
  vector<vectorSet> B_c;
  B_c.resize(B_p.size());
  for(int i=0;i<B_p.size();i++){
    B_c[i] = cross_sum_list(B_p[i]);
    B_c[i].index_a = i;
  }  
  return B_c;
}

vectorSet cross_sum_list(vector<vectorSet> &B_p_a){
  vectorSet temp_vSet;
  temp_vSet = cross(B_p_a[0],B_p_a[1]);
  for(int i=0;i<B_p_a.size()-2;i++){
    temp_vSet = cross(temp_vSet,B_p_a[i]);
  }
  return temp_vSet;
}

vectorSet cross(const vectorSet &A, const vectorSet &B){
  vector<float> tempVec;
  list<vector<float> > tempList;
  vectorSet vSet;
  int size_S;
  size_S = A.vSet.begin()->size();
  tempVec.resize(size_S);
  for(auto itr=A.vSet.begin();itr!=A.vSet.end();++itr){    
    for(auto itr2=B.vSet.begin();itr2!=B.vSet.end();++itr2){
      for(int i=0;i<size_S;i++){
        tempVec[i] = (*itr)[i] + (*itr2)[i];
      }
      tempList.push_back(tempVec);
    }
  }
  vSet.vSet = tempList;
  return vSet;
}

vectorSet vSet_unoin(vector<vectorSet> &B_c){
  vectorSet temp_vSet;
  for(int i=0;i<B_c.size();i++){
    for(auto itr=B_c[i].vSet.begin();itr!=B_c[i].vSet.end();++itr){
      temp_vSet.vSet.push_back(*itr);
    }
  }
  return temp_vSet;
}

void pomdp::print_vectorSet(){  
  for(auto itr=B.vSet.begin();itr!=B.vSet.end();++itr){    
    for(int i=0;i<itr->size();i++){
      cout<<(*itr)[i]<<" ";
    }
    cout<<endl;
  }
}

void pomdp::show_parameter(){
  cout<<size_S<<" "<<size_A<<" "<<size_Z<<endl;
  cout<<endl;  
  for(int i=0;i<T_Matrix.size();i++){
    for(int j=0;j<T_Matrix[i].size();j++){
      for(int k=0;k<T_Matrix[i][j].sMat.size();k++){
        for(int l=0;l<T_Matrix[i][j].sMat[k].size();l++){
          cout<<fixed<<T_Matrix[i][j].sMat[k][l]<<" ";
        }
        cout<<endl;
      }
      cout<<endl;
    }
    cout<<endl;
  }
  cout<<endl<<endl;
  for(int i=0;i<r_Matrix.size();i++){
    for(int j=0;j<r_Matrix[i].sVec.size();j++){
      cout<<fixed<<r_Matrix[i].sVec[j]<<" ";
    }
    cout<<endl;
  }
}
