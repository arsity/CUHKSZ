module MIPS_CPU
(
    //input
    input wire clk,
    input wire reset,
    input wire[31:0] inst,


    // from data memory
    input wire[31:0] datamem_mem_redata,
            
    //output to instROM
    output wire[31:0] insAddr_o,
    output wire insEn,

    // output to data memory
    output wire mem_datamem_wrn,
    output wire mem_datamem_ce,
    output wire[31:0] mem_datamem_addr,
    output wire[31:0] mem_datamem_wrdata

);

// -------- varaiables defines ---------
// The variables to connect IF_ID module and ID module
    wire[31:0] pc;
    wire[31:0] pipe_id_pc;
    wire[31:0] pipe_id_inst;

// ID + ID_EX
    wire[31:0] id_pipe_reData1;
    wire[31:0] id_pipe_reData2;
    wire id_pipe_wrn;
    wire[4:0] id_pipe_wrAddr;
    wire[5:0] id_pipe_aluOp;
    // wire[`ALUSelBus] id_pipe_aluSel;

    wire id_pipe_next_delayslotEn;

    wire[31:0] id_pipe_inst;

// ID + RegFiles
    wire id_regfile_ren1;
    wire id_regfile_ren2;
    wire[4:0] id_regfile_reData1Addr;
    wire[4:0] id_regfile_reData2Addr;
    wire[31:0] regfile_id_reData1;
    wire[31:0] regfile_id_reData2;

// ID_EX + EX
    wire[31:0] pipe_ex_reData1;
    wire[31:0] pipe_ex_reData2;
    wire pipe_ex_wrn;
    wire[4:0] pipe_ex_wrAddr;
    wire[5:0] pipe_ex_aluOp;
    // wire[`ALUSelBus] pipe_ex_aluSel;

    wire[31:0] pipe_ex_inst;

// EX + EX_MEM
    wire ex_pipe_wrn;
    wire[4:0] ex_pipe_wrAddr;
    wire[31:0] ex_pipe_result;
    

    wire[5:0] ex_pipe_alu_op;
    wire[31:0] ex_pipe_mem_addr;
    wire[31:0] ex_pipe_mem_data;

// EX_MEM + MEM
    wire pipe_mem_wrn;
    wire[4:0] pipe_mem_wrAddr;
    wire[31:0] pipe_mem_wrData;

    wire[5:0] pipe_mem_alu_op;
    wire[31:0] pipe_mem_mem_addr;
    wire[31:0] pipe_mem_mem_data;

// MEM + MEM_WB
    wire mem_pipe_wrn;
    wire[4:0] mem_pipe_wrAddr;
    wire[31:0] mem_pipe_wrData;
    

// MEM_WB + REGFILES
    wire pipe_wb_regfile_wrn;
    wire[4:0] pipe_wb_regfile_wrAddr;
    wire[31:0] pipe_wb_regfile_wrData;
    
// IDEX + ID
    wire ID_EX_delayslotEN;

// ID + PC
    wire id_pc_branchEN;
    wire[31:0] id_pc_branchAddr;

// MEM + DATAMEM
   //wire mem_datamem_wrn;
   //wire mem_datamem_ce;
   //wire[31:0] mem_datamem_addr;
   //wire[31:0] mem_datamem_wrdata;

// DATAMEM + MEM
    //wire[31:0] datamem_mem_redata;

// stallreq
    wire id_ctrl_stall_req;


// ctrl + stall
    wire[5:0] ctrl_stall;

// ------------------------------------------------------------

// PC 
    pc pc_0
    (
        // input
        .clk(clk), .reset(reset), .stall(ctrl_stall),
        .branchEnable(id_pc_branchEN), .branchAddress(id_pc_branchAddr), .currentIns(inst),
        // output
        .PC(pc), .instructionEnable(insEn)
    );	

assign insAddr_o = pc;
// IF_ID
    IF_ID IF_ID_0
    (
        // input
        .clk(clk), .reset(reset), .stall(ctrl_stall),
        .IF_PC(pc), .IF_Instruction(inst),
        // output
        .ID_PC(pipe_id_pc), .ID_Instruction(pipe_id_inst)
    );

// ID
    ID ID_0
    (
        // input 
        .reset(reset),
        .Instruction(pipe_id_inst), .PC(pipe_id_pc),
        .reg1Data(regfile_id_reData1), .reg2Data(regfile_id_reData2), // from regfiles

        .ex_writeBack(ex_pipe_wrn), .ex_writeBackAddr(ex_pipe_wrAddr), .ex_writeBackData(ex_pipe_result),
        .mem_write(mem_pipe_wrn), .mem_writeAddress(mem_pipe_wrAddr), .mem_writeData(mem_pipe_wrData),

        .delay(ID_EX_delayslotEN),

        .last_ALUopcode(pipe_ex_aluOp),

        // output to pipe
        .reg1Data_o(id_pipe_reData1), .reg2Data_o(id_pipe_reData2), 
        .writeBack(id_pipe_wrn), .writeBackAddr(id_pipe_wrAddr),
        .ALUopcode(id_pipe_aluOp), //.alu_sel(id_pipe_aluSel),
        
        //output to regfiles
        .reg1En(id_regfile_ren1), .reg2En(id_regfile_ren2),
        .reg1Addr(id_regfile_reData1Addr), .reg2Addr(id_regfile_reData2Addr),

        .next_delay(id_pipe_next_delayslotEn),
        
        .branchEn(id_pc_branchEN), 
        .branchAddress(id_pc_branchAddr),
        
        .Instruction_o(id_pipe_inst),
        .stall_req(id_ctrl_stall_req)
    );
    
