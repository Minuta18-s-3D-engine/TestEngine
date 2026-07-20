#ifndef ENGINE_CONTAINERS_REFLECTION_REFLECTION_HPP_
#define ENGINE_CONTAINERS_REFLECTION_REFLECTION_HPP_

#include <string_view>
#include <optional>

namespace Reflection {

template <typename Class, typename T>
struct FieldDef {
    std::string_view name;
    T Class::* member;
    std::optional<T> defaultValue = std::nullopt;
    bool required = true;
};

template <typename Class, typename T>
constexpr auto Field(std::string_view name, T Class::* member) {
    return FieldDef<Class, T>{ name, member, std::nullopt, true };
}

template <typename Class, typename T>
constexpr auto Field(
    std::string_view name, T Class::* member, const T& defaultValue
) {
    return FieldDef<Class, T>{ name, member, defaultValue, false };
}

template <typename T>
constexpr std::pair<const char*, T> EnumField(const char* name, T value) {
    return {name, value};
}

template <typename T>
struct Meta {
    static constexpr bool isMapped = true;
};

template <typename E>
constexpr auto EnumVal(std::string_view name, E val) {
    return std::pair<std::string_view, E>{ name, val };
}

template <typename T>
struct EnumMeta {
    static constexpr bool isMapped = true;  
};

} // namespace Reflection

#define STRUCT_SCHEMA(Type, ...) \
    namespace Reflection { \
        template <> \
        struct Meta<Type> { \
            static constexpr bool isMapped = true; \
            static constexpr auto fields() { \
                return std::make_tuple(__VA_ARGS__); \
            } \
        }; \
    }

#define ENUM_SCHEMA(Type, ...) \
    namespace Reflection { \
        template <> \
        struct EnumMeta<Type> { \
            static constexpr bool isMapped = true; \
            static constexpr auto values() { \
                return std::make_tuple(__VA_ARGS__); \
            } \
        }; \
    }

#endif // ENGINE_CONTAINERS_REFLECTION_REFLECTION_HPP_
