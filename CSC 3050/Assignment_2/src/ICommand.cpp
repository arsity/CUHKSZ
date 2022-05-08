#include "ICommand.h"
#include <bitset>
#include <string>
using namespace std;

// convert 16 bits 2's complement number string into int
int twoComStr2int(const string& str) {
  if (str[0] == '0') {
    return stoi(str, nullptr, 2);
  } else {
    return stoi(str, nullptr, 2) | 0b11111111111111110000000000000000;
  }
}

// implementation of I type instructions
void com_addi(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + twoComStr2int(instruction.substr(16, 16));
}

void com_addiu(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + twoComStr2int(instruction.substr(16, 16));
}

void com_andi(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          & twoComStr2int(instruction.substr(16, 16));
}

void com_beq(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      == registers[stoi(instruction.substr(11, 5), nullptr, 2)]) {
    special[0] += (twoComStr2int(instruction.substr(16, 16)) << 2);
  }
}

void com_bgez(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)] >= 0) {
    special[0] += (twoComStr2int(instruction.substr(16, 16)) << 2);
  }
}

void com_bgtz(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)] > 0) {
    special[0] += (twoComStr2int(instruction.substr(16, 16)) << 2);
  }
}

void com_blez(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)] <= 0) {
    special[0] += (twoComStr2int(instruction.substr(16, 16)) << 2);
  }
}

void com_bltz(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)] < 0) {
    special[0] += (twoComStr2int(instruction.substr(16, 16)) << 2);
  }
}

void com_bne(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      != registers[stoi(instruction.substr(11, 5), nullptr, 2)]) {
    special[0] += (twoComStr2int(instruction.substr(16, 16)) << 2);
  }
}

void com_lb(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + twoComStr2int(instruction.substr(16, 16))
          - START_ADDRESS);
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] = *p;
}

void com_lbu(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + twoComStr2int(instruction.substr(16, 16))
          - START_ADDRESS);
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] = (unsigned)*p;
}

void com_lh(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS);
  auto ptr = (short*)(p + twoComStr2int(instruction.substr(16, 16)));
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] = *ptr;
}

void com_lhu(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS);
  auto ptr = (short*)(p + twoComStr2int(instruction.substr(16, 16)));
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] = (unsigned)*ptr;
}

void com_lui(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)]
      = twoComStr2int(instruction.substr(16)) << 16;
}

void com_lw(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS);
  auto ptr = (int*)(p + twoComStr2int(instruction.substr(16, 16)));
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] = *ptr;
}

void com_ori(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          | twoComStr2int(instruction.substr(16, 16));
}

void com_sb(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + twoComStr2int(instruction.substr(16, 16))
          - START_ADDRESS);
  *p = (char)registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_slti(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          < twoComStr2int(instruction.substr(16, 16)) ? 1 : 0;
}

void com_sltiu(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      (unsigned)registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          < (unsigned)twoComStr2int(instruction.substr(16, 16)) ? 1 : 0;
}

void com_sh(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS);
  auto ptr = (short*)(p + twoComStr2int(instruction.substr(16, 16)));
  *ptr = (short)registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_sw(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS);
  auto ptr = (int*)(p + twoComStr2int(instruction.substr(16, 16)));
  *ptr = registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_xori(const string& instruction) {
  registers[stoi(instruction.substr(11, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          ^ twoComStr2int(instruction.substr(16, 16));
}

void com_lwl(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS) + twoComStr2int(instruction.substr(16, 16));

  // number of need to modified bytes
  int count = twoComStr2int(instruction.substr(16, 16)) % 4 + 1;
  string cache = {};
  for (int i = 0; i < count; i++) {
    cache += bitset<8>(*p--).to_string();
  }
  string now = bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)]).to_string();
  string res = cache + now.substr(8 * count);
  try {
    registers[stoi(instruction.substr(11, 5), nullptr, 2)] = stoi(res, nullptr, 2);
  }
  catch (out_of_range& e) {
    registers[stoi(instruction.substr(11, 5), nullptr, 2)] = stoul(res, nullptr, 2);
  }
}

void com_lwr(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS) + twoComStr2int(instruction.substr(16, 16));

  // number of need to modified bytes
  int count = 4 - twoComStr2int(instruction.substr(16, 16)) % 4;
  string cache = {};
  for (int i = 0; i < count; i++) {
    cache = bitset<8>(*p--).to_string() + cache;
  }
  string now = bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)]).to_string();
  string res = now.substr(0, 8 * (4 - count)) + cache;
  try {
    registers[stoi(instruction.substr(11, 5), nullptr, 2)] = stoi(res, nullptr, 2);
  }
  catch (out_of_range& e) {
    registers[stoi(instruction.substr(11, 5), nullptr, 2)] = stoul(res, nullptr, 2);
  }
}

void com_swl(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS) + twoComStr2int(instruction.substr(16, 16));

  // number of need to modified bytes
  int count = 4 - twoComStr2int(instruction.substr(16, 16)) % 4;
  string now = bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)]).to_string();
  for (int i = 0; i < count; i++) {
    *p++ = (char)stoi(now.substr(8 * i, 8), nullptr, 2);
  }
}

void com_swr(const string& instruction) {
  auto p = (char*)reserveMemoryPtr
      + (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - START_ADDRESS) + twoComStr2int(instruction.substr(16, 16));

  // number of need to modified bytes
  int count = twoComStr2int(instruction.substr(16, 16)) % 4 + 1;
  string now = bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)]).to_string();
  for (int i = 1; i <= count; i++) {
    *p-- = (char)stoi(now.substr(8 * (4 - i), 8), nullptr, 2);
  }
}
