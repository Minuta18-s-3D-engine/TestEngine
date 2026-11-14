#include "Resource.hpp"

#include "engine/project/File.hpp"
#include "engine/utils/exc/ImporterExceptions.hpp"
#include "serializers/Serializer.hpp"

[[nodiscard]] VirtualPath Resource::getMetaPath() const {
    return { path.resolve() + ".meta" };
}

[[nodiscard]] StructuredData::DataNode Resource::readMetaNode(
    Serializer& serializer, const bool isRequired
) const {
    try {
        File metaFile(getMetaPath());
        return serializer.fromString(metaFile.readAll());
    } catch (const exc::file_not_found& e) {
        if (isRequired) {
            throw exc::importer_exceptions::meta_file_not_found(e.what());
        }
        return StructuredData::DataNode(StructuredData::Object());
    } catch (const std::exception& e) {
        throw exc::importer_exceptions::importer_exception(e.what());
    }
}
