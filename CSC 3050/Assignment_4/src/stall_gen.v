module stall_gen (
    // ------ Input ------
    input wire reset,
    input wire signal,
    // ------ Output ------
    output reg[5:0] stall
);
    
    always @(*) begin
        if(reset == 1) begin
            stall <= 6'b000000;
        end else begin
            if(signal == 1) begin
                stall <= 6'b000111;
                // $display("Stall gen");
            end else begin
                stall <= 6'b000000;
            end
        end
    end
endmodule