#include "MipsJCom.h"

MipsJCom::MipsJCom() = default;
MipsJCom::~MipsJCom() = default;

// access method
const int MipsJCom::at(const std::string& str) const {
  return storage.at(str);
}

const int MipsJCom::operator[](const std::string& str) {
  return storage.at(str);
}

// contains method
const bool MipsJCom::isJType(const std::string& str) const {
  return storage.contains(str);
}