#ifndef ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_
#define ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_

#include <memory>

#include "engine/containers/structuredData/DataNode.hpp"
#include "engine/project/VirtualPath.hpp"
#include "engine/project/File.hpp"
#include "engine/containers/structuredData/Mapper.hpp"
#include "engine/resource/serializers/Serializer.hpp"
#include "engine/utils/exc/GeneralExceptions.hpp" 
#include "engine/utils/exc/ImporterExceptions.hpp"

class ResourceManager;

class IResourceImporter {
public:
    virtual ~IResourceImporter() = default;
};

template <typename T>
class ResourceImporter : public IResourceImporter {
protected:
    VirtualPath constructMetaPath(const VirtualPath& basePath);

    StructuredData::DataNode parseMetaFile(
        const VirtualPath& basePath, bool isMetaRequired = false
    );

    Serializer* serializer;
public:
    explicit ResourceImporter(Serializer& serializer_) : serializer(&serializer_) {}

    virtual ~ResourceImporter() = default;

    virtual T import(
        const VirtualPath& path, ResourceManager& manager
    ) = 0;
};

template <typename T>
VirtualPath ResourceImporter<T>::constructMetaPath(const VirtualPath& basePath) {
    return { basePath.resolve() + ".meta" };
}

template <typename T>
StructuredData::DataNode ResourceImporter<T>::parseMetaFile(
    const VirtualPath& basePath, bool isMetaRequired
) {
    StructuredData::DataNode emptyNode{StructuredData::Object()};

    VirtualPath metaPath = constructMetaPath(basePath);

    try {
        File metaFile(metaPath);
        std::string contents = metaFile.readAll();
        auto metaNode = serializer->fromString(contents);
        return metaNode;
    } catch (exc::file_not_found& e) {
        if (isMetaRequired)
            throw exc::importer_exceptions::meta_file_not_found(e.what());
        return emptyNode;
    } catch (std::exception& e) {
        throw exc::importer_exceptions::importer_exception(e.what());
    }
}


#endif // ENGINE_RESOURCE_RESOURCEIMPORTER_HPP_
