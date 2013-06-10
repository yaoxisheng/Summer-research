#include<iostream>
#include<string>
#include<cstdio>
#include<sstream>
#include"purge.h"
#include"lp_lib.h"

using namespace std;

bool test_dominance(const sVector &b, const vectorSet &F, int size_S){  
  lprec *lp;  
  int Ncol,result,i,*colno;  
  REAL *row;
  string str;
  ostringstream oStream;
  Ncol = size_S + 2;
  /* build the model */
  lp = make_lp(0,Ncol);
  if(lp==NULL){
    cerr<<"couldn't construct a new model!"<<endl;
    return false;
  }
  /* name the S+2 variables as: x1, ... , x|s|, v, ε */
  for(int j=1;j<=size_S;j++){
    oStream.str("");
    oStream.clear();
    oStream<<"x"<<j;
    str = oStream.str();
    char *colName = new char[str.size()];
    strcpy(colName,str.c_str());
    set_col_name(lp,j,colName);
    delete[] colName;    
  }  
  set_col_name(lp,size_S+1,"v");
  set_col_name(lp,size_S+2,"epsilon"); 
  /* allocate space for a row */
  colno = new int[Ncol];
  row = new REAL[Ncol];
  if(colno==NULL || row==NULL){
    cerr<<"not enough space!"<<endl;
    return false;
  }  
  /* construct first row: Σx_i = 1 */
  set_add_rowmode(lp,TRUE);
  i = 0;
  for(int j=1;j<=size_S;j++){
    colno[i] = j;
    row[i++] = 1;
  }
  colno[i] = size_S + 1;
  row[i++] = 0;
  colno[i] = size_S + 2;
  row[i++] = 0;
  if(!add_constraintex(lp,i,row,colno,EQ,1)){
    cerr<<"couldn't add row!"<<endl;
    return false;
  }  
  /* construct second row: v = b^T*x */
  i = 0;
  for(int j=1;j<=size_S;j++){
    colno[i] = j;
    row[i++] = b.v[j-1];
  }
  colno[i] = size_S + 1;
  row[i++] = -1;
  colno[i] = size_S + 2;
  row[i++] = 0;
  if(!add_constraintex(lp,i,row,colno,EQ,0)){
    cerr<<"couldn't add row!"<<endl;
    return false;
  }  
  /* construct |F| rows: α_i^T*x - v + ε <= 0 */    
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();++itr){
    i = 0;
    for(int j=1;j<=size_S;j++){
      colno[i] = j;
      row[i++] = itr->sVec.v[j-1];
    }
    colno[i] = size_S + 1;
    row[i++] = -1;
    colno[i] = size_S + 2;
    row[i++] = 1;
    if(!add_constraintex(lp,i,row,colno,LE,0)){
      cerr<<"couldn't add row!"<<endl;
      return false;
    }
  }  
  /* set the objective function: ε */
  set_add_rowmode(lp,FALSE);  
  i = 0;
  for(int j=1;j<=size_S;j++){
    colno[i] = j;
    row[i++] = 0;
  }
  colno[i] = size_S + 1;
  row[i++] = 0;
  colno[i] = size_S + 2;
  row[i++] = 1;  
  if(!set_obj_fnex(lp,i,row,colno)){
    cerr<<"couldn't set the objective!"<<endl;
    return false;
  }
  /* calculate the solution and print the output */  
  set_maxim(lp);
  write_LP(lp,stdout);
  set_verbose(lp,IMPORTANT);
  result = solve(lp);
  if(result==OPTIMAL){
    /* output for debugging
    cout<<"find result!"<<endl;
    cout<<"objective value: "<<get_objective(lp)<<endl;
    cout<<"variable values: ";
    get_variables(lp,row);
    for(int j=0;j<size_S;j++){      
      cout<<row[j]<<" ";
    }
    cout<<endl; */
    /* free allocated memory */
    if(row!=NULL){
      delete[] row;
    }
    if(colno!=NULL){
      delete[] colno;
    }
    if(lp!=NULL){  
      delete_lp(lp);
    }
    return true;
  }
  else{
    /* output for debugging
    cout<<"didn't find result!"<<endl; */
    /* free allocated memory */
    if(row!=NULL){
      delete[] row;
    }
    if(colno!=NULL){
      delete[] colno;
    }
    if(lp!=NULL){  
      delete_lp(lp);
    }
    return false;
  }  
}

void purge(vectorSet &F, int size_S){
  for(auto itr=F.vSet.begin();itr!=F.vSet.end();){
    if(!test_dominance(itr->sVec,F,size_S)){
      itr = F.vSet.erase(itr);
    }
    else{
      ++itr;
    }    
  }
  return;
}
