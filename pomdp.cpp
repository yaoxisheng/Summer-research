#include<vector>
#include<list>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include"pomdp.h"

using namespace std;

errorPomdp e;

pomdp::pomdp(string file){
  read_file(file);
}

void pomdp::read_file(string file){
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
  valid_Matrix.resize(size_A);
  for(int i=0;i<valid_Matrix.size();i++){
    valid_Matrix[i].resize(size_Z);
    for(int j=0;j<valid_Matrix[i].size();j++){
      valid_Matrix[i][j] = false;
    }
  }
  T_Matrix.resize(size_A);
  for(int i=0;i<size_A;){
    T_Matrix[i].resize(size_Z);    
    for(int j=0;j<size_Z;){
      T_Matrix[i][j].sMat.resize(size_S);
      for(int k=0;k<size_S;){
        getline(iFile,tempLine);
        if(tempLine.empty()) continue;
        T_Matrix[i][j].sMat[k].resize(size_S);       
        iStream.clear();     
        iStream.str(tempLine);
        for(int l=0;l<size_S;l++){
          iStream>>T_Matrix[i][j].sMat[k][l];          
          if(T_Matrix[i][j].sMat[k][l]!=0 && valid_Matrix[i][j]==false){
            valid_Matrix[i][j] = true;
          }
        }
        k++;
      }
      j++;
    }    
    i++;
  }
  r_Matrix.resize(size_A);
  for(int i=0;i<size_A;){
    getline(iFile,tempLine);
    if(tempLine.empty()) continue;
    r_Matrix[i].sVec.resize(size_S);    
    iStream.clear();     
    iStream.str(tempLine);
    for(int j=0;j<size_S;j++){
      iStream>>r_Matrix[i].sVec[j];
    }
    i++;
  }
  iFile.close(); 
}

vector<vector<sMatrix> > pomdp::get_T_Matrix(){
  return T_Matrix;
}

vector<sVector> pomdp::get_r_Matrix(){
  return r_Matrix;
}

vector<vector<bool> > pomdp::get_valid_Matrix(){
  return valid_Matrix;
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
  cout<<endl<<endl;
  for(int i=0;i<valid_Matrix.size();i++){
    for(int j=0;j<valid_Matrix[i].size();j++){
      cout<<valid_Matrix[i][j]<<" ";
    }
    cout<<endl;
  }
}

vectorSet update_vectorSet(const vectorSet &B, float gamma, const vector<vector<sMatrix> > T_Matrix,
                           const vector<sVector> r_Matrix, const vector<vector<bool> > &valid_Matrix){
  vector<vector<vectorSet> > B_p;
  vector<vectorSet> B_c;
  B_p = projection(B,gamma,T_Matrix,r_Matrix,valid_Matrix);  
  B_c = cross_sum(B_p);  
  return vSet_union(B_c);
}

vector<vector<vectorSet> > projection(const vectorSet &B, float gamma, const vector<vector<sMatrix> > &T_Matrix,
                                      const vector<sVector> &r_Matrix, const vector<vector<bool> > &valid_Matrix){
  vector<vector<vectorSet> > B_p;
  B_p.resize(T_Matrix.size());
  for(int i=0;i<T_Matrix.size();i++){
    B_p[i].resize(T_Matrix[i].size());
    for(int j=0;j<T_Matrix[i].size();j++){
      if(!valid_Matrix[i][j]) continue;
      B_p[i][j] = projection_list(B,gamma,T_Matrix[i][j],r_Matrix[i],T_Matrix[i].size());
      /* eliminate duplicate */      
      B_p[i][j].vSet_unique();
      /* set index */
      B_p[i][j].set_index(i,j);      
    }
  }
  return B_p;
}

vectorSet projection_list(const vectorSet &B, float gamma, const sMatrix &sMat,
                          const sVector &sVec, int size_Z){
  vectorSet vSet_a_z;
  list<sVector>::iterator itr_vSet;
  list<sVector>::const_iterator itr_vSet2; 
  vSet_a_z.vSet.resize(B.vSet.size());
  itr_vSet = vSet_a_z.vSet.begin();
  itr_vSet2 = B.vSet.begin();  
  for(int i=0;i<B.vSet.size();i++){
    itr_vSet->sVec = projection_vector(*itr_vSet2,gamma,sMat,sVec,size_Z);
    itr_vSet++;
    itr_vSet2++;
  }
  return vSet_a_z;
}

vector<float> projection_vector(const sVector &b, float gamma, const sMatrix &sMat,
                                const sVector &sVec, int size_Z){
  vector<float> tempVec;
  float tempSum;
  int size_S;
  size_S = b.sVec.size();
  tempVec.resize(size_S);
  for(int i=0;i<size_S;i++){
    tempSum = 0;
    for(int j=0;j<size_S;j++){
      tempSum += sMat.sMat[i][j]*b.sVec[j];
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
    /* eliminate duplicate */
    B_c[i].vSet_unique();
    /* set index */
    B_c[i].set_index(i,-1);
  }  
  return B_c;
}

vectorSet cross_sum_list(vector<vectorSet> &B_p_a){
  vectorSet temp_vSet;
  if(B_p_a.size()==0) return temp_vSet;
  else if(B_p_a.size()==1) return B_p_a[0];
  temp_vSet = cross(B_p_a[0],B_p_a[1]);
  for(int i=0;i<B_p_a.size()-2;i++){
    temp_vSet = cross(temp_vSet,B_p_a[i+2]);
  }
  return temp_vSet;
}

vectorSet cross(const vectorSet &A, const vectorSet &B){  
  vector<float> tempVec;
  list<vector<float> > tempList;
  vectorSet vSet;
  int size_S;
  if(A.vSet.empty() && !B.vSet.empty()) return B;
  if(!A.vSet.empty() && B.vSet.empty()) return A;
  if(A.vSet.empty() && B.vSet.empty()) return vSet;
  size_S = A.vSet.begin()->sVec.size();  
  tempVec.resize(size_S);
  for(auto itr=A.vSet.begin();itr!=A.vSet.end();++itr){    
    for(auto itr2=B.vSet.begin();itr2!=B.vSet.end();++itr2){
      for(int i=0;i<size_S;i++){
        tempVec[i] = itr->sVec[i] + itr2->sVec[i];
      }
      tempList.push_back(tempVec);
    }
  }
  vSet.set_vSet(tempList);
  return vSet;
}

vectorSet vSet_union(vector<vectorSet> &B_c){
  vectorSet temp_vSet;
  for(int i=0;i<B_c.size();i++){
    for(auto itr=B_c[i].vSet.begin();itr!=B_c[i].vSet.end();++itr){
      temp_vSet.vSet.push_back(*itr);
    }
  }
  /* eliminate duplicate */
  temp_vSet.vSet_unique();
  return temp_vSet;
}

void print_vSet(const vectorSet &v){
  for(auto itr=v.vSet.begin();itr!=v.vSet.end();++itr){
    for(int i=0;i<itr->sVec.size();i++){
      cout<<itr->sVec[i]<<" ";
    }
    cout<<endl;
  }
}
