#ifndef ENGINE_CORE_RESOURCE_HPP_
#define ENGINE_CORE_RESOURCE_HPP_

#include "engine/core/Object.hpp"
#include "engine/containers/structuredData/DataNode.hpp"
#include "engine/project/VirtualPath.hpp"

class Serializer;

class Resource : public Object {
protected:
    VirtualPath path;

    Resource() = default;

    [[nodiscard]] VirtualPath getMetaPath() const;
    [[nodiscard]] StructuredData::DataNode readMetaNode(
        Serializer& serializer, bool isRequired = false
    ) const;
public:
    ~Resource() override = default;

    [[nodiscard]] std::string getTypeName() const override { return "Resource"; };

    [[nodiscard]] const VirtualPath& getPath() const { return path; }
};

#endif // ENGINE_CORE_RESOURCE_HPP_
