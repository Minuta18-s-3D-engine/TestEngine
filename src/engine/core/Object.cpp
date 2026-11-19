#include "Object.hpp"

const FieldRegistry& Object::getStaticFieldRegistry() {
    static const FieldRegistry objectRegistry;
    return objectRegistry;
}

const FieldRegistry& Object::getFieldRegistry() const {
    return getStaticFieldRegistry();
}
