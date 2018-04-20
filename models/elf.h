#pragma once

#include "models/elfheader.h"
#include "models/sectionheader.h"
#include "models/programheader.h"

#include <vector>

struct Elf
{
    ElfHeader elfHeader;
    std::vector<SectionHeader> sectionHeaders;
    std::vector<ProgramHeader> programHeaders;
};
