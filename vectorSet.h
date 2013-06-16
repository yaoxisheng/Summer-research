#include<list>
#include<vector>
#include"struct.h"

using namespace std;

class vectorSet{
  public:
    list<sNode> vSet;
    vectorSet(){};   
    void set_vSet(list<sVector> vSet);
    void set_index(int index_a, int index_z);
    void vSet_unique();
    ~vectorSet(){};
};

void print_vSet(const vectorSet &v);
bool check_existence(const vectorSet &v, const sVector &sVec);
