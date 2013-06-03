#include<list>
#include<vector>

using namespace std;

struct sVector{
  vector<float> sVec;
  int index_a;
  int index_z;
};

class vectorSet{
  public:
    vectorSet(){};
    list<sVector> vSet;
    void set_vSet(list<vector<float> > vSet);
    void set_index(int index_a, int index_z);
    void vSet_unique();
    ~vectorSet(){};
};
