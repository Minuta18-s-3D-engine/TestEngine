#ifndef ENGINE_RESOURCE_SERIALIZATION_ENUMTRAITS_HPP
#define ENGINE_RESOURCE_SERIALIZATION_ENUMTRAITS_HPP

#include <type_traits>

#include "engine/utils/EnumMapper.hpp"

template <typename T>
struct EnumTraits {
    static constexpr bool isMapped = false;
};

#define ENUM_OPTION(enumValue, stringName) \
    { enumValue, stringName }

#define GENERATE_ENUM_SERIALIZATION(EnumType, ...) \
    template <> \
    struct EnumTraits<EnumType> { \
        using UnderlyingType = EnumType; \
        static constexpr bool isMapped = true; \
        static constexpr EnumMapper<EnumType> mapping = { \
            __VA_ARGS__ \
        } \
    };

template <typename T>
concept MappedEnum = std::is_enum_v<T> && EnumTraits<T>::isMapped;

#endif // ENGINE_RESOURCE_SERIALIZATION_ENUMTRAITS_HPP
