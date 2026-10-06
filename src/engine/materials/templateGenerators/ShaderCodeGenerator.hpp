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
public:
    struct FormattingOptions {
        uint32_t indentSize = 4;
    };
private:
    TemplateParser parser;
    TemplateEngine templateEngine;

    ResourceManager* resourceManager;
    FormattingOptions formattingOptions;

    [[nodiscard]] std::string generateIndentString(uint32_t indentLevels) const;

    [[nodiscard]] std::string getGLSLSamplerType(
        ShaderLayout::SamplerType type) const;
    [[nodiscard]] std::string getGLSLSamplerName(const std::string& name) const;

    [[nodiscard]] std::string generateParamsStruct(
        const ShaderLayout& layout, uint32_t indentLevels) const;

    [[nodiscard]] std::string generateSamplerUniforms(
        const ShaderLayout& layout) const;
    [[nodiscard]] std::string generateSamplerGetters(
        const ShaderLayout& layout) const;

    [[nodiscard]] std::string generatePropertyUnpack(
        const ShaderLayout::Property& prop, uint32_t inArrayOffset, 
        uint32_t indentLevels
    ) const;
    [[nodiscard]] std::string generateUnpack(
        const ShaderLayout& layout) const;

    [[nodiscard]] std::string generateCommentMessage() const;
public:
    ShaderCodeGenerator(
        ResourceManager& resourceManager_,
        FormattingOptions&& formattingOptions_
    );

    [[nodiscard]] std::string generateShaderParams(
        const ShaderLayout& layout
    ) const;

    [[nodiscard]] std::string generateShader(
        const ShaderLayout& layout,
        const std::string& userCode,
        const std::string& userFunc,
        bool loadParams = true 
    ) const;
};

namespace ShaderCodeGeneratorData {

struct UnpackInfo {
    std::string unpackFunc = "(";
    std::string vecFunc = "(";
    uint32_t startOffset = 0;
    uint32_t endOffset = 0;
    bool customIndexesUsed = false;
    std::vector<uint32_t> customIndexes = {};
};

const inline std::vector<UnpackInfo> typesUnpackInfo = {
    { "uintBitsToFloat(", "(", 0, 0, false, {} },
    { "int(", "(", 0, 0, false, {} },
    { "uint(", "(", 0, 0, false, {} },
    { "(0U != ", "(", 0, 0, false, {} },
    { "uintBitsToFloat(", "vec2(", 0, 1, false, {} },
    { "int(", "ivec2(", 0, 1, false, {} },
    { "uint(", "uvec2(", 0, 1, false, {} },
    { "uintBitsToFloat(", "vec3(", 0, 2, false, {} },
    { "int(", "ivec3(", 0, 2, false, {} },
    { "uint(", "uvec3(", 0, 2, false, {} },
    { "uintBitsToFloat(", "vec4(", 0, 3, false, {} },
    { "int(", "ivec4(", 0, 3, false, {} },
    { "uint(", "uvec4(", 0, 3, false, {} },
    { "uintBitsToFloat(", "mat2(", 0, 3, false, {} },
    { "uintBitsToFloat(", "mat3(", 0, 0, true, {0, 1, 2, 4, 5, 6, 8, 9, 10} },
    { "uintBitsToFloat(", "mat4(", 0, 15, false, {} }
};

};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_SHADERCODEGENRATOR_H_
