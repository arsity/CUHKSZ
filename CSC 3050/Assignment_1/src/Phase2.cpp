#include "Phase2.h"
#include <bitset>
#include <algorithm>
#include <fstream>
#include <iostream>

void translate(std::vector<std::string>& cache,
               int address,
               const LabelTable& labelTable,
               const MipsVar& mipsVar,
               const MipsRCom& mipsRCom,
               const MipsICom& mipsICom,
               const MipsJCom& mipsJCom) {
  for (std::string& line : cache) {
    if (!line.empty()) {
      //      Extract Command
      auto comStart = std::find_if(line.begin(), line.end(), isalpha);
      auto comEnd = std::find_if_not(comStart, line.end(), isalpha);
      std::string command(comStart, comEnd);

      if (mipsRCom.isRType(command)) { // R type
        transRType(command, line, labelTable, mipsVar, mipsRCom);
        address += 4;
      } else if (mipsJCom.isJType(command)) { // J type
        transJType(command, line, labelTable, mipsJCom);
        address += 4;
      } else { //I type
        transIType(address, command, line, labelTable, mipsVar, mipsICom);
        address += 4;
      }
    }
  }
}

std::string convertNum(auto& start, auto& end) {
  // binary
  if (*std::next(start) == 'b') {
    if (*std::prev(start) == '-') { //negative
      auto res = std::bitset<16>(-std::stoi(std::string(next(start, 2), end), nullptr, 2)).to_string();
      return res;
    } else {
      auto res = std::bitset<16>(std::stoi(std::string(next(start, 2), end), nullptr, 2)).to_string();
      return res;
    }
  } else if (*std::prev(start) == '-') { // other negative
    auto res = std::bitset<16>(std::stoi(std::string(prev(start), end), nullptr, 0)).to_string();
    return res;
  } else { // other postive
    auto res = std::bitset<16>(std::stoi(std::string(start, end), nullptr, 0)).to_string();
    return res;
  }
}

std::string transRegister(auto& start, const auto& end, const MipsVar& mipsVar) {
  // the '$' of register
  auto rgStart = std::find_if(start, end, [&](const auto& item) {
    return item == '$';
  });
  // the first non-alpha or number since '&'
  auto rgEnd = std::find_if_not(std::next(rgStart), end, isalnum);
  start = rgEnd; // maneuver start pointer
  return std::bitset<5>(mipsVar.at(std::string(rgStart, rgEnd))).to_string();
}

void transRType(const std::string& command,
                std::string& line,
                const LabelTable& labelTable,
                const MipsVar& mipsVar,
                const MipsRCom& mipsRCom) {

  if (command == "syscall") { // special case

    line = std::string("00000000000000000000000000001100");

  } else if (command == "jalr") { // rd rs case

    auto pos = line.begin();
    auto rd = transRegister(pos, line.end(), mipsVar);
    auto rs = transRegister(pos, line.end(), mipsVar);
    line = std::string("000000") + rs + "00000" + rd + "00000" + "001001";

  } else if (command.starts_with("d") || command.starts_with("mu")) { // rs rt case

    auto pos = line.begin();
    auto rs = transRegister(pos, line.end(), mipsVar);
    auto rt = transRegister(pos, line.end(), mipsVar);
    line = std::string("000000") + rs + rt + "00000" + "00000" + std::bitset<6>(mipsRCom.at(command)).to_string();

  } else if (command.starts_with("mt") || command == "jr") { // rs case

    auto pos = line.begin();
    auto rs = transRegister(pos, line.end(), mipsVar);
    line = std::string("000000") + rs + "00000" + "00000" + "00000" + std::bitset<6>(mipsRCom.at(command)).to_string();

  } else if (command.starts_with("mf")) { // rd case

    auto pos = line.begin();
    auto rd = transRegister(pos, line.end(), mipsVar);
    line = std::string("000000") + "00000" + "00000" + rd + "00000" + std::bitset<6>(mipsRCom.at(command)).to_string();

  } else if (command == "sll" || command == "sra" || command == "srl") { // rd rt sa case

    auto pos = line.begin();
    auto rd = transRegister(pos, line.end(), mipsVar);
    auto rt = transRegister(pos, line.end(), mipsVar);
    auto saStart = std::find_if(pos, line.end(), isdigit);
    auto saEnd = std::find_if_not(saStart, line.end(), isdigit);
    auto sa = convertNum(saStart, saEnd).substr(11);
    line = std::string("000000") + "00000" + rt + rd + sa + std::bitset<6>(mipsRCom.at(command)).to_string();

  } else if (command == "sllv" || command == "srav" || command == "srlv") { // rd rt rs case

    auto pos = line.begin();
    auto rd = transRegister(pos, line.end(), mipsVar);
    auto rt = transRegister(pos, line.end(), mipsVar);
    auto rs = transRegister(pos, line.end(), mipsVar);
    line = std::string("000000") + rs + rt + rd + "00000" + std::bitset<6>(mipsRCom.at(command)).to_string();

  } else { // rd rs rt case

    auto pos = line.begin();
    auto rd = transRegister(pos, line.end(), mipsVar);
    auto rs = transRegister(pos, line.end(), mipsVar);
    auto rt = transRegister(pos, line.end(), mipsVar);
    line = std::string("000000") + rs + rt + rd + "00000" + std::bitset<6>(mipsRCom.at(command)).to_string();

  }
}

