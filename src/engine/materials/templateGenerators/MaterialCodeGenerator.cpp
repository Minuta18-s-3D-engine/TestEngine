#include "MaterialCodeGenerator.hpp"

MaterialCodeGenerator::MaterialCodeGenerator() : 
    templateEngine("core://templates", parser) {}

std::string MaterialCodeGenerator::generateMaterialDefinition(
    const Material& material
) const {
    
}

std::string MaterialCodeGenerator::generateFullShader(
    const Material& material
) const {
    
}
