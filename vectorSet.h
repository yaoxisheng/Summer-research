#include<vector>
#include<list>

using namespace std;

struct vectorList{
  list<vector<float> > vList;
};

class errorVectorSet{};

class vectorSet{
  private:
    int size_S,size_Z,size_A;
    /* |S|,|Z|,|A| */
    float gamma;
    /* some factor */
    list<vector<float> > reward;
    /* reward: |A| vectors of size |S| */
    list<list<vector<float> > > prob_tr,prob_ob;
    /* prob_tr: |A| x |S| vectors of size |S|,
       prob_ob: |A| x |Z| vectors of size |S| */
    vector<float> alpha,belief_state;
    /* alpha: vector of size |S|,
       belief_state: vector of size |S| */
    vectorList cross(vectorList &A, vectorList &B);
    /* return cross sum of A and B */
  public:
    vectorSet(int size_S, int size_Z, int size_A, float gamma);
    /* constructor */
    void setReward(list<vector<float> > &reward);
    void setProb_tr(list<list<vector<float> > > &prob_tr);
    void setProb_ob(list<list<vector<float> > > &prob_ob);    
    void setBelief_state(vector<float> &belief_state);
    /* set parameters */
    void updateBelief_state(int action, int observation);
    /* Need random access for certain action and observation? */
    void show_parameter();
    /* print parameters */
    list<list<vectorList> > projection(list<vector<float> > &alpha);
    list<vectorList> cross_sum(list<list<vectorList> > &vSet_A_Z);
    vectorList vSet_union(list<vectorList> &vSet_A);
    /* calculate S' */
    ~vectorSet(){};
};
