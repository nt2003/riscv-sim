#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../external/doctest.h"
#include "../src/cpu.hpp"

struct CPUFixture {
    Memory mem;
    CPU cpu;
    
    CPUFixture() : mem(), cpu(mem) {
        mem.resize(0x10);
    }
};

/**
 * RISC-V instr guide:
 * 
 * R-Type:
 * |funct 7| rs2 | rs1 |fn3|  rd | opcode|
 * |xxxxxxx|xxxxx|xxxxx|xxx|xxxxx|xxxxxxx|
 *  0000000 00010 00011 110 00011 0110011
 */

TEST_CASE_FIXTURE(CPUFixture, "ADD adds two registers") {
    uint32_t instr = 0x2181B3; //r3 = r3+r2
    //Positive plus positive
    cpu.writeReg(0x2,0x2);
    cpu.writeReg(0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == 3);
    
    //Positive plus negative
    cpu.writeReg(-0x2,0x2);
    cpu.writeReg(0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == -1);

    //Negative plus negative
    cpu.writeReg(-0x2,0x2);
    cpu.writeReg(-0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == -3);

}
TEST_CASE_FIXTURE(CPUFixture, "SUB subtracts two registers") {
    //Positive minus positive
    uint32_t instr = 0x402181B3;    //r3 = r3-r2
    cpu.writeReg(0x2,0x2);           
    cpu.writeReg(0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == -1);

    //Positive minus negative
    cpu.writeReg(-0x2,0x2);           
    cpu.writeReg(0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == 3);

    //Negative minus positive
    cpu.writeReg(0x2,0x2);           
    cpu.writeReg(-0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == -3);

    //Negative minus negative
    cpu.writeReg(-0x2,0x2);           
    cpu.writeReg(-0x1,0x3);
    cpu.execute(decode(instr));
    CHECK(cpu.getRegFile().at(0x3) == 1);
}
TEST_CASE_FIXTURE(CPUFixture, "XOR xors two registers") {
    DecodedInstruction instr = decode(0x21C1B3);  //r3 = r3^r2
    //Random
    cpu.writeReg(0x6A72AFA9, 0x3);
    cpu.writeReg(0x94351B47, 0x2);
    cpu.execute(instr);
    CHECK(cpu.getRegFile().at(0x3) == 0xFE47B4EE);
    
    //Completely Equal
    cpu.writeReg(0x1, 0x3);
    cpu.writeReg(0x1, 0x2);
    cpu.execute(instr);
    CHECK(cpu.getRegFile().at(0x3) == 0x0);

    //Completely Unequal
    cpu.writeReg(0x0, 0x3);
    cpu.writeReg(0xFFFFFFFF, 0x2);
    cpu.execute(instr);
    CHECK(cpu.getRegFile().at(0x3) == 0xFFFFFFFF);
}
TEST_CASE_FIXTURE(CPUFixture, "OR ors two registers") {
    DecodedInstruction instr = decode(0x21E1B3); //r3 = r3 | r2

    //Random
    cpu.writeReg(0xCA8D4550, 0x3);
    cpu.writeReg(0x946A8A8D, 0x2);
    cpu.execute(instr);
    CHECK(cpu.getRegFile().at(0x3) == 0xDEEFCFDD);
    
}
TEST_CASE_FIXTURE(CPUFixture, "AND ands two registers") {
    DecodedInstruction instr = decode(0x21E1B3); //r3 = r3 | r2

    //Random
    cpu.writeReg(0xCA8D4550, 0x3);
    cpu.writeReg(0x946A8A8D, 0x2);
    cpu.execute(instr);
    CHECK(cpu.getRegFile().at(0x3) == 0xDEEFCFDD);
    
}










