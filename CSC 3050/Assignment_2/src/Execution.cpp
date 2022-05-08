#include "Execution.h"
#include "Dump.h"
#include "RCommand.h"
#include "JCommand.h"
#include "ICommand.h"
#include <bitset>
#include <iostream>
#include <set>
using namespace std;

set<int> checkPts(const string& path) {
  fstream checkpoints(path);
  string checkpointStr{};
  set<int> res{};
  while (getline(checkpoints, checkpointStr)) {
    res.insert(stoi(checkpointStr));
  }
  return res;
}

void execution(char*& dynamicDataStart_ptr, const set<int>& checkpts) {

  int i = 0; // used for dump
  int* insPtr = (int*)((char*)reserveMemoryPtr + (special[0] - START_ADDRESS)); // actual instruction address

  // dump check
  while (insPtr != nullptr) {
    if (checkpts.contains(i)) {
      dumpRegisters(i);
      dumpMemory(i);
    }
    i++;

    //    take the instruction
    string insStr = bitset<32>(*insPtr).to_string();
    //    move PC
    special[0] += 4;

    if (isRCom(insStr)) {
      if (insStr.ends_with("100000")) {
        com_add(insStr);
      } else if (insStr.ends_with("100001")) {
        com_addu(insStr);
      } else if (insStr.ends_with("100100")) {
        com_and(insStr);
      } else if (insStr.ends_with("011010")) {
        com_div(insStr);
      } else if (insStr.ends_with("011011")) {
        com_divu(insStr);
      } else if (insStr.ends_with("001001")) {
        com_jalr(insStr);
      } else if (insStr.ends_with("001000")) {
        com_jr(insStr);
      } else if (insStr.ends_with("010000")) {
        com_mfhi(insStr);
      } else if (insStr.ends_with("010010")) {
        com_mflo(insStr);
      } else if (insStr.ends_with("010001")) {
        com_mthi(insStr);
      } else if (insStr.ends_with("010011")) {
        com_mtlo(insStr);
      } else if (insStr.ends_with("011000")) {
        com_mult(insStr);
      } else if (insStr.ends_with("011001")) {
        com_multu(insStr);
      } else if (insStr.ends_with("100111")) {
        com_nor(insStr);
      } else if (insStr.ends_with("100101")) {
        com_or(insStr);
      } else if (insStr.ends_with("000000")) {
        com_sll(insStr);
      } else if (insStr.ends_with("000100")) {
        com_sllv(insStr);
      } else if (insStr.ends_with("101010")) {
        com_slt(insStr);
      } else if (insStr.ends_with("101011")) {
        com_sltu(insStr);
      } else if (insStr.ends_with("000011")) {
        com_sra(insStr);
      } else if (insStr.ends_with("000111")) {
        com_srav(insStr);
      } else if (insStr.ends_with("000010")) {
        com_srl(insStr);
      } else if (insStr.ends_with("000110")) {
        com_srlv(insStr);
      } else if (insStr.ends_with("100010")) {
        com_sub(insStr);
      } else if (insStr.ends_with("100011")) {
        com_subu(insStr);
      } else if (insStr.ends_with("001100")) {
        com_syscall(insStr, dynamicDataStart_ptr);
      } else if (insStr.ends_with("100110")) {
        com_xor(insStr);
      } else {
        throw runtime_error("Unknown R Command at " + to_string(i));
      }
    } else if (insStr.starts_with("00001")) {
      if (insStr.starts_with("000010")) {
        com_j(insStr);
      } else if (insStr.starts_with("000011")) {
        com_jal(insStr);
      } else {
        throw runtime_error("Unknown J Command at " + to_string(i));
      }
    } else {
      if (insStr.starts_with("001000")) {
        com_addi(insStr);
      } else if (insStr.starts_with("001001")) {
        com_addiu(insStr);
      } else if (insStr.starts_with("001100")) {
        com_andi(insStr);
      } else if (insStr.starts_with("000100")) {
        com_beq(insStr);
      } else if (insStr.starts_with("000001")) {
        com_bgez(insStr);
      } else if (insStr.starts_with("000111")) {
        com_bgtz(insStr);
      } else if (insStr.starts_with("000110")) {
        com_blez(insStr);
      } else if (insStr.starts_with("000001")) {
        com_bltz(insStr);
      } else if (insStr.starts_with("000101")) {
        com_bne(insStr);
      } else if (insStr.starts_with("100000")) {
        com_lb(insStr);
      } else if (insStr.starts_with("100100")) {
        com_lbu(insStr);
      } else if (insStr.starts_with("100001")) {
        com_lh(insStr);
      } else if (insStr.starts_with("100101")) {
        com_lhu(insStr);
      } else if (insStr.starts_with("001111")) {
        com_lui(insStr);
      } else if (insStr.starts_with("100011")) {
        com_lw(insStr);
      } else if (insStr.starts_with("001101")) {
        com_ori(insStr);
      } else if (insStr.starts_with("101000")) {
        com_sb(insStr);
      } else if (insStr.starts_with("001010")) {
        com_slti(insStr);
      } else if (insStr.starts_with("001011")) {
        com_sltiu(insStr);
      } else if (insStr.starts_with("101001")) {
        com_sh(insStr);
      } else if (insStr.starts_with("101011")) {
        com_sw(insStr);
      } else if (insStr.starts_with("001110")) {
        com_xori(insStr);
      } else if (insStr.starts_with("100010")) {
        com_lwl(insStr);
      } else if (insStr.starts_with("100110")) {
        com_lwr(insStr);
      } else if (insStr.starts_with("101010")) {
        com_swl(insStr);
      } else if (insStr.starts_with("101110")) {
        com_swr(insStr);
      } else {
        throw runtime_error("Unknown I Command at " + to_string(i));
      }
    }
    insPtr = (int*)((char*)reserveMemoryPtr + (special[0] - START_ADDRESS));
  }
}