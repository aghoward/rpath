#pragma once

#include <cstddef>
#include <experimental/filesystem>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

#include "models/elfheader.h"
#include "support/typetraits.h"

class FileReader {
    private:
        std::fstream m_fd;

    public:
        explicit FileReader(const std::string& filename);
        explicit FileReader(const std::experimental::filesystem::path& path);
        FileReader() = delete;

        FileReader(const FileReader &) = delete;
        FileReader& operator=(const FileReader &) = delete;

        FileReader(FileReader &&) = default;
        FileReader& operator=(FileReader &&) = default;

        ~FileReader() {
            if (m_fd.is_open())
                m_fd.close();
        }

        bool isOk() const;

        std::byte readByte();
        std::vector<std::byte> readBytes(size_t count, ByteOrder byteOrder);

        template <typename T, size_t N, typename BaseType = underlying_integral_t<T>, typename = std::enable_if_t<greater_equal_v<sizeof(T), N>>>
        T readBytes(ByteOrder byteOrder)
        {
            auto bytes = readBytes(N, byteOrder);
            auto i = bytes.size();
            BaseType ret;

            for (auto& b : bytes)
                ret += static_cast<BaseType>(b) << (--i * (sizeof(std::byte) * 8));

            return static_cast<T>(ret);
        }

        
        std::string readString();
        std::string readString(size_t length);
        
        void seek(size_t position);
        std::size_t tell();
};
