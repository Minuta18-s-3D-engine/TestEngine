#include "FieldRegistry.hpp"

const std::vector<FieldDescriptor>& FieldRegistry::getFields() const {
    return fields;
}

std::vector<FieldDescriptor>::iterator FieldRegistry::begin() {
    return fields.begin();
}

std::vector<FieldDescriptor>::iterator FieldRegistry::end() {
    return fields.end();
}
