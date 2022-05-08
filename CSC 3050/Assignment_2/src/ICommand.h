#ifndef ASSIGNMENT_2_SRC_ICOMMAND_H_
#define ASSIGNMENT_2_SRC_ICOMMAND_H_

#include "tester.h"
#include <string>
using namespace std;

// convert 16 bits 2's complement number string into int
int twoComStr2int(const string& str);

// implementation of I type instructions
void com_addi(const string& instruction);
void com_addiu(const string& instruction);
void com_andi(const string& instruction);
void com_beq(const string& instruction);
void com_bgez(const string& instruction);
void com_bgtz(const string& instruction);
void com_blez(const string& instruction);
void com_bltz(const string& instruction);
void com_bne(const string& instruction);
void com_lb(const string& instruction);
void com_lbu(const string& instruction);
void com_lh(const string& instruction);
void com_lhu(const string& instruction);
void com_lui(const string& instruction);
void com_lw(const string& instruction);
void com_ori(const string& instruction);
void com_sb(const string& instruction);
void com_slti(const string& instruction);
void com_sltiu(const string& instruction);
void com_sh(const string& instruction);
void com_sw(const string& instruction);
void com_xori(const string& instruction);
void com_lwl(const string& instruction);
void com_lwr(const string& instruction);
void com_swl(const string& instruction);
void com_swr(const string& instruction);

#endif //ASSIGNMENT_2_SRC_ICOMMAND_H_
