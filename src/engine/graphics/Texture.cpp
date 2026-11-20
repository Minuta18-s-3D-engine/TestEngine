#include "Texture.hpp"

#include "engine/utils/exc/GeneralExceptions.hpp"

GLenum Texture::getGLTarget() const {
    switch (type) {
        case SamplerType::Texture2D: return GL_TEXTURE_2D;
        case SamplerType::CubeMap2D: return GL_TEXTURE_CUBE_MAP;
    }
    return GL_TEXTURE_2D;
}

void Texture::initializeTexture(const ImageData &image) {
    if (!image.getData()) {
        throw exc::invalid_argument("empty image data.");
    }

    releaseGPUResources();

    width = image.getWidth();
    height = image.getHeight();
    format = image.getFormat();

    glGenTextures(1, &id);
    bind();

    GLint wrapParam = GL_REPEAT;
    switch (wrapMode) {
        case WrapMode::ClampToEdge:
            wrapParam = GL_CLAMP_TO_EDGE; break;
        case WrapMode::MirroredRepeat:
            wrapParam = GL_MIRRORED_REPEAT; break;
        case WrapMode::Repeat:
        default:
            wrapParam = GL_REPEAT; break;
    }

    const GLenum target = getGLTarget();
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapParam);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapParam);

    GLint magFilter = GL_LINEAR;
    GLint minFilter = GL_LINEAR;
    if (std::holds_alternative<FilterMode>(filtering)) {
        const FilterMode mode = std::get<FilterMode>(filtering);
        magFilter = (mode == FilterMode::Linear) ? GL_LINEAR : GL_NEAREST;
        minFilter = magFilter;
    } else {
        const auto& mode = std::get<DetailedFilterMode>(filtering);
        magFilter = (mode.mag == FilterMode::Linear) ? GL_LINEAR : GL_NEAREST;
        minFilter = (mode.min == FilterMode::Linear) ? GL_LINEAR : GL_NEAREST;
    }

    if (generateMipmaps) {
        minFilter = (minFilter == GL_LINEAR)
            ? GL_LINEAR_MIPMAP_LINEAR
            : GL_LINEAR_MIPMAP_NEAREST;
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum dataFormat = GL_RGBA;
    switch (format) {
        case ImageFormat::rgba:
            dataFormat = GL_RGBA; break;
        case ImageFormat::rgb:
            dataFormat = GL_RGB; break;
        case ImageFormat::grayscale:
            dataFormat = GL_RED; break;
        default:
            dataFormat = GL_RGBA; break;
    }

    GLint internalFormat = static_cast<GLint>(dataFormat);
    if (colorSpace == ColorSpace::sRGB) {
        if (dataFormat == GL_RGB) {
            internalFormat = GL_SRGB;
        } else if (dataFormat == GL_RGBA) {
            internalFormat = GL_SRGB_ALPHA;
        }
    }

    glTexImage2D(
        target, 0, internalFormat, static_cast<GLsizei>(width),
        static_cast<GLsizei>(height), 0, dataFormat, GL_UNSIGNED_BYTE,
        image.getData()
    );

    if (generateMipmaps) {
        glGenerateMipmap(target);
    }

    unbind();

    if (GLAD_GL_ARB_bindless_texture) {
        bindlessHandle = glGetTextureHandleARB(id);
        if (bindlessHandle != NO_HANDLE) {
            glMakeTextureHandleResidentARB(bindlessHandle);
        }
    }
}

void Texture::releaseGPUResources() {
    if (bindlessHandle != NO_HANDLE) {
        if (GLAD_GL_ARB_bindless_texture) {
            glMakeImageHandleNonResidentARB(bindlessHandle);
        }
        bindlessHandle = NO_HANDLE;
    }

    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

Texture::Texture(
    const ImageData& image, ColorSpace colorSpace_, WrapMode wrapMode_,
    bool generateMipmaps_, TextureFilterMode filtering_, SamplerType type_
) : colorSpace(colorSpace_),
    wrapMode(wrapMode_),
    generateMipmaps(generateMipmaps_),
    filtering(filtering_),
    type(type_)
{
    initializeTexture(image);
}

Texture::~Texture() {
    releaseGPUResources();
}

Texture::Texture(Texture&& other) noexcept
  : colorSpace(other.colorSpace),
    wrapMode(other.wrapMode),
    generateMipmaps(other.generateMipmaps),
    filtering(other.filtering),
    type(other.type),
    width(other.width),
    height(other.height),
    format(other.format),
    id(other.id),
    bindlessHandle(other.bindlessHandle)
{
    other.width = 0;
    other.height = 0;
    other.id = 0;
    other.bindlessHandle = NO_HANDLE;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        releaseGPUResources();

        colorSpace = other.colorSpace;
        wrapMode = other.wrapMode;
        generateMipmaps = other.generateMipmaps;
        filtering = other.filtering;
        type = other.type;
        width = other.width;
        height = other.height;
        format = other.format;
        id = other.id;
        bindlessHandle = other.bindlessHandle;

        other.width = 0;
        other.height = 0;
        other.id = 0;
        other.bindlessHandle = NO_HANDLE;
    }
    return *this;
}

void Texture::onAfterDeserialize() {
    // WIP
}

void Texture::bind() const {
    glBindTexture(getGLTarget(), id);
}

void Texture::unbind() const {
    glBindTexture(getGLTarget(), 0);
}
