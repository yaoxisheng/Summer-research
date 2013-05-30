#include<vector>
#include<list>
#include<string>
#include"vectorSet.h"

using namespace std;

struct sMatrix{
  vector<vector<float> > sMat;
};

struct sVector{
  vector<float> sVec;
};

class errorPomdp{};

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
    vectorSet B;
    /* current vector set */
  public:
    pomdp(){};
    void readfile(string file);
    /* read parameters from a file */    
    void update_vectorSet(vectorSet B, float gamma);
    /* use the input vector set to calculate a new one and update the current one */
    void print_vectorSet();
    /* print current vector set */
    void show_parameter();
    /* print the parameters */
    ~pomdp(){};  
};
