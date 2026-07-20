#ifndef ENGINE_RESOURCE_IMPORTERS_TEXTUREIMPORTER_HPP_
#define ENGINE_RESOURCE_IMPORTERS_TEXTUREIMPORTER_HPP_

#include <string>

#include "TextureSchema.hpp"
#include "engine/resource/ResourceImporter.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/debug/logging/Logging.hpp"
#include "engine/project/VirtualPath.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/containers/structuredData/DataNode.hpp"
#include "engine/containers/structuredData/Mapper.hpp"
#include "engine/resource/utils/ImageData.hpp"
#include "engine/resource/utils/Files.hpp"
#include "engine/resource/coders/images/PngCoder.hpp"
#include "engine/resource/coders/images/JpgCoder.hpp"

class TextureImporter : public ResourceImporter<Texture> {
    Logger logger;

    Texture loadTexture(const VirtualPath& path, ResourceManager& manager);
public:
    TextureImporter(Serializer* _serializer);

    Texture import(const VirtualPath& path, ResourceManager& manager) override;
};

#endif // ENGINE_RESOURCE_IMPORTERS_TEXTUREIMPORTER_HPP_
