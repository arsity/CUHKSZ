/*
    The ALU block
*/
module EX (
    // ------ Input ------
    input wire reset,
    input wire[31:0] Instruction,     // instruction
    input wire[31:0] reg1Data,        // reg1 Data
    input wire[31:0] reg2Data,        // reg2 Data
    input wire[5:0] ALUopcode,        // ALU opcode
    // input wire[2:0] ALUType,       // ALU execution type
    input wire writeEnable,           // write back signal
    input wire[4:0] writeAddress,     // write back address
    // ------ Output ------
    output reg[31:0] result,          // ALU result
    output reg o_writeEnable,         // write back signal
    output reg[4:0] o_writeAddress,   // write back address
    output reg[5:0] o_ALUopcode,      // ALU opcode
    output reg[31:0] memoryAddress,   // write to memory address
    output reg[31:0] memoryData       // write to memory data
);
    
    always @(*) begin
        case (ALUopcode)
            6'b0: begin // R type
                case (Instruction[5:0])
                    6'b100000: // add
                        begin
                            result <= reg1Data + reg2Data;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100001: // addu
                        begin
                            result <= reg1Data + reg2Data;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100010: // sub
                        begin
                            result <= reg1Data + ~(reg2Data) + 1;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100011: // subu
                        begin
                            result <= reg1Data + ~(reg2Data) + 1;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100100: // and
                        begin
                            result <= reg1Data & reg2Data;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100111: // nor
                        begin
                            result <= ~(reg1Data | reg2Data);
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100101: // or
                        begin
                            result <= reg1Data | reg2Data;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b100110: // xor
                        begin
                            result <= reg1Data ^ reg2Data;
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b101010: // slt
                        begin
                            if (reg1Data < reg2Data) begin
                                result <= 1;
                                o_writeEnable <= writeEnable;
                                o_writeAddress <= writeAddress;
                                o_ALUopcode <= ALUopcode;
                                memoryAddress <= 32'h0000_0000;
                                memoryData <= 32'h0000_0000;
                            end
                        end
                    6'b000000: // sll
                        begin
                            result <= reg1Data << ($signed(Instruction[10:6]));
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b000100: // sllv
                        begin
                            result <= reg1Data << ($signed(reg2Data[4:0]));
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b000010: // srl
                        begin
                            result <= reg1Data >> ($signed(Instruction[10:6]));
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b000110: // srlv
                        begin
                            result <= reg1Data >> ($signed(reg2Data[4:0]));
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b000011: // sra
                        begin
                            result <= reg1Data >>> ($signed(Instruction[10:6]));
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    6'b000111: // srav
                        begin
                            result <= reg1Data >>> ($signed(reg2Data[4:0]));
                            o_writeEnable <= writeEnable;
                            o_writeAddress <= writeAddress;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                    default: // jr
                        begin
                            result <= 0;
                            o_writeEnable <= 0;
                            o_writeAddress <= 0;
                            o_ALUopcode <= ALUopcode;
                            memoryAddress <= 32'h0000_0000;
                            memoryData <= 32'h0000_0000;
                        end
                endcase
            end
            6'b001000: // addi
                begin
                    result <= reg1Data + ($signed(Instruction[15:0]));
                    o_writeEnable <= writeEnable;
                    o_writeAddress <= writeAddress;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 32'h0000_0000;
                    memoryData <= 32'h0000_0000;
                end
            6'b001001: // addiu
                begin
                    result <= reg1Data + ($signed(Instruction[15:0]));
                    o_writeEnable <= writeEnable;
                    o_writeAddress <= writeAddress;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 32'h0000_0000;
                    memoryData <= 32'h0000_0000;
                    
                end
            6'b001100: // andi
                begin
                    result <= reg1Data & ($unsigned(Instruction[15:0]));
                    o_writeEnable <= writeEnable;
                    o_writeAddress <= writeAddress;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 32'h0000_0000;
                    memoryData <= 32'h0000_0000;
                end
            6'b001101: // ori
                begin
                    result <= reg1Data | ($unsigned(Instruction[15:0]));
                    o_writeEnable <= writeEnable;
                    o_writeAddress <= writeAddress;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 32'h0000_0000;
                    memoryData <= 32'h0000_0000;
                end
            6'b001110: // xori
                begin
                    result <= reg1Data ^ ($unsigned(Instruction[15:0]));
                    o_writeEnable <= writeEnable;
                    o_writeAddress <= writeAddress;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 32'h0000_0000;
                    memoryData <= 32'h0000_0000;
                end
            6'b000100: // beq
                begin
                    
                        result <= 0;
                        o_writeEnable <= 0;
                        o_writeAddress <= 0;
                        o_ALUopcode <= ALUopcode;
                        memoryAddress <= 32'h0000_0000;
                        memoryData <= 32'h0000_0000;
                    
                end
            6'b000101: // bne
                begin
                    
                        result <= 0;
                        o_writeEnable <= 0;
                        o_writeAddress <= 0;
                        o_ALUopcode <= ALUopcode;
                        memoryAddress <= 32'h0000_0000;
                        memoryData <= 32'h0000_0000;
                    
                end
            6'b100011: // lw
                begin
                    result <= 0;
                    o_writeEnable <= 1;
                    o_writeAddress <= writeAddress;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= reg1Data + $signed(Instruction[15:0]);
                    memoryData <= 0;
                end
            6'b101011: // sw
                begin
                    result <= 0;
                    o_writeEnable <= 0;
                    o_writeAddress <= 0;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= reg1Data + $signed(Instruction[15:0]);
                    memoryData <= reg2Data;
                end
            6'b000010: // j
                begin
                    result <= 0;
                    o_writeEnable <= 0;
                    o_writeAddress <= 0;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 0;
                    memoryData <= 0;
                end
            6'b000011: // jal
                begin
                    result <= reg1Data;
                    o_writeEnable <= 1;
                    o_writeAddress <= 5'b01111;
                    o_ALUopcode <= ALUopcode;
                    memoryAddress <= 0;
                    memoryData <= 0;
                end
            6'b111111:
                begin
                    result <= 32'hffff_ffff;
                    o_writeEnable <= 0;
                    o_writeAddress <= 5'b11111;
                    o_ALUopcode <= 6'b111111;
                    memoryAddress <= 32'hffff_ffff;
                    memoryData <= 32'hffff_ffff;
                end
        endcase
    end
endmodule
