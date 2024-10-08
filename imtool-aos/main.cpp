/* AOS */

#include <iostream>
using namespace std;

int main(int argc, char * argv[]) {
  cout << "Hello, World!" << endl;
  cout << "Number of args " << argc << endl;
  for (int i = 0; i < argc; i++) { cout << "Arg " << i+1 << " is " << argv[i] << endl; }
  return 0;
}