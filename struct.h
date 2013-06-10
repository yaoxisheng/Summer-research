#include<vector>
#include<cmath>

using namespace std;

struct sVector{
  vector<float> v;
  inline bool operator == (const sVector &a) const {
    if(a.v.size()!=v.size()){
      /* couldn't compare sVectors of different size */
    return false;
    }
    for(int i=0;i<a.v.size();i++){
      /* if the absolute difference of two numbers is smaller than
         or equal to 10^-6, we consider them equal */
      if(abs(a.v[i]-v[i])>0.000001){
        return false;
      }
    }
    return true;
  };
};

struct sMatrix{
  vector<vector<float> > m;
};

struct sNode{
  sVector sVec;
  int index_a;
  int index_z;
};
