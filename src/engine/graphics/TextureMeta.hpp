#ifndef ENGINE_GRAPHICS_TEXTUREMETA_H_
#define ENGINE_GRAPHICS_TEXTUREMETA_H_

#include <variant>

#include "engine/resource/serialization/Serialization.hpp"
#include "engine/resource/serialization/EnumTraits.hpp"

struct TextureMeta {
    enum class ColorSpace {
        sRGB,
        Linear
    };

    enum class FilterMode {
        Nearest,
        Linear
    };

    enum class WrapMode {
        Repeat,
        ClampToEdge,
        MirroredRepeat
    };

    struct TextureFiltering {
        FilterMode min = FilterMode::Linear;
        FilterMode mag = FilterMode::Linear;

        GENERATE_SERIALIZATION(TextureFiltering,
            SERIALIZE_FIELD_DEFAULT(min, FilterMode::Linear),
            SERIALIZE_FIELD_DEFAULT(mag, FilterMode::Linear)
        )
    };

    ColorSpace colorSpace = ColorSpace::Linear;
    WrapMode wrapMode = WrapMode::Repeat;
    bool generateMipmaps = false;
    std::variant<FilterMode, TextureFiltering> filtering = FilterMode::Linear;

    GENERATE_SERIALIZATION(TextureMeta,
        SERIALIZE_FIELD_DEFAULT(colorSpace, ColorSpace::Linear),
        SERIALIZE_FIELD_DEFAULT(wrapMode, WrapMode::Repeat),
        SERIALIZE_FIELD_DEFAULT(generateMipmaps, false),
        SERIALIZE_FIELD_DEFAULT(filtering, std::variant<FilterMode, TextureFiltering>(FilterMode::Linear))
    )
};

GENERATE_ENUM_SERIALIZATION(TextureMeta::ColorSpace,
    ENUM_OPTION(TextureMeta::ColorSpace::Linear, "linear"),
    ENUM_OPTION(TextureMeta::ColorSpace::sRGB, "sRGB")
)

GENERATE_ENUM_SERIALIZATION(TextureMeta::FilterMode,
    ENUM_OPTION(TextureMeta::FilterMode::Nearest, "nearest"),
    ENUM_OPTION(TextureMeta::FilterMode::Linear, "linear")
)

GENERATE_ENUM_SERIALIZATION(TextureMeta::WrapMode,
    ENUM_OPTION(TextureMeta::WrapMode::Repeat, "repeat"),
    ENUM_OPTION(TextureMeta::WrapMode::ClampToEdge, "clampToEdge"),
    ENUM_OPTION(TextureMeta::WrapMode::MirroredRepeat, "mirroredRepeat")
)

#endif // ENGINE_GRAPHICS_TEXTUREMETA_H_
