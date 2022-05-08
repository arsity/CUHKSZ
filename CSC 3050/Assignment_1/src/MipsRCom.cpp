#include "MipsRCom.h"

MipsRCom::MipsRCom() = default;
MipsRCom::~MipsRCom() = default;

// access method
const int MipsRCom::at(const std::string& str) const {
  return storage.at(str);
}

const int MipsRCom::operator[](const std::string& str) {
  return storage.at(str);
}

// check contains
const bool MipsRCom::isRType(const std::string& str) const {
  return storage.contains(str);
}
