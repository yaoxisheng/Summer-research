#include<vector>
#include<list>
#include<string>
#include"vectorSet.h"

using namespace std;

struct sMatrix{
  vector<vector<float> > sMat;
};

class errorPomdp{};

vectorSet update_vectorSet(const vectorSet &B, float gamma, const vector<vector<sMatrix> > T_Matrix,
                           const vector<sVector> r_Matrix, const vector<vector<bool> > &valid_Matrix);
vector<vector<vectorSet> > projection(const vectorSet &B, float gamma, const vector<vector<sMatrix> > &T_Matrix,
                                      const vector<sVector> &r_Matrix, const vector<vector<bool> > &valid_Matrix);
vectorSet projection_list(const vectorSet &B, float gamma, const sMatrix &sMat,
                          const sVector &sVec, int size_Z);
vector<float> projection_vector(const sVector &b, float gamma, const sMatrix &sMat,
                                const sVector &sVec, int size_Z);                                
vector<vectorSet> cross_sum(vector<vector<vectorSet> > &B_p);
vectorSet cross_sum_list(vector<vectorSet> &B_p_a);
vectorSet cross(const vectorSet &A, const vectorSet &B);
vectorSet vSet_union(vector<vectorSet> &B_c);
void print_vSet(const vectorSet &v);

class pomdp{
  private:
    int size_S,size_Z,size_A;
    /* |S|,|Z|,|A| */
    float gamma;
    /* some factor */
    vector<vector<sMatrix> > T_Matrix;
    /* T Matrix for projection */
    vector<sVector> r_Matrix;
    /* r Matrix for projection */
    vector<vector<bool> > valid_Matrix;   
  public:
    pomdp(){};
    /* create am empty pomdp class */
    pomdp(string file);
    /* create an pomdp class with parameters read from a file */
    void read_file(string file);
    /* read parameters from a file */
    vector<vector<sMatrix> > get_T_Matrix();
    /* return the T Matrix */
    vector<sVector> get_r_Matrix();
    /* return the r Matrix */
    vector<vector<bool> > get_valid_Matrix();
    /* return the valid Matrix */
    void show_parameter();
    /* print the parameters */
    ~pomdp(){};  
};
