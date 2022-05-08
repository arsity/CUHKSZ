#ifndef ASSIGNMENT_1_SRC_MIPSJCOM_H_
#define ASSIGNMENT_1_SRC_MIPSJCOM_H_

#include <string>
#include <unordered_map>

class MipsJCom {
 public:
  MipsJCom();
  ~MipsJCom();
  const int at(const std::string& str) const;
  const int operator[](const std::string& str);
  const bool isJType(const std::string& str) const;

 private:
  const std::unordered_map<std::string, int> storage = {
      {"j", 0b000010}, {"jal", 0b000011}
  };
};

#endif //ASSIGNMENT_1_SRC_MIPSJCOM_H_
