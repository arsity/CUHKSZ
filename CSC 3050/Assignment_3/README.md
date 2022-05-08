# Dependencies

- Icarus Verilog >= 11.0
- GNU Make >= 4.3

# Start

```bash
make
```

Built files are under `./build`.

# Basic Ideas

- `add` series (`addi`,`addu`,`addiu`,`lw`,`sw`) share similar logic. We can directly use the arithmetic add with some specific extra component to implement. Also, `sub` and `subu` can be treated as `add` and `addu`, respectively.
- We can use the bit-wise logical operations to achieve bit-wise logical instructions like `and`, `or`, and so on.
- `beq` and `bne` is the comparison between the data in two registers. The status of zero flag indicates the result.
- `slt` series (`slti`, `sltu`, `sltiu`) can use the `sub` and `subu` logic. The status of negative flag indicates the signed bit.

# Details

### Introduction

All ALU functions are implemented in `./src/ALU.v`. We use `switch` to distinguish different cases by `opcode` and `function` (R type instructions only).

### Mapping Tricks

As we only support two registers, some instructions like `add` are not necessary to find out which register `regA` corresponds with. We can just directly sum two registers up. Also, if we have mapped one register to correct place, the other must be corresponding to the left one.

### Overflow Detection

There is a basic rule for `add`: two positive numbers make a positive result, two negative numbers make a negative result. In this way, we just only need to check the signed bits of two sources and the result.

### Immediate Mapping for I Types

Immediate is represented as 16-bit signed numbers. To make the calculation more convenient, we can use 32-bit register to store this number. We can treat it as signed number and map it into 32 bits.

### Additive Inverse

In `sub` and `subu`, we can use additive inverse to make it as `add` and `addu` respectively. We can use 2's complement to achieve. First, invert all bits. Second, plus 1 to the result.