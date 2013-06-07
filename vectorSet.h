#include<list>
#include<vector>
#include"struct.h"

using namespace std;

class vectorSet{
  public:
    vectorSet(){};
    list<sNode> vSet;
    void set_vSet(list<sVector> vSet);
    void set_index(int index_a, int index_z);
    void vSet_unique();
    ~vectorSet(){};
};
