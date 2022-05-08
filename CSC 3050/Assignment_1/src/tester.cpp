#include "LabelTable.h"
#include "Phase1.h"
#include "Phase2.h"

// argv[1] = SourceCodeFile.asm
// argv[2] = Output.txt
// argv[3] = ExpectedOutput.txt
int main(int argc, char* argv[]) {
  if (argc > 4) {
    throw std::invalid_argument("too many arguments");
  }
  std::vector<std::string> cache;
  int START_ADDRESS = 0x400000;
  LabelTable labelTable;
  MipsVar mipsVar;
  MipsRCom mipsRCom;
  MipsICom mipsICom;
  MipsJCom mipsJCom;
  open(argv[1], cache);
  scan(cache, labelTable, START_ADDRESS);
  translate(cache, START_ADDRESS, labelTable, mipsVar, mipsRCom, mipsICom, mipsJCom);
  Output(argv[2], cache);
  compare(argv[2], argv[3]);
  return 0;
}