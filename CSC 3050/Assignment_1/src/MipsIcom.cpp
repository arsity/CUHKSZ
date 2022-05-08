#include "MipsIcom.h"

MipsICom::MipsICom() = default;
MipsICom::~MipsICom() = default;

// access method
const int MipsICom::at(const std::string& str) const {
  return storage.at(str);
}

const int MipsICom::operator[](const std::string& str) {
  return storage.at(str);
}
