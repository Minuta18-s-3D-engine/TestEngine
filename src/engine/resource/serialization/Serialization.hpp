#ifndef ENGINE_RESOURCE_SERIALIZATION_SERIALIZATION_HPP
#define ENGINE_RESOURCE_SERIALIZATION_SERIALIZATION_HPP

#include "TypeSerialization.hpp"
#include "FieldRegistry.hpp"
#include "EnumTraits.hpp"

#define SERIALIZE_FIELD(fieldName) \
    registry.registerField(#fieldName, &StructType::fieldName)

#define SERIALIZE_FIELD_DEFAULT(fieldName, defaultValue) \
    registry.registerField(#fieldName, &StructType::fieldName, defaultValue)

#define GENERATE_SERIALIZATION(Type, ...) \
    static const FieldRegistry& reflect() { \
        static const FieldRegistry instance = [] { \
            using StructType = Type; \
            FieldRegistry registry; \
            __VA_ARGS__; \
            return registry; \
        }(); \
        return instance; \
    }

#endif // ENGINE_RESOURCE_SERIALIZATION_SERIALIZATION_HPP
