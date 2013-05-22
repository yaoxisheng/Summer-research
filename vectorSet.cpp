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

void vectorSet::setAlpha(vector<float> &alpha){
  if(alpha.size()!=size_S){    
    throw e;
  }
  this->alpha.resize(size_S);
  this->alpha = alpha;  
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
  
  cout<<"alpha vector:"<<endl;
  for(auto itr=alpha.begin();itr!=alpha.end();++itr){
    cout<<*itr<<" ";
  }
  cout<<endl;
  
  cout<<"belief state vector:"<<endl;
  for(auto itr=belief_state.begin();itr!=belief_state.end();++itr){
    cout<<*itr<<" ";
  }
  cout<<endl;
}















