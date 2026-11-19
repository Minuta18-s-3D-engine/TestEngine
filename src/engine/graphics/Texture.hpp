#ifndef ENGINE_GRAPHICS_TEXTURE_H_
#define ENGINE_GRAPHICS_TEXTURE_H_

#include "engine/resource/Resource.hpp"
#include "engine/resource/serialization/Serialization.hpp"
#include "engine/resource/utils/ImageData.hpp"

#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SamplerType.hpp"


class Texture final : public Resource {
public:
    enum class ColorSpace { sRGB, Linear };
    enum class FilterMode { Nearest, Linear };
    enum class WrapMode { Repeat, ClampToEdge, MirroredRepeat };

    struct DetailedFilterMode {
        FilterMode min = FilterMode::Linear;
        FilterMode mag = FilterMode::Linear;

        GENERATE_SERIALIZATION(DetailedFilterMode,
            SERIALIZE_FIELD_DEFAULT(min, FilterMode::Linear),
            SERIALIZE_FIELD_DEFAULT(mag, FilterMode::Linear)
        )
    };

    using TextureFilterMode = std::variant<FilterMode, DetailedFilterMode>;

    struct Meta {
        ColorSpace colorSpace = ColorSpace::Linear;
        WrapMode wrapMode = WrapMode::Repeat;
        bool generateMipmaps = false;
        TextureFilterMode filtering = FilterMode::Linear;
        SamplerType type = SamplerType::Texture2D;

        GENERATE_SERIALIZATION(Meta,
            SERIALIZE_FIELD_DEFAULT(colorSpace, ColorSpace::Linear),
            SERIALIZE_FIELD_DEFAULT(wrapMode, WrapMode::Repeat),
            SERIALIZE_FIELD_DEFAULT(generateMipmaps, false),
            SERIALIZE_FIELD_DEFAULT(filtering,
                TextureFilterMode(FilterMode::Linear)),
            SERIALIZE_FIELD_DEFAULT(type, SamplerType::Texture2D)
        )
    };
protected:
    void onDeserialized() override;

private:
    static constexpr uint64_t NO_HANDLE = 0;

    uint32_t width = 0;
    uint32_t height = 0;
    ImageFormat format = ImageFormat::rgba;
    SamplerType type;
    uint32_t id = 0;
    uint32_t bindlessHandle = NO_HANDLE;

    Meta meta;

    Texture() = default;

    [[nodiscard]] GLenum getGLTarget() const;
public:
    Texture(
        const ImageData& image, 
        Meta&& meta
    );
    ~Texture() override;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    [[nodiscard]] std::string getTypeName() const override { return "Texture"; };
    [[nodiscard]] bool canSerialize() const override { return false; };

    void bind();
    void unbind();

    [[nodiscard]] uint32_t getWidth() const { return width; };
    [[nodiscard]] uint32_t getHeight() const { return height; };
    [[nodiscard]] uint32_t getId() const { return id;};
    [[nodiscard]] uint64_t getBindlessHandle() const { return bindlessHandle; };
    [[nodiscard]] SamplerType getType() const { return type; };
    [[nodiscard]] const Meta& getMeta() const { return meta; }

    GENERATE_SERIALIZATION(Texture,
        SERIALIZE_FIELD_DEFAULT(meta, Meta())
    )
};

GENERATE_ENUM_SERIALIZATION(Texture::ColorSpace,
    ENUM_OPTION(Texture::ColorSpace::Linear, "linear"),
    ENUM_OPTION(Texture::ColorSpace::sRGB, "sRGB")
)

GENERATE_ENUM_SERIALIZATION(Texture::FilterMode,
    ENUM_OPTION(Texture::FilterMode::Nearest, "nearest"),
    ENUM_OPTION(Texture::FilterMode::Linear, "linear")
)

GENERATE_ENUM_SERIALIZATION(Texture::WrapMode,
    ENUM_OPTION(Texture::WrapMode::Repeat, "repeat"),
    ENUM_OPTION(Texture::WrapMode::ClampToEdge, "clampToEdge"),
    ENUM_OPTION(Texture::WrapMode::MirroredRepeat, "mirroredRepeat")
)

#endif // ENGINE_GRAPHICS_TEXTURE_H_
