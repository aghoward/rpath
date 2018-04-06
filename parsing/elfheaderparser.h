#pragma once

#include <memory>
#include <string>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "failurereasons.h"
#include "support/result.h"

class ElfHeaderFactory {
    private:
        std::shared_ptr<FileReader> m_fileReader;

        bool magicBytesOk(const std::string& identity);
        void setBitness(const std::unique_ptr<ElfHeader>& elf);
        void setEndianess(const std::unique_ptr<ElfHeader>& elf);
        uint32_t getHalf(Bitness bitness, ByteOrder endianess);
        uint64_t getWord(Bitness bitness, ByteOrder endianess);

    public:
        ElfHeaderFactory() = delete;
        ElfHeaderFactory(std::shared_ptr<FileReader> fileReader) : m_fileReader(fileReader) {};

        Result<std::unique_ptr<ElfHeader>, ParseFailure> create();
};
