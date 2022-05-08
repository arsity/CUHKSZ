#ifndef ASSIGNMENT_2_SRC_DATASEGMENT_H_
#define ASSIGNMENT_2_SRC_DATASEGMENT_H_

#include "tester.h"
#include <string>
#include <vector>
using namespace std;

// extract .data section into a vector
vector<string> extractData(const string& path);

// put static data into memory
int* putData(const vector<string>& cache);

// substitute '\'&'\n' into '\n'
void substituteEscapeSequence(string& str);

#endif //ASSIGNMENT_2_SRC_DATASEGMENT_H_
