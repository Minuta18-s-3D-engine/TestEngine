#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_

#include <glad/glad.h>

#include <string>
#include <sstream>
#include <iostream>

#include "../../stringProcessing/templateEngine/TemplateEngine.hpp"
#include "../../stringProcessing/templateEngine/TemplateParser.hpp"
#include "../Material.hpp"

class ShaderCodeGenerator {
    TemplateParser parser;
    TemplateEngine templateEngine;

    std::string getIndentStr(uint32_t indent) const;
    std::string generateBase(uint32_t idx) const;
    std::string unpackInt(const std::string& base) const;
    std::string unpackUint(const std::string& base) const;
    std::string unpackInt64(int idx) const;
    std::string unpackUint64(int idx) const;
    std::string unpackFloat(const std::string& base) const;
    std::string unpackBool(const std::string& base) const;
    std::string unpackVec(
        const std::string& type, const std::vector<std::string>& values
    ) const;
    std::string unpackVecIndexes(
        const std::string& type, const std::vector<int>& values,
        const std::string& wrapFunc = ""
    ) const;
    std::string unpackVecIndexes(
        const std::string& type, int minValue, int maxValue,
        const std::string& wrapFunc = ""
    ) const;
    std::string generateMaterialUnpacker(
        const Material& mat, const std::string& prop, uint32_t indent = 4
    ) const;
    std::string generateUnpackerFunc(const Material& mat) const;

    std::string generateEngineGlobals() const;
    std::string generateMaterialProperties(
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

    std::string generateMaterialDefinition(const Material& mat) const;
    std::string generateShader(
        const Material& mat, const std::string& userCode, 
        const std::string& userFunc
    ) const;

    std::string generateCompShader(
        const std::string& userCode, const std::string& userFunc
    ) const;
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
