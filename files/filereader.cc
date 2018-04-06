#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <experimental/filesystem>
#include <fstream>
#include <string>

#include "models/elfheader.h"
#include "files/filereader.h"


FileReader::FileReader(const std::string& filename)
    : FileReader(std::experimental::filesystem::path(filename))
{
}

FileReader::FileReader(const std::experimental::filesystem::path& path)
{
    if (!(std::experimental::filesystem::exists(path) && std::experimental::filesystem::is_regular_file(path)))
        throw std::runtime_error(std::string("Path does not exist or is not regular file: ") + path.native());
    m_fd = std::fstream(path.native().c_str(), std::ios_base::in | std::ios_base::binary);
}

bool FileReader::isOk() const {
    return !(m_fd.bad() || m_fd.fail());
}

std::byte FileReader::readByte() {
    return static_cast<std::byte>(m_fd.get());
}

std::vector<std::byte> FileReader::readBytes(size_t count, ByteOrder byteOrder) {
    auto bytes = std::vector<std::byte>(count);
    std::for_each(bytes.begin(), bytes.end(), [&] (auto& b) { b = this->readByte(); });
    if (byteOrder == ByteOrder::LittleEndian)
        std::reverse(bytes.begin(), bytes.end());
    return bytes;
}

std::string FileReader::readString() {
    auto ret = std::string();
    auto c = readByte();

    while (std::to_integer<uint8_t>(c) != (uint8_t)0x00) {
        ret += static_cast<unsigned char>(std::to_integer<uint8_t>(c));
        c = readByte();
    }

    return ret;
}

std::string FileReader::readString(size_t length) {
    char cstr[length];
    m_fd.get(cstr, length);
    return std::string(cstr);
};

void FileReader::seek(size_t position) {
    m_fd.seekg(position);
}
