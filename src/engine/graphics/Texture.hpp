#ifndef ENGINE_GRAPHICS_TEXTURE_H_
#define ENGINE_GRAPHICS_TEXTURE_H_

#include "../utils/EngineTypes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

enum class ImageFormat {
    rgb,
    rgba,
};

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
};

#endif // ENGINE_GRAPHICS_TEXTURE_H_
