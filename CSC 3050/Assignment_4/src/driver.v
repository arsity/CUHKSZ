
module driver(
    // input
    input wire clk,
    input wire rst,
    input show,
    output wire[31:0] inst,
    output reg stop
);
    wire insEn;
    // wire[31:0] inst;
    
    
    
    wire[31:0] instAddr;
    
    wire[31:0] datamem_mem_redata;
    wire mem_datamem_wrn;
    wire mem_datamem_ce;
    wire[31:0] mem_datamem_addr;
    wire[31:0] mem_datamem_wrdata;


    //OpenMips OpenMips_0
    MIPS_CPU MIPS32
    (
        // input
        .clk(clk),
        .reset(rst),
        .inst(inst),
        
        // output
        .insAddr_o(instAddr),
        .insEn(insEn),
        .datamem_mem_redata(datamem_mem_redata),
        .mem_datamem_wrn(mem_datamem_wrn),
        .mem_datamem_ce(mem_datamem_ce),
        .mem_datamem_addr(mem_datamem_addr),
        .mem_datamem_wrdata(mem_datamem_wrdata)
    );

    InstructionRAM insMem_0
    (
        //input
        .CLOCK(clk),
        .RESET(rst),
        .FETCH_ADDRESS(instAddr>>2),
        .ENABLE(insEn),
        
        // output
        .DATA(inst)
    );

    // dataMemory
    MainMemory DATAMEM
    (
        .CLOCK(clk),
        .RESET(rst),
        .ENABLE(mem_datamem_ce),
        .FETCH_ADDRESS($signed(mem_datamem_addr)>>>2),
        .EDIT_SERIAL({mem_datamem_wrn,$signed(mem_datamem_addr)>>>2,mem_datamem_wrdata}),
        .DATA(datamem_mem_redata),
        .show(show)
    );

    //     always @(mem_datamem_addr, mem_datamem_wrdata) begin
    //     if (mem_datamem_addr==32'hffff_ffff && mem_datamem_wrdata==32'hffff_ffff)begin
    //         stop <= 1;
    //     end else begin
    //         stop <= 0;
    //     end
    // end
endmodule


