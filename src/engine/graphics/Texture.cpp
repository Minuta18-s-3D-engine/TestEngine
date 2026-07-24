#include "Texture.hpp"

GLenum Texture::getGLTarget() const {
    switch (type) {
        case SamplerType::Texture2D: return GL_TEXTURE_2D;
        case SamplerType::CubeMap2D: return GL_TEXTURE_CUBE_MAP;
    }
    return GL_TEXTURE_2D;
}

Texture::Texture(
    const ImageData& image, 
    const TextureSchema& schema,
    SamplerType _type
) : width(image.getWidth()), 
    height(image.getHeight()), 
    format(image.getFormat()), 
    type(_type),
    id(0)
{
    if (!image.getData()) {
        throw exc::invalid_argument(
            "Cannot create texture from empty image data");
    }

    glGenTextures(1, &id);
    this->bind();

    GLuint wrapParam = GL_REPEAT;
    switch (schema.wrapMode) {
        case TextureSchema::WrapMode::ClampToEdge: 
            wrapParam = GL_CLAMP_TO_EDGE;
            break;
        case TextureSchema::WrapMode::MirroredRepeat:
            wrapParam = GL_MIRRORED_REPEAT;
            break;
        default:
            wrapParam = GL_REPEAT;
            break;
    }

    GLenum target = getGLTarget();

    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapParam);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapParam);

    GLint magFilter = GL_LINEAR;
    GLint minFilter = GL_LINEAR;

    if (std::holds_alternative<TextureSchema::FilterMode>(schema.filtering)) {
        auto mode = std::get<TextureSchema::FilterMode>(schema.filtering);
        magFilter = minFilter = (mode == TextureSchema::FilterMode::Linear) ? 
            GL_LINEAR : GL_NEAREST;
    } else {
        auto tf = std::get<TextureSchema::TextureFiltering>(schema.filtering);
        magFilter = (tf.mag == TextureSchema::FilterMode::Linear) ? 
            GL_LINEAR : GL_NEAREST;
        minFilter = (tf.min == TextureSchema::FilterMode::Linear) ? 
            GL_LINEAR : GL_NEAREST;
    }

    if (schema.generateMipmaps) {
        minFilter = (minFilter == GL_LINEAR) ? 
            GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum dataFormat;
    switch (image.getFormat()) {
        case ImageFormat::rgba: dataFormat = GL_RGBA; break;
        case ImageFormat::rgb: dataFormat = GL_RGB; break;
        case ImageFormat::grayscale: dataFormat = GL_RED; break;
        default:
            dataFormat = GL_RGBA;
    }

    GLint internalFormat = dataFormat;
    if (schema.colorSpace == TextureSchema::ColorSpace::sRGB) {
        if (dataFormat == GL_RGB) internalFormat = GL_SRGB;
        else if (dataFormat == GL_RGBA) internalFormat = GL_SRGB_ALPHA;
    }

    glTexImage2D(
        target,
        0, 
        internalFormat, 
        width, 
        height, 
        0, 
        dataFormat, 
        GL_UNSIGNED_BYTE, 
        image.getData()
    );

    if (schema.generateMipmaps) {
        glGenerateMipmap(target);
    }

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

Texture::Texture(Texture&& other) noexcept
    : width(other.width), height(other.height),
      format(other.format), type(other.type),
      id(other.id), bindlessHandle(other.bindlessHandle) 
{
    other.id = 0;
    other.bindlessHandle = NO_HANDLE;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (bindlessHandle != NO_HANDLE) {
            glMakeTextureHandleNonResidentARB(bindlessHandle);
        }
        if (id != 0) {
            glDeleteTextures(1, &id);
        }

        width = other.width;
        height = other.height;
        format = other.format;
        type = other.type;
        id = other.id;
        bindlessHandle = other.bindlessHandle;

        other.id = 0;
        other.bindlessHandle = NO_HANDLE;
    }
    return *this;
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
