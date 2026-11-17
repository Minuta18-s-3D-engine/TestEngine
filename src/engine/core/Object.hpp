#ifndef ENGINE_CORE_OBJECT_HPP_
#define ENGINE_CORE_OBJECT_HPP_

#include <string>

#include "engine/resource/serialization/ObjectSerializer.hpp"

class Object {
protected:
    friend class ObjectSerializer;

    Object() = default;

    virtual void onSerializing() const {};
    virtual void onDeserialized() {};
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual std::string getTypeName() const { return "Object"; }

    [[nodiscard]] virtual bool canSerialize() const { return true; }
    [[nodiscard]] virtual bool canDeserialize() const { return true; }
};

#endif // ENGINE_CORE_OBJECT_HPP_
