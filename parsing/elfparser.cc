
#include "failurereasons.h"
#include "models/elf.h"
#include "parsing/elfparser.h"
#include "support/result.h"

#include <memory>
#include <string>

Result<Elf, ParseFailure> ElfParser::parse(const std::string& filename) const
{
    auto fileReader = m_fileReaderFactory(filename);
    auto elfHeaderResult = m_elfHeaderFactory->create(fileReader);

    return elfHeaderResult.match(
            [&] (auto&& elf) -> Result<Elf, ParseFailure>
            {
                return parseSectionHeaders(fileReader, elf);
            },
            [] (auto&& failure) -> Result<Elf, ParseFailure>
            {
                return failure;
            });
}

Result<Elf, ParseFailure> ElfParser::parseSectionHeaders(
        std::shared_ptr<FileReader>& fileReader,
        const ElfHeader& elf) const
{
    auto sectionResult = m_sectionHeaderFactory->create(fileReader, elf);
    return sectionResult.match(
            [&] (auto&& sectionHeaders) -> Result<Elf, ParseFailure>
            {
                return parseProgramHeaders(fileReader, elf, sectionHeaders);
            },
            [] (auto&& failure) -> Result<Elf, ParseFailure>
            {
                return failure;
            });
}

Result<Elf, ParseFailure> ElfParser::parseProgramHeaders(
        std::shared_ptr<FileReader>& fileReader,
        const ElfHeader& elf,
        const std::vector<SectionHeader>& sectionHeaders) const
{
    auto programHeaderResult = m_programHeaderFactory->create(fileReader, elf);
    return programHeaderResult.match(
            [&elf, &sectionHeaders] (auto&& programHeaders) -> Result<Elf, ParseFailure>
            {
                return Elf { elf, sectionHeaders, programHeaders };
            },
            [] (auto&& failure) -> Result<Elf, ParseFailure>
            { 
                return failure;
            });
}
