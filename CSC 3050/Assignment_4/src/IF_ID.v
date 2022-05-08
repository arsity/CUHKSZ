/*
    The register between IF stage and ID stage.
*/
module IF_ID (
    // ------ Input ------
    input wire reset,
    input wire clk,
    input wire[31:0] IF_PC,            // PC in IF stage
    input wire[31:0] IF_Instruction,   // Instruction fetched in IF stage
    input wire[5:0] stall,
    // ------ Output ------
    output reg[31:0] ID_PC,            // PC in ID stage
    output reg[31:0] ID_Instruction    // Instruction in ID stage
);
    
    always @(posedge clk) begin
        // reset case
        if(reset == 1) begin
            ID_PC <= 32'h0000_0000;
            ID_Instruction <= 32'h0000_0000;
        end else begin
            // stall IF
            if(stall[1] == 1 && stall[2] == 0) begin
                ID_PC <= 32'h0000_0000;
                ID_Instruction <= 32'h0000_0000;
            // default case
            end else if(stall[1] == 0) begin
                ID_PC <= IF_PC;
                ID_Instruction <= IF_Instruction;
            end
        end
    end
endmodule
