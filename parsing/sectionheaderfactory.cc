#include <iostream>
#include <memory>
#include <string>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "models/sectionheader.h"
#include "support/result.h"
#include "failurereasons.h"
#include "sectionheaderfactory.h"

#include <iostream>

template <size_t WordSize>
SectionHeader deserialize(ByteOrder endianess, std::shared_ptr<FileReader>& fileReader) {
    auto header = SectionHeader();

    header.name_offset = fileReader->readBytes<uint32_t, 4>(endianess);
    header.type = fileReader->readBytes<SectionType, 4>(endianess);
    header.flags = fileReader->readBytes<SectionFlags, WordSize>(endianess);
    header.address = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header.file_offset = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header.size = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header.link = fileReader->readBytes<uint32_t, 4>(endianess);
    header.info = fileReader->readBytes<uint32_t, 4>(endianess);
    header.address_alignment = fileReader->readBytes<uint64_t, WordSize>(endianess);
    header.entity_size = fileReader->readBytes<uint64_t, WordSize>(endianess);

    return header;
}

std::string SectionHeaderFactory::getName(
        std::shared_ptr<FileReader>& fileReader,
        const SectionHeader& strtab,
        uint64_t offset) {
    fileReader->seek(strtab.file_offset + offset);
    return fileReader->readString();
}

Result<std::vector<SectionHeader>, ParseFailure> SectionHeaderFactory::create(
        std::shared_ptr<FileReader> fileReader,
        const ElfHeader& elf) {
    auto headers = std::vector<SectionHeader>();

    fileReader->seek(elf.section_offset);
    if (!fileReader->isOk())
    {
        std::cout << __PRETTY_FUNCTION__ << " FIRST" << std::endl;
        return ParseFailure::FileReadError;
    }

    for(auto i=0; i<elf.section_entry_count; i++) {
        auto offset = elf.section_offset + (i * elf.section_entry_size);
        auto header = createSingle(fileReader, elf.bitness, elf.byteOrder);
        headers.push_back(header);

        if (!fileReader->isOk())
        {
            std::cout << __PRETTY_FUNCTION__ << " SECOND" << std::endl;
            return ParseFailure::FileReadError;
        }
    }

    auto strtab = headers.at(elf.section_name_index);
    for (auto& it : headers) {
        it.name = getName(fileReader, strtab, it.name_offset);
        it.contents = getContents(fileReader, it);

        if (!fileReader->isOk())
        {
            std::cout << __PRETTY_FUNCTION__ << " THIRD" << std::endl;
            return ParseFailure::FileReadError;
        }
    }

    return headers;
}

SectionHeader SectionHeaderFactory::createSingle(
        std::shared_ptr<FileReader>& fileReader,
        Bitness bitness,
        ByteOrder endianess) {
    return (bitness == Bitness::ThirtyTwo) ? deserialize<4>(endianess, fileReader) : deserialize<8>(endianess, fileReader);
}

std::string SectionHeaderFactory::getContents(
        std::shared_ptr<FileReader>& fileReader,
        const SectionHeader& header) {
    if (header.type == SectionType::None || header.type == SectionType::NoBits)
        return std::string();

    fileReader->seek(header.file_offset);
    return fileReader->readString(header.size);
}
