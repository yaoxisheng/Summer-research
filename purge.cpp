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
  size_S = F.vSet.begin()->sVec.size();
  Ncol = size_S + 2;  
  /* build the model and allocate space */  
  lp = make_lp(0,Ncol); 
  if(lp==NULL){
    cerr<<"couldn't construct a new model!"<<endl;
    return false;
  }  
  row = new REAL[Ncol+1];
  if(row==NULL){
    cerr<<"couldn't allocate space!"<<endl;
    return false;
  }
  /* set the objective function: ε */
  set_add_rowmode(lp,FALSE);  
  for(i=1;i<=size_S;i++){    
    row[i] = 0;
  }
  row[size_S+1] = 0;  
  row[size_S+2] = 1;
  if(!set_obj_fn(lp,row)){
    cerr<<"couldn't set the objective function!"<<endl;
    return false;
  }  
  /* construct the row: Σx_i = 1 */
  set_add_rowmode(lp,TRUE);
  for(i=1;i<=size_S;i++){
    row[i] = 1;
  }  
  row[size_S+1] = 0;
  row[size_S+2] = 0;
  if(!add_constraint(lp,row,EQ,1)){  
    cerr<<"couldn't set the row"<<endl;
    return false;
  }  
  /* construct the row: v = b^T*x */  
  for(i=1;i<=size_S;i++){
    row[i] = b[i-1];
  }  
  row[size_S+1] = -1;
  row[size_S+2] = 0;
  if(!add_constraint(lp,row,EQ,0)){  
    cerr<<"couldn't set the row"<<endl;
    return false;
  }  
  /* construct |F| rows: α_i^T*x - v + ε <= 0 */    
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){        
    for(i=1;i<=size_S;i++){
      row[i] = itr->sVec[i-1];
    }    
    row[size_S+1] = -1;
    row[size_S+2] = 1;
    if(!add_constraint(lp,row,LE,0)){    
      cerr<<"couldn't set the row "<<endl;
      return false;
    }
  }  
  /* calculate the solution and print the output */
  set_add_rowmode(lp,FALSE);  
  set_maxim(lp);  
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

vector<bool> test_dominance_excluded(lprec *lp_copy, list<sNode>::iterator itr, int row_no, int range){
  lprec *lp;
  vector<bool> dominant(range,true);
  int Ncol,result,size_S,i,*temp_con_type;
  REAL *row,*temp_row,*temp_rh_value;
  size_S = itr->sVec.size();
  Ncol = get_Ncolumns(lp_copy);  
  /* copy the model from lp_copy and allocate space */
  lp = copy_lp(lp_copy);  
  row = new REAL[Ncol+1];
  temp_row = new REAL[Ncol+1];
  temp_rh_value = new REAL;
  temp_con_type = new int;
  if(!row || !temp_row || !temp_rh_value || !temp_con_type){
    cerr<<"couldn't allocate space!"<<endl;
    return dominant;
  }
  /* store the information the (row_no)th row */
  if(!getRow(lp,row_no,temp_row,temp_con_type,temp_rh_value)){
    cerr<<"couldn't get the row!"<<endl;
    return dominant;
  }
  /* test dominance on *itr, construct the row: v = b^T*x and
     set it to the (row_no)th row */
  set_add_rowmode(lp,FALSE);
  for(i=1;i<=size_S;i++){
    row[i] = itr->sVec[i-1];
  }  
  row[size_S+1] = -1;
  row[size_S+2] = 0;
  if(!setRow(lp,row_no,row,EQ,0)){
    cerr<<"couldn't set the row!"<<endl;
    return dominant;
  }  
  set_maxim(lp);  
  set_verbose(lp,IMPORTANT);
  result = solve(lp);
  if(result==INFEASIBLE){
    dominant[0] = false;
  }
  /* test dominance from *(itr + 1) to *(itr + range - 1) */
  for(int j=0;j<range-1;j++){
    /* restore the (row_no + j)th row */
    if(!setRow(lp,row_no+j,temp_row,*temp_con_type,*temp_rh_value)){
      cerr<<"couldn't set the row!"<<endl;
      return dominant;
    }
    /* store the information the (row_no + j + 1)th row */
    if(!getRow(lp,row_no+j+1,temp_row,temp_con_type,temp_rh_value)){
      cerr<<"couldn't get the row!"<<endl;
      return dominant;
    }
    /* construct the row: v = b^T*x and set it to the (row_no + j + 1)th row */
    ++itr;
    for(i=1;i<=size_S;i++){
      row[i] = itr->sVec[i-1];
    }
    row[size_S+1] = -1;
    row[size_S+2] = 0;
    if(!setRow(lp,row_no,row,EQ,0)){
      cerr<<"couldn't set the row!"<<endl;
      return dominant;
    }    
    result = solve(lp);
    if(result==INFEASIBLE){
      dominant[j+1] = false;
    }
  }
  /* free memroy */
  free_mem(lp,row);
  delete[] temp_row;
  delete temp_rh_value;
  delete temp_con_type;
  return dominant;
}

