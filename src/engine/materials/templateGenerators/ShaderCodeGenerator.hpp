#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_

#include <string>
#include <sstream>

#include "../../stringProcessing/templateEngine/TemplateEngine.hpp"
#include "../../stringProcessing/templateEngine/TemplateParser.hpp"
#include "../Material.hpp"

class ShaderCodeGenerator {
    TemplateParser parser;
    TemplateEngine templateEngine;

    std::string generateEngineGlobals() const;
    std::string generateMaterialProperties(
        const Material& mat, uint32_t indent = 4
    ) const;
    std::string generateMaterialUnpacker(
        const Material& mat, uint32_t indent = 4
    ) const;
    std::string getGLSLSamplerName(const std::string& name) const;
    std::string getGLSLSamplerType(SamplerType type) const;
    std::string generateSamplerDefinitions(const Material& mat) const;
    std::string generateSamplerGetter(
        const Material& mat, const std::string& name
    ) const;
    std::string generateSamplerGetters(const Material& mat) const;
public:
    ShaderCodeGenerator();

    
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
