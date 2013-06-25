#include<vector>
#include<list>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include"pomdp.h"
#include"purge.h"

using namespace std;

errorPomdp e;

float existence_epsilon=1e-6;

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
      T_Matrix[i][j].resize(size_S);
      for(int k=0;k<size_S;){
        getline(iFile,tempLine);
        if(tempLine.empty()) continue;
        T_Matrix[i][j][k].resize(size_S);       
        iStream.clear();     
        iStream.str(tempLine);
        for(int l=0;l<size_S;l++){
          iStream>>T_Matrix[i][j][k][l];          
          if(T_Matrix[i][j][k][l]!=0 && valid_Matrix[i][j]==false){
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
    r_Matrix[i].resize(size_S);    
    iStream.clear();     
    iStream.str(tempLine);
    for(int j=0;j<size_S;j++){
      iStream>>r_Matrix[i][j];
    }
    i++;
  }
  iFile.close(); 
}

int pomdp::get_size_S(){
  return size_S;
}

int pomdp::get_size_Z(){
  return size_Z;
}

int pomdp::get_size_A(){
  return size_A;
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
      for(int k=0;k<T_Matrix[i][j].size();k++){
        for(int l=0;l<T_Matrix[i][j][k].size();l++){
          cout<<fixed<<T_Matrix[i][j][k][l]<<" ";
        }
        cout<<endl;
      }
      cout<<endl;
    }
    cout<<endl;
  }
  cout<<endl<<endl;
  for(int i=0;i<r_Matrix.size();i++){
    for(int j=0;j<r_Matrix[i].size();j++){
      cout<<fixed<<r_Matrix[i][j]<<" ";
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

vectorSet update_vectorSet(const vectorSet &B, float gamma, const vector<vector<sMatrix> > &T_Matrix,
                           const vector<sVector> &r_Matrix, const vector<vector<bool> > &valid_Matrix){
  vector<vector<vectorSet> > B_p;
  vector<vectorSet> B_c;
  vectorSet B_u;
  cout<< endl <<"start projection!"<<endl<<endl;
  B_p = projection(B,gamma,T_Matrix,r_Matrix,valid_Matrix); 
  cout<< endl << "finish projection!" << endl << endl;
  cout<<"start cross sum!"<<endl<< endl;  
  B_c = cross_sum(B_p);
  cout<<endl<<"finish cross sum!"<<endl << endl;  
  cout<<"start union!"<<endl<<endl;
  B_u = vSet_union(B_c);
  cout<<endl<<"finish union!"<<endl << endl;
  return B_u;
}

vector<vector<vectorSet> > projection(const vectorSet &B, float gamma, const vector<vector<sMatrix> > &T_Matrix,
                                      const vector<sVector> &r_Matrix, const vector<vector<bool> > &valid_Matrix){
  vector<vector<vectorSet> > B_p;
  B_p.resize(T_Matrix.size());
  for(int i=0;i<T_Matrix.size();i++){
    B_p[i].resize(T_Matrix[i].size());
    for(int j=0;j<T_Matrix[i].size();j++){
      //cout<<"Projecting list at (a,z)= (" << i<< "," << j << ")" <<endl;
      if(!valid_Matrix[i][j]) {
        //cout<<"Invalid (a,z)" << endl;
        continue;
      }
      B_p[i][j] = projection_list(B,gamma,T_Matrix[i][j],r_Matrix[i],T_Matrix[i].size());      
      /* set index */
      B_p[i][j].set_index(i,j);
      //cout<<"Projected list at (a,z)= (" << i<< "," << j << ") is "<< B_p[i][j].vSet.size() <<endl<<endl;
    }
  }
  return B_p;
}

vectorSet projection_list(const vectorSet &B, float gamma, const sMatrix &T_a_z,
                          const sVector &r_a, int size_Z){
  vectorSet vSet_a_z;
  sVector temp_sVec;
  sNode temp_sNode;
  for(auto itr_vSet=B.vSet.begin();itr_vSet!=B.vSet.end();++itr_vSet){
    temp_sVec = projection_vector(itr_vSet->sVec,gamma,T_a_z,r_a,size_Z);
    /* check if the projected vector exists in the current vectorSet, if so, 
       just throw it away */
    if(check_existence(vSet_a_z,temp_sVec,existence_epsilon)) {
      //cout << "Found duplicate node" << endl;
      continue;
    }
    temp_sNode.sVec = temp_sVec;
    vSet_a_z.vSet.push_back(temp_sNode);
  }
  //cout<<"size before purge:"<<vSet_a_z.vSet.size()<<endl;
  purge(vSet_a_z);
  //cout<<"size after purge:"<<vSet_a_z.vSet.size()<<endl;
  return vSet_a_z;
}

sVector projection_vector(const sVector &b, float gamma, const sMatrix &T_a_z,
                                const sVector &r_a, int size_Z){
  sVector temp_sVec;
  float tempSum;
  int size_S;
  size_S = b.size();
  temp_sVec.resize(size_S);
  for(int i=0;i<size_S;i++){
    tempSum = 0;
    for(int j=0;j<size_S;j++){
      tempSum += T_a_z[i][j]*b[j];
    }
    temp_sVec[i] = r_a[i]/size_Z + gamma*tempSum;
  }  
  return temp_sVec;
}

vector<vectorSet> cross_sum(const vector<vector<vectorSet> > &B_p){
  vector<vectorSet> B_c;  
  B_c.resize(B_p.size());  
  for(int i=0;i<B_p.size();i++){
    //cout<<"Crosssuming lists at a=" << i<<endl;
    B_c[i] = cross_sum_list(B_p[i]);
    /* set index */
    B_c[i].set_index(i,-1);
    //cout<<"Crosssuming list size at a=" << i << " is "<< B_c[i].vSet.size() <<endl;
  }
  return B_c;
}

vectorSet cross_sum_list(const vector<vectorSet> &B_p_a){
  vectorSet temp_vSet;
  if(B_p_a.size()==0) {
    cout << "List empty" << endl;
    return temp_vSet;
  }
  else if(B_p_a.size()==1) {
    cout << "List size is one" << endl;
    return B_p_a[0];
  }
  temp_vSet = cross(B_p_a[0],B_p_a[1]);  
  for(int i=0;i<B_p_a.size()-2;i++){
    temp_vSet = cross(temp_vSet,B_p_a[i+2]);    
  }
  //cout<<"size before purge:"<<temp_vSet.vSet.size()<<endl;
  purge(temp_vSet);
  //cout<<"size after purge:"<<temp_vSet.vSet.size()<<endl;
  return temp_vSet;
}

vectorSet cross(const vectorSet &A, const vectorSet &B){  
  sVector temp_sVec;
  sNode temp_sNode;  
  vectorSet temp_vSet;
  int size_S;
  if(A.vSet.empty() && !B.vSet.empty()) return B;  
  else if(!A.vSet.empty() && B.vSet.empty()) return A;
  else if(A.vSet.empty() && B.vSet.empty()) return temp_vSet;
  size_S = A.vSet.begin()->sVec.size();
  temp_sVec.resize(size_S);
  for(auto itr=A.vSet.begin();itr!=A.vSet.end();++itr){    
    for(auto itr2=B.vSet.begin();itr2!=B.vSet.end();++itr2){
      for(int i=0;i<size_S;i++){        
        temp_sVec[i] = itr->sVec[i] + itr2->sVec[i];
      }
      /* check if the cross summed vector exists in the current vectorSet, if so, 
         just throw it away */
      if(check_existence(temp_vSet,temp_sVec,existence_epsilon)) {
        //cout << "Found duplicate" << endl;
        continue;
      }
      temp_sNode.sVec = temp_sVec;
      temp_vSet.vSet.push_back(temp_sNode);
    }
  }  
  return temp_vSet;
}

vectorSet vSet_union(const vector<vectorSet> &B_c){
  vectorSet temp_vSet;
  for(int i=0;i<B_c.size();i++){
    for(auto itr=B_c[i].vSet.begin();itr!=B_c[i].vSet.end();++itr){
      /* check if the vector to be unioned exists in the current vectorSet, if so,
         just throw it away */
      if(check_existence(temp_vSet,itr->sVec,existence_epsilon)) {
        //cout << "Found duplicate" << endl;
        continue;
      }
      temp_vSet.vSet.push_back(*itr);
    }    
    //cout << "Unioning " <<i << endl;
    //cout<<"size before purge:"<<temp_vSet.vSet.size()<<endl;
    purge(temp_vSet);
    //cout<<"size after purge:"<<temp_vSet.vSet.size()<<endl;
  }  
  return temp_vSet;
}
