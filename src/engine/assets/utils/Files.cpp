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

ImageType getImageType(std::unique_ptr<uint8_t[]> imageData, size_t length) {
    if (length < 16) return ImageType::UNKNOWN;

    // .jpg:  FF D8 FF
    // .png:  89 50 4E 47 0D 0A 1A 0A

    if (std::strncmp((const char*) (imageData.get()), "\xFF\xD8\xFF", 3))
        return ImageType::JPG;
    if (std::strncmp((const char*) (imageData.get()), "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8))
        return ImageType::PNG;
    return ImageType::UNKNOWN;
}
