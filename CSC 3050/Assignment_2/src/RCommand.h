
#ifndef ASSIGNMENT_2_SRC_RCOMMAND_H_
#define ASSIGNMENT_2_SRC_RCOMMAND_H_

#include "tester.h"
#include <string>
using namespace std;

// check if the instructions is R type
bool isRCom(const string& instruction);

// instruction syscall implementation
void com_syscall(const string& instruction, char*& dynamicDataStart_ptr);

// instructions implementation
void com_add(const string& instruction);
void com_addu(const string& instruction);
void com_and(const string& instruction);
void com_div(const string& instruction);
void com_divu(const string& instruction);
void com_jalr(const string& instruction);
void com_jr(const string& instruction);
void com_mfhi(const string& instruction);
void com_mflo(const string& instruction);
void com_mthi(const string& instruction);
void com_mtlo(const string& instruction);
void com_mult(const string& instruction);
void com_multu(const string& instruction);
void com_nor(const string& instruction);
void com_or(const string& instruction);
void com_sll(const string& instruction);
void com_sllv(const string& instruction);
void com_slt(const string& instruction);
void com_sltu(const string& instruction);
void com_sra(const string& instruction);
void com_srav(const string& instruction);
void com_srl(const string& instruction);
void com_srlv(const string& instruction);
void com_sub(const string& instruction);
void com_subu(const string& instruction);
void com_xor(const string& instruction);

#endif //ASSIGNMENT_2_SRC_RCOMMAND_H_
