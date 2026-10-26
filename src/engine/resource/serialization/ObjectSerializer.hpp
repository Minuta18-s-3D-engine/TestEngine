#ifndef ENGINE_RESOURCE_SERIALIZATION_OBJECTSERIALER_HPP
#define ENGINE_RESOURCE_SERIALIZATION_OBJECTSERIALER_HPP

#include <concepts>

#include "engine/containers/structuredData/DataNode.hpp"
#include "FieldRegistry.hpp"

template <typename T>
concept Serializable = requires {
    { T::reflect() } -> std::same_as<const FieldRegistry&>;
};

class ObjectSerializer {
public:
    template <Serializable T>
    [[nodiscard]] static StructuredData::DataNode serialize(const T& instance);

    template <Serializable T>
    static void deserialize(const StructuredData::DataNode& node, T& instance);
};

template<Serializable T>
StructuredData::DataNode ObjectSerializer::serialize(const T& instance) {
    const FieldRegistry& registry = T::reflect();

    StructuredData::Object objData;
    for (const auto& field : registry.getFields()) {
        StructuredData::DataNode fieldNode;
        const void* fieldPtr = registry.getMemberPtr<T>(instance, field.offset);
        field.serialize(fieldPtr, fieldNode);
        objData[std::string(field.name)] = std::move(fieldNode);
    }

    return StructuredData::DataNode(std::move(objData));
}

template<Serializable T>
void ObjectSerializer::deserialize(
    const StructuredData::DataNode &node, T& instance
) {
    if (!node.isObject()) {
        throw std::runtime_error(
            "Failed to deserialize: root node is not an object");
    }

    const FieldRegistry& registry = T::reflect();
    const auto& objData = node.asObject();

    for (const auto& field : registry.getFields()) {
        void* fieldPtr = registry.getMemberPtr<T>(instance, field.offset);
        const StructuredData::DataNode* fieldNode =
            objData.find(std::string(field.name));

        if (fieldNode) {
            field.deserialize(fieldPtr, *fieldNode);
        } else if (field.hasDefault) {
            field.deserialize(fieldPtr, field.defaultValue);
        } else {
            throw std::runtime_error(
                "Missing required field: " + std::string(field.name));
        }
    }
}

#endif // ENGINE_RESOURCE_SERIALIZATION_OBJECTSERIALER_HPP
