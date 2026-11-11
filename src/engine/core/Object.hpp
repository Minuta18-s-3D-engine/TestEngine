#ifndef ENGINE_CORE_OBJECT_HPP_
#define ENGINE_CORE_OBJECT_HPP_

#include <string>

#include "engine/containers/structuredData/DataNode.hpp"

class Object {
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual std::string getTypeName() const { return "Object"; }

    virtual void serialize(StructuredData::DataNode& out) const;
    virtual void deserialize(const StructuredData::DataNode& in);
protected:
    virtual void onSerializing() const {};
    virtual void onDeserialized() {};
};

#endif // ENGINE_CORE_OBJECT_HPP_
