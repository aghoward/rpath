#pragma once

#include <cstdint>

enum class ProgramType : uint32_t {
    None = 0x0,
    Loadable = 0x1,
    Dynamic = 0x2,
    Interpreter = 0x3,
    Note = 0x4,
    Library = 0x5,
    ProgramHeader = 0x6,
    ProcessorLow = 0x70000000,
    ProcessorHigh = 0x7fffffff,
    StackFlags = 0x6474e551
};

enum class ProgramFlags : uint32_t {
    Execute = 0x1,
    Write = 0x2,
    Read = 0x4
};

class ProgramHeader {
    public:
        ProgramType type;
        uint32_t offset;
        uint32_t virtual_address;
        uint32_t physical_address;
        uint32_t file_size;
        uint32_t memory_size;
        ProgramFlags flags;
        uint32_t alignment;

        ProgramHeader() = default;
        ~ProgramHeader() = default;

        ProgramHeader(const ProgramHeader&) = default;
        ProgramHeader(ProgramHeader&&) = default;

        ProgramHeader& operator=(const ProgramHeader&) = default;
        ProgramHeader& operator=(ProgramHeader&&) = default;
};
