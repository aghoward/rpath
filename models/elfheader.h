#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

enum class Bitness : uint8_t
{
    Invalid = 0,
    ThirtyTwo = 1,
    SixtyFour = 2
};

enum class ByteOrder : uint8_t
{
    Invalid = 0,
    BigEndian = 1,
    LittleEndian = 2,
};

enum class ObjectFileType : uint16_t
{
    Invalid = 0,
    Relocatable = 1,
    Executable = 2,
    Dynamic = 3,
    Core = 4
};

enum class Architecture : uint16_t
{
    Invalid = 0,
    ATT32100 = 1,
    SPARC = 2,
    i386 = 3,
    M68000 = 4,
    M88000 = 5,
    i8086 = 7,
    MIPS = 8,
    PARISC = 15,
    SPARCPlus = 18,
    PowerPC = 20,
    PowerPC64 = 21,
    IBMS390 = 22,
    ARM = 40,
    SuperH = 42,
    SPARCv9 = 43,
    Itanium = 50,
    x86_64 = 62,
    VAX = 75
};

enum class FileVersion : uint32_t
{
    Invalid = 0,
    Current = 1
};

class ElfHeader {
    public:
        const std::size_t IDENT_CHARS = 0x10;

        std::string ident;
        Bitness bitness;
        ByteOrder byteOrder;
        ObjectFileType type;
        Architecture machine;
        FileVersion version;
        uint64_t entry;
        uint64_t program_offset;
        uint64_t section_offset;
        uint32_t flags;
        uint16_t program_size;
        uint16_t program_entry_size;
        uint16_t program_entry_count;
        uint16_t section_entry_size;
        uint16_t section_entry_count;
        uint16_t section_name_index;

        ElfHeader() = default;
        ~ElfHeader() = default;

        ElfHeader(const ElfHeader&) = default;
        ElfHeader(ElfHeader&&) = default;

        ElfHeader& operator=(const ElfHeader&) = default;
        ElfHeader& operator=(ElfHeader&&) = default;
};
