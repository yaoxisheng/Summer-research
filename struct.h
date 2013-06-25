#ifndef __STRUCT_H_INCLUDED__
#define __STRUCT_H_INCLUDED__

#include<vector>
#include<cmath>

using namespace std;

typedef vector<float> sVector;
typedef vector<vector<float> > sMatrix;

struct sNode{
  sVector sVec;
  int index_a;
  int index_z;
};

bool isEqual(const sVector &a, const sVector &b, float epsilon);
bool comp(const sNode &a, const sNode &b);
bool pred(const sNode &a, const sNode &b);

#endif
