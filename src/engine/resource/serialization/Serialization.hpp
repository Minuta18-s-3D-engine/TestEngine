#ifndef ENGINE_RESOURCE_SERIALIZATION_SERIALIZATION_HPP
#define ENGINE_RESOURCE_SERIALIZATION_SERIALIZATION_HPP

#include "TypeSerialization.hpp"
#include "FieldRegistry.hpp"
#include "EnumTraits.hpp"

#define SERIALIZE_FIELD(fieldName) \
    registry.registerField(#fieldName, &StructType::fieldName)

#define SERIALIZE_FIELD_DEFAULT(fieldName, defaultValue) \
    registry.registerField(#fieldName, &StructType::fieldName, defaultValue)

#define GENERATE_SERIALIZATION(StructType, ...) \
    static const FieldRegistry& reflect() { \
        static FieldRegistry registry; \
        static bool initialized = false; \
        if (!initialized) { \
            __VA_ARGS__; \
            initialized = true; \
        } \
        return registry; \
    }

#endif // ENGINE_RESOURCE_SERIALIZATION_SERIALIZATION_HPP
