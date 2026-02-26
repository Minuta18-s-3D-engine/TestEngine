#include "ShaderGenerator.hpp"

ShaderGenerator::ShaderGenerator() : 
    templateEngine("core://templates", parser), 
    geomShaderGenerator(templateEngine) {}

std::string ShaderGenerator::generateGeomShader(
    const Material& material
) const {
    return geomShaderGenerator.generate(material);
}
