#ifndef ENGINE_RESOURCE_JSONSERIALIZER_HPP_
#define ENGINE_RESOURCE_JSONSERIALIZER_HPP_

#include "Serializer.hpp"
#include <nlohmann/json_fwd.hpp>

class JsonSerializer final : public Serializer {
public:
    struct Options {
        bool minimize = false;
        int indent = 4;
    };
private:
    Options currentOptions;
public:
    StructuredData::DataNode fromFile(const VirtualPath& path) override;
    void toFile(
        const VirtualPath& path, const StructuredData::DataNode& node
    ) override;
    
    StructuredData::DataNode fromString(const std::string& str) override;
    std::string toString(const StructuredData::DataNode& node) override;

    StructuredData::DataNode fromJson(const nlohmann::json& json);
    nlohmann::json toJson(const StructuredData::DataNode& node);

    void setOptions(const Options& options) { currentOptions = options; };
};

#endif // ENGINE_RESOURCE_JSONSERIALIZER_HPP_