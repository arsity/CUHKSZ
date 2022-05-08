./build/wave.vcd: ./build/a.vvp
	vvp ./build/a.vvp

./build/a.vvp: ./test/test.v ./src/ALU.v
	iverilog -o ./build/a.vvp ./test/test.v ./src/ALU.v

.phony: clean
clean:
	rm ./build/a.vvp
