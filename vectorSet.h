#include<vector>
#include<list>

using namespace std;

class errorVectorSet{};

class vectorSet{
  private:
    int size_S,size_Z,size_A;
    /* |S|,|Z|,|A| */
    float gamma;
    /* some factor */
    list<vector<float> > reward,vSet_A;
    /* reward: |A| vectors of size |S|,
       vSet_A: intermediate vector set for each action */
    list<list<vector<float> > > prob_tr,prob_ob,vSet_A_Z;
    /* prob_tr: |A| x |S| vectors of size |S|,
       prob_ob: |A| x |Z| vectors of size |S|,
       vSet_A_Z: intermediate vector set for each action and observation */
    vector<float> alpha,belief_state,vSet;
    /* alpha: vector of size |S|,
       belief_state: vector of size |S|,
       vSet: resulted vector set*/
  public:
    vectorSet(int size_S, int size_Z, int size_A, float gamma);
    /* constructor */
    void setReward(list<vector<float> > &reward);
    void setProb_tr(list<list<vector<float> > > &prob_tr);
    void setProb_ob(list<list<vector<float> > > &prob_ob);
    void setAlpha(vector<float> &alpha);
    void setBelief_state(vector<float> &belief_state);
    /* set parameters */
    void updateBelief_state(int action, int observation);
    /* Need random access for certain action and observation? */
    void get_parameters();
    /* get parameters */
    void projection();
    void cross_sum();
    void set_union()
    ~vectorSet(){};
};
