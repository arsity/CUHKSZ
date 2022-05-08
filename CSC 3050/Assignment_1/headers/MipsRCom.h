#ifndef ASSIGNMENT_1_SRC_MIPSCOM_H_
#define ASSIGNMENT_1_SRC_MIPSCOM_H_

#include <string>
#include <unordered_map>

class MipsRCom {
 public:
  MipsRCom();
  ~MipsRCom();
  const int at(const std::string& str) const;
  const int operator[](const std::string& str);
  const bool isRType(const std::string& str) const;

 private:
  const std::unordered_map<std::string, int> storage = {
      {"add", 0b100000}, {"addu", 0b100001}, {"and", 0b100100},
      {"div", 0b011010}, {"divu", 0b011011},
      {"jalr", 0b001001}, {"jr", 0b001000},
      {"mfhi", 0b010000}, {"mflo", 0b010010},
      {"mthi", 0b010001}, {"mtlo", 0b010011},
      {"mult", 0b011000}, {"multu", 0b011001},
      {"nor", 0b100111}, {"or", 0b100101},
      {"sll", 0b000000}, {"sllv", 0b000100},
      {"slt", 0b101010}, {"sltu", 0b101011},
      {"sra", 0b000011}, {"srav", 0b000111},
      {"srl", 0b000010}, {"srlv", 0b000110},
      {"sub", 0b100010}, {"subu", 0b100011},
      {"syscall", 0b001100}, {"xor", 0b100110}
  };
};

#endif //ASSIGNMENT_1_SRC_MIPSCOM_H_
