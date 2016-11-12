#include "test.h"
#include <iostream>
using namespace std;

void Tests::runTests() {
  testBTLeafNode();
  cout << "all tests done" << endl;
}

void Tests::testBTLeafNode() {
  
  cout << "BT Leaf Node tests done" << endl;
}

void Tests::printBuffer(char *buffer, int s1, int s, int count) {
  cout << "Print buffer: " << endl;
  char *ptr = buffer + s1;
  for (int i = 0; i < count; i++) {
    if (*ptr == 0) break;
    cout << (int)(*ptr) << " ";
    ptr += s;
  }
  cout << "end" << endl;
}