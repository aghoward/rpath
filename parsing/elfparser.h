#pragma once

#include "failurereasons.h"
#include "files/filereader.h"
#include "models/elf.h"
#include "parsing/elfheaderfactory.h"
#include "parsing/sectionheaderfactory.h"
#include "parsing/programheaderfactory.h"
#include "support/result.h"

#include <functional>
#include <memory>
#include <string>

class ElfParser
{
        std::function<std::shared_ptr<FileReader> (const std::string& name)> m_fileReaderFactory;
        std::unique_ptr<ElfHeaderFactory> m_elfHeaderFactory;
        std::unique_ptr<SectionHeaderFactory> m_sectionHeaderFactory;
        std::unique_ptr<ProgramHeaderFactory> m_programHeaderFactory;

        Result<Elf, ParseFailure> parseSectionHeaders(
            std::shared_ptr<FileReader>& fileReader,
            const ElfHeader& elf) const;

        Result<Elf, ParseFailure> parseProgramHeaders(
            std::shared_ptr<FileReader>& fileReader,
            const ElfHeader& elf,
            const std::vector<SectionHeader>& sectionHeaders) const;

    public:
        ElfParser(
                std::function<std::shared_ptr<FileReader> (const std::string& name)> fileReaderFactory,
                std::unique_ptr<ElfHeaderFactory> elfHeaderFactory,
                std::unique_ptr<SectionHeaderFactory> sectionHeaderFactory,
                std::unique_ptr<ProgramHeaderFactory> programHeaderFactory)
            : m_fileReaderFactory(fileReaderFactory),
            m_elfHeaderFactory(std::move(elfHeaderFactory)),
            m_sectionHeaderFactory(std::move(sectionHeaderFactory)),
            m_programHeaderFactory(std::move(programHeaderFactory))
        {
        }

        Result<Elf, ParseFailure> parse(const std::string& filename) const;
};