void purge(vectorSet &F){
  lprec *lp;
  vector<bool> dominant,tempVec;
  list<sNode>::iterator itr;
  int Ncol,result,size_S,row_no,i,range,final_range;
  REAL *row;  
  /* check if F is empty */
  if(F.vSet.size()<=1){
    cerr<<"couldn't purge F!"<<endl;
    return;
  }
  size_S = F.vSet.begin()->sVec.size();
  Ncol = size_S + 2;
  /* build the model and allocate space */
  lp = make_lp(0,Ncol); 
  if(lp==NULL){
    cerr<<"couldn't construct a new model!"<<endl;
    return;
  }  
  row = new REAL[Ncol+1];
  if(row==NULL){
    cerr<<"couldn't allocate space!"<<endl;
    return;
  }
  /* set the objective function: ε */
  set_add_rowmode(lp,FALSE);  
  for(i=1;i<=size_S;i++){    
    row[i] = 0;
  }
  row[size_S+1] = 0;  
  row[size_S+2] = 1;
  if(!set_obj_fn(lp,row)){
    cerr<<"couldn't set the objective function!"<<endl;
    return;
  }  
  /* construct the row: Σx_i = 1 */
  set_add_rowmode(lp,TRUE);
  for(i=1;i<=size_S;i++){
    row[i] = 1;
  }  
  row[size_S+1] = 0;
  row[size_S+2] = 0;
  if(!add_constraint(lp,row,EQ,1)){  
    cerr<<"couldn't set the row"<<endl;
    return;
  }  
  /* construct |F| rows: α_i^T*x - v + ε <= 0 */    
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){        
    for(i=1;i<=size_S;i++){
      row[i] = itr->sVec[i-1];
    }    
    row[size_S+1] = -1;
    row[size_S+2] = 1;
    if(!add_constraint(lp,row,LE,0)){    
      cerr<<"couldn't set the row "<<endl;
      return;
    }
  }
  /* test dominance for each vector in F, every time test from
     *itr to *(itr + range - 1) */
  row_no = 2;
  range = 2;
  final_range = F.vSet.size()%range;
  itr = F.vSet.begin();
  for(int j=0;j<F.vSet.size()/range;j++){
    tempVec = test_dominance_excluded(lp,itr,row_no,range);
    dominant.insert(dominant.end(),tempVec.begin(),tempVec.end());    
    row_no += range;
    for(int k=0;k<range;k++){
      ++itr;
    } 
  }
  if(final_range!=0){
    tempVec = test_dominance_excluded(lp,itr,row_no,final_range);
    dominant.insert(dominant.end(),tempVec.begin(),tempVec.end());
  }
  /*
  for(int j=0;j<dominant.size();j++){
    cout<<dominant[j]<<" ";
  }
  cout<<endl;
  */
  /* purge F according to the dominant vector */
  itr = F.vSet.begin();
  for(int j=0;j<dominant.size();j++){
    if(!dominant[j]){
      itr = F.vSet.erase(itr);
    }
    else{
      ++itr;
    }
  }
  free_mem(lp,row);
}

vectorSet purge2(vectorSet &F){
  vectorSet W;
  int size_S,idx,max_idx;
  float max;
  vector<bool> dominant;  
  /* check if F is empty */
  if(F.vSet.size()<=1){
    cerr<<"couldn't purge F!"<<endl;
    return F;
  }
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

bool setRow(lprec *lp, int row_no, REAL *row, int con_type, REAL rh_value){
  if(!set_row(lp,row_no,row)){
    cerr<<"couldn't set the row!"<<endl;
    return false;
  }
  if(!set_constr_type(lp,row_no,con_type)){
    cerr<<"couldn't set the constraint type!"<<endl;
    return false;
  }
  if(!set_rh(lp,row_no,rh_value)){
    cerr<<"couldn't set the right hand side value!"<<endl;
    return false;
  }
  return true;
}

bool getRow(lprec *lp, int row_no, REAL *row, int *con_type, REAL *rh_value){
  if(!get_row(lp,row_no,row)){
    cerr<<"couldn't get the row!"<<endl;
    return false;
  }
  *con_type = get_constr_type(lp,row_no);
  if(*con_type==-1){
    cerr<<"couldn't get the constraint type!"<<endl;
    return false;
  }
  *rh_value = get_rh(lp,row_no);
  return true;  
}

void free_mem(lprec *lp, REAL *row){
  if(row!=NULL){
    delete[] row;
  }
  if(lp!=NULL){  
    delete_lp(lp);
  }
}
