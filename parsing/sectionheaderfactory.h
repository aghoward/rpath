#pragma once

#include <memory>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "models/sectionheader.h"
#include "support/result.h"
#include "failurereasons.h"

class SectionHeaderFactory {
    private:
        std::shared_ptr<FileReader> m_fileReader;

        std::shared_ptr<SectionHeader> createSingle(Bitness bitness, ByteOrder endianess);
        std::string getName(std::shared_ptr<SectionHeader> header, uint64_t offset);
        std::string getContents(std::shared_ptr<SectionHeader> header);

    public:
        SectionHeaderFactory() = delete;
        SectionHeaderFactory(std::shared_ptr<FileReader> fileReader) : m_fileReader(fileReader) {};

        Result<std::vector<std::shared_ptr<SectionHeader>>, ParseFailure> Create(std::shared_ptr<ElfHeader> elf);
};
