#include "Files.hpp"

std::unique_ptr<uint8_t[]> read_bytes(
    const fs::path& filename, size_t& length
) {
    std::ifstream input(filename, std::ios::binary);
    if (!input.is_open())
        throw std::runtime_error("Cant load file " + filename.string());

    input.seekg(0, std::ios_base::end);
    length = input.tellg();
    input.seekg(0, std::ios_base::beg);

    auto data = std::make_unique<uint8_t[]>(length);
    input.read((char*) data.get(), length);
    input.close();
    return data;
}
