#include<vector>
#include<list>
#include"vectorSet.h"

using namespace std;

int main(int argc, char* argv[]){
  vectorSet vSet(2,3,3,0);
  vector<float> testVec(2,1.5),testVec2(2,2.5),testVec3(2,3.5),alpha(2,4.5),belief_state(2,5.5);
  list<vector<float> > testList(2,testVec),testList2(3,testVec2),reward(3,testVec3);
  list<list<vector<float> > > testProb_tr(3,testList),testProb_ob(3,testList2);
  vSet.setReward(reward);
  vSet.setProb_tr(testProb_tr);
  vSet.setProb_ob(testProb_ob);
  vSet.setAlpha(alpha);
  vSet.setBelief_state(belief_state);
  vSet.show_parameter();
  return 0;
}
