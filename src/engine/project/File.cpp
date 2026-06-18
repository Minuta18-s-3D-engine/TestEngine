#include "File.hpp"

File::File(const VirtualPath& _path)
    : File(_path, IOMode::READ) {}

File::File(const VirtualPath& _path, IOMode _mode)
    : path(_path), mode(_mode) {
    std::ios_base::openmode stdMode{};

    if (mode & IOMode::READ)   stdMode |= std::ios::in;
    if (mode & IOMode::WRITE)  stdMode |= std::ios::out;
    if (mode & IOMode::APPEND) stdMode |= std::ios::app;
    if (mode & IOMode::BINARY) stdMode |= std::ios::binary;

    std::string physicalPath = path.resolve();
    
    stream.open(physicalPath);
    if(!stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + physicalPath);
    }
}

File::~File() {
    if (stream.is_open()) {
        stream.flush();
        stream.close();
    }
}

File::File(File&& other) noexcept
  : path(std::move(other.path)),
    mode(other.mode),
    stream(std::move(other.stream)) {}

File& File::operator=(File&& other) noexcept {
    if (this != &other) {
        path = std::move(other.path);
        mode = other.mode;
        stream = std::move(other.stream);
    }
    return *this;
}

void File::write(const std::string& data) {
    if (mode & IOMode::WRITE || mode & IOMode::APPEND) {
        stream.write(data.c_str(), data.size());
    }
}

void File::writeBytes(const uint8_t* bytes, size_t size) {
    if (mode & IOMode::WRITE || mode & IOMode::APPEND) {
        stream.write(reinterpret_cast<const char*>(bytes), size);
    }
}

void File::flush() {
    stream.flush();
}

std::string File::read(size_t characters) {
    std::string result(characters, '\0');
    stream.read(result.data(), characters);
    
    result.resize(stream.gcount()); 
    return result;
}

std::string File::readLine() {
    std::string line;
    std::getline(stream, line);
    return line;
}

std::string File::readAll() {
    auto currentPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(currentPos);

    std::string result(size, '\0');
    stream.read(result.data(), size);
    return result;
}

std::vector<uint8_t> File::readBytes(size_t size) {
    std::vector<uint8_t> result(size);
    stream.read(reinterpret_cast<char*>(result.data()), size);
    result.resize(stream.gcount());
    return result;
}

std::vector<uint8_t> File::readLineBytes() {
    std::string line = readLine();
    return std::vector<uint8_t>(line.begin(), line.end());
}

std::vector<uint8_t> File::readAllBytes() {
    auto currentPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(currentPos);

    std::vector<uint8_t> result(size);
    stream.read(reinterpret_cast<char*>(result.data()), size);
    return result;
}

void File::seek(size_t offset, SeekDir dir) {
    std::ios_base::seekdir stdDir;
    switch (dir) {
        case SeekDir::BEGIN:   stdDir = std::ios::beg; break;
        case SeekDir::CURRENT: stdDir = std::ios::cur; break;
        case SeekDir::END:     stdDir = std::ios::end; break;
    }
    
    stream.clear(); 
    
    if (mode & IOMode::READ)  stream.seekg(offset, stdDir);
    if (mode & IOMode::WRITE) stream.seekp(offset, stdDir);
}

size_t File::getFileSize() {
    auto currentPos = (mode & IOMode::READ) ? 
        stream.tellg() : 
        stream.tellp();
    
    if (mode & IOMode::READ) { 
        stream.seekg(0, std::ios::end);
    } else {
        stream.seekp(0, std::ios::end);
    }

    size_t size = (mode & IOMode::READ) ? 
        stream.tellg() : 
        stream.tellp();
    
    if (mode & IOMode::READ) {
        stream.seekg(currentPos);
    } else {
        stream.seekp(currentPos);
    }

    return size;
}
