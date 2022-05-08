module alu (instruction, regA, regB, result, flags);

    // the address of regA is 00000, the address of regB is 00001
    input[31:0] instruction,regA,regB;// rs, rt
    output[31:0] result;
    reg[31:0] reg_result;
    // the first bit is zero flag, the second bit is negative flag,
    // the third bit is overflow flag.
    output[2:0] flags;
    reg[2:0] reg_flags;

    integer i; // used for iteration
    integer carry; // used in add
    reg[31:0] immediate; 
    reg[31:0] rs;
    reg[31:0] rt;
    reg[31:0] temp1;
    
    always @(instruction, regA, regB) begin
    reg_flags = 0;
    reg_result = 0;
    
        case (instruction[31:26])
            6'b0: begin // R type
                case (instruction[5:0])
                    6'b100000: // add
                        begin
                            reg_result = regA + regB;
                            if (regA[31]==0&&regB[31]==0&&reg_result[31]==1 || regA[31]==1&&regB[31]==1&&reg_result[31]==0) begin
                                reg_flags[2] = 1;
                            end
                        end
                    6'b100001: // addu
                        begin
                            reg_result = regA + regB;
                        end
                    6'b100010: // sub
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end    
                            temp1 = ~rt;
                            temp1 = temp1 + 1;
                            reg_result = rs + temp1;
                            if (rs[31]==0&&temp1[31]==0&&reg_result[31]==1 || rs[31]==1&&temp1[31]==1&&reg_result[31]==0) begin
                                reg_flags[2] = 1;
                            end
                        end
                    6'b100011: // subu
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end
                            reg_result = rs - rt;
                        end
                    6'b100100: // and
                        begin
                            reg_result = regA & regB;
                        end
                    6'b100111: // nor
                        begin
                            reg_result = ~(regA | regB);
                        end
                    6'b100101: // or
                        begin
                            reg_result = regA | regB;
                        end
                    6'b100110: // xor
                        begin
                            reg_result = regA ^ regB;
                        end
                    6'b101010: // slt
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end
                            reg_result = rs - rt;
                            if (reg_result[31] == 1) begin
                                reg_flags[1] = 1;
                            end
                        end
                    6'b101011: // sltu
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end   
                            if (regA < regB) begin
                                reg_flags[1] = 1;
                            end
                        end
                    6'b000000: // sll
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                            end else begin
                                rt = regB;
                            end
                            immediate = $signed(instruction[10:6]);
                            reg_result = rt << immediate;
                        end
                    6'b000100: // sllv
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end
                            i = $signed(rs[4:0]);
                            reg_result = rt << i;
                        end
                    6'b000010: // srl
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                            end else begin
                                rt = regB;
                            end
                            immediate = $signed(instruction[10:6]);
                            reg_result = rt >> immediate;
                        end
                    6'b000110: // srlv
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end
                            i = $signed(rs[4:0]);
                            reg_result = rt >> i;
                        end
                    6'b000011: // sra
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                            end else begin
                                rt = regB;
                            end
                            immediate = $signed(instruction[10:6]);
                            reg_result = rt >>> immediate;
                        end
                    6'b000111: // srav
                        begin
                            if (instruction[20:16] == 5'b00000) begin
                                rt = regA;
                                rs = regB;
                            end else begin
                                rt = regB;
                                rs = regA;
                            end
                            i = $signed(rs[4:0]);
                            reg_result = rt >>> i;
                        end
                endcase
            end
            6'b001000: // addi
                begin
                    immediate = $signed(instruction[15:0]);
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    reg_result = rs + immediate;
                    if (rs[31]==0&&immediate[31]==0&&reg_result[31]==1||rs[31]==1&&immediate[31]==1&&reg_result[31]==0) begin
                        reg_flags[2] = 1;
                    end
                end
            6'b001001: // addiu
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate[15:0] = instruction[15:0];
                    reg_result = rs + immediate;
                    
                end
            6'b001100: // andi
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate = $signed(instruction[15:0]);
                    reg_result = rs & immediate;
                end
            6'b001101: // ori
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate = $signed(instruction[15:0]);
                    reg_result = rs | immediate;
                end
            6'b001110: // xori
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate = $signed(instruction[15:0]);
                    reg_result = rs ^ immediate;
                end
            6'b000100: // beq
                begin
                    if (regA == regB) begin
                        reg_flags[0] = 1;
                    end
                end
            6'b000101: // bne
                begin
                    if (regA != regB) begin
                        reg_flags[0] = 0;
                    end
                end
            6'b001010: // slti
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate = $signed(instruction[15:0]);
                    reg_result = rs - immediate;
                    if (reg_result[31] == 1) begin
                        reg_flags[1] = 1;
                    end
                end
            6'b001011: // sltiu
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate[15:0] = instruction[15:0];
                    if (rs < immediate) begin
                        reg_flags[1] = 1;
                    end
                end
            6'b100011: // lw
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate = $signed(instruction[15:0]);
                    reg_result = rs + immediate;
                end
            6'b101011: // sw
                begin
                    if (instruction[25:21] == 5'b00000) begin
                        rs = regA;
                    end else begin
                        rs = regB;
                    end
                    immediate = $signed(instruction[15:0]);
                    reg_result = rs + immediate;
                end
        endcase
    end
    
    assign result = reg_result;
    assign flags = reg_flags;
endmodule
