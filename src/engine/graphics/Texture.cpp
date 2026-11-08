#include "Texture.hpp"

std::unique_ptr<Logger> Texture::textureLogger = nullptr;

GLenum Texture::getGLTarget() const {
    switch (type) {
        case SamplerType::Texture2D: return GL_TEXTURE_2D;
        case SamplerType::CubeMap2D: return GL_TEXTURE_CUBE_MAP;
    }
    return GL_TEXTURE_2D;
}

Texture::Texture(
    const ImageData& image, 
    const TextureMeta& schema,
    SamplerType _type
) : width(image.getWidth()), 
    height(image.getHeight()), 
    format(image.getFormat()), 
    type(_type),
    id(0)
{
    if (textureLogger == nullptr) {
        textureLogger = std::make_unique<Logger>(Logging::createLogger(
            "engine.graphics.texture"
        ));
    }

    if (!image.getData()) {
        throw exc::invalid_argument(
            "Cannot create texture from empty image data");
    }

    glGenTextures(1, &id);
    this->bind();

    GLuint wrapParam = GL_REPEAT;
    switch (schema.wrapMode) {
        case TextureMeta::WrapMode::ClampToEdge:
            wrapParam = GL_CLAMP_TO_EDGE;
            break;
        case TextureMeta::WrapMode::MirroredRepeat:
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

    if (std::holds_alternative<TextureMeta::FilterMode>(schema.filtering)) {
        auto mode = std::get<TextureMeta::FilterMode>(schema.filtering);
        magFilter = minFilter = (mode == TextureMeta::FilterMode::Linear) ?
            GL_LINEAR : GL_NEAREST;
    } else {
        auto tf = std::get<TextureMeta::TextureFiltering>(schema.filtering);
        magFilter = (tf.mag == TextureMeta::FilterMode::Linear) ?
            GL_LINEAR : GL_NEAREST;
        minFilter = (tf.min == TextureMeta::FilterMode::Linear) ?
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
    if (schema.colorSpace == TextureMeta::ColorSpace::sRGB) {
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
        textureLogger->debug("glGetTextureHandleARB status: {}", (glGetTextureHandleARB == nullptr) ? "unloaded" : "loaded");
        bindlessHandle = glGetTextureHandleARB(id);
        glMakeTextureHandleResidentARB(bindlessHandle);
        textureLogger->debug(
            "Created texture with id {} and handle {}", id, bindlessHandle);
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

uint Texture::getWidth() const {
    return width;
}

uint Texture::getHeight() const {
    return height;
}

uint Texture::getId() const {
    return id;
}

uint64_t Texture::getHandle() const {
    return bindlessHandle;
}

SamplerType Texture::getType() const {
    return type;
}
