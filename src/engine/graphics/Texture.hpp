#ifndef ENGINE_GRAPHICS_TEXTURE_H_
#define ENGINE_GRAPHICS_TEXTURE_H_

#include "../utils/EngineTypes.h"
#include "../assets/utils/ImageData.hpp"
#include "SamplerType.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>

class Texture {
    const uint64_t NO_HANDLE = 0;

    uint width, height;
    ImageFormat format;
    SamplerType type;

    uint id;
    uint64_t bindlessHandle = NO_HANDLE;

    GLenum getGLTarget() const;
public:
    Texture(
        uint width, uint height, 
        ImageFormat format, 
        const uint8_t* image_data,
        SamplerType _type = SamplerType::Texture2D
    );
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept = delete; // TODO

    void bind();
    void unbind();

    uint getWidth();
    uint getHeight();
    uint getId();
    uint64_t getHandle() const;
    SamplerType getType() const;

    static std::shared_ptr<Texture> create(const ImageData* img);
};

#endif // ENGINE_GRAPHICS_TEXTURE_H_
