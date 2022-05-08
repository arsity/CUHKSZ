res: instructions.bin ./src/testBench.v ./src/driver.v ./include/InstructionRAM.v ./include/MainMemory.v ./src/CPU.v ./src/EX_MEM.v ./src/EX.v ./src/ID_EX.v ./src/ID.v ./src/IF_ID.v ./src/MEM_WB.v ./src/MEM.v ./src/pc.v ./src/stall_gen.v
	iverilog -o res ./src/testBench.v ./src/driver.v ./include/InstructionRAM.v ./include/MainMemory.v ./src/CPU.v ./src/EX_MEM.v ./src/EX.v ./src/ID_EX.v ./src/ID.v ./src/IF_ID.v ./src/MEM_WB.v ./src/MEM.v ./src/pc.v ./src/stall_gen.v

run: res
	vvp res

res.vcd: res
	vvp -n res -lxt2

wave: res.vcd
	gtkwave res.vcd

.phony: clear
clear:
	rm ./res
	rm ./res.vcd