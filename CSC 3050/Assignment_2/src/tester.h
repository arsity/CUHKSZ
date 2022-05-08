#ifndef ASSIGNMENT_2_INCLUDE_TESTER_H_
#define ASSIGNMENT_2_INCLUDE_TESTER_H_

#include <fstream>
using namespace std;

extern int registers[32]; // 32 general purpose registers

extern unsigned int special[3]; // $PC, $HI, $LO

extern void* reserveMemoryPtr; // the start of simulation memory block

extern unsigned int START_ADDRESS; // simulation start address

extern ifstream iFileStream; // input
extern ofstream oFileStream; // output

int main(int argc, char* argv[]);
#endif //ASSIGNMENT_2_INCLUDE_TESTER_H_
