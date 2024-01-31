#include <iostream>
using namespace std;

class Test {
public:
  int a;
  char b;
};

void InterestingProcedure() {
  cout << " Calling function " << __func__ << endl;
  Test* test = new Test();
  delete(test);
}

int main() { InterestingProcedure(); }
