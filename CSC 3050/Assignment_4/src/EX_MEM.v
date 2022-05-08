/*
    The register between EX stage and MEM stage
*/
module EX_MEM (
    // ------ Input ------
    input wire reset,
    input wire clk,
    input wire[31:0] EX_result,           // result in EX stage
    input wire EX_writeEnable,            // write back signal in EX stage
    input wire[4:0] EX_writeAddress,      // write back address in EX stage
    input wire[5:0] EX_ALUopcode,         // ALU opcode in EX stage
    input wire[31:0] EX_memoryAddress,    // calculated memory address in EX stage
    input wire[31:0] EX_memoryData,       // memory data in EX stage
    input wire[5:0] stall,
    // ------ Output ------
    output reg[31:0] MEM_result,          // result in MEM stage
    output reg MEM_writeEnable,           // write back signal in MEM stage
    output reg[4:0] MEM_writeAddress,     // write back address in MEM stage
    output reg[5:0] MEM_ALUopcode,        // ALU opcode in MEM stage
    output reg[31:0] MEM_memoryAddress,   // calculated memory address in MEM stage
    output reg[31:0] MEM_memoryData       // memory data in MEM stage
);
    
    always @(posedge clk) begin
        if(reset == 1) begin
            MEM_result <= 32'h0000_0000;
            MEM_writeEnable <= 0;
            MEM_writeAddress <= 5'b00000;
            MEM_ALUopcode <= 8'h00;
            MEM_memoryAddress <= 32'h0000_0000;
            MEM_memoryData <= 32'h0000_0000;
        end else begin
            // stall case
            if(stall[3] == 1 && stall[4] == 0) begin
                MEM_result <= 32'h0000_0000;
                MEM_writeEnable <= 0;
                MEM_writeAddress <= 5'b00000;
                MEM_ALUopcode <= 8'h00;
                MEM_memoryAddress <= 32'h0000_0000;
                MEM_memoryData <= 32'h0000_0000;
            // default case
            end else if(stall[3] == 0) begin
                MEM_result <= EX_result;
                MEM_writeEnable <= EX_writeEnable;
                MEM_writeAddress <= EX_writeAddress;
                MEM_ALUopcode <= EX_ALUopcode;
                MEM_memoryAddress <= EX_memoryAddress;
                MEM_memoryData <= EX_memoryData;
            end
        end
    end
endmodule