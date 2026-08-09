#pragma once
#include <vector>


class Memory {       
    private:
        /** @brief Raw byte storage representing the simulated address space. */             
        std::vector<uint8_t> buffer;
    
    public:
        /** @brief Constructs an empty Memory with no allocated bytes. */
        Memory();

        /**
         * @brief Reads an 8-bit byte from memory at the given address.
         * @param addr The byte address to read from.
         * @return The 8-bit byte retrieved from [addr].
         * @throws std::out_of_range if the byte falls outside the buffer.
         */
        uint8_t read(uint32_t addr);

        /**
         * @brief Reads a 16-bit half-word from memory at the given address, 
         *        combining 2 bytes in little-endian order.
         * @param addr The starting byte address to read from.
         * @return The 16-bit half-word formed from bytes [addr, addr+1].
         * @throws std::out_of_range if any of the 2 bytes fall outside the buffer.
         */
        uint16_t readH(uint32_t addr);

        /**
         * @brief Reads a 32-bit word from memory at the given address, combining
         *        4 bytes in little-endian order.
         * @param addr The starting byte address to read from.
         * @return The 32-bit word formed from bytes [addr, addr+3].
         * @throws std::out_of_range if any of the 4 bytes fall outside the buffer.
         */
        uint32_t readW(uint32_t addr);


        /**
         * @brief Writes an 8-bit byte to memory at the given address.
         * @param addr The byte address to write to.
         * @param byte The byte being written.
         * @throws std::out_of_range if the address falls outside the buffer.
         */
        void write(uint32_t addr, uint8_t byte);

        /**
         * @brief Writes a 16-bit half-word to memory starting at the given address
         *        in little-endian order.
         * @param addr The first byte address to write to.
         * @param half The half-word being written.
         * @throws std::out_of_range if any of the addresses [addr,addr+1] fall 
         *         outside the buffer.
         */
        void writeH(uint32_t addr, uint16_t half);

        /**
         * @brief Writes an 32-bit word to memory starting at the given address
         *        in little-endian order.
         * @param addr The first byte address to write to.
         * @param word The word being written.
         * @throws std::out_of_range if any of the addresses [addr, addr+3] fall 
         *         outside the buffer.
         */
        void writeW(uint32_t addr, uint32_t word);

        /** 
         * @brief Modifies 'buffer' size to 'size' 
         * @param size new size of 'buffer', in bytes 
         */
        void resize(size_t size);

        /** 
         * @brief Returns a direct pointer to the underlying buffer, bypassing bounds 
         *        checks. Intended for bulk operations like loadBin; prefer read/write 
         *        for normal access.
         */
        uint8_t* data();

        /** @brief Removes all elements in 'buffer' and modifies size to 0. */
        void clear();

        /** @brief Returns size of 'buffer'. */
        size_t size();
};