#pragma once
#include <cstdint>

enum class Op {
    ADD, SUB, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU,
    ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU,
    LB, LH, LW, LBU, LHU, SB, SH, SW, BEQ, BNE, BLT, BGE, BLTU,
    BGEU, JAL, JALR, LUI, AUIPC, ECALL, EBREAK, UNKNOWN
};

struct DecodedInstruction {
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    int32_t imm;
    Op op;
};

uint32_t opcode(uint32_t instr);
uint32_t funct3(uint32_t instr);
uint32_t funct7(uint32_t instr);
uint32_t rd(uint32_t instr);
uint32_t rs1(uint32_t instr);
uint32_t rs2(uint32_t instr);
int32_t immI(uint32_t instr);
int32_t immS(uint32_t instr);
int32_t immB(uint32_t instr);
int32_t immU(uint32_t instr);
int32_t immJ(uint32_t instr);


DecodedInstruction decode(uint32_t addr);