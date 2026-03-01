#include "GeomShaderGenerator.hpp"

GeomShaderGenerator::GeomShaderGenerator(
    const TemplateEngine& _engine
) : templateEngine(_engine) {}

std::string GeomShaderGenerator::generateMaterialDefinition(
    const Material& material
) const {
    TemplateArguments args;
    args.set("material_name", material.getName());
    
    std::string propertiesStr;
    const TypedPropertyStorage& properties = material.getPropertyStorage();
    auto propertyMap = properties.getProperties();
    size_t mappedCnt = 0;
    for (auto [ propertyName, property ] : propertyMap) {
        propertiesStr += "    " 
            + TypedPropertyStorage::getGLSLType(property.type);
        propertiesStr += " " + propertyName + ";";

        mappedCnt++;
        if (mappedCnt < propertyMap.size()) propertiesStr += "\n";
    }
    args.set("material_properties", propertiesStr);

    std::string samplersStr;
    auto textureMap = material.getTextureStorage();
    mappedCnt = 0;
    for (auto [ textureName, texturePtr ] : textureMap) {
        samplersStr += "uniform sampler2D " + textureName + ";";
        mappedCnt++;
        if (mappedCnt < textureMap.size()) samplersStr += "\n";
    }
    args.set("samplers_definitions", samplersStr);
    
    return templateEngine.render("shaders/materialDefinition.glsl", args);
}

std::string GeomShaderGenerator::generate(
    const Material& material
) const {
    TemplateArguments args;
    args.set("material_name", material.getName());
    args.set("material_definition", generateMaterialDefinition(material));
    return templateEngine.render("shaders/userGeomShader.glsl", args);
}
