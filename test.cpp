#include<vector>
#include<list>
#include"pomdp.h"

using namespace std;

int main(int argc, char* argv[]){
  pomdp p;
  vectorSet s;
  vector<float> tempVec(9,2.0);
  list<vector<float> > tempList(3,tempVec);
  s.vSet = tempList;
  p.readfile("foo.pomdp");
  //p.show_parameter();  
  p.update_vectorSet(s,1.0);
  //p.print_vectorSet();
  return 0;
}
