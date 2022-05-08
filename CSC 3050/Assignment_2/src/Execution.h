#ifndef ASSIGNMENT_2_SRC_EXECUTION_H_
#define ASSIGNMENT_2_SRC_EXECUTION_H_

#include "tester.h"
#include <string>
#include <set>
using namespace std;

// machine cycle
void execution(char*& dynamicDataStart_ptr, const set<int>& checkpts);

// checkpoints
set<int> checkPts(const string& path);

#endif //ASSIGNMENT_2_SRC_EXECUTION_H_
