#include<iostream>
#include<string>
#include<cstdio>
#include<sstream>
#include"purge.h"
#include"lp_lib.h"

using namespace std;

bool test_dominance(const sVector &b, const vectorSet &F){  
  lprec *lp;  
  int Ncol,result,size_S,i;
  REAL *row;
  /* check if F or b is empty */
  size_S = F.vSet.begin()->sVec.size();
  Ncol = size_S + 2;  
  /* build the model */  
  lp = make_lp(0,Ncol); 
  if(lp==NULL){
    cerr<<"couldn't construct a new model!"<<endl;
    return false;
  } 
  /* allocate space for a row */  
  row = new REAL[Ncol+1];
  if(row==NULL){
    cerr<<"couldn't allocate space for a row!"<<endl;
    return false;
  }
  /* set the objective function: ε */
  set_add_rowmode(lp,FALSE);  
  for(i=1;i<=size_S;i++){    
    row[i] = 0;
  }
  row[i++] = 0;  
  row[i++] = 1;
  if(!set_obj_fn(lp,row)){
    cerr<<"couldn't set the objective function!"<<endl;
    return false;
  }  
  /* construct the row: Σx_i = 1 */
  set_add_rowmode(lp,TRUE);
  for(i=1;i<=size_S;i++){
    row[i] = 1;
  }  
  row[i++] = 0;
  row[i++] = 0;
  if(!add_constraint(lp,row,EQ,1)){  
    cerr<<"couldn't set the row"<<endl;
    return false;
  }  
  /* construct the row: v = b^T*x */  
  for(i=1;i<=size_S;i++){
    row[i] = b[i-1];
  }  
  row[i++] = -1;
  row[i++] = 0;
  if(!add_constraint(lp,row,EQ,0)){  
    cerr<<"couldn't set the row"<<endl;
    return false;
  }  
  /* construct |F| rows: α_i^T*x - v + ε <= 0 */    
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){        
    for(i=1;i<=size_S;i++){
      row[i] = itr->sVec[i-1];
    }    
    row[i++] = -1;
    row[i++] = 1;
    if(!add_constraint(lp,row,LE,0)){    
      cerr<<"couldn't set the row "<<endl;
      return false;
    }
  }  
  /* calculate the solution and print the output */
  set_add_rowmode(lp,FALSE);  
  set_maxim(lp);
  write_LP(lp,stdout);  
  set_verbose(lp,IMPORTANT);  
  result = solve(lp); 
  if(result==OPTIMAL){    
    /* free allocated memory */
    free_mem(lp,row);
    return true;
  }
  else if(result==INFEASIBLE){    
    /* free allocated memory */
    free_mem(lp,row);
    return false;
  }
  else{
    /* handle unexpected exceptions */
    free_mem(lp,row);
    return true;
  }
}

bool test_dominance_excluded(lprec *lp_copy, const sVector &b, int row_no){
  lprec *lp;  
  int Ncol,result,size_S,i;
  REAL *row;
  size_S = b.size();
  Ncol = get_Ncolumns(lp_copy);
  /* build the model */ 
  lp = copy_lp(lp_copy);
  /* delete the row which should be excluded */
  if(!del_constraint(lp,row_no)){
    cerr<<"couldn't delete the row!"<<endl;
    return false;
  }
  /* allocate space for a row */  
  row = new REAL[Ncol+1];
  if(row==NULL){
    cerr<<"couldn't allocate space for a row!"<<endl;
    return false;
  }
  /* construct the row: v = b^T*x */  
  for(i=1;i<=size_S;i++){
    row[i] = b[i-1];
  }  
  row[i++] = -1;
  row[i++] = 0;
  if(!add_constraint(lp,row,EQ,0)){  
    cerr<<"couldn't set the row"<<endl;
    return false;
  }
  /* calculate the solution and print the output */
  set_add_rowmode(lp,FALSE);  
  set_maxim(lp);
  write_LP(lp,stdout);  
  set_verbose(lp,IMPORTANT);  
  result = solve(lp);
  if(result==OPTIMAL){    
    /* free allocated memory */
    free_mem(lp,row);
    return true;
  }
  else if(result==INFEASIBLE){    
    /* free allocated memory */
    free_mem(lp,row);
    return false;
  }
  else{
    /* handle unexpected exceptions */
    free_mem(lp,row);
    return true;
  }
}

void purge(vectorSet &F){
  lprec *lp;  
  int Ncol,result,size_S,row_no,i;
  REAL *row;
  /* check if F or b is empty */
  size_S = F.vSet.begin()->sVec.size();
  Ncol = size_S + 2;  
  /* build the model */  
  lp = make_lp(0,Ncol); 
  if(lp==NULL){
    cerr<<"couldn't construct a new model!"<<endl;
    return;
  } 
  /* allocate space for a row */  
  row = new REAL[Ncol+1];
  if(row==NULL){
    cerr<<"couldn't allocate space for a row!"<<endl;
    return;
  }
  /* set the objective function: ε */
  set_add_rowmode(lp,FALSE);  
  for(i=1;i<=size_S;i++){    
    row[i] = 0;
  }
  row[i++] = 0;  
  row[i++] = 1;
  if(!set_obj_fn(lp,row)){
    cerr<<"couldn't set the objective function!"<<endl;
    return;
  }  
  /* construct the first row: Σx_i = 1 */
  set_add_rowmode(lp,TRUE);
  for(i=1;i<=size_S;i++){
    row[i] = 1;
  }  
  row[i++] = 0;
  row[i++] = 0;
  if(!add_constraint(lp,row,EQ,1)){  
    cerr<<"couldn't set the row"<<endl;
    return;
  }  
  /* construct |F| rows: α_i^T*x - v + ε <= 0 */    
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){        
    for(i=1;i<=size_S;i++){
      row[i] = itr->sVec[i-1];
    }    
    row[i++] = -1;
    row[i++] = 1;
    if(!add_constraint(lp,row,LE,0)){    
      cerr<<"couldn't set the row "<<endl;
      return;
    }
  }
  /* test dominance for each vector in F */
  row_no = 2;
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();){    
    if(!test_dominance_excluded(lp,itr->sVec,row_no)){
      itr = F.vSet.erase(itr);
    }
    else{
      ++itr;
    }
    row_no++;
  }
  free_mem(lp,row);
}

vectorSet purge2(vectorSet &F){
  vectorSet W;
  int size_S,idx,max_idx;
  float max;
  vector<bool> dominant;  
  /* check if F is empty */
  size_S = F.vSet.begin()->sVec.size();
  dominant.assign(F.vSet.size(),false);
  /* select trivial dominant vectors in F */
  for(int i=0;i<size_S;i++){
    idx = 0;
    max_idx = 0;    
    max = F.vSet.begin()->sVec[i];
    for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
      if(itr->sVec[i]>max){
        max_idx = idx;
        max = itr->sVec[i];        
      }
      idx++;
    }
    dominant[max_idx] = true;    
  }
  idx = 0;
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();){
    if(dominant[idx]){
      W.vSet.push_back(*itr);
      itr = F.vSet.erase(itr);      
    }
    else{
      ++itr;
    }
    idx++;
  }
  /* test dominance for remaining vectors */
  while(!F.vSet.empty()){
    /* see the pomdp paper to optimize */
    if(test_dominance(F.vSet.front().sVec,W)){
      W.vSet.push_back(F.vSet.front());
    }
    F.vSet.pop_front();
  }
  return W;
}

void free_mem(lprec *lp, REAL *row){
  if(row!=NULL){
    delete[] row;
  }
  if(lp!=NULL){  
    delete_lp(lp);
  }
}
