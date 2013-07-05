#include<iostream>
#include<string>
#include<cmath>
#include<sstream>
#include<pthread.h>
#include"purge.h"
#include"lp_lib.h"

using namespace std;

#define PURGE1 1
#define PURGE2 2

vector<vector<bool> > dominant_matrix;
pthread_mutex_t dominant_matrix_lock;
pthread_barrier_t barrier;
int purge_type = PURGE1;

vectorSet purge(vectorSet &F, int cores, float epsilon){
  if(purge_type==PURGE1){
    purge1(F,cores,epsilon);
    return F;    
  }
  else{
    vectorSet temp_vSet;
    temp_vSet = purge2(F,epsilon);
    return temp_vSet;
  }
} 
void purge1(vectorSet &F, int cores, float epsilon){
  lprec *lp;
  vector<bool> dominant_vector;
  list<sNode>::iterator itr;
  int Ncol,Nrow,Nthread,result,size_S,row_no,range,final_range,i;
  REAL *row;
  size_S = F.vSet.begin()->sVec.size();
  Ncol = size_S + 2;
  Nrow = 1+F.vSet.size();
  /* check if F is empty */
  if(F.vSet.size()<=1){
    cerr<<"F has only one element, so it is not purged"<<endl;
    return;
  }  
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
  resize_lp(lp,Nrow,Ncol);
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
    cerr<<"couldn't set the row!"<<endl;
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
      cerr<<"couldn't set the row!"<<endl;
      return;
    }
  }  
  /* set the bounds for v: -Inf to +Inf */
  if(!set_bounds(lp,Ncol-1,-get_infinite(lp),get_infinite(lp))){
    cerr<<"couldn't set the bounds!"<<endl;
    return;
  }
  /* Although not needed we also set upper bounds for x_i <=1 */
  for(i=1;i<=size_S;i++) {
    if(!set_bounds(lp,i,0.0,1.0)) {
      cerr<<"couldn't set the bound!"<<endl;
      return;
    }
  }
  /* set objective function to maximize and other solver settings */
  set_maxim(lp);  
  set_verbose(lp,IMPORTANT);
  set_scaling(lp, SCALE_NONE);
  set_add_rowmode(lp,FALSE);
  //set_epslevel(lp, EPS_MEDIUM);  // default is EPS_TIGHT
  /* Sometimes the solver runs in an infinite loop. This may be the 
     result of not scalling. To overcome that we put a timeout. */
  set_timeout(lp,10);
  /* test dominance for each vector v in F, each core tests from
     v to (v + range - 1) */
  /* initialize parameters */
  row_no = 2;
  if(cores>=F.vSet.size()){
    range = 1;
    final_range = 0;
    Nthread = F.vSet.size();
  }
  else{
    range = F.vSet.size()/cores;
    final_range = F.vSet.size() - range*(cores - 1);
    Nthread = cores;
  }
  pthread_t thread_id[Nthread];
  vector<parameter> parameter_vector(Nthread,{lp,0,range,epsilon,0});
  dominant_matrix.resize(Nthread);  
  
  pthread_attr_t attr;  // use &attr as a parameter to pthread_create
  pthread_attr_init(&attr);
  //pthread_attr_setstacksize(&attr,1024*1024); //could be used to set stack size
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
   
  pthread_barrier_init(&barrier,NULL,Nthread+1);
  pthread_mutex_init(&dominant_matrix_lock,NULL);  
  /* create and run (Nthread) threads */
  if(final_range==0){
    for(i=0;i<Nthread;i++){
      parameter_vector[i].row_no = row_no;
      parameter_vector[i].thread_id = i;      
      pthread_create(&thread_id[i],&attr,test_dominance_excluded_helper,&parameter_vector[i]);    
      row_no += range;
    }
  }
  else{
    for(i=0;i<Nthread-1;i++){      
      parameter_vector[i].row_no = row_no;
      parameter_vector[i].thread_id = i;  
      pthread_create(&thread_id[i],&attr,test_dominance_excluded_helper,&parameter_vector[i]);    
      row_no += range;      
    }
    parameter_vector[i].row_no = row_no;
    parameter_vector[i].range = final_range;
    parameter_vector[i].thread_id = i;    
    pthread_create(&thread_id[i],&attr,test_dominance_excluded_helper,&parameter_vector[i]);   
  }
  /* wait until all threads finish generating their dominant vectors */ 
  pthread_barrier_wait(&barrier);
  pthread_barrier_destroy(&barrier);
  //cout<<"barrier destroyed!"<<endl;
  /* build dominant vector from dominant matrix */
  for(int j=0;j<dominant_matrix.size();j++){
    dominant_vector.insert(dominant_vector.end(),dominant_matrix[j].begin(),dominant_matrix[j].end());
  }
  /* purge F according to the dominant vector */
  itr = F.vSet.begin();
  for(int j=0;j<dominant_vector.size();j++){
    if(!dominant_vector[j]){
      itr = F.vSet.erase(itr);
    }
    else{
      ++itr;
    }
  }
  free_mem(lp,row);
}

