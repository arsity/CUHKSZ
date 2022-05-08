`timescale 1fs/1fs

module testBench(
);
    reg CLOCK;
    reg clk2;
    reg rst;
    wire[31:0] inst;

    integer i;
    integer t;
    wire[31:0] data;
    wire stop;
    reg show;
    // initial
    // begin
        // rst = 1;
        // #20 rst = 0;
        // #100000 $display("  Word Unit : Data");
        // clk2 = 1'b0;
        // for (i=511; i >= 0; i = i - 1) begin
        //     #10 clk2 = ~clk2;
        //     $display("%d : %h", i, data);
        // end
        // $monitor("%b : %b", i, data);
    //     #100000 $stop;
    // end


    // always #5 CLOCK=~CLOCK;

    initial
    begin            
        $dumpfile("res.vcd");        //生成的vcd文件名称
        $dumpvars(0, testBench);     //tb模块名称
    end
    
always begin
    #5 CLOCK = ~CLOCK;
    t = t + 1;
end

    initial begin
        t = 0;
        CLOCK = 1'b0;
        rst = 1;
        // inst = 0;
        #30 rst = 0;
        show <= 0;
        // while (stop != 1 ) begin
        //     #5 CLOCK=~CLOCK;
        // end
        #100000 show <= 1;
        #100 $display("Total %h clocks",t);
        #10 $stop;
    end




    driver driver_0
    (	
        .clk(CLOCK),
        .rst(rst),
        .inst(inst),
        .show(show),
        .stop(stop)
    );
endmodule
