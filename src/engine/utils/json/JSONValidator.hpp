#ifndef ENGINE_UTILS_JSON_JSONVALIDATOR_HPP_
#define ENGINE_UTILS_JSON_JSONVALIDATOR_HPP_

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

#include "engine/resource/Resource.hpp"
#include "engine/resource/ResourceHandle.hpp"

class JSONValidator : public Resource {
    nlohmann::json schema;
public:
    JSONValidator(
        VirtualPath _path, 
        nlohmann::json _schema
    );

    void validate(const nlohmann::json& data);
};

#endif // ENGINE_UTILS_JSON_JSONVALIDATOR_HPP_
