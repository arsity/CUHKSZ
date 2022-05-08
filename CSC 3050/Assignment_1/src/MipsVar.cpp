#include "MipsVar.h"

MipsVar::MipsVar() = default;
MipsVar::~MipsVar() = default;

// access method
const int MipsVar::at(const std::string& str) const {
  return storage.at(str);
}
const int MipsVar::operator[](const std::string& str) {
  return storage.at(str);
}
