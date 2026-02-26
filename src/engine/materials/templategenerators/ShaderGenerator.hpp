#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERGENRATOR_H_
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERGENRATOR_H_

#include "../../stringprocessing/templateengine/TemplateEngine.hpp"
#include "../../stringprocessing/templateengine/TemplateParser.hpp"
#include "../../project/VirtualPath.hpp"
#include "GeomShaderGenerator.hpp"

class ShaderGenerator {
    TemplateParser parser;
    TemplateEngine templateEngine;
    GeomShaderGenerator geomShaderGenerator;
public:
    ShaderGenerator();

    std::string generateGeomShader(const Material& material) const;
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERGENRATOR_H_
