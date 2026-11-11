#include "Object.hpp"

void Object::serialize(StructuredData::DataNode &out) const {
    onSerializing();
}

void Object::deserialize(const StructuredData::DataNode &in) {
    onDeserialized();
}
