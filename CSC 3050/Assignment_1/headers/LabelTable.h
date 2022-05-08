#ifndef ASSIGNMENT_1_SRC_LABELTABLE_H_
#define ASSIGNMENT_1_SRC_LABELTABLE_H_

#include <map>
#include <string>

class LabelTable {
 public:
  LabelTable();
  ~LabelTable();
  const bool contains(const std::string& label);
  void put(const std::string& label, int address);
  const int at(const std::string& str) const;
  const int operator[](const std::string& str);
 private:
  std::map<std::string, int> storage;
};

#endif //ASSIGNMENT_1_SRC_LABELTABLE_H_
