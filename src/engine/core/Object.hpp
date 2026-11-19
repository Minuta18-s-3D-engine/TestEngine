#ifndef ENGINE_CORE_OBJECT_HPP_
#define ENGINE_CORE_OBJECT_HPP_

#include <string>

#include "engine/resource/serialization/FieldRegistry.hpp"

// Functionality will be added a little later
#define OBJECT_BODY \
public: \
\
private:

class Object {
protected:
    Object() = default;

    virtual void onBeforeSerialize() {};
    virtual void onAfterDeserialize() {};
public:
    virtual ~Object() = default;

    [[nodiscard]] virtual std::string getTypeName() const { return "Object"; }

    static const FieldRegistry& getStaticFieldRegistry();
    [[nodiscard]] virtual const FieldRegistry& getFieldRegistry() const;
};

#endif // ENGINE_CORE_OBJECT_HPP_
