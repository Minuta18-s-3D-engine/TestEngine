#ifndef ENGINE_RESOURCE_SERIALIZATION_TYPESERIALIZATION_HPP
#define ENGINE_RESOURCE_SERIALIZATION_TYPESERIALIZATION_HPP

#include <format>
#include <type_traits>
#include <string>

#include <glm/glm.hpp>

#include "engine/containers/structuredData/DataNode.hpp"
#include "EnumTraits.hpp"

class FieldRegistry;

template <typename T>
concept Serializable = requires {
    { T::reflect() } -> std::same_as<const FieldRegistry&>;
};

namespace TypeSerialization {
    inline void serializeField(StructuredData::DataNode& out, bool value);
    inline void serializeField(StructuredData::DataNode& out, const std::string& value);

    template <typename T> requires std::is_integral_v<T>
    inline void serializeField(StructuredData::DataNode& out, T value);

    template <typename T> requires std::is_floating_point_v<T>
    inline void serializeField(StructuredData::DataNode& out, T value);

    template <glm::length_t L, typename T, glm::qualifier Q>
    inline void serializeField(
        StructuredData::DataNode& out, const glm::vec<L, T, Q>& value);

    template <glm::length_t L, glm::length_t R, typename T, glm::qualifier Q>
    inline void serializeField(
        StructuredData::DataNode& out, const glm::mat<L, R, T, Q>& value);

    template <typename T, glm::qualifier Q>
    inline void serializeField(
        StructuredData::DataNode& out, const glm::qua<T, Q>& value);

    template <MappedEnum T>
    inline void serializeField(StructuredData::DataNode& out, T value);

    template <Serializable T>
    inline void serializeField(StructuredData::DataNode& out, const T& value);


    inline void deserializeField(const StructuredData::DataNode& in,
                                 bool& value);
    inline void deserializeField(const StructuredData::DataNode& in,
        std::string& value);

    template <typename T> requires std::is_integral_v<T>
    inline void deserializeField(const StructuredData::DataNode& in, T& value);

    template <typename T> requires std::is_floating_point_v<T>
    inline void deserializeField(const StructuredData::DataNode& in, T& value);

    template <glm::length_t L, typename T, glm::qualifier Q>
    inline void deserializeField(
        const StructuredData::DataNode& in, glm::vec<L, T, Q>& value);

    template <glm::length_t L, glm::length_t R, typename T, glm::qualifier Q>
    inline void deserializeField(
        const StructuredData::DataNode& in, glm::mat<L, R, T, Q>& value);

    template <typename T, glm::qualifier Q>
    inline void deserializeField(
        const StructuredData::DataNode& in, glm::qua<T, Q>& value);

    template <MappedEnum T>
    inline void deserializeField(const StructuredData::DataNode& in, T& value);

    template <Serializable T>
    inline void deserializeField(const StructuredData::DataNode& in, T& value);
};

inline void TypeSerialization::serializeField(
    StructuredData::DataNode &out, const bool value
) {
    out = StructuredData::DataNode(value);
}

inline void TypeSerialization::serializeField(
    StructuredData::DataNode &out, const std::string &value
) {
    out = StructuredData::DataNode(value);
}

template<typename T> requires std::is_integral_v<T>
void TypeSerialization::serializeField(StructuredData::DataNode &out, T value) {
    out = StructuredData::DataNode(static_cast<int64_t>(value));
}

template<typename T> requires std::is_floating_point_v<T>
void TypeSerialization::serializeField(StructuredData::DataNode &out, T value) {
    out = StructuredData::DataNode(static_cast<double>(value));
}

template<glm::length_t L, typename T, glm::qualifier Q>
void TypeSerialization::serializeField(
    StructuredData::DataNode &out, const glm::vec<L, T, Q> &value
) {
    StructuredData::Array arr;
    arr.reserve(L);
    for (glm::length_t i = 0; i < L; ++i) {
        StructuredData::DataNode elemNode;
        serializeField(elemNode, value[i]);
        arr.push_back(std::move(elemNode));
    }
    out = StructuredData::DataNode(std::move(arr));
}

