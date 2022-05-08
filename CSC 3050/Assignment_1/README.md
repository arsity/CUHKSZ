## Environment

- Ubuntu 21.10 (Impish)

- Linux version 5.10.60.1-microsoft-standard-WSL2

- g++ (gcc) >= 11.2.0

- GNU ld (GNU Binutils for Ubuntu) >= 2.37

- cmake >= 3.18.4

- Ninja >= 1.10.1

## Start

```shell
cmake CMakeLists.txt -G Ninja
cmake --build /path_to_target_directory
/path_to_target_directory/Assignment_1 \
/path_to_source_code \
/path_to_output_file \
/path_to_expected_output
```

## Basic Ideas

- The data from the source file (in `.text` section) can be separated into two types. One is that it contains complete
  information in itself, such as instruction commands and registers’ names, which are fully elaborated in the MIPS
  manual and we can directly translate them into binary code. The other is that it needs to be further espoused with
  information during execution, such as labels and static data, which may be referred to in some instructions. We need
  to identify them case by case. So, we may scan the source file twice and adopt the following approaches to handle them
  respectively.

- For the first time, we can read the source file line by line and store them into cache. In this process, we can
  dismiss comments and then look for whether there exists a label and store it into a predefined table with its mapping
  address.

- For the second time, we traverse the cache and turn instructions into machine code. We may use a predefined table to
  map registers and commands into binary code directly. Also, we use the table maintained in previous section to
  transfer labels into corresponding address in binary form.

- Although the computer executes binary code from memory, in this project we only need to decide whether it makes a
  correct translation. Therefore, we output the binary code check it with the expected answer.

## Implementation

### Figure

<img title="" src="https://github.com/arsity/Assignment_1/blob/master/figure.png?raw=true" alt="" width="586" data-align="center">

### Introduction

The `main` function locates in `/src/tester.cpp`. Any other class/method/function is declared in
corresponding `/headers/*.h` files first and then defined in `/src/*.cpp` files with identical file names. And the file
names indicates what the class/function is or when the function is used.

### `LableTable.h/cpp`

