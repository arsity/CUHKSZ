#include <fstream>
#include <string>
#include "Dump.h"
using namespace std;

void dumpRegisters(int lineNum) {
  ofstream outputFile(string("register_") + to_string(lineNum) + ".bin", ios::trunc);
  if (outputFile.is_open()) {
    outputFile.write((char*)&registers[0], 128);
    outputFile.write((char*)&special[0], 12);
    outputFile.close();
  } else {
    throw runtime_error(string("Cannot open file: ") + "register_" + to_string(lineNum) + ".bin");
  }
}

void dumpMemory(int lineNum) {
  ofstream outputFile(string("memory_") + to_string(lineNum) + ".bin", ios::trunc);
  if (outputFile.is_open()) {
    outputFile.write((char*)reserveMemoryPtr, 0x600000);
    outputFile.close();
  } else {
    throw runtime_error(string("Cannot open file: ") + "memory_" + to_string(lineNum) + ".bin");
  }
}
