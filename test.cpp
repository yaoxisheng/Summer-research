#include<vector>
#include<list>
#include<iostream>
#include"vectorSet.h"

using namespace std;

int main(int argc, char* argv[]){
  list<list<vectorList> > vSet_A_Z;
  list<vectorList> vSet_A;
  vectorList vList;
  vectorSet vSet(2,3,3,0);
  vector<float> testVec(2,1.5),testVec2(2,2.5),testVec3(2,3.5),tempAlpha(2,4.5),belief_state(2,5.5);
  list<vector<float> > testList(2,testVec),testList2(3,testVec2),reward(3,testVec3),alpha(2,tempAlpha);
  list<list<vector<float> > > testProb_tr(3,testList),testProb_ob(3,testList2);
  vSet.setReward(reward);
  vSet.setProb_tr(testProb_tr);
  vSet.setProb_ob(testProb_ob);  
  vSet.setBelief_state(belief_state);  
  vSet.show_parameter();
  vSet_A_Z = vSet.projection(alpha);  
  vSet_A = vSet.cross_sum(vSet_A_Z);  
  vList = vSet.vSet_union(vSet_A); 
  for(auto itr=vSet_A_Z.begin();itr!=vSet_A_Z.end();++itr){
    for(auto itr2=itr->begin();itr2!=itr->end();++itr2){
      for(auto itr3=itr2->vList.begin();itr3!=itr2->vList.end();++itr3){
        for(auto itr4=itr3->begin();itr4!=itr3->end();++itr4){
          cout<<*itr4<<" ";
        }
        cout<<" -> ";        
      }
      cout<<"|| ";
    }
    cout<<endl;
  }
  return 0;
}
