#ifndef ASSIGNMENT_1_SRC_PHASE2_H_
#define ASSIGNMENT_1_SRC_PHASE2_H_

#include <iostream>
#include <vector>
#include "LabelTable.h"
#include "MipsVar.h"
#include "MipsRCom.h"
#include "MipsIcom.h"
#include "MipsJCom.h"

void translate(std::vector<std::string>& cache,
               int address,
               const LabelTable& labelTable,
               const MipsVar& mipsVar,
               const MipsRCom& mipsRCom,
               const MipsICom& mipsICom,
               const MipsJCom& mipsJCom);

std::string transRegister(auto& start, const auto& end, const MipsVar& mipsVar);
std::string convertNum(auto& start, auto& end);

void transRType(const std::string& command,
                std::string& line,
                const LabelTable& labelTable,
                const MipsVar& mipsVar,
                const MipsRCom& mipsRCom);

void transJType(const std::string& command, std::string& line, const LabelTable& labelTable, const MipsJCom& mipsJCom);

void transIType(int address,
                const std::string& command,
                std::string& line,
                const LabelTable& labelTable,
                const MipsVar& mipsVar,
                const MipsICom& mipsICom);

void Output(const std::string& outputFileDir, const std::vector<std::string>& cache);
void compare(const std::string& actOut, const std::string& expOut);

#endif //ASSIGNMENT_1_SRC_PHASE2_H_

