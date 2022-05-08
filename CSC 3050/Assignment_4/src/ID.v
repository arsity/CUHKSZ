/*
    ID stage module	
*/
module ID (
    // ------ Input ------
    input wire reset,
    input wire[31:0] PC,
    input wire[31:0] Instruction,
    input wire[31:0] reg1Data,
    input wire[31:0] reg2Data,

    input wire ex_writeBack,
    input wire[4:0] ex_writeBackAddr,
    input wire[31:0] ex_writeBackData,
    
    input wire mem_write,
    input wire[4:0] mem_writeAddress,
    input wire[31:0] mem_writeData,

    input wire delay,
    
    input wire[5:0] last_ALUopcode,
    
    // ------ Output ------
    output reg reg1En,
    output reg reg2En,
    output reg[4:0] reg1Addr,
    output reg[4:0] reg2Addr,
    output reg[31:0] reg1Data_o,
    output reg[31:0] reg2Data_o,

    output reg writeBack,
    output reg[4:0] writeBackAddr,
    // output reg[31:0] writeBackData,

    output reg[5:0] ALUopcode,
    output reg branchEn,
    output reg[31:0] branchAddress,

    output reg next_delay,
    
    output reg[31:0] Instruction_o,

    output wire stall_req
);
    
    wire[5:0] op = Instruction[31:26];
    wire[4:0] rs = Instruction[25:21];
    wire[4:0] rt = Instruction[20:16];
    wire[4:0] rd = Instruction[15:11];
    wire[4:0] shamt = Instruction[10:6];
    wire[5:0] func = Instruction[5:0];
    wire[15:0] imm = Instruction[15:0];

    reg[31:0] ext_imm; // extend immediate

    // variables for branch hazards
    wire[31:0] pc_plus_4;
    wire[31:0] imm_sll2_ext; // shift left twice
    assign pc_plus_4 = PC + 4;
    assign imm_sll2_ext = {{14{imm[15]}},imm,2'b00};

    // variables for load stall
    wire pre_instruc_load;
    assign pre_instruc_load = (last_ALUopcode == 6'b100011) ? 1'b1 : 1'b0;

    reg reg1_stall;
    reg reg2_stall;

    // control signal
    always @(*) begin
        if(reset == 1) begin
            reg1En <= 0;
            reg2En <= 0;
            reg1Addr <= 0;
            reg2Addr <= 0;
            reg1Data_o <= 0;
            reg2Data_o <= 0;
            writeBack <= 0;
            writeBackAddr <= 0;
            ALUopcode <= 0;
            branchEn <= 0;
            branchAddress <= 0;
            next_delay <= 0;
            Instruction_o <= 0;
        end else begin
            // reg1Data_o <= 0;
            // reg2Data_o <= 0;
            ALUopcode <= op;
            branchEn <= 0;
            branchAddress <= 0;
            next_delay <= 0;
            Instruction_o <= Instruction;
            if(delay == 0) begin
                case (op)
                    6'b000000: begin // R type
                        case (func)
                            6'b100000: begin // add
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100001: begin // addu
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100010: begin // sub
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100011: begin // subu
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100100: begin // and
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100111: begin // nor
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100101: begin // or
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b100110: begin // xor
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b101010: begin // slt
                                reg1En <= 1;
                                reg2En <= 1;
                                reg1Addr <= rs;
                                reg2Addr <= rt;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b000000: begin // sll
                                reg1En <= 1;
                                reg1Addr <= rt;
                                reg2En <= 0;
                                reg2Addr <= 0;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b000100: begin // sllv
                                reg1En <= 1;
                                reg1Addr <= rt;
                                reg2En <= 1;
                                reg2Addr <= rs;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b000010: begin // srl
                                reg1En <= 1;
                                reg1Addr <= rt;
                                reg2En <= 0;
                                reg2Addr <= 0;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b000110: begin // srlv
                                reg1En <= 1;
                                reg1Addr <= rt;
                                reg2En <= 1;
                                reg2Addr <= rs;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b000011: begin // sra
                                reg1En <= 1;
                                reg1Addr <= rt;
                                reg2En <= 0;
                                reg2Addr <= 0;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b000111: begin // srav
                                reg1En <= 1;
                                reg1Addr <= rt;
                                reg2En <= 1;
                                reg2Addr <= rs;
                                writeBack <= 1;
                                writeBackAddr <= rd;
                            end
                            6'b001000: begin // jr
                                reg1En <= 1;
                                reg1Addr <= rs;
                                reg2En <= 0;
                                reg2Addr <= 0;
                                branchEn <= 1;
                                branchAddress <= reg1Data_o;
                                next_delay <= 1;
                            end
                            default:begin
                            end
                        endcase
                    end
                    6'b001000: begin // addi
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rt;
                        ext_imm <= {{16{imm[15]}}, imm[15:0]};
                    end
                    6'b001001: begin // addiu
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rt;
                        ext_imm <= {{16{imm[15]}}, imm[15:0]};
                    end
                    6'b001100: begin // andi
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rt;
                        ext_imm <= {16'b0, imm};
                    end
                    6'b001101: begin // ori
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rt;
                        ext_imm <= {16'b0, imm};
                    end
                    6'b001110: begin // xori
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rt;
                        ext_imm <= {16'b0, imm};
                    end
                    6'b000100: begin // beq
                        reg1En <= 1;
                        reg1Addr <= rt;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rs;
                        if (reg1Data_o == $signed(imm)) begin
                            branchEn <= 1;
                            branchAddress <= pc_plus_4 + imm_sll2_ext;
                            next_delay <= 1;
                        end
                    end
                    6'b000101: begin // bne
                        reg1En <= 1;
                        reg1Addr <= rt;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rs;
                        if (reg1Data_o != $signed(imm)) begin
                            branchEn <= 1;
                            branchAddress <= pc_plus_4 + imm_sll2_ext;
                            next_delay <= 1;
                        end
                    end
                    6'b000010: begin // j
                        branchEn <= 1;
                        branchAddress <= {pc_plus_4[31:28],Instruction[25:0],2'b0};
                        next_delay <= 1;
                    end
                    6'b000011: begin // jal 
                        branchEn <= 1;
                        branchAddress <= {pc_plus_4[31:28],Instruction[25:0],2'b0};
                        next_delay <= 1;
                        reg1Data_o <= pc_plus_4;
                        writeBack <= 1;
                        writeBackAddr <= 5'b01111;
                    end
                    6'b100011: begin // lw
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 0;
                        reg2Addr <= 0;
                        writeBack <= 1;
                        writeBackAddr <= rt;
                    end
                    6'b101011: begin // sw
                        reg1En <= 1;
                        reg1Addr <= rs;
                        reg2En <= 1;
                        reg2Addr <= rt;
                        writeBack <= 0;
                        writeBackAddr <= 0;
                    end
                    6'b111111: begin
                                reg1En <= 0;
                                reg2En <= 0;
                                reg1Addr <= 0;
                                reg2Addr <= 0;
                                writeBack <= 0;
                                writeBackAddr <= 0;
                    end
                endcase
            end
        end
    end

    always @(*) begin
        reg1_stall <= 0;
        if(reset == 1) begin
            reg1Data_o <= 0;
        end else if(reg1En == 1) begin
            if(pre_instruc_load == 1 && ex_writeBackAddr == reg1Addr) begin
                reg1_stall <= 1;
                // $display("lw hazard %h",PC);
            end else if(ex_writeBack == 1 && ex_writeBackAddr == reg1Addr) begin
                reg1Data_o <= ex_writeBackData;
                // $display("R type hazard %h",PC);
            end else if(mem_write == 1 && mem_writeAddress == reg1Addr) begin
                reg1Data_o <= mem_writeData;
                // $display("sw hazard %h",PC);
            end else begin
                reg1Data_o <= reg1Data;
            end
        end else if(reg1En == 0) begin
            reg1Data_o <= ext_imm;
        end 
        // else begin
        //     reg1Data_o <= 0;
        // end
    end

    always @(*) begin
        reg2_stall <= 0;
        if(reset == 1) begin
            reg2Data_o <= 0;
        end else if(reg2En == 1) begin
            if(pre_instruc_load == 1 && ex_writeBackAddr == reg2Addr) begin
                reg2_stall <= 1;
                // $display("lw hazard %h",PC);
            end else if(ex_writeBack == 1 && ex_writeBackAddr == reg2Addr) begin
                reg2Data_o <= ex_writeBackData;
                // $display("R type hazard %h",PC);
            end else if(mem_write == 1 && mem_writeAddress == reg2Addr) begin
                reg2Data_o <= mem_writeData;
                // $display("sw hazard %h",PC);
            end else begin
                reg2Data_o <= reg2Data;
            end
        end else if(reg2En == 0) begin
            reg2Data_o <= ext_imm;
        end 
        // else begin
        //     reg2Data_o <= 0;
        // end
    end

    assign stall_req = reg1_stall | reg2_stall;
endmodule


/*
    Fetch and write data in general registers of the CPU.
*/
module Registers (
    // ------ Input ------
    input wire reset,
    input wire clk,
    input wire r1En,             // fetch from rs signal
    input wire r2En,             // fetch from rt signal
    input wire[4:0] r1Address,       // rs address
    input wire[4:0] r2Address,       // rt address
    input wire WB_Enable,            // signal in WB stage
    input wire[4:0] WB_Address,      // data address in WB stage
    input wire[31:0] WB_Data,        // data in WB stage
    // ------ Output ------
    output reg[31:0] reg1Data,         // rs data
    output reg[31:0] reg2Data          // rt data
);
    
    // initialze all registers
    reg[31:0] registers[31:0];
    integer i;
    initial begin
        for (i = 0; i < 32; i = i + 1) begin
            registers[i] = 32'h0000_0000;
        end
    end

    // write
    always @(posedge clk) begin
        if(reset == 0 && WB_Enable == 1 && WB_Address != 5'b0) begin
            registers[WB_Address] <= WB_Data;
        end
    end

    // rs read
    always @(*) begin
        if(reset == 1) begin
            reg1Data <= 32'h0000_0000;
        // direct fetch
        end else if(WB_Enable == 1 && WB_Address == r1Address && r1En == 1) begin
            reg1Data <= WB_Data;
        // normal case
        end else if(r1En == 1) begin
            reg1Data <= registers[r1Address];
        end else begin
            reg1Data <= 32'h0000_0000;
        end
    end

    // rt read
    always @(*) begin
        if(reset == 1) begin
            reg2Data <= 32'h0000_0000;
        // direct fetch
        end else if(WB_Enable == 1 && WB_Address == r2Address && r2En == 1) begin
            reg2Data <= WB_Data;
        // normal case
        end else if(r2En == 1) begin
            reg2Data <= registers[r2Address];
        end else begin
            reg2Data <= 32'h0000_0000;
        end
    end
endmodule
