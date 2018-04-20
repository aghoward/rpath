#include <fstream>
#include <memory>

#include "programheaderfactory.h"
#include "models/programheader.h"
#include "models/elfheader.h"
#include "support/result.h"
#include "failurereasons.h"

#include <iostream>

Result<std::vector<ProgramHeader>, ParseFailure> ProgramHeaderFactory::create(
        std::shared_ptr<FileReader> fileReader,
        const ElfHeader& elf) {
    auto headers = std::vector<ProgramHeader>();

    for (auto i = 0; i < elf.program_entry_count; i++) {
        auto offset = elf.program_offset + (i * elf.program_entry_size);
        fileReader->seek(offset);
        if (!fileReader->isOk())
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return ParseFailure::FileReadError;
        }

        auto header = createSingle(fileReader, elf.bitness, elf.byteOrder);
        headers.push_back(header);
    }

    return headers;
}

ProgramHeader ProgramHeaderFactory::deserialize32(
        std::shared_ptr<FileReader> fileReader,
        ByteOrder byteOrder) {
    auto header = ProgramHeader();
    
    header.type = fileReader->readBytes<ProgramType, 4>(byteOrder);
    header.offset = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.virtual_address = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.physical_address = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.file_size = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.memory_size = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.flags = fileReader->readBytes<ProgramFlags, 4>(byteOrder);
    header.alignment = fileReader->readBytes<uint32_t, 4>(byteOrder);

    return header;
}

ProgramHeader ProgramHeaderFactory::deserialize64(
        std::shared_ptr<FileReader> fileReader,
        ByteOrder byteOrder) {
    auto header = ProgramHeader();

    header.type = fileReader->readBytes<ProgramType, 4>(byteOrder);
    header.flags = fileReader->readBytes<ProgramFlags, 4>(byteOrder);
    header.offset = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.virtual_address = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.physical_address = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.file_size = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.memory_size = fileReader->readBytes<uint32_t, 4>(byteOrder);
    header.alignment = fileReader->readBytes<uint32_t, 4>(byteOrder);

    return header;
}

ProgramHeader ProgramHeaderFactory::createSingle(
        std::shared_ptr<FileReader> fileReader,
        Bitness bitness,
        ByteOrder byteOrder) {
    return (bitness == Bitness::ThirtyTwo) 
        ? deserialize32(fileReader, byteOrder)
        : deserialize64(fileReader, byteOrder);
}
