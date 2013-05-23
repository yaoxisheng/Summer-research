#include<exception>
#include<vector>
#include<list>
#include<iostream>
#include"vectorSet.h"

using namespace std;

errorVectorSet e;

vectorSet::vectorSet(int size_S, int size_Z, int size_A, float gamma){
  if(size_S<=0 || size_Z<=0 || size_A<=0 || gamma<0 || gamma>1){    
    throw e;
  }
  this->size_S = size_S;
  this->size_Z = size_Z;
  this->size_A = size_A;
  this->gamma = gamma;
}

vectorList vectorSet::cross(vectorList &A, vectorList &B){
  vector<float> tempVec(size_S);
  list<vector<float> > tempList;
  vectorList vList;
  for(auto itr=A.vList.begin();itr!=A.vList.end();++itr){
    for(auto itr2=B.vList.begin();itr2!=B.vList.end();++itr2){
      for(int i=0;i<size_S;i++){
        tempVec[i] = (*itr)[i] + (*itr2)[i];
      }
      tempList.push_back(tempVec);
    }
  }
  vList.vList = tempList;
  return vList;
}

void vectorSet::setReward(list<vector<float> > &reward){
  if(reward.size()!=size_A){    
    throw e;
  }
  list<vector<float> >::iterator itr,itr2;
  for(itr=reward.begin();itr!=reward.end();++itr){
    if(itr->size()!=size_S){      
      throw e;
    }
  }
  itr = reward.begin();
  this->reward.resize(size_A);
  for(itr2=this->reward.begin();itr2!=this->reward.end();++itr2){
    itr2->resize(size_S);
    *itr2 = *itr++;
  }
}

void vectorSet::setProb_tr(list<list<vector<float> > > &prob_tr){
  if(prob_tr.size()!=size_A){
    throw e;
  }
  list<list<vector<float> > >::iterator itr,itr3;
  list<vector<float> >::iterator itr2,itr4;
  for(itr=prob_tr.begin();itr!=prob_tr.end();++itr){
    if(itr->size()!=size_S){      
      throw e;
    }
    for(itr2=itr->begin();itr2!=itr->end();++itr2){
      if(itr2->size()!=size_S){
        throw e;
      }
    }
  }
  itr = prob_tr.begin();
  itr2 = itr->begin();
  this->prob_tr.resize(size_A);  
  for(itr3=this->prob_tr.begin();itr3!=this->prob_tr.end();++itr3){
    itr3->resize(size_S);
    for(itr4=itr3->begin();itr4!=itr3->end();++itr4){
      itr4->resize(size_S);
      *itr4 = *itr2++;
    }   
    ++itr;
    itr2 = itr->begin();
  }
}

void vectorSet::setProb_ob(list<list<vector<float> > > &prob_ob){
  if(prob_ob.size()!=size_A){
    throw e;
  }
  list<list<vector<float> > >::iterator itr,itr3;
  list<vector<float> >::iterator itr2,itr4;
  for(itr=prob_ob.begin();itr!=prob_ob.end();++itr){
    if(itr->size()!=size_Z){      
      throw e;
    }
    for(itr2=itr->begin();itr2!=itr->end();++itr2){
      if(itr2->size()!=size_S){
        throw e;
      }
    }
  }
  itr = prob_ob.begin();
  itr2 = itr->begin();
  this->prob_ob.resize(size_A);  
  for(itr3=this->prob_ob.begin();itr3!=this->prob_ob.end();++itr3){
    itr3->resize(size_Z);
    for(itr4=itr3->begin();itr4!=itr3->end();++itr4){
      itr4->resize(size_S);
      *itr4 = *itr2++;
    }   
    ++itr;
    itr2 = itr->begin();
  }
}

void vectorSet::setBelief_state(vector<float> &belief_state){
  if(belief_state.size()!=size_S){    
    throw e;
  }
  this->belief_state.resize(size_S);
  this->belief_state = belief_state;
}

