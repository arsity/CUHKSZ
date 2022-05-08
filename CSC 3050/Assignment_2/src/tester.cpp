#include <iostream>
#include "tester.h"

#include "DataSegment.h"
#include "TextSegment.h"
#include "Execution.h"
using namespace std;

// simulation start address
unsigned int START_ADDRESS = 0x400000;

int registers[32] = {}; // 32 general purpose registers
unsigned int special[3] = {}; // $PC, $HI, $LO

// the start of simulation memory block
void* reserveMemoryPtr = malloc(0x600000);

ifstream iFileStream; // input
ofstream oFileStream; // output

int main(int argc, char* argv[]) {
  // check the number of arguments
  if (argc > 6) {
    throw invalid_argument("Too many arguments!");
  }

  // put binary instructions into memory
  putInstructions(argv[2]);

  // put static data into memory, return the start address of dynamic data
  auto dynamicDataStart_ptr = (char*)putData(extractData(argv[1]));

  // input and output
  iFileStream.open(argv[4]);
  oFileStream.open(argv[5], ios::trunc | fstream::binary);

  // initialize pointers stored in registers
  special[0] = START_ADDRESS;
  registers[29] = 0xa00000;
  registers[30] = registers[29];
  registers[28] = 0x508000;

  // machine cycle
  execution(dynamicDataStart_ptr, checkPts(argv[3]));

  return 0;
}