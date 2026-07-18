#ifndef ENGINE_CONTAINERS_STRUCTUREDDATA_MAPPER_HPP_
#define ENGINE_CONTAINERS_STRUCTUREDDATA_MAPPER_HPP_

#include "engine/containers/structuredData/DataNode.hpp"
#include "engine/containers/reflection/Reflection.hpp"
#include <type_traits>
#include <vector>
#include <string>

namespace StructuredData {

template <typename> struct is_vector : std::false_type {};
template <typename T, typename A> struct is_vector<std::vector<T, A>> : 
    std::true_type {};
template <typename T> inline constexpr bool is_vector_v = is_vector<T>::value;

template <typename T>
inline constexpr bool is_primitive_v = 
    std::is_arithmetic_v<T> || std::is_same_v<T, std::string>;

class Mapper {
public:
    template <typename T> static DataNode write(const T& value);
    template <typename T> static bool read(const DataNode&, T& out);
private:
    template <typename T> static DataNode writePrimitive(const T& value);
    template <typename T> static DataNode writeArray(const T& value);
    template <typename T> static DataNode writeEnum(const T& value);
    template <typename T> static DataNode writeObject(const T& value);

    template <typename T> static bool readPrimitive(const DataNode& node, T& out);
    template <typename T> static bool readArray(const DataNode& node, T& out);
    template <typename T> static bool readEnum(const DataNode& node, T& out);
    template <typename T> static bool readObject(const DataNode& node, T& out);
};

template <typename T>
inline DataNode Mapper::write(const T& value) {
    using DecayT = std::decay_t<T>;

    if constexpr (is_primitive_v<DecayT>) return writePrimitive(value);
    else if constexpr (is_vector_v<DecayT>) return writeArray(value);
    else if constexpr (std::is_enum_v<DecayT>) return writeEnum(value);
    else if constexpr (Reflection::Meta<DecayT>::isMapped)
        return writeObject(value);
    else static_assert(false, "Unsupported type");
}

template <typename T>
inline bool Mapper::read(const DataNode& node, T& out) {
    using DecayT = std::decay_t<T>;

    if constexpr (is_primitive_v<DecayT>) return readPrimitive(node, out);
    else if constexpr (is_vector_v<DecayT>) return readArray(node, out);
    else if constexpr (std::is_enum_v<DecayT>) return readEnum(node, out);
    else if constexpr (Reflection::Meta<DecayT>::isMapped)
        return readObject(node, out);
    else static_assert(false, "Unsupported type");
}

template <typename T>
inline DataNode Mapper::writePrimitive(const T& value) {
    if constexpr (std::is_same_v<T, bool>)
        return DataNode(value);
    else if constexpr (std::is_integral_v<T>)
        return DataNode(static_cast<int64_t>(value));
    else if constexpr (std::is_floating_point_v<T>)
        return DataNode(static_cast<double>(value));
    else if constexpr (std::is_same_v<T, std::string>)
        return DataNode(value);
}

template <typename T>
inline DataNode Mapper::writeArray(const T& value) {
    Array arr;
    arr.reserve(value.size());
    for (const auto& item : value) {
        arr.push_back(write(item));
    }
    return DataNode(std::move(arr));
}

template <typename T>
inline DataNode Mapper::writeEnum(const T& value) {
    if constexpr (!Reflection::EnumMeta<T>::isMapped) {
        return writePrimitive(static_cast<std::underlying_type_t<T>>(value));
    }

    std::string result = "unknown";
    
    auto checkPair = [&result, &value](const auto& pair) {
        if (value == pair.second) {
            result = pair.first;
        }
    };

    std::apply([&](const auto&... pairs) {
        (checkPair(pairs), ...);
    }, Reflection::EnumMeta<T>::values());
    
    return DataNode(std::move(result));
}

template <typename T>
inline DataNode Mapper::writeObject(const T& value) {
    Object obj;
    
    auto processField = [&obj, &value](const auto& fieldDef) {
        const auto& val = value.*(fieldDef.member);
        bool isDefault = fieldDef.defaultValue.has_value() && 
            (val == fieldDef.defaultValue.value());
        
        if (fieldDef.required || !isDefault) {
            obj[std::string(fieldDef.name)] = write(val);
        }
    };

    std::apply([&](const auto&... fields) {
        (processField(fields), ...);
    }, Reflection::Meta<T>::fields());
    
    return DataNode(std::move(obj));
}

template <typename T>
inline bool Mapper::readPrimitive(const DataNode& node, T& out) {
    if constexpr (std::is_same_v<T, bool>) {
        if (!node.isBoolean()) return false;
        out = node.asBoolean();
    } 
    else if constexpr (std::is_integral_v<T>) {
        if (!node.isInteger()) return false;
        out = static_cast<T>(node.asInteger());
    } 
    else if constexpr (std::is_floating_point_v<T>) {
        if (!node.isNumber()) return false;
        out = static_cast<T>(node.asNumber());
    } 
    else if constexpr (std::is_same_v<T, std::string>) {
        if (!node.isString()) return false;
        out = node.asString();
    }
    return true;
}

template <typename T>
inline bool Mapper::readArray(const DataNode& node, T& out) {
    if (!node.isArray()) return false;
    
    const auto& arr = node.asArray();
    out.clear();
    out.reserve(arr.size());
    
    bool success = true;
    for (const auto& itemNode : arr) {
        typename std::decay_t<T>::value_type temp{};
        if (!read(itemNode, temp)) {
            success = false;
        }
        out.push_back(std::move(temp));
    }
    return success;
}

template <typename T>
inline bool Mapper::readEnum(const DataNode& node, T& out) {
    if constexpr (!Reflection::EnumMeta<T>::isMapped) {
        std::underlying_type_t<T> val;
        if (readPrimitive(node, val)) {
            out = static_cast<T>(val);
            return true;
        }
        return false;
    }

    if (!node.isString()) return false;
    
    std::string s = node.asString();
    bool found = false;
    
    auto checkPair = [&s, &out, &found](const auto& pair) {
        if (!found && s == pair.first) {
            out = pair.second;
            found = true;
        }
    };

    std::apply([&](const auto&... pairs) {
        (checkPair(pairs), ...);
    }, Reflection::EnumMeta<T>::values());
    
    return found;
}

template <typename T>
inline bool Mapper::readObject(const DataNode& node, T& out) {
    if (!node.isObject()) return false;
    
    const auto& obj = node.asObject();
    bool success = true;

    auto processField = [&obj, &out, &success](const auto& fieldDef) {
        if (const DataNode* child = obj.find(std::string(fieldDef.name))) {
            if (!read(*child, out.*(fieldDef.member))) success = false;
            return;
        }

        if (fieldDef.required) {
            success = false; 
        } else if (fieldDef.defaultValue.has_value()) {
            out.*(fieldDef.member) = fieldDef.defaultValue.value();
        }
    };

    std::apply([&](const auto&... fields) {
        (processField(fields), ...);
    }, Reflection::Meta<T>::fields());
    
    return success;
}

}; // namespace StructuredData

#endif // ENGINE_CONTAINERS_STRUCTUREDDATA_MAPPER_HPP_
