#include "JSONValidator.hpp"

JSONValidator::JSONValidator(
    VirtualPath _path, 
    nlohmann::json _schema
) : Resource(_path), schema(std::move(_schema)) {}

void JSONValidator::validate(const nlohmann::json& data) {
    
}
