#ifndef ENGINE_CORE_OBJECT_HPP_
#define ENGINE_CORE_OBJECT_HPP_

#include <string>

#include "engine/resource/serialization/ObjectSerializer.hpp"

class Object {
protected:
    friend class ObjectSerializer;

    Object() = default;
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual std::string getTypeName() const { return "Object"; }

    static const FieldRegistry& getStaticFieldRegistry();
    [[nodiscard]] virtual const FieldRegistry& getFieldRegistry() const;
};

#endif // ENGINE_CORE_OBJECT_HPP_