// RegFiles
    Registers RegFiles_0
    (
        // input
        .clk(clk), .reset(reset),
        .r1En(id_regfile_ren1), .r2En(id_regfile_ren2),
        .r1Address(id_regfile_reData1Addr), .r2Address(id_regfile_reData2Addr),
        
        .WB_Enable(pipe_wb_regfile_wrn), .WB_Address(pipe_wb_regfile_wrAddr), .WB_Data(pipe_wb_regfile_wrData),
        //output
        .reg1Data(regfile_id_reData1), .reg2Data(regfile_id_reData2)
    );

// ID_EX
    ID_EX ID_EX_0
    (
        // input
        .clk(clk), .reset(reset), .ID_Instruction(id_pipe_inst), .stall(ctrl_stall),
        .ID_r1Data(id_pipe_reData1), .ID_r2Data(id_pipe_reData2),
        .ID_writeEnable(id_pipe_wrn), .ID_writeAddress(id_pipe_wrAddr),
        .ID_ALUopcode(id_pipe_aluOp),// .id_alu_sel(id_pipe_aluSel),

        .ID_DelayEnable(id_pipe_next_delayslotEn),
        // output
        .EX_r1Data(pipe_ex_reData1), .EX_r2Data(pipe_ex_reData2),
        .EX_writeEnable(pipe_ex_wrn), .EX_writeAddress(pipe_ex_wrAddr),
        .EX_ALUopcode(pipe_ex_aluOp),// .ex_alu_sel(pipe_ex_aluSel),

        .EX_DelayEnable(ID_EX_delayslotEN),
        .EX_Instruction(pipe_ex_inst)
    );

// EX
    EX EX_0
    (
        // input
        .reset(reset), .Instruction(pipe_ex_inst),
        .reg1Data(pipe_ex_reData1), .reg2Data(pipe_ex_reData2),
        .writeEnable(pipe_ex_wrn), .writeAddress(pipe_ex_wrAddr),
        .ALUopcode(pipe_ex_aluOp), //.alu_sel(pipe_ex_aluSel),
        
        // output
        .o_writeEnable(ex_pipe_wrn), .o_writeAddress(ex_pipe_wrAddr),
        .result(ex_pipe_result),

        // output about mem
        .o_ALUopcode(ex_pipe_alu_op), .memoryAddress(ex_pipe_mem_addr), .memoryData(ex_pipe_mem_data)
    );

// EX_MEM
    EX_MEM EX_MEM_0
    (
        // input
        .clk(clk), .reset(reset),.stall(ctrl_stall),
        .EX_writeEnable(ex_pipe_wrn), .EX_writeAddress(ex_pipe_wrAddr),
        .EX_result(ex_pipe_result),

        .EX_ALUopcode(ex_pipe_alu_op), .EX_memoryAddress(ex_pipe_mem_addr), .EX_memoryData(ex_pipe_mem_data),
        
        //output
        .MEM_writeEnable(pipe_mem_wrn), .MEM_writeAddress(pipe_mem_wrAddr),
        .MEM_result(pipe_mem_wrData),

        .MEM_ALUopcode(pipe_mem_alu_op), .MEM_memoryAddress(pipe_mem_mem_addr), .MEM_memoryData(pipe_mem_mem_data)
    );

// MEM
    MEM MEM_0
    (
        // input
        .reset(reset),
        .writeEnable(pipe_mem_wrn), .writeAddress(pipe_mem_wrAddr), .result(pipe_mem_wrData),
        
        // about memory
        .ALUopcode(pipe_mem_alu_op), .memoryAddress(pipe_mem_mem_addr), .memoryWriteData(pipe_mem_mem_data),

        // from memory
        .memoryReadData(datamem_mem_redata),

        //output
        .o_writeEnable(mem_pipe_wrn), .o_writeAddress(mem_pipe_wrAddr), .o_result(mem_pipe_wrData),

        // into memory
        .o_memoryWrite(mem_datamem_wrn), .o_readOrWrite(mem_datamem_ce), .o_memData(mem_datamem_wrdata), .o_memAddress(mem_datamem_addr)
    );
    
// MEM_WB
    MEM_WB MEM_WB_0
    (
        // input
        .clk(clk), .reset(reset),.stall(ctrl_stall),
        .MEM_writebackEnable(mem_pipe_wrn), .MEM_writebackAddress(mem_pipe_wrAddr), .MEM_writebackData(mem_pipe_wrData),

        // output
        .WB_writebackEnable(pipe_wb_regfile_wrn), .WB_writebackAddress(pipe_wb_regfile_wrAddr), .WB_writebackData(pipe_wb_regfile_wrData)
    );


// CTRL
    stall_gen CTRL
    (
        .reset(reset),
        .signal(id_ctrl_stall_req),
        //.ex_stall(),
        .stall(ctrl_stall)
    );


endmodule
