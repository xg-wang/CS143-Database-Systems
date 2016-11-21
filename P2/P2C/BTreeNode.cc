#include "BTreeNode.h"
#include <iostream>

using namespace std;

// Utils
//step1: size of recordid or pageid
char* binarySearchKey(char *beg, char *end, int key, int step1, int size) 
{
	beg += step1;
	end += step1;
	while (beg <= end) {
		char *mid = beg + ((end - beg)/size >> 1) * size;
		if (*reinterpret_cast<int*>(mid) > key || *mid == 0) {
			end = mid - size;
		} else if (*reinterpret_cast<int*>(mid) < key) {
			beg = mid + size;
		} else /*if (*reinterpret_cast<int*>(mid) == key)*/ {
			return mid - step1;
		}
	}
	return beg - step1;
}


BTLeafNode::BTLeafNode() 
{
	fill(buffer, buffer + PageFile::PAGE_SIZE, 0);
	size = sizeof(RecordId) + sizeof(int);
	maxKeyCount = (PageFile::PAGE_SIZE - sizeof(PageId)) / size;
}
/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
	nodePid = pid;
	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
	// binary search first key == 0
	char *beg = buffer + sizeof(RecordId);
	char *end = buffer + (maxKeyCount-1)*size + sizeof(RecordId);
	while (beg <= end) {
		char *mid = beg + ((end - beg)/size >> 1) * size;
		if (*reinterpret_cast<int*>(mid) != 0) {
			beg = mid + size;
		} else if (beg == mid) {
			return (mid - buffer - sizeof(RecordId)) / size;
		} else if (*reinterpret_cast<int*>(mid-size) == 0) {
			end = mid - size;
		} else /*if (*(mid-size) != 0)*/ {
			return (mid - buffer - sizeof(RecordId)) / size;
		}
	}
	return (beg - buffer - sizeof(RecordId)) / size; 
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	int keyCount = getKeyCount();
	int size = sizeof(RecordId) + sizeof(int);
	if (keyCount * size >= PageFile::PAGE_SIZE-sizeof(PageId)) {
		return RC_NODE_FULL;
	}
	char *pos = binarySearchKey(buffer,
															buffer + (keyCount-1)*size,
															key,
															sizeof(RecordId), size);
	memmove(pos + size, pos, buffer + keyCount*size - pos);
	memcpy(pos, &rid, sizeof(RecordId));
	memcpy(pos + sizeof(RecordId), &key, sizeof(int));
	return 0;
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	int keyCount = getKeyCount();
	if (keyCount*size < PageFile::PAGE_SIZE-sizeof(PageId) ||
			sibling.getKeyCount() > 0) {
		return RC_INVALID_ATTRIBUTE;
	}
	char *pos = binarySearchKey(buffer,
															buffer + (keyCount-1)*size,
															key,
															sizeof(RecordId), size);
  char *midPos = buffer + keyCount / 2 * size ;
  if (pos <= midPos) {
		memcpy(sibling.buffer, midPos, buffer+PageFile::PAGE_SIZE-sizeof(int)-midPos);
		memmove(pos+size, pos, midPos-pos);
		memcpy(pos, &rid, sizeof(RecordId));
		memcpy(pos+sizeof(RecordId), &key, sizeof(int));
	} else {
		memcpy(sibling.buffer, midPos+size, pos-midPos+size);
		memcpy(sibling.buffer+(pos-midPos+size), &rid, sizeof(RecordId));
		memcpy(sibling.buffer+(pos-midPos+size)+sizeof(RecordId), &key, sizeof(int));
		memcpy(sibling.buffer+(pos-midPos+2*size), pos, buffer+PageFile::PAGE_SIZE-sizeof(int)-pos);
	}
	fill(midPos+size, buffer+PageFile::PAGE_SIZE-sizeof(PageId), 0);
	siblingKey = *reinterpret_cast<int*>(sibling.buffer+sizeof(RecordId));
	// TODO: [TBD] set pid for next node here or in the BTIndex.
	// setNextNodePtr();
	return 0; 
}

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	int keyCount = getKeyCount();
	int size = sizeof(RecordId) + sizeof(int);
	if (keyCount * size >= PageFile::PAGE_SIZE-sizeof(PageId)) {
		return RC_NODE_FULL;
	}
	char *pos = binarySearchKey(buffer,
															buffer + (keyCount-1)*size,
															searchKey,
															sizeof(RecordId), size);
	eid = (pos - buffer) / size;
	return *reinterpret_cast<int*>(pos+sizeof(RecordId)) == searchKey ? 0 : RC_NO_SUCH_RECORD; 
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
	char *ptr = (buffer + eid*size);
	rid = *reinterpret_cast<RecordId*>(ptr);
	key = *reinterpret_cast<int*>(ptr+sizeof(RecordId));
	return 0; 
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
	// NOTE: this is wrong when int value is larger than 1 Byte, use cast function
	// return (PageId)*(buffer+PageFile::PAGE_SIZE-sizeof(PageId));
	return *reinterpret_cast<PageId*>(buffer+PageFile::PAGE_SIZE-sizeof(PageId));
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	*reinterpret_cast<PageId*>(buffer+PageFile::PAGE_SIZE-sizeof(PageId)) = pid;
	return 0; 
}




////////////////////////////////////////////////////////////////////////////

