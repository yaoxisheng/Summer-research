#include"vectorSet.h"
#include"lp_lib.h"

using namespace std;

bool test_dominance(const sVector &b, const vectorSet &F);
bool test_dominance_excluded(lprec *lp_copy, const sVector &b, int row_no);
void purge(vectorSet &F);
vectorSet purge2(vectorSet &F);
void free_mem(lprec *lp, REAL *row);
