#include "Texture.hpp"

GLenum Texture::getGLTarget() const {
    switch (type) {
        case SamplerType::Texture2D: return GL_TEXTURE_2D;
        case SamplerType::CubeMap2D: return GL_TEXTURE_CUBE_MAP;
    }
    return GL_TEXTURE_2D;
}

Texture::Texture(
    uint width, uint height, ImageFormat format, 
    const uint8_t* image_data, SamplerType _type
) : width(width), height(height), format(format), type(_type) {
    glGenTextures(1, &id);
    this->bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum fmt = GL_RGB;
    if (format == ImageFormat::rgba) fmt = GL_RGBA;

    GLenum target = getGLTarget();

    if (type == SamplerType::Texture2D) {
        glTexImage2D(
            GL_TEXTURE_2D, 0, fmt, width, height, 0, 
            fmt, GL_UNSIGNED_BYTE, image_data
        );
    } else if (type == SamplerType::CubeMap2D) {
        for (uint i = 0; i < 6; ++i) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt, 
                width, height, 0, fmt, GL_UNSIGNED_BYTE, image_data
            );
        }
    }

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (type == SamplerType::CubeMap2D) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    }

    glTexParameteri(target, 
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(target, 
        GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(target);
    this->unbind();

    if (GL_ARB_bindless_texture) {
        bindlessHandle = glGetTextureHandleARB(id);
        glMakeTextureHandleResidentARB(bindlessHandle);
    }
}

Texture::~Texture() {
    if (bindlessHandle != NO_HANDLE) {
        glMakeTextureHandleNonResidentARB(bindlessHandle);
    }
    glDeleteTextures(1, &id);
}

void Texture::bind() {
    glBindTexture(getGLTarget(), id);
}

void Texture::unbind() {
    glBindTexture(getGLTarget(), 0);
}

uint Texture::getWidth() {
    return width;
}

uint Texture::getHeight() {
    return height;
}

uint Texture::getId() {
    return id;
}

uint64_t Texture::getHandle() const {
    return bindlessHandle;
}

SamplerType Texture::getType() const {
    return type;
}

std::shared_ptr<Texture> Texture::create(const ImageData* img) {
    uint width = img->getWidth();
    uint height = img->getHeight();
    void* data = img->getData();
    std::shared_ptr<Texture> result = std::make_shared<Texture>(
        width, height, img->getFormat(), static_cast<uint8_t*>(data) 
    );
    return result;
}

