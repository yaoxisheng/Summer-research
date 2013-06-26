#include<iostream>
#include"struct.h"

using namespace std;

float sort_unique_epsilon = 1e-6;

float dot(const sVector &a, const sVector &b){
  float temp_sum=0;
  if(a.size()!=b.size()){
    cerr<<"couldn't dot tow sVectors of different size!"<<endl;
  }
  for(int i=0;i<a.size();i++){
    temp_sum += a[i]*b[i];
  }
  return temp_sum;
}

bool isEqual(const sVector &a, const sVector &b, float epsilon){
  if(a.size()!=b.size()){
    cerr<<"couldn't compare sVectors of different size!"<<endl;
    return false;
  }
  for(int i=0;i<a.size();i++){
    /* if the absolute difference of two numbers is smaller than
       or equal to epsilon, we consider them equal */
    if(abs(a[i]-b[i])>epsilon){
      return false;
    }
  }
  return true;
}

bool comp(const sNode &a, const sNode &b){
  /* compare two sVecs of sNodes lexicographically */
  return comp2(a.sVec,b.sVec);
}

bool comp2(const sVector &a, const sVector &b){
  /* compare two sVecs lexicographically */
  auto first1=a.begin(),last1=a.end(),first2=b.begin(),last2=b.end();
  while(first1!=last1){
    if(first2==last2 || *first1-*first2>sort_unique_epsilon) return false;
    else if(*first2-*first1>sort_unique_epsilon) return true;
    ++first1;
    ++first2;
  }
  return (first2!=last2);
}

bool pred(const sNode &a, const sNode &b){
  return isEqual(a.sVec,b.sVec,sort_unique_epsilon);
}
