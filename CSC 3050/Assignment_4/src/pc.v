/*
    The pc module is used to simulate the pc action.
*/
module pc(
    // ------ Input ------
    input wire reset,                // reset (omitted following)
    input wire clk,                  // clock (omitted following)

    input wire branchEnable,         // branch enable
    input wire[31:0] branchAddress,	 // branch address

    input wire[5:0] stall,           // stall control signal
    input wire[31:0] currentIns,

    // ------ Output ------
    output reg[31:0] PC,             // new PC
    output reg instructionEnable     // instructionEnable signal
);
    
    
    // reset signal detect
    always@(posedge clk) begin
        if(reset == 1) begin
            instructionEnable <= 0;
            PC <= 32'h0000_0000;
        end else begin
            instructionEnable <= 1;
        end
    end

    // stall detect and instructionEnable detect
    always@(posedge clk) begin
        if(instructionEnable == 0) begin
            PC <= 32'h0000_0000;
        end else begin
            if(stall[0] == 0) begin
                if(branchEnable == 1) begin
                    PC <= branchAddress;
                end else begin
                    PC <= PC + 4;
                end
            end
        end
        // $display("%s %h, %s %d","PC is at",PC,"Word Unit: ",PC>>2);
    end
endmodule