//nonleaf node constructor
BTNonLeafNode::BTNonLeafNode()
{
	std::fill(buffer, buffer + PageFile::PAGE_SIZE, 0);
	keyPidSize = sizeof(PageId) + sizeof(int);
	maxKeyCount = (PageFile::PAGE_SIZE - sizeof(PageId)) / keyPidSize;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
	nodePid = pid;	
	return pf.read(pid, buffer); 
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{
	return pf.write(pid, buffer); 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{
	char *beg = buffer + sizeof(PageId);
	char *end = buffer + (maxKeyCount-1)*keyPidSize + sizeof(PageId);
	while (beg <= end) {
		char *mid = beg + ((end - beg)/keyPidSize >> 1) * keyPidSize;
		if (*reinterpret_cast<int*>(mid) != 0) {
			beg = mid + keyPidSize;
		} else if (beg == mid) {
			return (mid - buffer - sizeof(PageId)) / keyPidSize;
		} else if (*reinterpret_cast<int*>(mid-keyPidSize) == 0) {
			end = mid - keyPidSize;
		} else /*if (*(mid-keyPidSize) != 0)*/ {
			return (mid - buffer - sizeof(PageId)) / keyPidSize;
		}
	}
	return (beg - buffer - sizeof(PageId)) / keyPidSize; 
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{	
	// maxKeyCount(127): the maximum number of (key, pid) pairs that can be stored in one page
	int keyCount = getKeyCount();
	if(keyCount == maxKeyCount){
		return RC_NODE_FULL;
	}
	char* pos = binarySearchKey(buffer, buffer + (keyCount - 1)*keyPidSize, 
								key, sizeof(PageId), keyPidSize);
	// move back one keyPidSize from pos to the last pageid 
	memmove(pos + keyPidSize, pos, buffer + keyCount*keyPidSize - pos);
	memcpy(pos, &pid, sizeof(PageId));
	memcpy(pos + sizeof(PageId), &key, sizeof(int));
	return 0;
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ 
	if(getKeyCount() != maxKeyCount){
		return RC_INVALID_FILE_FORMAT;
	}
	if(sibling.getKeyCount() != 0){
		return RC_INVALID_ATTRIBUTE;
	}

	int halfKeyPos = ((int)((getKeyCount() + 1) / 2))*keyPidSize + 4;
	int preKey = -1;
	int posKey = -1;
	preKey = *reinterpret_cast<int*>(buffer + halfKeyPos - 8);
	posKey = *reinterpret_cast<int*>(buffer + halfKeyPos);

	if(key < preKey){
		//copy the (key, pid) pairs after prekey to the sibling
		memcpy(sibling.buffer + 4, buffer + halfKeyPos, PageFile::PAGE_SIZE - halfKeyPos);
		//copy the pid immediately before prekey to the sibling
		memcpy(sibling.buffer, buffer + halfKeyPos - 4, sizeof(PageId));
		//copy the prekey value to midkey which will be push to the parent later
		midKey = preKey;
		//clear the buffer after prekey to be 0
		std::fill(buffer + halfKeyPos - 8, buffer + PageFile::PAGE_SIZE, 0);
		// insert the (key, pid) into buffer
		insert(key, pid);
	}else if(key > posKey){
		//copy the (key, pid) pairs after poskey to the sibling
		memcpy(sibling.buffer + 4, buffer + halfKeyPos + 8, PageFile::PAGE_SIZE - halfKeyPos - 8);
		//copy the pid immediately before poskey to the sibling
		memcpy(sibling.buffer, buffer + halfKeyPos + 4, sizeof(PageId));
		//copy the poskey value to midkey which will be push to the parent later
		midKey = posKey;
		//clear the buffer after postkey to be 0
		std::fill(buffer + halfKeyPos, buffer + PageFile::PAGE_SIZE, 0);
		// insert the (key, pid) into sibling buffer
		sibling.insert(key, pid);
	}else{
		//copy the (key, pid) pairs from poskey to the sibling
		memcpy(sibling.buffer + 4, buffer + halfKeyPos, PageFile::PAGE_SIZE - halfKeyPos);
		//set the begaining pid in sibling to be pid
		memcpy(sibling.buffer, &pid, sizeof(PageId));
		//clear the buffer from postkey to be 0
		std::fill(buffer + halfKeyPos, buffer + PageFile::PAGE_SIZE, 0);
		//copy the key value to midkey which will be push to the parent later
		midKey = key;
	}
	
	cout << "NonLeafNode split, key = " << key << ", midKey = " << midKey << endl;
	return 0; 
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{
	char* temp = buffer + 4;
	int key = 0;
	key = *reinterpret_cast<int*>(temp);
	while(key != 0){
		if(searchKey < key){
			pid = *reinterpret_cast<PageId*>(temp - 4);
			return 0;
		}
		if(temp + keyPidSize < buffer + 1024){
			temp += keyPidSize;
			key = *reinterpret_cast<int*>(temp);
		}else{
			break;
		}
	}
	pid = *reinterpret_cast<PageId*>(temp - 4);
	return 0; 
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	//std::fill(buffer, buffer + PageFile::PAGE_SIZE, 0);
	memcpy(buffer, &pid2, sizeof(PageId));
	RC error = insert(key, pid1); //insert (pid, key) pair
	if(error != 0){
		return error;
	}
	return 0; 
}
