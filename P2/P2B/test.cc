#include "test.h"
#include "BTreeNode.h"
#include <iostream>
using namespace std;

void Tests::runTests() {
  testBTLeafNode();
  cout << "### BTLeafNode tests done" << endl;
  cout << endl;
  cout << "***********************************************************************" << endl;
  cout << endl;
  testBTNonLeafNode();
  cout << "### BTNonLeafNode tests done" << endl;
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
  
  cout << "### Test get/set NextNodePtr" << endl;
  node.setNextNodePtr(666);
  cout << "node's next node ptr = " << node.getNextNodePtr() << endl;
  
  cout << "### BT Leaf Node tests done" << endl;
}

void Tests::testBTNonLeafNode(){
  BTNonLeafNode node;
  cout << "node.maxKeyCount = " << node.maxKeyCount << endl;
  cout << "node.size = " << node.keyPidSize << endl;
  cout << "### Test initializeRoot:" << endl;
  node.initializeRoot(0,1,1);
  printBuffer(node.buffer, sizeof(PageId), node.keyPidSize, 1);

  cout << "### Test insert:" << endl;
  PageId pid = 4;
  for (int i = 5; i > 1; i--) {
    node.insert(i, pid--);
    cout << "keycount = " << node.getKeyCount() << endl;
  }
  for (int i = 6; i <= node.maxKeyCount - 5; i++) {
    node.insert(i, 5 + pid++);
  }
  for (int i = node.maxKeyCount-2; i <= node.maxKeyCount+2; i++) {
    node.insert(i, 7 + pid++);
  }
  printBuffer(node.buffer, sizeof(PageId), node.keyPidSize, node.maxKeyCount);

  cout << "### Test split insert" << endl;
  int insertKey = node.maxKeyCount-4;
  BTNonLeafNode siblingNode;
  int midKey = 0;
  node.insertAndSplit(insertKey, pid, siblingNode, midKey);
  cout << "midKey key = " << midKey << endl;
  printBuffer(node.buffer, sizeof(PageId), node.keyPidSize, node.maxKeyCount);
  cout << "node.keycount = " << node.getKeyCount() << endl;
  printBuffer(siblingNode.buffer, sizeof(PageId), siblingNode.keyPidSize, siblingNode.maxKeyCount);
  cout << "sibling.keycount = " << siblingNode.getKeyCount() << endl;

  cout << "### Test locateChildPtr" << endl;
  PageId childPid = -1;
  siblingNode.locateChildPtr(124 , childPid);
  cout << "childPid = " << childPid << endl;
}

/*void Tests::printBuffer(char *buffer, int s1, int s, int count) {
  cout << "### Print buffer: " << endl;
  char *ptr = buffer;
  for (int i = 0; i < count + 1; i++) {
    //if (*ptr == 0) break;
    cout << (PageId)(*ptr) << " ";
    cout << (int)(*(ptr+sizeof(PageId))) << " ";
    ptr += s;
  }
  cout << endl << "### Print end" << endl;
}*/

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