#pragma once

#include <ostream>
#include <string>
#include <type_traits>

#include "models/elfheader.h"

std::ostream& operator<<(std::ostream& out, Bitness bitness)
{
    switch (bitness)
    {
        case Bitness::Invalid:
            out << "Invalid";
            break;
        case Bitness::ThirtyTwo:
            out << "32bit";
            break;
        case Bitness::SixtyFour:
            out << "64bit";
            break;
        default:
            out << "Out of bounds: " << std::to_string(static_cast<std::underlying_type_t<Bitness>>(bitness));
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, ByteOrder byteOrder)
{
    switch (byteOrder)
    {
        case ByteOrder::Invalid:
            out << "Invalid";
            break;
        case ByteOrder::LittleEndian:
            out << "Little Endian";
            break;
        case ByteOrder::BigEndian:
            out << "Big Endian";
            break;
        default:
            out << "Out of bounds: " << std::to_string(static_cast<std::underlying_type_t<ByteOrder>>(byteOrder));
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, ObjectFileType fileType)
{
    switch (fileType)
    {
        case ObjectFileType::Invalid:
            out << "Invalid";
            break;
        case ObjectFileType::Relocatable:
            out << "Relocatable";
            break;
        case ObjectFileType::Executable:
            out << "Executable";
            break;
        case ObjectFileType::Dynamic:
            out << "Dynamic";
            break;
        case ObjectFileType::Core:
            out << "Core";
            break;
        default:
            out << "Out of bounds: " << std::to_string(static_cast<std::underlying_type_t<ObjectFileType>>(fileType));
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, Architecture arch)
{
    switch (arch)
    {
        case Architecture::Invalid:
            out << "Invalid";
            break;
        case Architecture::ATT32100:
            out << "AT&T 32100";
            break;
        case Architecture::SPARC:
            out << "SPARC";
            break;
        case Architecture::i386:
            out << "Intel 386";
            break;
        case Architecture::M68000:
            out << "Motorola 68000";
            break;
        case Architecture::M88000:
            out << "Motorola 88000";
            break;
        case Architecture::i8086:
            out << "Intel 8086";
            break;
        case Architecture::MIPS:
            out << "MIPS";
            break;
        case Architecture::PARISC:
            out << "HP PA-RISC";
            break;
        case Architecture::SPARCPlus:
            out << "SPARC+";
            break;
        case Architecture::PowerPC:
            out << "Power PC";
            break;
        case Architecture::PowerPC64:
            out << "Power PC 64";
            break;
        case Architecture::IBMS390:
            out << "IBM System/390";
            break;
        case Architecture::ARM:
            out << "ARM";
            break;
        case Architecture::SuperH:
            out << "SuperH";
            break;
        case Architecture::SPARCv9:
            out << "SPARC V9";
            break;
        case Architecture::Itanium:
            out << "Intel Itanium";
            break;
        case Architecture::x86_64:
            out << "x86-64";
            break;
        case Architecture::VAX:
            out << "VAX";
            break;
        default:
            out << "Out of bounds: " << std::to_string(static_cast<std::underlying_type_t<Architecture>>(arch));
    }

    return out;
}

std::ostream& operator<<(std::ostream& out, FileVersion version)
{
    switch(version)
    {
        case FileVersion::Invalid:
            out << "Invalid";
            break;
        case FileVersion::Current:
            out << "Current";
            break;
        default:
            out << "Out of bounds: " << std::to_string(static_cast<std::underlying_type_t<FileVersion>>(version));
    }

    return out;
}
