/*
    The register between ID stage and EX stage
*/
module ID_EX (
    // ------ Input ------
    input wire reset,
    input wire clk,
    input wire[31:0] ID_Instruction,     // instruction from ID stage
    input wire[31:0] ID_r1Data,          // rs Data from ID stage
    input wire[31:0] ID_r2Data,          // rt Data from ID stage
    input wire[5:0] ID_ALUopcode,        // ALU opcode from ID stage
    // input wire[2:0] ID_ALUType,          // ALU execution type from ID stage
    input wire ID_writeEnable,           // write back signal from ID stage
    input wire[4:0] ID_writeAddress,     // write back address from ID stage
    input wire[5:0] stall,               // stall control signal from ID stage
    input wire ID_DelayEnable,           // delay control signal from ID stage
    // ------ Output ------
    output reg[31:0] EX_Instruction,     // instruction in EX stage
    output reg[31:0] EX_r1Data,          // rs Data in EX stage
    output reg[31:0] EX_r2Data,          // rt Data in EX stage
    output reg[5:0] EX_ALUopcode,        // ALU opcode in EX stage
    // output reg[2:0] EX_ALUType,          // ALU execution type in EX stage
    output reg EX_writeEnable,           // write back signal in EX stage
    output reg[4:0] EX_writeAddress,     // write back address in EX stage
    output reg EX_DelayEnable            // delay control signal in EX stage
);

    always @(posedge clk) begin
        if(reset == 1)begin
            EX_Instruction <= 32'h0000_0000;
            EX_r1Data <= 32'h0000_0000;
            EX_r2Data <= 32'h0000_0000;
            EX_ALUopcode <= 8'h00;
            // EX_ALUType <= 3'b000;
            EX_writeEnable <= 0;
            EX_writeAddress <= 5'b00000;
            EX_DelayEnable <= 0;
        end else begin
            // stall case
            if(stall[2] == 1 && stall[3] == 0) begin
                EX_Instruction <= 32'h0000_0000;
                EX_r1Data <= 32'h0000_0000;
                EX_r2Data <= 32'h0000_0000;
                EX_ALUopcode <= 8'h00;
                // EX_ALUType <= 3'b000;
                EX_writeEnable <= 0;
                EX_writeAddress <= 5'b00000;
                EX_DelayEnable <= 0;
            // default case
            end else if(stall[2] == 0) begin
                EX_Instruction <= ID_Instruction;
                EX_r1Data <= ID_r1Data;
                EX_r2Data <= ID_r2Data;
                EX_ALUopcode <= ID_ALUopcode;
                // EX_ALUType <= ID_ALUType;
                EX_writeEnable <= ID_writeEnable;
                EX_writeAddress <= ID_writeAddress;
                EX_DelayEnable <= ID_DelayEnable;
            end
        end
    end
endmodule
