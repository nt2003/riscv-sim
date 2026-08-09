#include "memory.hpp"
#include <vector>

Memory::Memory(){}

uint8_t Memory::read(uint32_t addr) {
    return buffer.at(addr);
}

uint16_t Memory::readH(uint32_t addr) {
    uint16_t half = {};
    for (int i=0; i<2; i++) {
        half += static_cast<uint32_t>(read(addr + i)) << i*8;
    }
    return half;
}

uint32_t Memory::readW(uint32_t addr) {
    uint32_t word = {};
    for (int i=0; i<4; i++) {
        word += static_cast<uint32_t>(read(addr + i)) << i*8;
    }
    return word;
}

void Memory::write(uint32_t addr, uint8_t byte) {
    buffer.at(addr) = byte;
}

void Memory::writeH(uint32_t addr, uint16_t half) {
    buffer.at(addr+1);
    for (int i=0; i<2; i++) {
        buffer.at(addr+i) = half >> i*8;
    }
}

void Memory::writeW(uint32_t addr, uint32_t word) {
    buffer.at(addr+3);
    for (int i=0; i<4; i++) {
        buffer.at(addr+i) = word >> i*8;
    }
}

void Memory::resize(size_t size) {
    buffer.resize(size);
}

uint8_t* Memory::data() {
    return buffer.data();
}

void Memory::clear() {
    buffer.clear();
}

size_t Memory::size() {
    return buffer.size();
}
