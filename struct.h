#include<vector>

using namespace std;

struct sVector{
  vector<float> v;
};

struct sMatrix{
  vector<vector<float> > m;
};

struct sNode{
  sVector sVec;
  int index_a;
  int index_z;
};
