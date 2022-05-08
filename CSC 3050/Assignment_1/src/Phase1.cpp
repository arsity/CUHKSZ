#include "Phase1.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "LabelTable.h"

void open(const std::string& sourceFile, std::vector<std::string>& src) {
  std::ifstream srcFileStream(sourceFile);
  if (srcFileStream.is_open()) {
    std::string line;
    bool start = false; // .text indicator
    while (std::getline(srcFileStream, line)) {
      // Only store lines into cache after .text
      if (start) {
        src.push_back(line);
      } else if (line.find(".text") != std::string::npos) {
        start = true;
      }
    }
    srcFileStream.close();
    std::cout << "Success to read from " << sourceFile << std::endl;
  } else {
    std::cerr << "Failed to read from " << sourceFile << std::endl;
    throw std::invalid_argument(sourceFile);
  }
}

void scan(std::vector<std::string>& src, LabelTable& labelTable, int ADDRESS) {
  bool flag = false; // to identify label address without trailing instruction
  std::string label;
  for (std::string& line : src) {
    // remove comments first
    if (line.find('#') != std::string::npos) {
      line.erase(line.find('#'));
    }
    // check if empty
    if (std::find_if(line.begin(), line.end(), [&](const auto& item) {
      return item == '_' || isalnum(item);
    }) != line.end()) {
      // check if label exist
      if (line.find(':') != std::string::npos) {
        // extract label
        label.clear();
        for (int i = 0; i < line.find(':'); i++) {
          if (line[i] == '_' || isalnum(line[i])) {
            label += line[i];
          }
        }
        // check if trailing instructions
        if (*std::find_if_not(line.rbegin(), line.rend(), isspace) == ':') {
          flag = true;
          line.clear();
        } else {
          labelTable.put(label, ADDRESS);
          ADDRESS += 4;
          line.erase(0, line.find(':') + 1);
        }
      } else {
        if (flag) {
          labelTable.put(label, ADDRESS);
          flag = false;
        }
        ADDRESS += 4;
      }
    } else {
      line.clear();
    }
  }
}
