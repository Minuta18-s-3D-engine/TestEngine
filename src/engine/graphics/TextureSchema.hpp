#ifndef ENGINE_GRAPHICS_TEXTURESCHEMA_H_
#define ENGINE_GRAPHICS_TEXTURESCHEMA_H_

#include <variant>
#include <tuple>

#include "engine/containers/reflection/Reflection.hpp"

struct TextureSchema {
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
    };

    ColorSpace colorSpace = ColorSpace::Linear;
    WrapMode wrapMode = WrapMode::Repeat;
    bool generateMipmaps = false; 
    std::variant<FilterMode, TextureFiltering> filtering = FilterMode::Linear; 
};

ENUM_SCHEMA(TextureSchema::FilterMode,
    EnumField("nearest", TextureSchema::FilterMode::Nearest),
    EnumField("linear", TextureSchema::FilterMode::Linear)
)

ENUM_SCHEMA(TextureSchema::WrapMode,
    EnumField("repeat", TextureSchema::WrapMode::Repeat),
    EnumField("clampToEdge", TextureSchema::WrapMode::ClampToEdge),
    EnumField("mirroredRepeat", TextureSchema::WrapMode::MirroredRepeat)
)

ENUM_SCHEMA(TextureSchema::ColorSpace,
    EnumField("sRGB",   TextureSchema::ColorSpace::sRGB),
    EnumField("linear", TextureSchema::ColorSpace::Linear)
)

STRUCT_SCHEMA(TextureSchema::TextureFiltering,
    Field("min", &TextureSchema::TextureFiltering::min, 
        TextureSchema::FilterMode::Linear),
    Field("mag", &TextureSchema::TextureFiltering::mag, 
        TextureSchema::FilterMode::Linear)
)

STRUCT_SCHEMA(TextureSchema,
    Field("colorSpace", 
        &TextureSchema::colorSpace, TextureSchema::ColorSpace::Linear),
    Field("wrapMode", 
        &TextureSchema::wrapMode, TextureSchema::WrapMode::Repeat),
    Field("generateMipmaps", &TextureSchema::generateMipmaps, false),
    Field("filtering", &TextureSchema::filtering,       
          std::variant<TextureSchema::FilterMode, 
          TextureSchema::TextureFiltering>(TextureSchema::FilterMode::Linear))
)

#endif // ENGINE_GRAPHICS_TEXTURESCHEMA_H_