Define a class called `LabelTable` to store pairs of label and address. Considered we need to support inserting pairs
and making access to specific pair with a label, we use `map` from C++ Standard Library, `std`, as actual container
of `<label,address>` pairs. According to [cppreference.com](https://en.cppreference.com/w/cpp/container/map),
the `insert()` method and access method `at()` (or `operator []`) takes O $(\lg n)$ time,where $n$ represents the size
of the `map`. It is believed as a fast and mature implementation.

### `MipsVar.h/cpp` `MipsRCom.h/cpp` `MipsICom.h/cpp` `MipsJCom.h/cpp`

These four classes share the same structure. We just need to consider to make access to specific `<key,value>` pair. And
we do not need to support inserting pairs, since we can initialize them with corresponding data. Class `MipsVar` is to
store registers' names and binary forms, and `MipsRCom`, `MipsICom` and `MipsJcom` stores R-type commands, I-type
commands and J-Types commands respectively. We use `unordered_map` from `std` as actual container of these pairs.

### `Phase1.h/cpp`

- ```cpp
  void open(const std::string& sourceFile, std::vector<std::string>& src)
  ```

  The first parameter `sourceFile` is consistent with the first parameter of `main` function, representing the directory
  of the source code file. The second parameter is the `vector` to store the source code in cache and transfer them into
  machine code.

  We can use `ifstream` to open file and use `getline()` to read the file line by line and copy it into `string`
  after `.text`, then insert the `string` into the `vector` to store these instructions in cache.

- ```cpp
  void scan(std::vector<std::string>& src, LabelTable& labelTable, int ADDRESS)
  ```

  The first parameter is the same `vector` in previous functions used to store instructions in cache. The second
  parameter is the `LabelTable` to store label and mapping address. The third parameter is used to maintain the
  instruction address with the initial value 0x400000 in this project.

  As we traverse the `src`, we make a reference to the element and naming it with `line`. For each line, we need to
  remove comment first. Then, it needs to identify whether there exists any label or instruction. If there is a label,
  we also need to find the address of its trailing instruction. Then, put this pair of `<label, address>`.

### `Phase2.h/cpp`

- ```cpp
  void translate(std::vector<std::string>& cache,
                 int address,
                 const LabelTable& labelTable,
                 const MipsVar& mipsVar,
                 const MipsRCom& mipsRCom,
                 const MipsICom& mipsICom,
                 const MipsJCom& mipsJCom);
  ```

  The `cache` is the `vector` to store source codes. The `address` is to maintain the address of instructions during the
  process. The `labelTable`, `mipsVar`, `mipsRCom`, `mipsICom` and `mipsJCom` in this phase are inherited from
  the `main`.

  We read the cache line by line, and extract the command. In this way, we can classify the type of command and convey
  them into proper function.

- ```cpp
  std::string transRegister(auto& start, const auto& end, const MipsVar& mipsVar);
  ```

  `start` is the `iterator` pointing to the address in a `string` which may contains a register name begin with '\$',
  while `end` is another `iterator` to illustrates the searching border.

  Any register name must be started with '\$'. We can find it as the indicator for the beginning of a register name.
  Then, register name must be a combination of alphas and numbers, called `alnum`. We can also find the first character
  which is not `alnum` after '\$' as the end of the register name. Therefore, we can extract the register name and
  mapping it into binary form. Also, to make continuous searching more convenient, we make the `start` to point at the
  address of the end of the register name.

- ```cpp
  std::string convertNum(auto& start, auto& end);
  ```

  `start` is the `iterator` pointing to the address in a `string` which is the first digit of the number, while `end` is
  another `iterator` to illustrates the end of number.

  The functions is to convert `immediate` and `sa` into binary form. Note that it supports `0x`/`0X`, `0` and `0b`
  prefix as the indicator of hexadecimal, octal and binary number respectively. Also, to take negative value, just put a
  minus sign at the beginning. And the binary form does not pertain 2's complement.

- ```cpp
  void transRType(const std::string& command,
                  std::string& line,
                  const LabelTable& labelTable,
                  const MipsVar& mipsVar,
                  const MipsRCom& mipsRCom);
  ```

  `command` derives from `translate()` function in previous. `line` is the string stored in cache.

  R type commands can be separated into different kinds following its pattern. No matter what kind it is, we can always
  use `transRegister()` to handle register names after `pos` (note that the functions will change `pos`) following its
  sequence. The only special case is the one involved `sa`. We just need to find its position as `iterator`, and
  use `convertNum()` to deal with it. Since it returns a 16-bit machine code in default, we need to take a sub-string.

- ```cpp
  void transJType(const std::string& command, 
                  std::string& line, 
                  const LabelTable& labelTable, 
                  const MipsJCom& mipsJCom);
  ```

  J type commands are the easiest ones. We just need to find the label and then map them into address. Only need to care
  that the address needs to be divided by 4 before transformed into machine code.

- ```cpp
  void transIType(int address,
                  const std::string& command,
                  std::string& line,
                  const LabelTable& labelTable,
                  const MipsVar& mipsVar,
                  const MipsICom& mipsICom);
  ```

  `address` indicates the address of current instruction to handle PC relative commands.

  The basic procedures are similar to `transRType()`, which is based on the command pattern and registers sequence.
  However, the most different part is the `immediate`, which is mostly numbers. Also, the address represented as
  PC-relative form also needs to be achieved by certain calculation.

- ```cpp
  void Output(const std::string& outputFileDir, 
              const std::vector<std::string>& cache);
  ```

  Open (or create) the file in the directory as the first parameter. And write the result from cache into it.

- ```cpp
  void compare(const std::string& actOut, const std::string& expOut);
  ```

  Open two files and compare.

## Reference

[cppreference.com](https://en.cppreference.com/w/)

[Reference - C++ Reference](https://cplusplus.com/reference/)