template<glm::length_t L, glm::length_t R, typename T, glm::qualifier Q>
void TypeSerialization::serializeField(
    StructuredData::DataNode &out, const glm::mat<L, R, T, Q> &value
) {
    StructuredData::Array arr;
    arr.reserve(L);
    for (glm::length_t i = 0; i < L; ++i) {
        StructuredData::DataNode colNode;
        serializeField(colNode, value[i]);
        arr.push_back(std::move(colNode));
    }
    out = StructuredData::DataNode(std::move(arr));
}

template<typename T, glm::qualifier Q>
void TypeSerialization::serializeField(
    StructuredData::DataNode &out, const glm::qua<T, Q> &value
) {
    StructuredData::Array arr;
    arr.reserve(4);
    arr.emplace_back(static_cast<double>(value.x));
    arr.emplace_back(static_cast<double>(value.y));
    arr.emplace_back(static_cast<double>(value.z));
    arr.emplace_back(static_cast<double>(value.w));
    out = StructuredData::DataNode(std::move(arr));
}

template<MappedEnum T>
void TypeSerialization::serializeField(StructuredData::DataNode &out, T value) {
    out = StructuredData::DataNode(std::string(
        EnumTraits<T>::mapping.toString(value)
    ));
}

inline void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, bool &value
) {
    value = in.asBoolean();
}

inline void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, std::string &value
) {
    value = in.asString();
}

template<typename T> requires std::is_integral_v<T>
void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, T &value
) {
    value = static_cast<T>(in.asInteger());
}

template<typename T> requires std::is_floating_point_v<T>
void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, T &value
) {
    value = static_cast<T>(in.asNumber());
}

template<glm::length_t L, typename T, glm::qualifier Q>
void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, glm::vec<L, T, Q> &value
) {
    const auto& arr = in.asArray();
    if (arr.size() != L) {
        std::string errorMessage = std::format(
            "Invalid vector size: got {}, expected {}", arr.size(), L
        );
        throw std::runtime_error(errorMessage);
    }

    for (glm::length_t i = 0; i < L; ++i) {
        deserializeField(arr[i], value[i]);
    }
}

template<glm::length_t L, glm::length_t R, typename T, glm::qualifier Q>
void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, glm::mat<L, R, T, Q> &value
) {
    const auto& arr = in.asArray();
    glm::ivec2 matrixDimensions(0, arr.size());
    if (!arr.empty()) {
        matrixDimensions.x = static_cast<int>(arr[0].asArray().size());
    }

    if (matrixDimensions.y != L || matrixDimensions.x != R) {
        std::string errorMessage = std::format(
            "Invalid matrix size: got {}x{}, expected {}x{}",
            matrixDimensions.y, matrixDimensions.x, L, R
        );
        throw std::runtime_error(errorMessage);
    }

    for (glm::length_t i = 0; i < L; ++i) {
        deserializeField(arr[i], value[i]);
    }
}

template<typename T, glm::qualifier Q>
void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, glm::qua<T, Q> &value
) {
    const auto& arr = in.asArray();
    if (arr.size() != 4) {
        std::string errorMessage = std::format(
            "Invalid vector size: got {}, expected 4", arr.size()
        );
        throw std::runtime_error(errorMessage);
    }

    value.x = static_cast<T>(arr[0].asNumber());
    value.y = static_cast<T>(arr[1].asNumber());
    value.z = static_cast<T>(arr[2].asNumber());
    value.w = static_cast<T>(arr[3].asNumber());
}

template<MappedEnum T>
void TypeSerialization::deserializeField(
    const StructuredData::DataNode &in, T &value
) {
    auto enumValue = EnumTraits<T>::mapping.tryToEnum(in.asString());
    if (!enumValue.has_value()) {
        std::string errorMessage = std::format(
            "{} is not a valid option", in.asString()
        );
        throw std::runtime_error(errorMessage);
    }
    value = *enumValue;
}

#endif // ENGINE_RESOURCE_SERIALIZATION_TYPESERIALIZATION_HPP

