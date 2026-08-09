#include <stdexcept>
#include "cpu.hpp"
#include "memory.hpp"

CPU::CPU(Memory& mem, uint32_t entryPoint) : regFile(32), pc(entryPoint), memory(mem), halted(false) {  
}
uint32_t CPU::fetch() {
    return memory.readW(pc);
}

void CPU::writeReg(uint32_t word, uint32_t reg) {
    if (reg != 0x0) {
        regFile.at(reg) = word;
    }
}

void CPU::execute(DecodedInstruction instr) {
    uint32_t currPC = pc;
    pc += 4;

    switch(instr.op) {
        case Op::ADD:
            writeReg(regFile.at(instr.rs1) + regFile.at(instr.rs2), instr.rd);
            break;
        case Op::SUB:
            writeReg(regFile.at(instr.rs1) - regFile.at(instr.rs2), instr.rd);
            break;
        case Op::XOR:
            writeReg(regFile.at(instr.rs1) ^ regFile.at(instr.rs2), instr.rd);
            break;
        case Op::OR:
            writeReg(regFile.at(instr.rs1) | regFile.at(instr.rs2), instr.rd);
            break;
        case Op::AND:
            writeReg(regFile.at(instr.rs1) & regFile.at(instr.rs2), instr.rd);
            break;
        case Op::SLL:
            writeReg(regFile.at(instr.rs1) << (regFile.at(instr.rs2)&0x1F), instr.rd);
            break;
        case Op::SRL:
            writeReg(regFile.at(instr.rs1) >> (regFile.at(instr.rs2)&0x1F), instr.rd);
            break;
        case Op::SRA:
            writeReg(
                static_cast<uint32_t>(
                    static_cast<int32_t>(regFile.at(instr.rs1)) >> (regFile.at(instr.rs2)&0x1F)),
                    instr.rd);
            break;
        case Op::SLT:
            writeReg(
                (static_cast<int32_t>(regFile.at(instr.rs1)) 
                    < static_cast<int32_t>(regFile.at(instr.rs2)))?1:0, instr.rd);
            break;
        case Op::SLTU:
            writeReg((regFile.at(instr.rs1) < regFile.at(instr.rs2))?1:0, instr.rd);
            break;

        case Op::ADDI:
            writeReg(regFile.at(instr.rs1) + instr.imm, instr.rd);
            break;
        case Op::XORI:
            writeReg(regFile.at(instr.rs1) ^ instr.imm, instr.rd);
            break;
        case Op::ORI:
            writeReg(regFile.at(instr.rs1) | instr.imm, instr.rd);
            break;
        case Op::ANDI:
            writeReg(regFile.at(instr.rs1) & instr.imm, instr.rd);
            break;
        case Op::SLLI:
            writeReg(regFile.at(instr.rs1) << (instr.imm&0x1F), instr.rd);
            break;
        case Op::SRLI:
            writeReg(regFile.at(instr.rs1) >> (instr.imm&0x1F), instr.rd);
            break;
        case Op::SRAI:
            writeReg(
                static_cast<uint32_t>(
                    static_cast<int32_t>(regFile.at(instr.rs1)) >> (instr.imm&0x1F)), 
                    instr.rd);
            break;
        case Op::SLTI:
            writeReg((static_cast<int32_t>
                        (regFile.at(instr.rs1))<instr.imm)?1:0, instr.rd);
            break;
        case Op::SLTIU:
            writeReg((regFile.at(instr.rs1)<instr.imm)?1:0, instr.rd);
            break;

        case Op::LB:
            writeReg(
                static_cast<uint32_t>(
                    static_cast<int32_t>(
                        static_cast<int8_t>(
                            memory.read(regFile.at(instr.rs1)+instr.imm)))), instr.rd);
            break;
        case Op::LH:
            writeReg(
                static_cast<uint32_t>(
                    static_cast<int32_t>(
                        static_cast<int16_t>(
                            memory.readH(regFile.at(instr.rs1)+instr.imm)))), instr.rd);
            break;
        case Op::LW:
            writeReg(memory.readW(regFile.at(instr.rs1)+instr.imm), instr.rd);
            break;
        case Op::LBU:
            writeReg(
                static_cast<uint32_t>(
                    memory.read(regFile.at(instr.rs1)+instr.imm)), instr.rd);
            break;
        case Op::LHU:
            writeReg(
                static_cast<uint32_t>(
                    static_cast<uint16_t>(
                        memory.readH(regFile.at(instr.rs1)+instr.imm))), instr.rd);
            break;

        case Op::SB:
            memory.write(
                regFile.at(instr.rs1)+instr.imm, static_cast<uint8_t>(
                                                    regFile.at(instr.rs2)));
            break;
        case Op::SH:
            memory.writeH(
                regFile.at(instr.rs1)+instr.imm, static_cast<uint16_t>(
                                                    regFile.at(instr.rs2)));
            break;
        case Op::SW:
            memory.writeW(
                regFile.at(instr.rs1)+instr.imm, regFile.at(instr.rs2));
            break;
            
        case Op::BEQ:
            if (regFile.at(instr.rs1) == regFile.at(instr.rs2)) {
                pc = currPC + instr.imm;
            }
            break;
        case Op::BNE:
            if (regFile.at(instr.rs1) != regFile.at(instr.rs2)) {
                pc = currPC + instr.imm;
            }
            break;
        case Op::BLT:
            if (static_cast<int32_t>(regFile.at(instr.rs1)) < 
                static_cast<int32_t>(regFile.at(instr.rs2))) {
                    pc = currPC + instr.imm;
            }
            break;
        case Op::BGE:
            if (static_cast<int32_t>(regFile.at(instr.rs1)) >= 
                static_cast<int32_t>(regFile.at(instr.rs2))) {
                    pc = currPC + instr.imm;
            }
            break;
        case Op::BLTU:
            if (regFile.at(instr.rs1) < regFile.at(instr.rs2)) {
                pc = currPC + instr.imm;
            }
            break;
        case Op::BGEU:
            if (regFile.at(instr.rs1) >= regFile.at(instr.rs2)) {
                pc = currPC + instr.imm;
            }
            break;

        case Op::JAL:
            writeReg(pc, instr.rd);
            pc = currPC + instr.imm;
            break;
        case Op::JALR:
            writeReg(pc, instr.rd);
            pc = (regFile.at(instr.rs1) + instr.imm)&0xFFFFFFFE;
            break;

        case Op::LUI:
            writeReg(instr.imm, instr.rd);
            break;
        case Op::AUIPC:
            writeReg(currPC + (instr.imm), instr.rd);
            break;
        
        case Op::ECALL:
            halted = true;
            break;
        case Op::EBREAK:
            halted = true;
            break;

        default:
            throw std::runtime_error("Unknown instruction");
    }
}

void CPU::step() {
    uint32_t addr = fetch();
    DecodedInstruction instr = decode(addr);
    execute(instr);
}

bool CPU::isHalted() const {
    return halted;
}

uint32_t CPU::getPC() {
    return pc;
}

std::vector<uint32_t> CPU::getRegFile() {
    return regFile;
}
