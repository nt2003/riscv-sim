#include "elf_loader.hpp"
#include <string>
#include <iostream>
#include <vector>
#include "memory.hpp"
#include "cpu.hpp"
#include <stdexcept>

int main() {
    std::string path = "/Users/nicolastrejo/riscv-sim/tests/mini.elf";
    uint32_t entryPoint;
    Memory memory;

    if (!loadElf(path, memory, entryPoint)) {
        std::cerr << "Failed to load" << '\n';
        return 1;
    }

    CPU cpu(memory, entryPoint);

    try {
        while (!cpu.isHalted()) {
            cpu.step();
        }
    } catch (std::runtime_error& e) {
        std::cout << "Unknown instruction " << e.what() << '\n';
    } catch (std::out_of_range& e) {
        std::cout << "Memory Bounds error " << e.what() << '\n';
    }

    std::cout << "Success!" << '\n';
    for (auto reg : cpu.getRegFile()) {
        std::cout << reg << '\n';
    } 
    return 0;
}