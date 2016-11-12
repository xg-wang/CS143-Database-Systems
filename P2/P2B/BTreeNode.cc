#include "BTreeNode.h"
#include <iostream>

using namespace std;

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
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
	int numkeys = 0;
	int size = sizeof(RecordId) + sizeof(int);
	for (char *pointer = buffer + sizeof(RecordId); 
			 pointer < buffer + PageFile::PAGE_SIZE && (int)(*pointer) != -1; 
			 pointer += size) {
		numkeys++;
	}
	return 0; 
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 

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
{ return 0; }

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
{ return 0; }

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ return 0; }

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ return 0; }

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ return 0; }




////////////////////////////////////////////////////////////////////////////

//nonleaf node constructor
BTNonLeafNode::BTNonLeafNode()
{
	std::fill(buffer, buffer + PageFile::PAGE_SIZE, 0);
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
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
	int numkeys = 0;
	//skip the first pid
	char* pointer = buffer+4;
	int keyPidSize = sizeof(PageId) + sizeof(int);
	int key = -1;
	memcpy(&key, pointer, sizeof(int));
	while(key != -1){
		numkeys++;
		if(pointer + keyPidSize < buffer + 1024){
			pointer += keyPidSize;
			memcpy(&key, pointer, sizeof(int));
		}else{
			break;
		}
	}
	return numkeys; 
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{	
	int keyPidSize = sizeof(PageId) + sizeof(int);
	// 127: the maximum number of (key, pid) pairs that can be stored in one page
	if(getKeyCount() == 127){
		return RC_NODE_FULL;
	}
	//skip the first pid
	char* searchKeyPos = buffer + 4;
	char* tempBuffer = (char*)malloc(PageFile::PAGE_SIZE);
	std::fill(tempBuffer, tempBuffer + PageFile::PAGE_SIZE, 0);
	int iteratorKey = -1;
	memcpy(&iteratorKey, searchKeyPos, sizeof(int));
	while(iteratorKey != -1){
		if(searchKeyPos + keyPidSize < buffer + 1024 && iteratorKey < key && iteratorKey != 0){
			searchKeyPos += keyPidSize;
			memcpy(&iteratorKey, searchKeyPos, sizeof(int));
		}else{
			break;
		}
	}
	int offset = searchKeyPos - buffer;
	//copy the previous (key, pid) into tempbuffer
	memcpy(tempBuffer, buffer, offset);

	//insert the new (key, pid) into the tempbuffer
	memcpy(tempBuffer + offset, &key, sizeof(int));
	memcpy(tempBuffer + offset + sizeof(int), &pid, sizeof(PageId));

	//append the rest (key, pid) from buffer to tempbuffer
	memcpy(tempBuffer + offset + keyPidSize, buffer + offset, getKeyCount()*keyPidSize - offset + 4);

	//cope the entire (key, pid) from tempbuffer to buffer
	memcpy(buffer, tempBuffer, PageFile::PAGE_SIZE);
	free(tempBuffer);

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
	int keyPidSize = sizeof(PageId) + sizeof(int);
	if(getKeyCount() != 127){
		return RC_INVALID_FILE_FORMAT;
	}
	if(sibling.getKeyCount() != 0){
		return RC_INVALID_ATTRIBUTE;
	}

	int halfKeyPos = ((int)((getKeyCount() + 1) / 2))*keyPidSize + 4;
	int preKey = -1;
	int posKey = -1;
	memcpy(&preKey, buffer + halfKeyPos - 8, sizeof(int));
	memcpy(&posKey, buffer + halfKeyPos, sizeof(int));

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
	int key = -1;
	int keyPidSize = sizeof(PageId) + sizeof(int);
	memcpy(&key, temp, sizeof(int));
	while(key != -1){
		if(searchKey < key){
			memcpy(&pid, temp - 4, sizeof(PageId));
			return 0;
		}
		if(temp + keyPidSize < buffer + 1024){
			temp += keyPidSize;
			memcpy(&key, temp, sizeof(int));
		}else{
			break;
		}
	}
	memcpy(&pid, temp + 4, sizeof(PageId));
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
	std::fill(buffer, buffer + PageFile::PAGE_SIZE, 0);
	memcpy(buffer, &pid1, sizeof(PageId));
	RC error = insert(key, pid2);
	if(error != 0){
		return error;
	}
	return 0; 
}
