#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

#include "parsing/elfheaderparser.h"
#include "files/filereader.h"
#include "models/elfheader.h"
#include "failurereasons.h"
#include "support/result.h"

Result<std::unique_ptr<ElfHeader>, ParseFailure> ElfHeaderFactory::create() {
    m_fileReader->seek(0);
    if (!m_fileReader->isOk())
        return ParseFailure::FileReadError;

    auto elf = std::make_unique<ElfHeader>();

    elf->ident = m_fileReader->readString(elf->IDENT_CHARS);
    if (!magicBytesOk(elf->ident))
        return ParseFailure::InvalidFileFormat;

    setBitness(elf);
    setEndianess(elf);

    elf->type = m_fileReader->readBytes<ObjectFileType, 2>(elf->byteOrder);
    elf->machine = m_fileReader->readBytes<Architecture, 2>(elf->byteOrder);
    elf->version = m_fileReader->readBytes<FileVersion, 4>(elf->byteOrder);
    elf->entry = getWord(elf->bitness, elf->byteOrder);
    elf->program_offset = getWord(elf->bitness, elf->byteOrder);
    elf->section_offset = getWord(elf->bitness, elf->byteOrder);
    elf->flags = m_fileReader->readBytes<uint32_t, 4>(elf->byteOrder);
    elf->program_size = m_fileReader->readBytes<uint16_t, 2>(elf->byteOrder);
    elf->program_entry_size = m_fileReader->readBytes<uint16_t, 2>(elf->byteOrder);
    elf->program_entry_count = m_fileReader->readBytes<uint16_t, 2>(elf->byteOrder);
    elf->section_entry_size = m_fileReader->readBytes<uint16_t, 2>(elf->byteOrder);
    elf->section_entry_count = m_fileReader->readBytes<uint16_t, 2>(elf->byteOrder);
    elf->section_name_index = m_fileReader->readBytes<uint16_t, 2>(elf->byteOrder);

    return std::move(elf);
}

bool ElfHeaderFactory::magicBytesOk(const std::string& identity) {
    return identity.find(std::string("\x7F") + std::string("ELF")) == 0;
}

void ElfHeaderFactory::setBitness(const std::unique_ptr<ElfHeader>& elf) {
    auto num = static_cast<Bitness>(elf->ident[4]);
    elf->bitness = num;
}

void ElfHeaderFactory::setEndianess(const std::unique_ptr<ElfHeader>& elf) {
    auto num = static_cast<ByteOrder>(elf->ident[5]);
    elf->byteOrder = num;
}

uint32_t ElfHeaderFactory::getHalf(Bitness bitness, ByteOrder byteorder) {
    if (bitness == Bitness::ThirtyTwo)
        return m_fileReader->readBytes<uint32_t, 2>(byteorder);
    return m_fileReader->readBytes<uint32_t, 4>(byteorder);
}

uint64_t ElfHeaderFactory::getWord(Bitness bitness, ByteOrder byteorder) {
    if (bitness == Bitness::ThirtyTwo)
        return m_fileReader->readBytes<uint64_t, 4>(byteorder);
    return m_fileReader->readBytes<uint64_t, 8>(byteorder);
}

