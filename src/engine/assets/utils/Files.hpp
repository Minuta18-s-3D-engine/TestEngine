#ifndef ENGINE_ASSETS_FILES_H_
#define ENGINE_ASSETS_FILES_H_

#include <memory>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

std::unique_ptr<uint8_t[]> read_bytes(
    const fs::path& filename, size_t& length
);

#endif // ENGINE_ASSETS_FILES_H_
