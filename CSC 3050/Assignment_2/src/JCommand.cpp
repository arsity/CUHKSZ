
#include "JCommand.h"
#include <bitset>
using namespace std;

// J type instructions implementation
// keep first 4 bits and connect to 26 bits and end with two zeros
void com_j(const string& instruction) {
  special[0] = stoi(bitset<32>(special[0]).to_string().substr(0, 4)
                        + instruction.substr(6) + "00", nullptr, 2);
}

void com_jal(const string& instruction) {
  registers[31] = special[0];
  special[0] = stoi(bitset<32>(special[0]).to_string().substr(0, 4)
                        + instruction.substr(6) + "00", nullptr, 2);
}
