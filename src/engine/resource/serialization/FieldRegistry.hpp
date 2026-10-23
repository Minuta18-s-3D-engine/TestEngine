#ifndef ENGINE_RESOURCE_SERIALIZATION_FIELDREGISTRY_HPP
#define ENGINE_RESOURCE_SERIALIZATION_FIELDREGISTRY_HPP

#include <string>

#include "engine/containers/structuredData/DataNode.hpp"

using SerializeFunc = void (*)(
    const void* fieldPtr, StructuredData::DataNode& outNode);
using DeserializeFunc = void (*)(
    void* fieldPtr, const StructuredData::DataNode& inNode);

struct FieldDescriptor {
    std::string_view name;
    size_t offset = 0;
    bool hasDefault = false;

    SerializeFunc serialize = nullptr;
    DeserializeFunc deserialize = nullptr;
};

class FieldRegistry {
    std::vector<FieldDescriptor> fields;

    template <typename C, typename T>
    size_t getMemberOffset(T C::*member) const {
        // FIXME: Technically UB, but should work for almost all popular
        // compilers. DOES NOT work with multiple inheritance. I'll definitely
        // replace this one later, but I don't know what I should put here
        // instead.
        return reinterpret_cast<std::size_t>(
            &(static_cast<C*>(nullptr)->*member)
        );
    }
public:
    FieldRegistry() = default;

    template <typename C, typename T>
    void registerField(std::string_view name, T C::*member);

    [[nodiscard]] const std::vector<FieldDescriptor>& getFields() const;
    [[nodiscard]] std::vector<FieldDescriptor>::iterator begin();
    [[nodiscard]] std::vector<FieldDescriptor>::iterator end();
};

template <typename C, typename T>
void FieldRegistry::registerField(std::string_view name, T C::*member) {
    FieldDescriptor d;
    d.name = name;
    d.offset = getMemberOffset<C, T>(member);
    d.hasDefault = false;

    d.serialize = [](const void* fieldPtr, StructuredData::DataNode& outNode) {
        const T* typedValue = static_cast<const T*>(fieldPtr);
        // TODO
    };
    d.deserialize = [](const void* fieldPtr, StructuredData::DataNode& inNode) {
        T* typedValue = static_cast<T*>(fieldPtr);
        // TODO
    };

    fields.push_back(d);
}

#endif // ENGINE_RESOURCE_SERIALIZATION_FIELDREGISTRY_HPP
