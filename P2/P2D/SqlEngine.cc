/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  RecordId   rid;  // record cursor for table scanning
  BTreeIndex index; // index for table
  IndexCursor idxCursor;

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }

  bool valueHasCond = false;
  bool keyHasCond = false;
  int keyEq = -1;
  std::string valueEq = "";
  int LargerEq = -1, Larger = -1;
  int SmallerEq = -1, Smaller = -1;

  // prepross the condition for index
  for(int i = 0; i < cond.size(); i++){
    SelCond temp = cond[i];
    
    if(temp.attr == 1){
      keyHasCond = true;
      int tempValue = atoi(temp.value);   // get the key need to be compared later
      if(temp.comp == SelCond::EQ){
        keyEq = tempValue;
        break;
      }else if(temp.comp == SelCond::LT){
        if(Smaller == -1 || tempValue < Smaller){
          Smaller = tempValue;
        }
      }else if(temp.comp == SelCond::GT){
        if(Larger == -1 || tempValue > Larger){
          Larger = tempValue;
        }
      }else if(temp.comp == SelCond::LE){
        if(SmallerEq == -1 || tempValue < SmallerEq){
          SmallerEq = tempValue;
        }
      }else if(temp.comp == SelCond::GE){
        if(LargerEq == -1 || tempValue > LargerEq){
          LargerEq = tempValue;
        }
      }
    }else if(temp.attr == 2){
      valueHasCond = true;
      std::string tempValue(temp.value);   // get the value need to be compared later
      if(temp.comp == SelCond::EQ) 
      {
        if(valueEq == "" || strcmp(valueEq.c_str(), cond[i].value)==0) //if no value equality conditions have been specified yet or this condition is the same as one before
          valueEq = tempValue;
        else
          goto end_select;
      }
    }
  }
  // if index exsits and no conditions for both key and value attribute and count(*)
  if(index.open(table + ".idx", 'r') == 0 && !keyHasCond && !valueHasCond && attr == 4){
    rid.pid = rid.sid = 0;
    count = 0;
    index.locate(0, idxCursor);
    while(index.readForward(idxCursor, key, rid) == 0){
      count++;
    }
    goto end_select;
  }

  // if there exists any conditions on the key attribute, go for the index
  // else use the normal select
  if(index.open(table + ".idx", 'r') != 0 || !keyHasCond){
    // scan the table file from the beginning
    rid.pid = rid.sid = 0;
    count = 0;
    while (rid < rf.endRid()) {
      // read the tuple
      if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
          diff = key - atoi(cond[i].value);
        break;
        case 2:
          diff = strcmp(value.c_str(), cond[i].value);
        break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
          if (diff != 0) goto next_tuple;
        break;
        case SelCond::NE:
          if (diff == 0) goto next_tuple;
        break;
        case SelCond::GT:
          if (diff <= 0) goto next_tuple;
        break;
        case SelCond::LT:
          if (diff >= 0) goto next_tuple;
        break;
        case SelCond::GE:
          if (diff < 0) goto next_tuple;
        break;
        case SelCond::LE:
          if (diff > 0) goto next_tuple;
        break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }

      // move to the next tuple
      next_tuple:
      ++rid;
    }
  }else{  // it must exits conditions on key attribute
    rid.pid = rid.sid = 0;
    count = 0;

    //locate starting point
    if(keyEq != -1){
      index.locate(keyEq, idxCursor);
    }else if(LargerEq != -1){
      index.locate(LargerEq, idxCursor);
    }else if(Larger != -1){
      index.locate(Larger, idxCursor);
    }else{  // search key smaller or there is only conditions on value attribute
      index.locate(0, idxCursor);
    }

    while(index.readForward(idxCursor, key, rid) == 0){
      if(!valueHasCond && attr == 4){ // no condition on value attribute and only need to count(*)
        if(keyEq != -1 && key != keyEq) goto end_select;
        if(Larger != -1 && key <= Larger) goto end_select;
        if(LargerEq != -1 && key < LargerEq)  goto end_select;
        if(Smaller != -1 && key >= Smaller) goto end_select;
        if(SmallerEq != -1 && key > SmallerEq)  goto end_select;
        count++;
        continue;
      }

      // read the tuple
      if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
          diff = key - atoi(cond[i].value);
        break;
        case 2:
          diff = strcmp(value.c_str(), cond[i].value);
        break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
          if (diff != 0){
            if(cond[i].attr == 1) goto end_select;
            goto while_continue;
          }
        break;
        case SelCond::NE: goto while_continue;
        break;
        case SelCond::GT:
          if (diff <= 0) goto while_continue;
        break;
        case SelCond::LT:
          if (diff >= 0){
            if(cond[i].attr == 1) goto end_select;
            goto while_continue;
          }
        break;
        case SelCond::GE:
          if (diff < 0) goto while_continue;
        break;
        case SelCond::LE:
          if (diff > 0){
            if(cond[i].attr == 1) goto end_select;
            goto while_continue;
          }
        break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }
      while_continue:
      cout << "";
    }
  }
  
  end_select:
  // print matching tuple count if "select count(*)"
  if (attr == 4) {
    fprintf(stdout, "%d\n", count);
  }
  rc = 0;

  // close the table file and return
  exit_select:
  index.close();
  rf.close();
  return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  fstream     lf;
  RecordFile  rf;
  RecordId    rid;
  BTNonLeafNode node;
  BTNonLeafNode sibling;
  RC      rc;

  // open load file
  lf.open(loadfile, ios::in);
  if (!lf.is_open()) {
    fprintf(stderr, "Error: loadfile %s does not exist\n", loadfile.c_str());
    return -1;
  } 
  // open the table file
  if ((rc = rf.open(table + ".tbl", 'w')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }
  
  // parse each line
  string line, value;
  int key;
  /*
  RC error1, error2;
  PageId pid;
  int midKey;
  int i = 100;
  */
  while (getline(lf, line)) {
    if ((rc = SqlEngine::parseLoadLine(line, key, value)) < 0) {
      fprintf(stderr, "Error: loadfile parse error, line: %s\n", line.c_str());
      return rc;
    }
    rf.append(key, value, rid);
    //if(i < 227) error1 = node.insert(key, i++);
    //else  error2 = node.insertAndSplit(key, i++, sibling, midKey);
  }
  //node.locateChildPtr(50, pid);
  //cout << "locate child ptr: " << pid << endl;
  //node.initializeRoot(100, 1, 101);
  //node.print(error1);
  //sibling.print(error2);
  // finish loading, close all files
  rf.close();
  lf.close();

  return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
  const char *s;
  char        c;
  string::size_type loc;
  
  // ignore beginning white spaces
  c = *(s = line.c_str());
  while (c == ' ' || c == '\t') { c = *++s; }

  // get the integer key value
  key = atoi(s);

  // look for comma
  s = strchr(s, ',');
  if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

  // ignore white spaces
  do { c = *++s; } while (c == ' ' || c == '\t');
  
  // if there is nothing left, set the value to empty string
  if (c == 0) { 
    value.erase();
    return 0;
  }

  // is the value field delimited by ' or "?
  if (c == '\'' || c == '"') {
    s++;
  } else {
    c = '\n';
  }

  // get the value string
  value.assign(s);
  loc = value.find(c, 0);
  if (loc != string::npos) { value.erase(loc); }

  return 0;
}
