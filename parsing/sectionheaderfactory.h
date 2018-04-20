#pragma once

#include <memory>

#include "files/filereader.h"
#include "models/elfheader.h"
#include "models/sectionheader.h"
#include "support/result.h"
#include "failurereasons.h"

class SectionHeaderFactory {
    private:
        SectionHeader createSingle(
                std::shared_ptr<FileReader>& fileReader,
                Bitness bitness,
                ByteOrder endianess);
        std::string getName(
                std::shared_ptr<FileReader>& fileReader,
                const SectionHeader& header,
                uint64_t offset);
        std::string getContents(
                std::shared_ptr<FileReader>& fileReader,
                const SectionHeader& header);

    public:
        SectionHeaderFactory() = default;

        Result<std::vector<SectionHeader>, ParseFailure> create(
                std::shared_ptr<FileReader> fileReader,
                const ElfHeader& elf);
};
