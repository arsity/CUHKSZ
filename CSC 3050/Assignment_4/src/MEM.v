/*
    The module used to control the access of memory 
*/
module MEM (
    // ------ Input ------
    input wire reset,
    input wire[31:0] result,           // ALU calculated result
    input wire writeEnable,            // write back signal
    input wire[4:0] writeAddress,      // write back address
    input wire[5:0] ALUopcode,         // ALU opcode
    input wire[31:0] memoryAddress,    // memory address
    input wire[31:0] memoryWriteData,  // memory write data
    input wire[31:0] memoryReadData,   // memory read data
    // ------ Output ------

    // to register things
    output reg[31:0] o_result,         // ALU calculated result
    output reg o_writeEnable,          // write back signal
    output reg[4:0] o_writeAddress,    // write back address

    // to memory things
    output reg o_memoryWrite,          // memory write signal
    output reg o_readOrWrite,          // memory operation signal
    output reg[31:0] o_memAddress,     // memory address to memory
    output reg[31:0] o_memData         // memory data to memory
);

    always @(*) begin
        if(reset == 1) begin
            o_result <= 32'h0000_0000;
            o_writeEnable <= 0;
            o_writeAddress <= 5'b00000;
            o_memoryWrite <= 0;
            o_readOrWrite <= 0;
            o_memAddress <= 32'h0000_0000;
            o_memData <= 32'h0000_0000;
        end else begin
            o_result <= result;
            o_writeEnable <= writeEnable;
            o_writeAddress <= writeAddress;
            o_memoryWrite <= 0;
            o_readOrWrite <= 0;
            o_memAddress <= 32'h0000_0000;
            o_memData <= 32'h0000_0000;
            case (ALUopcode)
                6'b101011: begin // sw
                    o_memoryWrite <= 1;
                    o_readOrWrite <= 1;
                    o_memAddress <= memoryAddress;
                    o_memData <= memoryWriteData;
                end
                6'b100011: begin // lw
                    o_memoryWrite <= 0;
                    o_readOrWrite <= 1;
                    o_memAddress <= memoryAddress;
                    o_memData <= memoryReadData;
                end
                6'b111111: begin
                    o_memoryWrite <= 0;
                    o_readOrWrite <= 1;
                    o_memAddress <= 32'hffff_ffff;
                    o_memData <= 32'hffff_ffff;
                end
            endcase
        end
    end
endmodule