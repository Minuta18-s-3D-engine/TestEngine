#include "TextureImporter.hpp"

TextureImporter::TextureImporter(Serializer* _serializer) 
  : logger(Logging::createLogger("engine.resource.importers.textureImporter")),
    ResourceImporter(_serializer) {
    
}

Texture TextureImporter::loadTexture(
    const VirtualPath& path, ResourceManager& manager
) {
    logger.info("Loading texture: {}...", path.resolve());

    StructuredData::DataNode metaContents = parseMetaFile(path);
 
    TextureSchema schema;
    StructuredData::Mapper::read<TextureSchema>(metaContents, schema);
    
    File textureFile(path);
    auto textureFileContent = textureFile.readAllBytes();
    
    ImageType imgFormat = getImageType(
        textureFileContent.data(), textureFileContent.size());
    std::shared_ptr<ImageData> image = nullptr;

    if (imgFormat == ImageType::PNG) {
        image = PngCoder::load_image(
            textureFileContent.data(), textureFileContent.size(), 
            path.getVirtual() 
        );
    } else if (imgFormat == ImageType::JPG) {
        image = JpgCoder::load_image(
            textureFileContent.data(), textureFileContent.size(), 
            path.getVirtual() 
        );
    } else {
        throw exc::importer_exceptions::resource_invalid(
            "File format not supported. Use .png or .jpg"
        );
    }

    logger.info("Texture {} loaded successfully", path.resolve());

    return {*image, schema, SamplerType::Texture2D};
}

Texture TextureImporter::import(
    const VirtualPath& path, ResourceManager& manager
) {
    try {
        return loadTexture(path, manager);
    } catch (exc::importer_exceptions::importer_exception& e) {
        throw e;
    } catch (std::exception& e) {
        throw exc::importer_exceptions::importer_exception(e.what());
    }
}
