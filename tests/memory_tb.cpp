#include "../external/doctest.h"
#include "../src/memory.hpp"

// --- resize / size / clear ---

TEST_CASE("Resize and size") {
    Memory mem;
    CHECK(mem.size() == 0);

    mem.resize(8);
    CHECK(mem.size() == 8);

    mem.resize(0xFF);
    CHECK(mem.size() == 0xFF);
}

TEST_CASE("Clear resets size to 0") {
    Memory mem;
    mem.resize(16);
    mem.clear();
    CHECK(mem.size() == 0);
}

// --- byte read/write ---

TEST_CASE("write then read returns the same byte") {
    Memory mem;
    mem.resize(4);
    mem.write(2, 0xAB);
    CHECK(mem.read(2) == 0xAB);
}

TEST_CASE("read/write throw on out-of-bounds byte address") {
    Memory mem;
    mem.resize(4);
    CHECK_THROWS_AS(mem.read(10), std::out_of_range);
    CHECK_THROWS_AS(mem.write(10, 0xFF), std::out_of_range);
}

// --- halfword read/write ---

TEST_CASE("writeH then readH returns the same halfword") {
    Memory mem;
    mem.resize(4);
    mem.writeH(0, 0x1234);
    CHECK(mem.readH(0) == 0x1234);
}

TEST_CASE("readH/writeH throw when any byte spills out of bounds") {
    Memory mem;
    mem.resize(4);
    // base address in bounds, but addr+1 is not
    CHECK_THROWS_AS(mem.readH(3), std::out_of_range);
    CHECK_THROWS_AS(mem.writeH(3, 0x1234), std::out_of_range);
}

// --- word read/write ---

TEST_CASE("writeW then readW returns the same word") {
    Memory mem;
    mem.resize(4);
    mem.writeW(0, 0x12345678);
    CHECK(mem.readW(0) == 0x12345678);
}

TEST_CASE("readW/writeW throw when any byte spills out of bounds") {
    Memory mem;
    mem.resize(4);
    // base address in bounds, but addr+3 is not
    CHECK_THROWS_AS(mem.readW(2), std::out_of_range);
    CHECK_THROWS_AS(mem.writeW(2, 0x12345678), std::out_of_range);
}