#ifndef ENGINE_RESOURCE_IMPORTERS_TEXTUREIMPORTER_HPP_
#define ENGINE_RESOURCE_IMPORTERS_TEXTUREIMPORTER_HPP_

#include "engine/resource/ResourceImporter.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/debug/logging/Logging.hpp"
#include "engine/project/VirtualPath.hpp"

class TextureImporter : public ResourceImporter<Texture> {
    Logger logger;

    Texture loadTexture(const VirtualPath& path, ResourceManager& manager);
public:
    explicit TextureImporter(Serializer& _serializer);

    Texture import(const VirtualPath& path, ResourceManager& manager) override;
};

#endif // ENGINE_RESOURCE_IMPORTERS_TEXTUREIMPORTER_HPP_