vector<bool> test_dominance_excluded(lprec *lp_copy, int row_no, int range, float epsilon){
  lprec *lp;
  vector<bool> dominant(range,true);
  int Ncol,Nrow,result;
  Ncol = get_Ncolumns(lp_copy);
  Nrow = get_Nrows(lp_copy);
  REAL pv[1+Nrow+Ncol];
  /* copy the model from lp_copy */
  lp = copy_lp(lp_copy);
  if(lp==NULL){
    cout<<"couldn't copy the LP!"<<endl;
    return dominant;
  }
  /* test dominance on vector v from range (row_no) to (row_no + range -1) */
  for(int i=0;i<range;i++){
    /* change the (row_no + i)th row to v = b^T*x */
    if(!set_mat(lp,row_no+i,Ncol,0.0)){
      cerr<<"couldn't set the matrix!"<<endl;
      continue;
    }
    if(!set_constr_type(lp,row_no+i,EQ)) {
      cerr<<"couldn't set the constraint type!"<<endl;
      continue;
    }
    /* solve the model */
    //print_lp(lp);    
    result = solve(lp);    
    /* modify dominant vector according to the result */
    if(result==OPTIMAL){
      if(!get_primal_solution(lp,pv)){
        cerr<<"couldn't get the solution!"<<endl;
        continue;
      }
      if(pv[0]<epsilon) {
        //cout<<"optimal but small objective"<<endl;
        dominant[i] = false;
      }
      else{
        //cout<<"optimal"<<endl;
        //cout<<"objective = "<<pv[0]<<endl;
      }
    }
    else if(result==INFEASIBLE){
      //cout<<"infeasible"<<endl;
      dominant[i] = false;
    }
    else{
      cerr<<"other than optimal/Infeasible"<<endl;
      continue;
    }
    /* restore the (row_no + i)th row */
    if(!set_mat(lp,row_no+i,Ncol,1.0)){
      cerr<<"couldn't set the matrix!"<<endl;
      continue;
    }
    if(!set_constr_type(lp,row_no+i,LE)){
      cerr<<"couldn't set the constraint type!"<<endl;
      continue;
    }
  }  
  delete_lp(lp);  
  return dominant;
}

void *test_dominance_excluded_helper(void *arg){  
  parameter *temp_parameter = (parameter*) arg;
  vector<bool> tempVec;  
  tempVec = test_dominance_excluded(temp_parameter->lp,temp_parameter->row_no,
    temp_parameter->range,temp_parameter->epsilon);
/*    
  pthread_mutex_lock(&dominant_matrix_lock);
  cout<<"own id:"<<pthread_self()<<endl;
  cout<<"thread id:"<<temp_parameter->thread_id<<" row_no:"<<temp_parameter->row_no
      <<" range:"<<temp_parameter->range<<endl;
  cout<<"dominant vector:";
  for(int i=0;i<tempVec.size();i++){
    cout<<tempVec[i]<<" ";
  }
  cout<<endl;
  pthread_mutex_unlock(&dominant_matrix_lock);
*/  
  pthread_mutex_lock(&dominant_matrix_lock);
  dominant_matrix[temp_parameter->thread_id] = tempVec;
  pthread_mutex_unlock(&dominant_matrix_lock);
  /* wait until all threads finish generating their dominant vectors */  
  pthread_barrier_wait(&barrier);
  pthread_exit(0);
}

