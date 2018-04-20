#include "parsing/factoriesmodule.h"

#include "cdif/cdif.h"
#include "parsing/elfheaderfactory.h"
#include "parsing/elfparser.h"
#include "parsing/programheaderfactory.h"
#include "parsing/sectionheaderfactory.h"
#include "files/filereader.h"

#include <functional>
#include <memory>

void FactoriesModule::load(cdif::Container& ctx)
{
    ctx.bind<FileReader, const std::string&>().build();
    ctx.bind<std::shared_ptr<FileReader>, const std::string&>(
            [] (const auto& fn) { return std::make_shared<FileReader>(fn); })
        .build();

    ctx.bind<ElfHeaderFactory>().build();
    ctx.bind<SectionHeaderFactory>().build();
    ctx.bind<ProgramHeaderFactory>().build();

    ctx.bind<ElfParser,
        std::function<std::shared_ptr<FileReader> (const std::string&)>,
        std::unique_ptr<ElfHeaderFactory>,
        std::unique_ptr<SectionHeaderFactory>,
        std::unique_ptr<ProgramHeaderFactory>>()
        .build();
}
