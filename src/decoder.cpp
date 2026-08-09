#include "decoder.hpp"

uint32_t opcode(uint32_t instr) {
    return instr & 0x7F;
}

uint32_t funct3(uint32_t instr) {
    return (instr & 0x7000) >> 12;
}

uint32_t funct7(uint32_t instr) {
    return (instr & 0xFE000000) >> 25;
}

uint32_t rd(uint32_t instr) {
    return (instr & 0xF80) >> 7;
}

uint32_t rs1(uint32_t instr) {
    return (instr & 0xF8000) >> 15;
}

uint32_t rs2(uint32_t instr) {
    return (instr & 0x1F00000) >> 20;
}

int32_t immI(uint32_t instr) {
    return static_cast<int32_t> (instr) >> 20;
}

int32_t immS(uint32_t instr) {
    uint32_t lo = (instr & 0xF80) << 13;
    uint32_t hi = instr & 0xFE000000;
    return static_cast<int32_t>(hi + lo) >> 20;
}

int32_t immB(uint32_t instr) {
    uint32_t lo = (instr & 0xF00) << 12;
    uint32_t hi = (instr & 0x7E000000) >> 1;
    uint32_t b12 = instr & 0x80000000;
    uint32_t b11 = (instr & 0x80) << 23;
    return static_cast<int32_t>(b12+b11+hi+lo) >> 19;
}

int32_t immU(uint32_t instr) {
    return static_cast<int32_t> (instr & 0xFFFFF000);
}

int32_t immJ(uint32_t instr) {
    uint32_t b20 = instr & 0x80000000;
    uint32_t b11 = (instr & 0x100000) << 2;
    uint32_t lo = (instr & 0x7FE00000) >> 9;
    uint32_t hi = (instr & 0xFF000) << 11;
    return static_cast<int32_t>(b20+b11+lo+hi) >> 11;
}

DecodedInstruction decode(uint32_t instr) {
    DecodedInstruction d1 = {
                                opcode(instr),
                                funct3(instr),
                                funct7(instr),
                                rd(instr),
                                rs1(instr),
                                rs2(instr)
    };

    uint32_t opc = d1.opcode;

    //R-Type operation extraction
    if (opc == 0x33) {

        switch (d1.funct3) {
            case 0x0:
                if (d1.funct7 == 0x0) {
                    d1.op = Op::ADD;
                } else {d1.op = Op::SUB;}
                break;
            case 0x4:
                d1.op = Op::XOR;
                break;
            case 0x6:
                d1.op = Op::OR;
                break;
            case 0x7:
                d1.op = Op::AND;
                break;
            case 0x1:
                d1.op = Op::SLL;
                break;
            case 0x5:
                if (d1.funct7 == 0x0) {
                    d1.op = Op::SRL;
                } else {d1.op = Op::SRA;}
                break;
            case 0x2:
                d1.op = Op::SLT;
                break;
            case 0x3:
                d1.op = Op::SLTU;
                break;
            default:
                d1.op = Op::UNKNOWN;
                break;
        }
    }

    //I-Type operation and immediate extraction
    else if (opc == 0x13 || opc == 0x3 || 
        opc == 0x67 || opc == 0x73) {
            
            d1.imm = immI(instr);

            if (opc == 0x13) {
                switch (d1.funct3) {
                    case 0x0:
                        d1.op = Op::ADDI;
                        break;
                    case 0x4:
                        d1.op = Op::XORI;
                        break;
                    case 0x6:
                        d1.op = Op::ORI;
                        break;
                    case 0x7:
                        d1.op = Op::ANDI;
                        break;
                    case 0x1:
                        d1.op = Op::SLLI;
                        break;
                    case 0x5:
                        if (((d1.imm&0xFE0)>>5) == 0x00) {
                            d1.op = Op::SRLI;
                        } else {d1.op = Op::SRAI;}
                        break;
                    case 0x2:
                        d1.op = Op::SLTI;
                        break;
                    case 0x3:
                        d1.op = Op::SLTIU;
                        break;
                    default:
                        d1.op = Op::UNKNOWN;
                        break;
                }
            }
            else if (opc == 0x3) {
                switch (d1.funct3) {
                    case 0x0:
                        d1.op = Op::LB;
                        break;
                    case 0x1:
                        d1.op = Op::LH;
                        break;
                    case 0x2: 
                        d1.op = Op::LW;
                        break;
                    case 0x4:
                        d1.op = Op::LBU;
                        break;
                    case 0x5:
                        d1.op = Op::LHU;
                        break;
                    default:
                        d1.op = Op::UNKNOWN;
                        break;
                }
            }
            else if (opc == 0x67) {
                d1.op = Op::JALR;
            }
            else if (opc == 0x73) {
                if (d1.imm == 0x0) {
                    d1.op = Op::ECALL;
                } else {d1.op = Op::EBREAK;}
            }
    }

    //S-Type operation and immediate extraction
    else if (opc == 0x23) {

        d1.imm = immS(instr);

        switch(d1.funct3) {
            case 0x0:
                d1.op = Op::SB;
                break;
            case 0x1:
                d1.op = Op::SH;
                break;
            case 0x2:
                d1.op = Op::SW;
                break;
            default:
                d1.op = Op::UNKNOWN;
                break;
        }
    }

    //B-Type operation and immediate extraction
    else if (opc == 0x63) {

        d1.imm = immB(instr);

        switch(d1.funct3) {
            case 0x0:
                d1.op = Op::BEQ;
                break;
            case 0x1:
                d1.op = Op::BNE;
                break;
            case 0x4:
                d1.op = Op::BLT;
                break;
            case 0x5:
                d1.op = Op::BGE;
                break;
            case 0x6:
                d1.op = Op::BLTU;
                break;
            case 0x7:
                d1.op = Op::BGEU;
                break;
            default:
                d1.op = Op::UNKNOWN;
                break;
        }
    }

    //U-Type operation and immediate extraction
    else if (opc == 0x37 || opc == 0x17) {

        d1.imm = immU(instr);

        if (opc == 0x37) {
            d1.op = Op::LUI;
        } else {d1.op = Op::AUIPC;}
    }

    //J-Type operation and immediate extraction
    else if (opc == 0x6F) {

        d1.imm = immJ(instr);
        d1.op = Op::JAL;
    }

    else {
        d1.op = Op::UNKNOWN;

    }



    return d1;
}
