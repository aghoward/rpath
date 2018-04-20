#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

#include "parsing/elfheaderfactory.h"
#include "files/filereader.h"
#include "models/elfheader.h"
#include "failurereasons.h"
#include "support/result.h"

#include <iostream>

Result<ElfHeader, ParseFailure> ElfHeaderFactory::create(std::shared_ptr<FileReader> fileReader) {
    fileReader->seek(0);
    if (!fileReader->isOk())
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return ParseFailure::FileReadError;
    }

    auto elf = ElfHeader();

    elf.ident = fileReader->readString(elf.IDENT_CHARS);
    if (!magicBytesOk(elf.ident)) 
        return ParseFailure::InvalidFileFormat;

    setBitness(elf);
    setEndianess(elf);

    elf.type = fileReader->readBytes<ObjectFileType, 2>(elf.byteOrder);
    elf.machine = fileReader->readBytes<Architecture, 2>(elf.byteOrder);
    elf.version = fileReader->readBytes<FileVersion, 4>(elf.byteOrder);
    elf.entry = getWord(fileReader, elf.bitness, elf.byteOrder);
    elf.program_offset = getWord(fileReader, elf.bitness, elf.byteOrder);
    elf.section_offset = getWord(fileReader, elf.bitness, elf.byteOrder);
    elf.flags = fileReader->readBytes<uint32_t, 4>(elf.byteOrder);
    elf.program_size = fileReader->readBytes<uint16_t, 2>(elf.byteOrder);
    elf.program_entry_size = fileReader->readBytes<uint16_t, 2>(elf.byteOrder);
    elf.program_entry_count = fileReader->readBytes<uint16_t, 2>(elf.byteOrder);
    elf.section_entry_size = fileReader->readBytes<uint16_t, 2>(elf.byteOrder);
    elf.section_entry_count = fileReader->readBytes<uint16_t, 2>(elf.byteOrder);
    elf.section_name_index = fileReader->readBytes<uint16_t, 2>(elf.byteOrder);

    return elf;
}

bool ElfHeaderFactory::magicBytesOk(const std::string& identity) {
    return identity.find(std::string("\x7F") + std::string("ELF")) == 0;
}

void ElfHeaderFactory::setBitness(ElfHeader& elf) {
    auto num = static_cast<Bitness>(elf.ident[4]);
    elf.bitness = num;
}

void ElfHeaderFactory::setEndianess(ElfHeader& elf) {
    auto num = static_cast<ByteOrder>(elf.ident[5]);
    elf.byteOrder = num;
}

uint64_t ElfHeaderFactory::getWord(std::shared_ptr<FileReader>& fileReader, Bitness bitness, ByteOrder byteorder) {
    if (bitness == Bitness::ThirtyTwo)
        return fileReader->readBytes<uint64_t, 4>(byteorder);
    return fileReader->readBytes<uint64_t, 8>(byteorder);
}