void vectorSet::show_parameter(){  
  cout<<"reward matrix:"<<endl;
  for(auto itr=reward.begin();itr!=reward.end();++itr){
    for(auto itr2=itr->begin();itr2!=itr->end();++itr2){
      cout<<*itr2<<" ";
    }
    cout<<endl;
  }
  
  cout<<"transition probability matrix:"<<endl;
  for(auto itr=prob_tr.begin();itr!=prob_tr.end();++itr){
    for(auto itr2=itr->begin();itr2!=itr->end();++itr2){
      for(auto itr3=itr2->begin();itr3!=itr2->end();++itr3){
        cout<<*itr3<<" ";
      }
      cout<<"|| ";
    }
    cout<<endl;
  }
  
  cout<<"transition probability matrix:"<<endl;
  for(auto itr=prob_ob.begin();itr!=prob_ob.end();++itr){
    for(auto itr2=itr->begin();itr2!=itr->end();++itr2){
      for(auto itr3=itr2->begin();itr3!=itr2->end();++itr3){
        cout<<*itr3<<" ";
      }
      cout<<"|| ";
    }
    cout<<endl;
  }
    
  cout<<"belief state vector:"<<endl;
  for(auto itr=belief_state.begin();itr!=belief_state.end();++itr){
    cout<<*itr<<" ";
  }
  cout<<endl;
}

list<list<vectorList> > vectorSet::projection(list<vector<float> > &alpha){
  vector<float> tempVec(size_S);
  list<vector<float> > tempList;  
  list<vectorList> temp_vSet(size_Z);
  list<list<vectorList> > vSet_A_Z(size_A,temp_vSet);
  list<list<vectorList> >::iterator itr_vSet;
  list<vectorList>::iterator itr_vSet2;  
  list<list<vector<float> > >::iterator itrProb_tr,itrProb_ob;
  list<vector<float> >::iterator itrReward,itrProb_tr2,itrProb_ob2;
  float tempSum;
  itrProb_tr = prob_tr.begin();
  itrProb_ob = prob_ob.begin();
  itr_vSet = vSet_A_Z.begin();
  itrReward = reward.begin();  
  itrProb_tr2 = itrProb_tr->begin();
  itrProb_ob2 = itrProb_ob->begin();
  itr_vSet2 = itr_vSet->begin();  
  for(int i=0;i<size_A;i++){          
    for(int j=0;j<size_Z;j++){
      tempList.clear();     
      for(auto itrAlpha=alpha.begin();itrAlpha!=alpha.end();++itrAlpha){
        itrProb_tr2 = itrProb_tr->begin();
        for(int k=0;k<size_S;k++){
          tempSum = 0;          
          for(int l=0;l<size_S;l++){          
            tempSum += (*itrAlpha)[l]*(*itrProb_ob2)[l]*(*itrProb_tr2)[l];          
          }
          tempVec[k] = (*itrReward)[k]/size_Z + gamma*tempSum;          
          ++itrProb_tr2;
        }
        tempList.push_back(tempVec);        
      }      
      ++itrProb_ob2;
      itr_vSet2->vList = tempList;
      ++itr_vSet2;      
    }
    ++itrProb_tr;
    itrProb_tr2 = itrProb_tr->begin();
    ++itrProb_ob;
    itrProb_ob2 = itrProb_ob->begin();    
    ++itr_vSet;
    itr_vSet2 = itr_vSet->begin();
    ++itrReward;      
  } 
  return vSet_A_Z;
}

list<vectorList> vectorSet::cross_sum(list<list<vectorList> > &vSet_A_Z){
  list<vectorList> vSet(size_A);  
  list<vectorList>::iterator itr_vSet_A_Z2,itr_vSet_A_Z3,itr_vSet;
  vectorList temp_vList;  
  itr_vSet = vSet.begin();
  for(auto itr_vSet_A_Z=vSet_A_Z.begin();itr_vSet_A_Z!=vSet_A_Z.end();++itr_vSet_A_Z){
    itr_vSet_A_Z2 = itr_vSet_A_Z->begin();
    itr_vSet_A_Z3 = itr_vSet_A_Z->begin();
    ++itr_vSet_A_Z3;    
    temp_vList = cross(*itr_vSet_A_Z2,*itr_vSet_A_Z3);    
    for(int j=0;j<size_Z-2;j++){
      ++itr_vSet_A_Z3;
      temp_vList = cross(temp_vList,*itr_vSet_A_Z3);       
    }
    itr_vSet->vList = temp_vList.vList;
  }  
  return vSet;
}

vectorList vectorSet::vSet_union(list<vectorList> &vSet_A){
  vectorList temp_vList;
  for(auto itr_vSet=vSet_A.begin();itr_vSet!=vSet_A.end();++itr_vSet){
    for(auto itr=itr_vSet->vList.begin();itr!=itr_vSet->vList.end();++itr){
      temp_vList.vList.push_back(*itr);
    }
  }
  return temp_vList;
}
