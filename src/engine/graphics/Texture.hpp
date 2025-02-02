#ifndef ENGINE_GRAPHICS_TEXTURE_H_
#define ENGINE_GRAPHICS_TEXTURE_H_

#include "../utils/EngineTypes.h"
#include "../assets/ImageData.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

class Texture {
    uint width, height;
    ImageFormat format;

    uint id;
public:
    Texture(
        uint width, uint height, 
        ImageFormat format, 
        std::unique_ptr<uint8_t[]> image_data
    );
    ~Texture();

    void bind();
    void unbind();

    uint getWidth();
    uint getHeight();
    uint getId();

    static std::unique_ptr<Texture> create(const ImageData* img);
};

#endif // ENGINE_GRAPHICS_TEXTURE_H_
