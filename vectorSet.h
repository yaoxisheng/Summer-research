#ifndef __VECTORSET_H_INCLUDED__
#define __VECTORSET_H_INCLUDED__

#include<list>
#include<vector>
#include<string>
#include"struct.h"

using namespace std;

#define INF (1.0e6)

class vectorSet{
  public:
    list<sNode> vSet;
    vectorSet(){};   
    void set_vSet(list<sVector> sVec_list);
    void set_vSet(string file);
    void set_index(int index_a, int index_z);
    void sort_unique();
    ~vectorSet(){};
};

void print_vSet(const vectorSet &v);
bool check_existence(const vectorSet &v, const sVector &sVec, float epsilon);
float weak_distance(const vectorSet &x, const vectorSet &y);

#endif
