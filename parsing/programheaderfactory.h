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
        ProgramHeader createSingle(
            std::shared_ptr<FileReader> fileReader,
            Bitness bitness,
            ByteOrder endianess);
        ProgramHeader deserialize32(std::shared_ptr<FileReader> fileReader, ByteOrder endianess);
        ProgramHeader deserialize64(std::shared_ptr<FileReader> fileReader, ByteOrder endianess);

    public:
        ProgramHeaderFactory() = default;

        Result<std::vector<ProgramHeader>, ParseFailure> create(
                std::shared_ptr<FileReader> fileReader,
                const ElfHeader& elf);
};
