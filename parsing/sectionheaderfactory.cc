#include <iostream>
#include <memory>
#include <string>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "models/sectionheader.h"
#include "support/result.h"
#include "failurereasons.h"
#include "sectionheaderfactory.h"


template <size_t WordSize>
std::shared_ptr<SectionHeader> deserialize(ByteOrder endianess, std::shared_ptr<FileReader>& fileReader) {
    auto header = std::make_shared<SectionHeader>();

    header->name_offset = fileReader->readBytes<uint32_t, 4>(endianess);
    header->type = fileReader->readBytes<SectionType, 4>(endianess);
    header->flags = fileReader->readBytes<SectionFlags, WordSize>(endianess);
    header->address = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header->file_offset = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header->size = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header->link = fileReader->readBytes<uint32_t, 4>(endianess);
    header->info = fileReader->readBytes<uint32_t, 4>(endianess);
    header->address_alignment = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header->entity_size = fileReader->readBytes<uint64_t, WordSize>(endianess);

    return header;
}

std::string SectionHeaderFactory::getName(std::shared_ptr<SectionHeader> strtab, uint64_t offset) {
    m_fileReader->seek(strtab->file_offset + offset);
    return m_fileReader->readString();
}

Result<std::vector<std::shared_ptr<SectionHeader>>, ParseFailure> SectionHeaderFactory::Create(std::shared_ptr<ElfHeader> elf) {
    auto headers = std::vector<std::shared_ptr<SectionHeader>>();

    m_fileReader->seek(elf->section_offset);
    if (!m_fileReader->isOk())
        return ParseFailure::FileReadError;

    for(auto i=0; i<elf->section_entry_count; i++) {
        auto offset = elf->section_offset + (i * elf->section_entry_size);
        auto header = createSingle(elf->bitness, elf->byteOrder);
        headers.push_back(header);

        if (!m_fileReader->isOk())
            return ParseFailure::FileReadError;
    }

    auto strtab = headers.at(elf->section_name_index);
    for (auto it : headers) {
        it->name = getName(strtab, it->name_offset);
        it->contents = getContents(it);

        if (!m_fileReader->isOk())
            return ParseFailure::FileReadError;
    }

    return headers;
}

std::shared_ptr<SectionHeader> SectionHeaderFactory::createSingle(Bitness bitness, ByteOrder endianess) {
    return (bitness == Bitness::ThirtyTwo) ? deserialize<4>(endianess, m_fileReader) : deserialize<8>(endianess, m_fileReader);
}

std::string SectionHeaderFactory::getContents(std::shared_ptr<SectionHeader> header) {
    if (header->type == SectionType::None || header->type == SectionType::NoBits)
        return std::string();

    m_fileReader->seek(header->file_offset);
    return m_fileReader->readString(header->size);
}
