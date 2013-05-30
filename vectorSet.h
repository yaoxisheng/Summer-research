#include<vector>
#include<list>

using namespace std;

class vectorSet{
  public:
    vectorSet(){
      index_a = -1;
      index_z = -1;
    };
    list<vector<float> > vSet;
    int index_a,index_z;
    ~vectorSet(){};
};
