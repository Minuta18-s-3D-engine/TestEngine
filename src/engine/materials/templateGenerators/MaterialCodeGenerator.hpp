#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_MATERIALCODEGENRATOR_H_
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_MATERIALCODEGENRATOR_H_

#include "../../stringProcessing/templateEngine/TemplateEngine.hpp"
#include "../../stringProcessing/templateEngine/TemplateParser.hpp"
#include "../Material.hpp"

class MaterialCodeGenerator {
    TemplateParser parser;
    TemplateEngine templateEngine;
public:
    MaterialCodeGenerator();

    std::string generateMaterialDefinition(const Material& material) const;
    std::string generateFullShader(const Material& material) const;
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERGENRATOR_H_
