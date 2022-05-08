#include <iostream>
#include "LabelTable.h"

LabelTable::LabelTable() = default;

LabelTable::~LabelTable() = default;

// check contains
const bool LabelTable::contains(const std::string& label) {
  return storage.contains(label);
}

// store in method
void LabelTable::put(const std::string& label, int address) {
  if (contains(label)) {
    std::cerr << "ERROR: The label already exists.\n";
    throw std::bad_exception();
  } else {
    storage[label] = address;
  }
}

// access method
const int LabelTable::at(const std::string& str) const {
  return storage.at(str);
}

const int LabelTable::operator[](const std::string& str) {
  return storage[str];
}
