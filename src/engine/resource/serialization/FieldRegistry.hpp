#ifndef TESTENG_FIELDREGISTRY_HPP
#define TESTENG_FIELDREGISTRY_HPP

#include <string>

#include "engine/containers/structuredData/DataNode.hpp"

struct FieldDescriptor {
    std::string_view name;
    uint32_t memberOffset;

    StructuredData::DataNode (*toNode) (const void*);
    void (*fromNode) (void*, const StructuredData::DataNode&);
};

class FieldRegistry {
    std::vector<FieldDescriptor> fields;

    template <typename C, typename T>
    uint32_t getMemberOffset(T C::*member);
public:
    FieldRegistry() = default;

    template <typename C, typename T>
    void registerField(std::string_view name, T C::*member);

    [[nodiscard]] const std::vector<FieldDescriptor>& getFields() const;
    [[nodiscard]] std::vector<FieldDescriptor>::iterator begin();
    [[nodiscard]] std::vector<FieldDescriptor>::iterator end();
};

template<typename C, typename T>
uint32_t FieldRegistry::getMemberOffset(T C::*member) {
    const C* base = nullptr;
    return static_cast<uint32_t>(
        reinterpret_cast<const char*>(&(base->*member))
      - reinterpret_cast<const char*>(base)
    );
}

template <typename C, typename T>
void FieldRegistry::registerField(std::string_view name, T C::*member) {
    FieldDescriptor d;
    d.name = name;
    d.memberOffset = getMemberOffset<C, T>(member);
    d.toNode = [](const void* p) -> StructuredData::DataNode {
        // TODO
    };
    d.fromNode = [](void* p, const StructuredData::DataNode& n) {
        // TODO
    };
    fields.push_back(d);
}

#endif //TESTENG_FIELDREGISTRY_HPP
