# Dependencies

- Ubuntu 21.10 (Impish)

- Linux version 5.10.60.1-microsoft-standard-WSL2

- g++ (gcc) >= 11.2.0

- GNU ld (GNU Binutils for Ubuntu) >= 2.37

- cmake >= 3.18.4

- Ninja >= 1.10.1

# How to Start

```bash
cmake CMakeLists.txt -G Ninja
cmake --build /path_to_target_directory
cd /path_to_target_directory
./Assignment_2 $asm $binary_code $checkpoints $input $output 
```

# Basic Ideas

- All instructions are 32 bits, identical with the `int`. However, as the `stoi()` method does not consider the input `string` pertaining 2's complement, which may lead to overflow the upper bound of `int`, we could use `catch` and `stoul()` to convert it first. Then, we could make it an `unsigned int`, and it is applicable to store it as an `int` now. 

- For `ascii` and `asciiz` data, we could write them into memory byte by byte using `char`, while `half` and `word` correspond to `short` and `int` respectively. We could maintain a pointer illustrating the next available address. And we could make it step to the next until it is a multiple of 4 after inserting a line of data. 

- Define an array with length of 32 to simulates the registers, and another array with size of 3 to act as `$PC`, `$HI`, `$LO` respectively. 

- Implement each instruction with a function, while `syscall` can be differed by the value in `$v0` using `switch`. Also, as the default `stoi` does not support 2's complement, we may use the bit-wise or to achieve correct return value. 

# Implementation Details

## Introduction

The `main` function locates in the `/src/tester.cpp`. And other `.h` `.cpp` files are used to implement corresponding part as their filenames.

## `tester.h/cpp`

Define all global variables and the basic structure of `main` function. `START_ADDRESS` means the simulation address at the starting point, that is 0x400000. `registers[32]` is used to store the data of 32 general purpose registers. And `special[3]` corresponds to `PC` `HI` and `LO`. `reserveMemoryPtr` is the actual address pointer to the 6MB block. `iFileStream` and `oFileStream` are used to handle the input and output, respectively.

In the `main` function, `dynamicDataStart_ptr` is the actual address pointer to the next available position on heap. We initialize the *pointers* in the *registers* and begin to simulate in the while loop of `execution`.

## `TextSegment.h/cpp`

Open the machine code file and put all instructions into the memory line by line. All instructions are 32 bits, identical with the `int`. However, as the `stoi()` method does not consider the input `string` pertaining 2's complement, which may lead to overflow the upper bound of `int`, we could use `catch` and `stoul()` to convert it first. Then, we could make it an `unsigned int`, and it is applicable to store it as an `int` now.

## `DataSegment.h/cpp`

- ```cpp
  vector<string> extractData(const string& path)
  ```
  
  We may extract the `.data` section into the vector and then remove all comments with the position of `#` .

- ```cpp
  int* putData(const vector<string>& cache)
  ```
  
  The return value is the starting point of dynamic data.
  
  For `ascii` and `asciiz` type, we can read them one by one and write into the corresponding address as `char`.
  
  For `word` type, it can be consider as `int` actually, with the same value range. We just need to pay attention to that it may contains more than one item in one line. Also, the minus sign is need to be transferred to `stoi`.
  
  For `half` type, it can be consider as `short` actually, with the same value range. We just need to pay attention to that it may contains more than one item in one line. Also, the minus sign is need to be transferred to `stoi`. We transfer the value into `int` first and then cast it into `short`.

- ```cpp
  void substituteEscapeSequence(string& str)
  ```
  
  Input a string, substitute the *fake* escape sequences into the *true* one. 
  
  We use `getline` to read the data in `ascii` and `asciiz`, which may lead to incorrect answer if  we directly put them into the memory. For example, the `'\n'` will be considered as `'\\'` and `'n'` disparately. This function is to modify them into the correct one.

## `Execution.h/cpp`

- ```cpp
  set<int> checkPts(const string& path)
  ```
  
  Open the checkpoint file and store the trigger into set.

- ```cpp
  void execution(char*& dynamicDataStart_ptr, const set<int>& checkpts)
  ```
  
  It is used to simulate the machine cycle when executing a program.
  
  1. Put the instruction pointing by `$PC` into execution cache.
  
  2. `$PC += 4`
  
  3. Execute the instruction in cache
  
  Also add some extra command to meet the requirement of dump and mapping the simulation address into the actual one.

## `Dump.h/cpp`

Dump files implementation.

## `JCommand.h/cpp`

Implementation of J type instructions. According to the tutorial class, PC keeps its first 4 bits and concatenate it with the 26 bits in the instruction. And end it with "00".

## `RCommand.h/cpp` `ICommand.h/cpp`

Implementations of R type instructions.

For `syscall`, some instructions use the Linux API directly. And other instructions follow a similar pattern. We identify which register it involves or what the shift amount/immediate is. Then, according to the manual of MIPS, we do the calculation or action.

- ```cpp
  int twoComStr2int(const string& str)
  ```
  
  This function is used to convert the 16 bits binary string into corresponding int with 2's complement.

# Reference

[cppreference.com](https://en.cppreference.com/)

[cplusplus.com](https://cplusplus.com/)
