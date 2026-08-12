# riscv-sim

A cycle-by-cycle functional simulator for the RV32I RISC-V instruction set, written in C++. This version executes real, compiled RISC-V binaries instruction by instruction but does not yet model pipeline timing (that's the next phase of the project).

## Features

- **Real ELF32 loader** — parses actual ELF headers and program tables (no external `libelf` dependency; ELF struct definitions are hand-included) to load compiled RISC-V binaries at their correct linked addresses and resolve the true entry point.
- **Byte-addressable memory model** — bounds-checked reads and writes at byte, halfword, and word granularity, with little-endian encoding matching the RISC-V spec.
- **Full RV32I decoder** — extracts every instruction field (opcode, funct3, funct7, rd, rs1, rs2) and correctly assembles and sign-extends immediates for all six instruction formats (R/I/S/B/U/J).
- **Complete instruction execution** — implements the full RV32I base integer instruction set: arithmetic and logical operations, immediate variants, loads and stores (byte/half/word, signed and unsigned), conditional branches, jumps (`JAL`/`JALR`), upper-immediate instructions (`LUI`/`AUIPC`), and program termination via `ECALL`/`EBREAK`.
- **Unit-tested** — a `doctest`-based test suite covers memory operations, instruction decoding (including hand-derived edge cases for every immediate format), and CPU execution semantics.

## Project structure

```
riscv-sim/
├── CMakeLists.txt
├── src/
│   ├── elf_loader.hpp / .cpp   # ELF32 parsing and program loading
│   ├── elf_types.hpp           # Hand-defined ELF32 structs (Elf32_Ehdr, Elf32_Phdr)
│   ├── memory.hpp / .cpp       # Byte-addressable memory with bounds checking
│   ├── decoder.hpp / .cpp      # Instruction field extraction and identification
│   ├── cpu.hpp / .cpp          # Register file, PC, fetch/decode/execute
│   └── main.cpp                # Entry point: loads a binary and runs it to completion
├── tests/
│   ├── test_memory.cpp
│   ├── test_decoder.cpp
│   └── test_cpu.cpp
└── external/
    └── doctest.h                # Header-only test framework
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

This produces two executables in `build/`:
- `riscv-sim` — the simulator itself
- `tests` — the unit test suite

## Running

```bash
./riscv-sim
```

Loads and executes the RISC-V ELF binary specified in `main.cpp`, running until an `ECALL`/`EBREAK` instruction halts the CPU or an error occurs (unrecognized instruction or out-of-bounds memory access, both reported with the failing program counter).

## Testing

```bash
./tests
```

Runs the full `doctest` suite, covering:
- Memory read/write correctness and bounds-checking (including "spills past the end" edge cases)
- Decoder immediate extraction for all five immediate formats, verified against hand-computed values
- CPU instruction execution and register file semantics (including the architectural rule that `x0` is always zero)

## Toolchain

Built and tested against the `riscv-gnu-toolchain` (RV32I, `ilp32` ABI) for compiling and assembling test programs, and cross-checked against `spike` (the official RISC-V ISA simulator) as a reference implementation.

## Status

This is the functional (untimed) core of the simulator: every instruction executes correctly, but as a single atomic step rather than across pipeline stages. A hardware-structural pipelined version (ALU, register file, control unit, and datapath muxes as distinct modules feeding a 5-stage IF/ID/EX/MEM/WB pipeline) is in active development.
