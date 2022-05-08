#ifndef ASSIGNMENT_1_SRC_MIPSICOM_H_
#define ASSIGNMENT_1_SRC_MIPSICOM_H_

#include <string>
#include <unordered_map>

class MipsICom {
 public:
  MipsICom();
  ~MipsICom();
  const int at(const std::string& str) const;
  const int operator[](const std::string& str);

 private:
  const std::unordered_map<std::string, int> storage = {
      {"addi", 0b001000}, {"addiu", 0b001001}, {"andi", 0b001100},
      {"beq", 0b000100}, {"bgez", 0b000001}, {"bgtz", 0b000111},
      {"blez", 0b000110}, {"bltz", 0b000001}, {"bne", 0b000101},
      {"lb", 0b100000}, {"lbu", 0b100100}, {"lh", 0b100001},
      {"lhu", 0b100101}, {"lui", 0b001111}, {"lw", 0b100011},
      {"ori", 0b001101}, {"sb", 0b101000}, {"slti", 0b001010},
      {"sltiu", 0b001011}, {"sh", 0b101001}, {"sw", 0b101011},
      {"xori", 0b001110}, {"lwl", 0b100010}, {"lwr", 0b100110},
      {"swl", 0b101010}, {"swr", 0b101110}
  };
};

#endif //ASSIGNMENT_1_SRC_MIPSICOM_H_
