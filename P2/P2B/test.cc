#include "test.h"
#include "BTreeNode.h"
#include <iostream>
using namespace std;

void Tests::runTests() {
  testBTLeafNode();
  cout << "### all tests done" << endl;
}

void Tests::testBTLeafNode() {
  BTLeafNode node;
  cout << "node.maxKeyCount = " << node.maxKeyCount << endl;
  cout << "node.size = " << node.size << endl;

  cout << "### Test insert:" << endl;
  RecordId rid;
  rid.pid = 0; rid.sid = 0;
  for (int i = 5; i > 0; i--) {
    node.insert(i, rid);
    cout << "keycount = " << node.getKeyCount() << endl;
  }
  for (int i = 6; i <= node.maxKeyCount-5; i++) {
    node.insert(i, rid);
  }
  for (int i = node.maxKeyCount-3; i <= node.maxKeyCount+1; i++) {
    node.insert(i, rid);
  }
  printBuffer(node.buffer, sizeof(RecordId), node.size, node.maxKeyCount);
  
  cout << "### Test split insert" << endl;
  int insertKey = node.maxKeyCount-4;
  BTLeafNode siblingNode;
  int siblingKey = 0;
  node.insertAndSplit(insertKey, rid, siblingNode, siblingKey);
  cout << "sibling key = " << siblingKey << endl;
  printBuffer(node.buffer, sizeof(RecordId), node.size, node.maxKeyCount);
  printBuffer(siblingNode.buffer, sizeof(RecordId), siblingNode.size, siblingNode.maxKeyCount);

  cout << "### Test locate & readEntry" << endl;
  int searchKey = 33;
  int eid = -1;
  node.locate(searchKey, eid);
  cout << "eid for " << searchKey << " = " << eid << endl;
  int key;
  RecordId rid2;
  node.readEntry(eid, key, rid2);
  cout << "readEntry: key = " << key 
       << ", rid = [" << rid2.pid << ", " << rid2.sid << "]" 
       << endl; 
  
  cout << "### BT Leaf Node tests done" << endl;
}

void Tests::printBuffer(char *buffer, int s1, int s, int count) {
  cout << "### Print buffer: " << endl;
  char *ptr = buffer + s1;
  for (int i = 0; i < count; i++) {
    if (*ptr == 0) break;
    cout << (int)(*ptr) << " ";
    ptr += s;
  }
  cout << endl << "### Print end" << endl;
}