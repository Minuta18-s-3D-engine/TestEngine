#ifndef TESTENG_FIELDREGISTRY_HPP
#define TESTENG_FIELDREGISTRY_HPP

#include <string>

#include "engine/containers/structuredData/DataNode.hpp"

struct FieldDescriptor {
    std::string_view name;
    StructuredData::Type serializedType;

};

class FieldRegistry {
    std::vector<FieldDescriptor> fields;
public:
    FieldRegistry() = default;

    template <typename C, typename T>
    void registerField(std::string_view name, T C::* member);

    [[nodiscard]] const std::vector<FieldDescriptor>& getFields() const;
    [[nodiscard]] std::vector<FieldDescriptor>::iterator* begin() const;
    [[nodiscard]] std::vector<FieldDescriptor>::iterator* end() const;
};

#endif //TESTENG_FIELDREGISTRY_HPP
