#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../external/doctest.h"
#include "../src/decoder.hpp"

// TEST_CASE("immS extracts and sign-extends correctly") {
//     CHECK(immS(0x280) == 5);
//     CHECK(immS(0x00000000) == 0);
//     CHECK(immS(0x00000080) == 1);
//     CHECK(immS(0x7E000F80) == 2047);
//     CHECK(immS(0x80000000) == -2048);
//     CHECK(immS(0xFE000000) == -32);
//     CHECK(immS(0xFE000F80) == -1);
// }

// TEST_CASE("immB extracts and sign-extends correctly") {
//     CHECK(immB(0x00000000) == 0);
//     CHECK(immB(0x00000100) == 2);
//     CHECK(immB(0x00000F00) == 30);
//     CHECK(immB(0x7E000F80) == 4094);
//     CHECK(immB(0x80000000) == -4096);
//     CHECK(immB(0xFE000F80) == -2);
//     CHECK(immB(0xFE000080) == -32);
// }

// TEST_CASE("immU extracts and sign-extends correctly") {    
//     CHECK(immU(0x00000000) == 0);
//     CHECK(immU(0x00001000) == 4096);
//     CHECK(immU(0x000AB000) == 700416);
//     CHECK(immU(0x7FFFF000) == 2147479552);
//     CHECK(immU(0x80000000) == INT32_MIN);
//     CHECK(immU(0xFFFFF000) == -4096);
//     CHECK(immU(0xABCDE000) == -1412571136);
// }

// TEST_CASE("immJ extracts and sign-extends correctly") {
//     CHECK(immJ(0x00000000) == 0);
//     CHECK(immJ(0x00200000) == 2);
//     CHECK(immJ(0x00100000) == 2048);
//     CHECK(immJ(0x7FFFF000) == 1048574);
//     CHECK(immJ(0x80000000) == -1048576);
//     CHECK(immJ(0xFFFFF000) == -2);
//     CHECK(immJ(0xFFFFF800) == -2);
// }
