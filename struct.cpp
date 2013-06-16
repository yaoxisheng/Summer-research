#include"struct.h"

using namespace std;

bool isEqual(const sVector &a, const sVector &b, float epsilon){
  if(a.size()!=b.size()){
    /* couldn't compare sVectors of different size */
    return false;
  }
  for(int i=0;i<a.size();i++){
    /* if the absolute difference of two numbers is smaller than
       or equal to epsilon, we consider them equal */
    if(abs(a[i]-b[i])>epsilon){
      return false;
    }
  }
  return true;
}