void transJType(const std::string& command, std::string& line, const LabelTable& labelTable, const MipsJCom& mipsJCom) {
  // extract label
  auto labelEnd = std::find_if(line.rbegin(), line.rend(), [&](const auto& item) {
    return item == '_' || isalnum(item);
  });
  auto labelStart = std::find_if_not(labelEnd, line.rend(), [&](const auto& item) {
    return item == '_' || isalnum(item);
  });
  std::string label(std::string(labelStart.base(), labelEnd.base()));
  line = std::bitset<6>(mipsJCom.at(command)).to_string() + std::bitset<26>(labelTable.at(label) / 4).to_string();
}

void transIType(int address,
                const std::string& command,
                std::string& line,
                const LabelTable& labelTable,
                const MipsVar& mipsVar,
                const MipsICom& mipsICom) {
  if (command == "lui") { // special case

    auto pos = line.begin();
    auto rt = transRegister(pos, line.end(), mipsVar);
    auto immeStart = std::find_if(pos, line.end(), isdigit);
    auto immeEnd = std::find_if_not(immeStart, line.end(), isdigit);
    auto imme = convertNum(immeStart, immeEnd);
    line = std::string("00111100000") + rt + imme;

  } else if (command == "beq" || command == "bne") { // rs rt label case

    auto pos = line.begin();
    auto rs = transRegister(pos, line.end(), mipsVar);
    auto rt = transRegister(pos, line.end(), mipsVar);
    auto labelStart = std::find_if(pos, line.end(), [&](const auto& item) {
      return item == '_' || isalnum(item);
    });
    auto labelEnd = std::find_if_not(labelStart, line.end(), [&](const auto& item) {
      return item == '_' || isalnum(item);
    });
    std::string label(labelStart, labelEnd);
    std::string imme = std::bitset<16>((labelTable.at(label) - (address + 4)) / 4).to_string();
    line = std::bitset<6>(mipsICom.at(command)).to_string() + rs + rt + imme;

  } else if (command.starts_with("bg") || command.starts_with("bl")) { // rs label case

    auto pos = line.begin();
    auto rs = transRegister(pos, line.end(), mipsVar);
    auto labelStart = std::find_if(pos, line.end(), [&](const auto& item) {
      return item == '_' || isalnum(item);
    });
    auto labelEnd = std::find_if_not(labelStart, line.end(), [&](const auto& item) {
      return item == '_' || isalnum(item);
    });
    std::string label(labelStart, labelEnd);
    std::string imme = std::bitset<16>((labelTable.at(label) - (address + 4)) / 4).to_string();
    if (command == "bgez") {
      line = std::string("000001") + rs + "00001" + imme;
    } else {
      line = std::bitset<6>(mipsICom.at(command)).to_string() + rs + "00000" + imme;
    }

  } else if (command.starts_with("a") || command == "ori" || command.starts_with("sl") || command.starts_with("x")) {
    // rt rs immediate case
    auto pos = line.begin();
    auto rt = transRegister(pos, line.end(), mipsVar);
    auto rs = transRegister(pos, line.end(), mipsVar);
    auto immeStart = std::find_if(pos, line.end(), isdigit);
    auto immeEnd = std::find_if_not(immeStart, line.end(), isdigit);
    auto imme = convertNum(immeStart, immeEnd);
    line = std::bitset<6>(mipsICom.at(command)).to_string() + rs + rt + imme;

  } else { // rs immediate (rs) case

    auto pos = line.begin();
    auto rt = transRegister(pos, line.end(), mipsVar);
    auto immeStart = std::find_if(pos, line.end(), isdigit);
    auto immeEnd = std::find_if_not(immeStart, line.end(), isdigit);
    auto imme = convertNum(immeStart, immeEnd);
    auto rs = transRegister(immeEnd, line.end(), mipsVar);
    line = std::bitset<6>(mipsICom.at(command)).to_string() + rs + rt + imme;

  }
}

void Output(const std::string& outputFileDir, const std::vector<std::string>& cache) {
  // open and clear if with content
  std::ofstream outputFile(outputFileDir, std::ios::trunc);
  if (outputFile.is_open()) {
    // write
    for (auto& line : cache) {
      if (!line.empty()) {
        outputFile << line << std::endl;
      }
    }
    outputFile.close();
    std::cout << "Success to write to " << outputFileDir << std::endl;
  } else {
    std::cerr << "Failed to write to " << outputFileDir << std::endl;
    throw std::invalid_argument(outputFileDir);
  }
}

void compare(const std::string& actOut, const std::string& expOut) {
  std::ifstream actFS(actOut);
  std::ifstream expFS(expOut);
  std::string actStr;
  std::string expStr;
  int i = 1;
  while (getline(actFS, actStr) && getline(expFS, expStr)) {
    if (actStr != expStr) {
      std::cout << "Error: at line " << i << std::endl;
      throw std::bad_exception();
    }
    i++;
  }
  std::cout << actOut << " and " << expOut << " are identical.\n";
  actFS.close();
  expFS.close();
}