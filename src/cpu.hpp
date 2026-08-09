#pragma once
#include <vector>
#include "decoder.hpp"
#include "memory.hpp"

class CPU {
    private:
        /** @brief 32 general purpose RISC-V registers (x0-x31). */
        std::vector<uint32_t> regFile;

        /** @brief Program counter — address of the next instruction to fetch. */
        uint32_t pc;

        /** @brief External memory object (see memory class) */
        Memory& memory;

        /** @brief Set to true when ECALL/EBREAK executes; signals the run loop to stop. */
        bool halted;

    public:
        /** 
         * @brief Constructs cpu object with a register file of size 32, memory 
         *        object mem, program counter at entryPoint, and halted flag as false.
         */
        CPU(Memory& mem, uint32_t entryPoint);
        
        /**
         * @brief Executes a decoded instruction, updating registers, memory, and pc as needed.
         * @param instr The decoded instruction to execute.
         * @throws std::runtime_error if instr.op is Op::UNKNOWN.
         */
        void execute(DecodedInstruction instr);
        
        /** @brief Returns instructions in memory at address 'pc'. */
        uint32_t fetch();

        /** @brief Fetches, decodes, and executes pc instructions */
        void step();

        /** 
         * @brief Writes a 32-bit word to one of the 32 cpu registers in regFile. Any
         *        writes to reg x0 are discarded.
         * @param word The word being written
         * @param reg The register being written to.
         */
        void writeReg(uint32_t word, uint32_t reg);

        /** @brief Returns if the instructions loop should stop (ECALL/EBREAK called) */
        bool isHalted() const;

        /** @brief Returns pc value. */
        uint32_t getPC();

        /** @brief Returns regFile. */
        std::vector<uint32_t> getRegFile();
};