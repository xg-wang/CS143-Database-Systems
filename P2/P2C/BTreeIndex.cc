/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <vector>
#include <memory>

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
  rootPid = -1;
  treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
  RC rc;
  // open the table file
  if ((rc = pf.open(indexname + ".idx", mode)) < 0) {
    fprintf(stderr, "Error: index file %s open failed.\n", indexname.c_str());
    return rc;
  }
  if (pf.endPid() != 0) {
    char buffer[PageFile::PAGE_SIZE];
    if ((rc = pf.read(0, buffer)) < 0) {
      fprintf(stderr, "Error: page file read initial data failed.\n");
      return rc;
    }
    rootPid = *reinterpret_cast<int*>(buffer);
    treeHeight = *reinterpret_cast<int*>(buffer + sizeof(int));
  } else {
    rootPid = -1;
    treeHeight = 0;
  }
  return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
  RC rc;
  char buffer[PageFile::PAGE_SIZE];
  *reinterpret_cast<int*>(buffer) = rootPid;
  *reinterpret_cast<int*>(buffer + sizeof(int)) = treeHeight;
  if ((rc = pf.write(0, buffer)) < 0) {
    fprintf(stderr, "Error: page file write leading data failed.\n");
    return rc;
  }
  // close the table file
  if ((rc = pf.close()) < 0) {
    fprintf(stderr, "Error: page file close failed.\n");
    return rc;
  }
  // cleanup
  rootPid = -1;
  treeHeight = 0;
  return 0;
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
  RC rc;

  if (key < 0) { return RC_INVALID_ATTRIBUTE; }

  if (treeHeight == 0) {
    BTLeafNode node;
    node.insert(key, rid);
    treeHeight = 1;
    rootPid = (pf.endPid() == 0 ? 1 : pf.endPid());
    return node.write(rootPid, pf); 
  }

  vector<unique_ptr<BTNonLeafNode>> nodePtrStack;
  PageId pid = rootPid;
  int currHeight = 1;
  while (currHeight < treeHeight) {
    nodePtrStack.emplace_back(new BTNonLeafNode());
    nodePtrStack.back()->read(pid, pf);
    nodePtrStack.back()->locateChildPtr(key, pid);
    currHeight++; 
  }
  // deal with leaf node
  BTLeafNode leaf;
  leaf.read(pid, pf);
  if (RC_NODE_FULL == leaf.insert(key, rid)) {
    BTLeafNode sibling;
    int siblingKey = -1;
    PageId siblingPid = pf.endPid();
    if ((rc = leaf.insertAndSplit(key, rid, sibling, siblingKey)) < 0) {
      fprintf(stderr, "Error: insertAndSplit failed.\n");
      return rc;
    }
    sibling.setNextNodePtr(leaf.getNextNodePtr());
    leaf.setNextNodePtr(siblingPid);
    // save updated sibling
    if ((rc = sibling.write(siblingPid, pf)) < 0) {
      fprintf(stderr, "Error: writing splited sibling leaf to page file failed.\n");
      return rc;
    }
    // insert back the key to parents
    PageId pid1; // for root init
    while (nodePtrStack.size() > 0 && 
           RC_NODE_FULL == nodePtrStack.back()->insert(siblingKey, siblingPid)) 
    {
      BTNonLeafNode currSibling;
      int midKey = -1;
      PageId currSiblingPid = pf.endPid();
      if ((rc = nodePtrStack.back()->insertAndSplit(siblingKey, siblingPid, currSibling, midKey)) < 0) {
        fprintf(stderr, "Error: insertAndSplit Nonleaf failed.\n");
        return rc;
      }
      // save changes
      if ((rc = nodePtrStack.back()->write(nodePtrStack.back()->getPid(), pf)) < 0 ||
          (rc = currSibling.write(currSiblingPid, pf)) < 0) 
      {
        fprintf(stderr, "Error: writing to page file failed.\n");
        return rc;
      }
      // update node info for next insertion
      siblingKey = midKey;
      siblingPid = currSiblingPid;
      pid1 = nodePtrStack.back()->getPid(); 
      nodePtrStack.pop_back();
    }
    // if we need a new root
    if (nodePtrStack.size() < 1) {
      BTNonLeafNode newRoot;
      newRoot.initializeRoot(pid1, siblingKey, siblingPid);
      if ((rc = newRoot.write(pf.endPid(), pf)) < 0) {
        fprintf(stderr, "Error: writing leaf to page file failed.\n");
        return rc;
      }
      rootPid = pf.endPid();
      treeHeight++;
    }
  } // if (RC_NODE_FULL == leaf.insert(key, rid))
  // save updated leaf anyway
  if ((rc = leaf.write(pid, pf)) < 0) {
    fprintf(stderr, "Error: writing leaf to page file failed.\n");
    return rc;
  }

  return 0;
}

/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
  return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
  return 0;
}
