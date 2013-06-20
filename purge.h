#ifndef __PURGE_H_INCLUDED__
#define __PURGE_H_INCLUDED__

#include"vectorSet.h"
#include"lp_lib.h"

using namespace std;

bool test_dominance(const sVector &b, const vectorSet &F);
vector<bool> test_dominance_excluded(lprec *lp_copy, list<sNode>::iterator itr, int row_no, int range);
void purge(vectorSet &F);
vectorSet purge2(vectorSet &F);
bool setRow(lprec *lp, int row_no, REAL *row, int con_type, REAL rh_value);
bool getRow(lprec *lp, int row_no, REAL *row, int *con_type, REAL *rh_value);
void free_mem(lprec *lp, REAL *row);

#endif
