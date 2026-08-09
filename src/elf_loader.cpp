#include "elf_loader.hpp"
#include "elf_types.hpp"
#include <fstream>
#include <vector>

bool loadElf(const std::string& path, Memory& memory, uint32_t& entryPoint) {
    // Read the whole file into a temporary in-memory buffer first, since we
    // need to jump around it (header, then program header table, then each
    // segment's data) rather than reading it straight through sequentially.
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    auto size = file.tellg();
    if (size < 0) {
        return false;
    }
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> fileBytes(size);
    file.read(reinterpret_cast<char*>(fileBytes.data()), size);

    // The ELF header sits at the very start of the file. Since fileBytes
    // already holds those bytes in the right layout, we can just
    // reinterpret the start of the buffer as an Elf32_Ehdr directly.
    if (static_cast<size_t>(size) < sizeof(Elf32_Ehdr)) {
        return false; // file too small to even hold a header
    }
    Elf32_Ehdr header;
    std::memcpy(&header, fileBytes.data(), sizeof(Elf32_Ehdr));

    entryPoint = header.e_entry;

    // Walk the program header table: e_phoff tells us where it starts in
    // the file, e_phnum tells us how many entries there are, and each
    // entry is e_phentsize bytes (normally sizeof(Elf32_Phdr), but we
    // trust the field rather than assume).
    for (int i = 0; i < header.e_phnum; i++) {
        size_t offset = header.e_phoff + i * header.e_phentsize;
        if (offset + sizeof(Elf32_Phdr) > fileBytes.size()) {
            return false; // corrupt/truncated file
        }

        Elf32_Phdr ph;
        std::memcpy(&ph, fileBytes.data() + offset, sizeof(Elf32_Phdr));

        if (ph.p_type != PT_LOAD) {
            continue; // not a segment we need to load
        }

        // Make sure memory is big enough to hold this segment at its
        // target address. p_memsz can exceed p_filesz (e.g. .bss) -- the
        // extra bytes should end up zeroed, which resize() already does
        // for newly-added elements.
        uint32_t neededSize = ph.p_vaddr + ph.p_memsz;
        if (memory.size() < neededSize) {
            memory.resize(neededSize);
        }

        // Copy just the file-backed portion; any bytes beyond p_filesz
        // (up to p_memsz) stay zeroed from the resize above.
        for (uint32_t b = 0; b < ph.p_filesz; b++) {
            memory.write(ph.p_vaddr + b, fileBytes.at(ph.p_offset + b));
        }
    }

    return true;
}