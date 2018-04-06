#include <fstream>
#include <memory>

#include "programheaderfactory.h"
#include "models/programheader.h"
#include "models/elfheader.h"
#include "support/result.h"
#include "failurereasons.h"

Result<std::vector<std::shared_ptr<ProgramHeader>>, ParseFailure> ProgramHeaderFactory::Create(std::shared_ptr<ElfHeader> elf) {
    auto headers = std::vector<std::shared_ptr<ProgramHeader>>();

    for (auto i = 0; i < elf->program_entry_count; i++) {
        auto offset = elf->program_offset + (i * elf->program_entry_size);
        m_fileReader->seek(offset);
        if (!m_fileReader->isOk())
            return ParseFailure::FileReadError;

        auto header = createSingle(elf->bitness, elf->byteOrder);
        headers.push_back(header);
    }

    return headers;
}

std::shared_ptr<ProgramHeader> ProgramHeaderFactory::deserialize32(ByteOrder byteOrder) {
    auto header = std::make_shared<ProgramHeader>();
    
    header->type = m_fileReader->readBytes<ProgramType, 4>(byteOrder);
    header->offset = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->virtual_address = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->physical_address = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->file_size = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->memory_size = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->flags = m_fileReader->readBytes<ProgramFlags, 4>(byteOrder);
    header->alignment = m_fileReader->readBytes<uint32_t, 4>(byteOrder);

    return header;
}

std::shared_ptr<ProgramHeader> ProgramHeaderFactory::deserialize64(ByteOrder byteOrder) {
    auto header = std::make_shared<ProgramHeader>();

    header->type = m_fileReader->readBytes<ProgramType, 4>(byteOrder);
    header->flags = m_fileReader->readBytes<ProgramFlags, 4>(byteOrder);
    header->offset = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->virtual_address = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->physical_address = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->file_size = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->memory_size = m_fileReader->readBytes<uint32_t, 4>(byteOrder);
    header->alignment = m_fileReader->readBytes<uint32_t, 4>(byteOrder);

    return header;
}

std::shared_ptr<ProgramHeader> ProgramHeaderFactory::createSingle(Bitness bitness, ByteOrder byteOrder) {
    return (bitness == Bitness::ThirtyTwo) ? deserialize32(byteOrder) : deserialize64(byteOrder);
}
