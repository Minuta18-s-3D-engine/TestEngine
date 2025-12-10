#ifndef ENGINE_ASSETS_UTILS_FILES_H_
#define ENGINE_ASSETS_UTILS_FILES_H_

#include <memory>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cstring>

namespace fs = std::filesystem;

std::unique_ptr<uint8_t[]> read_bytes(
    const fs::path& filename, size_t& length
);

enum ImageType {
    PNG,
    JPG,
    UNKNOWN
};

ImageType getImageType(std::unique_ptr<uint8_t[]> imageData, size_t length);

#endif // ENGINE_ASSETS_UTILS_FILES_H_
