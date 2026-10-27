#ifndef ENGINE_RESOURCE_SERIALIZATION_OBJECTSERIALER_HPP
#define ENGINE_RESOURCE_SERIALIZATION_OBJECTSERIALER_HPP

#include "engine/containers/structuredData/DataNode.hpp"
#include "FieldRegistry.hpp"

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

        const bool present = fieldNode &&
            !(field.hasDefault && fieldNode->isNull());

        if (!present && !field.hasDefault) {
            throw std::runtime_error(
                "Missing required field: " + std::string(field.name));
        }

        const StructuredData::DataNode& source =
            present ? *fieldNode : field.defaultValue;

        try {
            field.deserialize(fieldPtr, source);
        } catch (const std::exception& e) {
            throw std::runtime_error(
                std::string(field.name) + ": " + e.what()
            );
        }
    }
}

template <Serializable T>
inline void TypeSerialization::serializeField(
    StructuredData::DataNode& out, const T& value
) {
    out = ObjectSerializer::serialize(value);
}

template <Serializable T>
inline void TypeSerialization::deserializeField(
    const StructuredData::DataNode& in, T& value
) {
    ObjectSerializer::deserialize(in, value);
}

#endif // ENGINE_RESOURCE_SERIALIZATION_OBJECTSERIALER_HPP
