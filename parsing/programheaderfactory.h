#pragma once

#include <fstream>
#include <memory>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "models/programheader.h"
#include "support/result.h"
#include "failurereasons.h"

class ProgramHeaderFactory {
    private:
        std::shared_ptr<FileReader> m_fileReader;

        std::shared_ptr<ProgramHeader> createSingle(Bitness bitness, ByteOrder endianess);
        std::shared_ptr<ProgramHeader> deserialize32(ByteOrder endianess);
        std::shared_ptr<ProgramHeader> deserialize64(ByteOrder endianess);

    public:
        ProgramHeaderFactory() = delete;
        ProgramHeaderFactory(std::shared_ptr<FileReader> fileReader) : m_fileReader(fileReader) {};

        Result<std::vector<std::shared_ptr<ProgramHeader>>, ParseFailure> Create(std::shared_ptr<ElfHeader> elf);
};
