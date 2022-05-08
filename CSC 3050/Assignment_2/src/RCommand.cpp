#include "RCommand.h"
#include <string>
#include <fcntl.h>
#include <bitset>
#include <unistd.h>
using namespace std;

// check if the instructions is R type
bool isRCom(const string& instruction) {
  return instruction.starts_with("000000");
}

// instruction syscall implementation
void com_syscall(const string& instruction, char*& dynamicDataStart_ptr) {
  switch (registers[2]) {
    case 1:
      oFileStream << registers[4];
      oFileStream.flush();
      break;
    case 4: {
      const char* p = (char*)reserveMemoryPtr + (registers[4] - START_ADDRESS);
      oFileStream << p;
      oFileStream.flush();
      break;
    }
    case 5: {
      string cache{};
      getline(iFileStream, cache);
      registers[2] = stoi(cache);
      break;
    }
    case 8: {
      string cache{};
      getline(iFileStream, cache);
      registers[5] = cache.length();
      char* p = (char*)reserveMemoryPtr + (registers[4] - START_ADDRESS);
      for (char c : cache) {
        *p++ = c;
      }
      break;
    }
    case 9:
      registers[2] = reinterpret_cast<intptr_t>(dynamicDataStart_ptr) - reinterpret_cast<intptr_t>(reserveMemoryPtr)
          + START_ADDRESS;
      dynamicDataStart_ptr += registers[4];
      break;
    case 10:
      exit(0);
    case 11:
      oFileStream << char(registers[4]);
      oFileStream.flush();
      break;
    case 12: {
      string cache{};
      getline(iFileStream, cache);
      registers[2] = (unsigned char)cache[0];
      break;
    }
    case 13: {
      const char* p = (char*)reserveMemoryPtr + (registers[4] - START_ADDRESS);
      registers[4] = open(p, registers[5], registers[6]);
    }
      break;
    case 14:
      registers[4] =
          read(registers[4], (void*)((char*)reserveMemoryPtr + (registers[5] - START_ADDRESS)), registers[6]);
      break;
    case 15:
      registers[4] =
          write(registers[4], (void*)((char*)reserveMemoryPtr + (registers[5] - START_ADDRESS)), registers[6]);
      break;
    case 16:
      close(registers[4]);
      break;
    case 17:
      exit(registers[4]);
    default:
      throw runtime_error("Uncovered syscall.");
  }
}

// instructions implementation
void com_add(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_addu(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          + registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_and(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          & registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_div(const string& instruction) {
  special[2] = registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      / registers[stoi(instruction.substr(11, 5), nullptr, 2)];
  special[1] = registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      % registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_divu(const string& instruction) {
  special[2] = registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      / registers[stoi(instruction.substr(11, 5), nullptr, 2)];
  special[1] = registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      % registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_jalr(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] = special[0];
  special[0] = registers[stoi(instruction.substr(6, 5), nullptr, 2)];
}

void com_jr(const string& instruction) {
  special[0] = registers[stoi(instruction.substr(6, 5), nullptr, 2)];
}

void com_mfhi(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] = special[1];
}

void com_mflo(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] = special[2];
}

void com_mthi(const string& instruction) {
  special[1] = registers[stoi(instruction.substr(6, 5), nullptr, 2)];
}

void com_mtlo(const string& instruction) {
  special[2] = registers[stoi(instruction.substr(6, 5), nullptr, 2)];
}

void com_mult(const string& instruction) {
  long result = (long)registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      * (long)registers[stoi(instruction.substr(11, 5), nullptr, 2)];
  auto resBit = bitset<64>(result).to_string();
  special[1] = stoi(resBit.substr(0, 32), nullptr, 2);
  special[2] = stoul(resBit.substr(32), nullptr, 2);
}

void com_multu(const string& instruction) {
  long result = (long)registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      * (long)registers[stoi(instruction.substr(11, 5), nullptr, 2)];
  auto resBit = bitset<64>(result).to_string();
  special[1] = stoi(resBit.substr(0, 32), nullptr, 2);
  special[2] = stoul(resBit.substr(32), nullptr, 2);
}

void com_nor(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      ~(registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          | registers[stoi(instruction.substr(11, 5), nullptr, 2)]);
}

void com_or(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          | registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_sll(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      (bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)])
          << stoi(instruction.substr(21, 5), nullptr, 2)).to_ulong();
}

void com_sllv(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      (bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)])
          << (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
              & 0b00000000000000000000000000011111)).to_ulong();
}

void com_slt(const string& instruction) {
  if (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      < registers[stoi(instruction.substr(11, 5), nullptr, 2)]) {
    registers[stoi(instruction.substr(16, 5), nullptr, 2)] = 1;
  }
}

void com_sltu(const string& instruction) {
  if ((unsigned)registers[stoi(instruction.substr(6, 5), nullptr, 2)]
      < (unsigned)registers[stoi(instruction.substr(11, 5), nullptr, 2)]) {
    registers[stoi(instruction.substr(16, 5), nullptr, 2)] = 1;
  }
}

void com_sra(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(11, 5), nullptr, 2)]
          >> stoi(instruction.substr(21, 5), nullptr, 2);
}

void com_srav(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(11, 5), nullptr, 2)]
          >> (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
              & 0b00000000000000000000000000011111);
}

void com_srl(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      (bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)])
          >> stoi(instruction.substr(21, 5), nullptr, 2)).to_ulong();
}

void com_srlv(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      (bitset<32>(registers[stoi(instruction.substr(11, 5), nullptr, 2)])
          >> (registers[stoi(instruction.substr(6, 5), nullptr, 2)]
              & 0b00000000000000000000000000011111)).to_ulong();
}

void com_sub(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_subu(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          - registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}

void com_xor(const string& instruction) {
  registers[stoi(instruction.substr(16, 5), nullptr, 2)] =
      registers[stoi(instruction.substr(6, 5), nullptr, 2)]
          ^ registers[stoi(instruction.substr(11, 5), nullptr, 2)];
}
