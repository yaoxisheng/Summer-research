#ifndef __POMDP_H_INCLUDED__
#define __POMDP_H_INCLUDED__

#include<vector>
#include<list>
#include<string>
#include"vectorSet.h"

using namespace std;

class errorPomdp{};

vectorSet update_vectorSet(const vectorSet &B, float gamma, const vector<vector<sMatrix> > &T_Matrix,
                           const vector<sVector> &r_Matrix, const vector<vector<bool> > &valid_Matrix);
vector<vector<vectorSet> > projection(const vectorSet &B, float gamma, const vector<vector<sMatrix> > &T_Matrix,
                                      const vector<sVector> &r_Matrix, const vector<vector<bool> > &valid_Matrix);
vectorSet projection_list(const vectorSet &B, float gamma, const sMatrix &T_a_z,
                          const sVector &r_a, int size_Z);
sVector projection_vector(const sVector &b, float gamma, const sMatrix &T_a_z,
                                const sVector &r_a, int size_Z);                                
vector<vectorSet> cross_sum(const vector<vector<vectorSet> > &B_p);
vectorSet cross_sum_list(const vector<vectorSet> &B_p_a);
vectorSet cross(const vectorSet &A, const vectorSet &B);
vectorSet vSet_union(const vector<vectorSet> &B_c);

class pomdp{
  private:
    int size_S,size_Z,size_A;
    /* |S|,|Z|,|A| */
    float gamma;
    /* some factor */
    vector<vector<sMatrix> > T_Matrix;
    /* T matrix for projection */
    vector<sVector> r_Matrix;
    /* r matrix for projection */
    vector<vector<bool> > valid_Matrix;
    /* valid matrix for projection */
  public:
    pomdp(){};
    /* create am empty pomdp class */
    pomdp(string file);
    /* create an pomdp class with parameters read from a file */
    void read_file(string file);
    /* read parameters from a file */
    int get_size_S();
    /* return the size_S */
    int get_size_Z();
    /* return the size_Z */
    int get_size_A();
    /* return the size_A */
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

#endif
