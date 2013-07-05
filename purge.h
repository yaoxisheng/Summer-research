#ifndef __PURGE_H_INCLUDED__
#define __PURGE_H_INCLUDED__

#include"vectorSet.h"
#include"lp_lib.h"

using namespace std;

struct parameter{
  lprec* lp;
  int row_no;
  int range;
  float epsilon;
  int thread_id;
};

vectorSet purge(vectorSet &F, int cores, float epsilon);
void purge1(vectorSet &F, int cores, float epsilon);
vector<bool> test_dominance_excluded(lprec *lp_copy, int row_no, int range, float epsilon);
void *test_dominance_excluded_helper(void *arg);
vectorSet purge2(vectorSet &F, float epsilon);
sVector test_dominance(const sVector &b, const vectorSet &F, float epsilon);
bool setRow(lprec *lp, int row_no, REAL *row, int con_type, REAL rh_value);
bool getRow(lprec *lp, int row_no, REAL *row, int *con_type, REAL *rh_value);
void free_mem(lprec *lp, REAL *row);

#endif
