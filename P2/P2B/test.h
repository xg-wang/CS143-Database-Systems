#ifndef TEST_H
#define TEST_H

class Tests {
public:
  void runTests();
  void testBTLeafNode();

private:
  void printBuffer(char *buffer, int s1, int s, int count);
};

#endif // !TEST_H