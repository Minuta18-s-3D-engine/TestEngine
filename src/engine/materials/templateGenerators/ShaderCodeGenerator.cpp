#include "ShaderCodeGenerator.hpp"

ShaderCodeGenerator::ShaderCodeGenerator(
    ResourceManager& resourceManager_,
    FormattingOptions&& formattingOptions_
) : templateEngine("core://assets/templates", parser),
    resourceManager(&resourceManager_),
    formattingOptions(formattingOptions_) {}

std::string ShaderCodeGenerator::generateIndentString(
    const uint32_t indentLevels
) const {
    return std::string(indentLevels * formattingOptions.indentSize, ' ');
}

std::string ShaderCodeGenerator::getGLSLSamplerType(
    ShaderLayout::SamplerType type
) const {
     switch (type) {
         case ShaderLayout::SamplerType::Texture2D: return "sampler2D";
         case ShaderLayout::SamplerType::CubeMap2D: return "samplerCube";
         default: return "sampler2D /* Type not supported */";
     }
}

std::string ShaderCodeGenerator::getGLSLSamplerName(
    const std::string& name
) {
    return "__u_GeneratedSampler_" + name;
}

std::string ShaderCodeGenerator::generateParamsStruct(
    const ShaderLayout& layout, const uint32_t indentLevels
) const {
    const auto& properties = layout.getProperties();

    std::stringstream result;
    for (const auto& prop : properties) {
        result << generateIndentString(indentLevels);
        result << shader_layout::typeInfo(prop.type).glslName << " ";
        result << prop.name << ";";
    }

    return result.str();
}

std::string ShaderCodeGenerator::generatePropertyUnpack(
    const ShaderLayout::Property& prop
) {

}

std::string ShaderCodeGenerator::generateShaderParams(
    const ShaderLayout& layout
) const {
    TemplateArguments args;
    args.set("shader_properties", generateParamsStruct(layout, 1));
    args.set("sampler_definitions", "");
    args.set("sampler_getters", "");
    args.set("unpack_lines", "");
    return templateEngine.render(
        "shaders/shaderParams.glsl", args
    );
}
