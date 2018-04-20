
#include "cdif.h"
#include "parsing/elfparser.h"
#include "parsing/factoriesmodule.h"
#include "support/typetraits.h"

#include <cstdlib>
#include <iostream>
#include <string>

void bail(const std::string& failure)
{
    std::cout << "Error: " << failure << std::endl;
    exit(1);
}

cdif::Container createContainer()
{
    auto container = cdif::Container();
    container.registerModule<FactoriesModule>();
    return container;
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        bail(std::string("Usage: ") + argv[0] + " <elfpath>");

    auto ctx = createContainer();
    auto elfParser = ctx.resolve<ElfParser>();
    
    auto parseResult = elfParser.parse(std::string(argv[1]));
    
    parseResult.match(
        [] (auto&& elf)
        {
            std::cout << "Section Headers: " << elf.sectionHeaders.size() << std::endl;
            std::cout << "Program Headers: " << elf.programHeaders.size() << std::endl;
        },
        [] (auto&& failure)
        {
            switch (failure)
            {
                case ParseFailure::FileReadError:
                    bail(std::string("Failed to read file!"));
                    break;
                case ParseFailure::InvalidFileFormat:
                    bail(std::string("Invalid File Format!"));
                    break;
                case ParseFailure::FileDoesNotExist:
                    bail(std::string("File does not exist!"));
                    break;

            }
        }
    );

    return 0;
}