vectorSet purge2(vectorSet &F, float epsilon){
  vectorSet W;
  sVector temp_sVec;
  int size_S,idx,max_idx;
  float max;
  vector<bool> dominant(F.vSet.size(),false);
  list<sNode>::iterator max_itr;
  size_S = F.vSet.begin()->sVec.size();
  /* check if F is empty */
  if(F.vSet.size()<=1){
    cerr<<"F has only one element, so it is not purged"<<endl;
    return F;
  }  
  /* select trivial dominant vectors in F and move them to W */
  for(int i=0;i<size_S;i++){
    idx = 0;
    max_idx = 0;
    max_itr = F.vSet.begin();
    max = max_itr->sVec[i];
    for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
      if((itr->sVec[i]>max) || (itr->sVec[i]==max) && comp2(max_itr->sVec,itr->sVec)){
        max_idx = idx;
        max_itr = itr;
        max = max_itr->sVec[i];        
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
  /* test dominance for remaining vectors in F against W, if dominant,
     move it from F to W, otherwise just remove it from F */
  while(!F.vSet.empty()){
    /* test_dominance will return the coordinate x(x_1, ... ,x_|S|) where
       the vector v dominates "most" or simply (-1.0, ... ,-1.0) if v is
       not dominant against F */
    temp_sVec = test_dominance(F.vSet.front().sVec,W,epsilon);    
    if(temp_sVec.empty()){
      /* the tested vector v is not dominant against W or some error
         happens during testing, so we just remove v from F */
      F.vSet.pop_front();
      continue;
    }
    /* with the coordinate x, we try to find the vector v' in F which dominates
       "most" at x i.e. (v')^T*(x_1, ... ,x_|S|) >= v^T*(x_1, ... ,x_|S|) for any
       v in F and move v' to W */
    max_itr = F.vSet.begin();    
    max = dot(max_itr->sVec,temp_sVec);   
    for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
      if((dot(itr->sVec,temp_sVec)>max) ||
         ((dot(itr->sVec,temp_sVec)==max) && comp2(max_itr->sVec,itr->sVec))){
        max_itr = itr;
        max = dot(max_itr->sVec,temp_sVec);
      }
    }
    W.vSet.push_back(*max_itr);
    F.vSet.erase(max_itr);    
  }
  return W;
}

sVector test_dominance(const sVector &b, const vectorSet &F, float epsilon){
  lprec *lp;  
  int Ncol,Nrow,result,size_S,i;
  REAL *row;
  sVector temp_sVec;
  size_S = F.vSet.begin()->sVec.size();
  Ncol = size_S + 2;
  Nrow = 1 + F.vSet.size();
  REAL pv[1+Nrow+Ncol];
  /* build the model and allocate space */  
  lp = make_lp(0,Ncol);
  if(lp==NULL){
    cerr<<"couldn't construct a new model!"<<endl;
    return temp_sVec;
  }  
  row = new REAL[Ncol+1];
  if(row==NULL){
    cerr<<"couldn't allocate space!"<<endl;
    return temp_sVec;
  }
  resize_lp(lp,Nrow,Ncol);
  /* set the objective function: ε */
  set_add_rowmode(lp,FALSE);  
  for(i=1;i<=size_S;i++){    
    row[i] = 0;
  }
  row[size_S+1] = 0;  
  row[size_S+2] = 1;
  if(!set_obj_fn(lp,row)){
    cerr<<"couldn't set the objective function!"<<endl;
    return temp_sVec;
  }
  /* construct the row: Σx_i = 1 */
  set_add_rowmode(lp,TRUE);
  for(i=1;i<=size_S;i++){
    row[i] = 1;
  }  
  row[size_S+1] = 0;
  row[size_S+2] = 0;
  if(!add_constraint(lp,row,EQ,1)){  
    cerr<<"couldn't set the row!"<<endl;
    return temp_sVec;
  }  
  /* construct the row: v = b^T*x */  
  for(i=1;i<=size_S;i++){
    row[i] = b[i-1];
  }  
  row[size_S+1] = -1;
  row[size_S+2] = 0;
  if(!add_constraint(lp,row,EQ,0)){  
    cerr<<"couldn't set the row!"<<endl;
    return temp_sVec;
  }  
  /* construct |F| rows: α_i^T*x - v + ε <= 0 */    
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){        
    for(i=1;i<=size_S;i++){
      row[i] = itr->sVec[i-1];
    }    
    row[size_S+1] = -1;
    row[size_S+2] = 1;
    if(!add_constraint(lp,row,LE,0)){    
      cerr<<"couldn't set the row!"<<endl;
      return temp_sVec;
    }
  }
  /* set the bounds for v: -Inf to +Inf */
  if(!set_bounds(lp,Ncol-1,-get_infinite(lp),get_infinite(lp))){
    cerr<<"couldn't set the bounds!"<<endl;
    return temp_sVec;
  }
  /* Although not needed we also set upper bounds for x_i <=1 */
  for(i=1;i<=size_S;i++) {
    if(!set_bounds(lp,i,0.0,1.0)) {
      cerr<<"couldn't set the bound!"<<endl;
      return temp_sVec;
    }
  }
  /* set objective function to maximize and other solver settings */
  set_maxim(lp);  
  set_verbose(lp,IMPORTANT);
  set_scaling(lp, SCALE_NONE);
  set_add_rowmode(lp,FALSE);
  //set_epslevel(lp, EPS_MEDIUM); // default is EPS_TIGHT
  /* Sometimes the solver runs in an infinite loop. This may be the 
     result of not scalling. To overcome that we put a timeout. */
  set_timeout(lp,10);
  /* solve the model */
  //print_lp(lp);  
  result = solve(lp); 
  if(result==OPTIMAL){
    if(!get_primal_solution(lp,pv)){
      cerr<<"couldn't get the solution!"<<endl;
      return temp_sVec;
    }
    if(pv[0]<epsilon) {
      //cout<<"optimal but small objective"<<endl;      
    }
    else{      
      //cout<<"optimal"<<endl;
      //cout<<"objective = "<<pv[0]<<endl;
      temp_sVec.assign(pv+1+Nrow,pv+1+Nrow+size_S);   
    }
  }
  else if(result==INFEASIBLE){
    //cout<<"infeasible"<<endl;    
  }
  else{
    cerr<<"other than optimal/Infeasible"<<endl;    
  }
  return temp_sVec;
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
