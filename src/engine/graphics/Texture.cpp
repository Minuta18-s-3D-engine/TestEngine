#include "Texture.hpp"

Texture::Texture(
    uint width, uint height, ImageFormat format, 
    const uint8_t* image_data
) : width(width), height(height), format(format) {
    glGenTextures(1, &id);
    this->bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum fmt = GL_RGB;

    if (format == ImageFormat::rgba) fmt = GL_RGBA;

    glTexImage2D(
        GL_TEXTURE_2D, 0, fmt, width, height, 0, 
        fmt, GL_UNSIGNED_BYTE, image_data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    this->unbind();
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
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

std::shared_ptr<Texture> Texture::create(const ImageData* img) {
    uint width = img->getWidth();
    uint height = img->getHeight();
    void* data = img->getData();
    std::shared_ptr<Texture> result = std::make_shared<Texture>(
        width, height, img->getFormat(), static_cast<uint8_t*>(data) 
    );
    return result;
}

