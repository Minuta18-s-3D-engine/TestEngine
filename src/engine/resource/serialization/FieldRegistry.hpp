#ifndef ENGINE_RESOURCE_SERIALIZATION_FIELDREGISTRY_HPP
#define ENGINE_RESOURCE_SERIALIZATION_FIELDREGISTRY_HPP

#include <string>

#include "engine/containers/structuredData/DataNode.hpp"

struct FieldDescriptor {
    std::string_view name;

    bool hasDefault = false;
};

class FieldRegistry {
    std::vector<FieldDescriptor> fields;
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


    fields.push_back(d);
}

#endif // ENGINE_RESOURCE_SERIALIZATION_FIELDREGISTRY_HPP
