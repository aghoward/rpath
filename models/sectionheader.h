#pragma once

#include <cstdint>
#include <string>

enum class SectionType : uint32_t {
    None = 0,
    ProgramBits = 1,
    SymbolTable = 2,
    StringTable = 3,
    RelocatableAddends = 4,
    HashTable = 5,
    Dynamic = 6,
    Note = 7,
    NoBits = 8,
    Relocatable = 9,
    Unspecified = 10,
    DynamicSymbol = 11,
    ProcessorLow = 0x70000000,
    ProcessorHigh = 0x7FFFFFFF,
    // These may actually be negative, check it out
    UserLow = 0x80000000,
    UserHigh = 0x8FFFFFFF
};

enum class SectionFlags : uint64_t {
    Writeable = 1,
    Allocatable = 2,
    Executable = 4,
    ProcessorMask = 0xF0000000
};

class SectionHeader {
    public:
        std::string name;
        uint32_t name_offset;
        SectionType type;
        SectionFlags flags;
        uint64_t address;
        uint64_t file_offset;
        uint64_t size;
        uint32_t link;
        uint32_t info;
        uint64_t address_alignment;
        uint64_t entity_size;
        std::string contents;

        SectionHeader() = default;
        ~SectionHeader() = default;

        SectionHeader(const SectionHeader&) = default;
        SectionHeader(SectionHeader&&) = default;

        SectionHeader& operator=(const SectionHeader&) = default;
        SectionHeader& operator=(SectionHeader&&) = default;
};

