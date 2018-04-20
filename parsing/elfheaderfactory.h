#pragma once

#include <memory>
#include <string>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "failurereasons.h"
#include "support/result.h"

class ElfHeaderFactory {
    private:
        bool magicBytesOk(const std::string& identity);
        void setBitness(ElfHeader& elf);
        void setEndianess(ElfHeader& elf);
        uint64_t getWord(std::shared_ptr<FileReader>& fileReader, Bitness bitness, ByteOrder endianess);

    public:
        ElfHeaderFactory() = default;

        Result<ElfHeader, ParseFailure> create(std::shared_ptr<FileReader> fileReader);
};
