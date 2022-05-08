/*
    The register between MEM stage and WB stage
*/
module MEM_WB (
    // ------ Input ------
    input wire reset,
    input wire clk,
    input wire MEM_writebackEnable,         // write back signal
    input wire[4:0] MEM_writebackAddress,   // write back address
    input wire[31:0] MEM_writebackData,     // write back data
    input wire[5:0] stall,                  // stall status
    // ------ Output ------
    output reg WB_writebackEnable,          // write back signal 
    output reg[4:0] WB_writebackAddress,    // write back address
    output reg[31:0] WB_writebackData       // write back data
);

    always @(posedge clk) begin
        if(reset == 1) begin
            WB_writebackEnable <= 0;
            WB_writebackAddress <= 5'b00000;
            WB_writebackData <= 32'h0000_0000;
        end else begin
            // stall case
            if(stall[4] == 1 && stall[5] == 0) begin
                WB_writebackEnable <= 0;
                WB_writebackAddress <= 5'b00000;
                WB_writebackData <= 32'h0000_0000;
            // default case
            end else if(stall[4] == 0) begin
                WB_writebackEnable <= MEM_writebackEnable;
                WB_writebackAddress <= MEM_writebackAddress;
                WB_writebackData <= MEM_writebackData;
            end
        end
    end
endmodule