#ifndef ENGINE_PROJECT_FILE_HPP_
#define ENGINE_PROJECT_FILE_HPP_

#include <vector>
#include <string>
#include <cstdint>
#include <format>
#include <iostream>
#include <fstream>

#include "VirtualPath.hpp"

class File {
public:
    enum class SeekDir {
        BEGIN,
        CURRENT,
        END
    };

    enum class IOMode {
        READ   = 1 << 0,
        WRITE  = 1 << 1,
        APPEND = 1 << 2,
        BINARY = 1 << 3
    };
private:
    VirtualPath path;
    IOMode mode;

    std::fstream stream;
public:
    explicit File(const VirtualPath& _path);
    explicit File(const VirtualPath& _path, IOMode _mode);
    ~File();

    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&& other) noexcept;
    File& operator=(File&& other) noexcept;

    void write(const std::string& data);
    std::string read(size_t characters);
    std::string readLine();
    std::string readAll();

    template <typename... Args>
    void writeFormat(const std::string& fmt, Args&&... args) {
        write(std::vformat(
            fmt, 
            std::make_format_args(args...)
        ));
    }

    void writeBytes(const uint8_t* bytes, size_t size);
    std::vector<uint8_t> readBytes(size_t size);
    std::vector<uint8_t> readLineBytes();
    std::vector<uint8_t> readAllBytes();

    void flush();

    void seek(size_t offset, SeekDir dir = SeekDir::BEGIN);
    size_t getFileSize();

    const VirtualPath& getPath() const { return path; }
    IOMode getIOMode() const { return mode; }
};

inline File::IOMode operator|(File::IOMode l, File::IOMode r) {
    return static_cast<File::IOMode>(
        static_cast<uint8_t>(l) | static_cast<uint8_t>(r)
    );
}

inline bool operator&(File::IOMode l, File::IOMode r) {
    return (static_cast<uint8_t>(l) & static_cast<uint8_t>(r)) != 0;
}

#endif // ENGINE_PROJECT_FILE_HPP_
