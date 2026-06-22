#ifndef ENGINE_ASSETS_CODECS_IMAGES_PNGCODER_HPP_
#define ENGINE_ASSETS_CODECS_IMAGES_PNGCODER_HPP_

#include "../../utils/ImageData.hpp"
#include "../../../graphics/Texture.hpp"
#include <memory>
#include <stdexcept>
#include <cstring>
#include <iostream>

#include <png.h>

/* Thanks to 
 * https://github.com/MihailRis/VoxelEngine-Cpp/blob/main/src/coders/png.cpp
 */

class PngCoder {
public:
    static std::shared_ptr<ImageData> load_image(
        const uint8_t* bytes, size_t size, std::string name
    );

    static std::shared_ptr<Texture> load_texture(
        const uint8_t* bytes, size_t size, std::string name
    );
};

#endif // ENGINE_ASSETS_CODECS_IMAGES_PNGCODER_HPP_
