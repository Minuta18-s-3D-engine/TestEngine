#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_

#include <string>
#include <sstream>

#include "../../stringProcessing/templateEngine/TemplateEngine.hpp"
#include "../../stringProcessing/templateEngine/TemplateParser.hpp"
#include "engine/graphics/ShaderLayout.hpp"
#include "engine/resource/ResourceHandle.hpp"

class ResourceManager;
class Shader;

class ShaderCodeGenerator {
    TemplateParser parser;
    TemplateEngine templateEngine;

    ResourceManager* resourceManager;

    struct FormattingOptions {
        uint32_t indentSize = 4;
    };

    FormattingOptions formattingOptions;

    [[nodiscard]] std::string generateIndentString(uint32_t indentLevels) const;

    [[nodiscard]] std::string getGLSLSamplerType(
        ShaderLayout::SamplerType type) const;
    [[nodiscard]] std::string getGLSLSamplerName(const std::string& name);

    [[nodiscard]] std::string generatePropertyUnpack(
        const ShaderLayout::Property& prop);
    [[nodiscard]] std::string generateUnpack(
        const ShaderLayout& layout);

    [[nodiscard]] std::string generateParamsStruct(
        const ShaderLayout& layout, uint32_t indentLevels);

    [[nodiscard]] std::string generateSamplerUniforms(
        const ShaderLayout& layout) const;
    [[nodiscard]] std::string generateSamplerGetters(
        const ShaderLayout& layout) const;
public:
    ShaderCodeGenerator(
        ResourceManager& resourceManager_,
        FormattingOptions&& formattingOptions_
    );

    [[nodiscard]] std::string generateShaderParams(
        const ShaderLayout& layout
    ) const;

    [[nodiscard]] std::string generateShader(
        ResourceHandle<Shader> shader,
        std::string userCode
    ) const;
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
