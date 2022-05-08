#ifndef ASSIGNMENT_1_SRC_PHASE1_H_
#define ASSIGNMENT_1_SRC_PHASE1_H_

#include <iostream>
#include <vector>
#include <string>
#include "LabelTable.h"

void open(const std::string& sourceFile, std::vector<std::string>& src);
void scan(std::vector<std::string>& src, LabelTable& labelTable, int ADDRESS);

#endif //ASSIGNMENT_1_SRC_PHASE1_H_
