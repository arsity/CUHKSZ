#include <fstream>
#include "TextSegment.h"
using namespace std;

// put binary instructions into memory
void putInstructions(const string& path) {
  ifstream input(path); // open the file
  string src{}; // used for getline()
  if (input.is_open()) {
    auto ptr = (int*)reserveMemoryPtr; // point at the start
    while (getline(input, src)) {
      // consider some instructions start with 1
      try {
        *ptr = stoi(src, nullptr, 2);
      }
      catch (out_of_range& e) {
        *ptr = (int)stol(src, nullptr, 2);
      }
      ptr++;
    }
    input.close();
  } else {
    // invalid path
    throw invalid_argument("Could not open source of machine code " + path);
  }
}
