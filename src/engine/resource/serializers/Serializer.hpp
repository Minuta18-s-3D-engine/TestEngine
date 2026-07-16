#ifndef ENGINE_RESOURCE_SERIALIZERS_SERIALIZER_HPP_
#define ENGINE_RESOURCE_SERIALIZERS_SERIALIZER_HPP_

#include <string>
#include <nlohmann/json.hpp>
#include "engine/containers/structuredData/DataNode.hpp"
#include "engine/project/VirtualPath.hpp"
#include "engine/project/File.hpp"

class Serializer {
public:
    virtual ~Serializer() = default;

    virtual StructuredData::DataNode fromFile(const VirtualPath& path) = 0;
    virtual void toFile(
        const VirtualPath& path, const StructuredData::DataNode& node
    ) = 0;

    virtual StructuredData::DataNode fromString(const std::string& str) = 0;
    virtual std::string toString(const StructuredData::DataNode& node) = 0;
};

#endif // ENGINE_RESOURCE_SERIALIZER_HPP_