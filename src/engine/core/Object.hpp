#ifndef ENGINE_CORE_OBJECT_HPP_
#define ENGINE_CORE_OBJECT_HPP_

#include <string>

#include "engine/containers/structuredData/DataNode.hpp"

class Object {
protected:
    Object() = default;

    virtual void onSerializing() const {};
    virtual void onDeserialized() {};
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual std::string getTypeName() const { return "Object"; }

    [[nodiscard]] virtual bool canSerialize() const { return true; }
    [[nodiscard]] virtual bool canDeserialize() const { return true; }

    virtual void serialize(StructuredData::DataNode& out) const;
    virtual void deserialize(const StructuredData::DataNode& in);
};

#endif // ENGINE_CORE_OBJECT_HPP_
