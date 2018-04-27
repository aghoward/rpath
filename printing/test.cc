#include <elf.h>
#include <string>
#include <iostream>

int main()
{
    //std::cout << std::hex;
    std::cout << std::to_string(ELFDATA2LSB) << std::endl;
    std::cout << std::to_string(ELFDATA2MSB) << std::endl;
    return 0;
}
