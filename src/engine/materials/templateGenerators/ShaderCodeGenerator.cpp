#include "ShaderCodeGenerator.hpp"

#include <numeric>

#include "engine/resource/ResourceManager.hpp"
#include "engine/graphics/Shader.hpp"
#include "cmakeConfig.h"

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
) const {
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
        result << prop.name << ";\n";
    }
    return result.str();
}

std::string ShaderCodeGenerator::generateShaderParamsStruct(
    const ShaderLayout &layout, uint32_t indentLevels
) const {
    TemplateArguments args;
    args.set("shader_properties", generateParamsStruct(
        layout, indentLevels
    ));
    return templateEngine.render("shaders/shaderParamsStruct.glsl", args);
}

std::string ShaderCodeGenerator::generateSamplerUniforms(
    const ShaderLayout &layout
) const {
    std::stringstream result;

    for (const auto& sampler : layout.getSamplers()) {
        result << "uniform " << getGLSLSamplerType(sampler.type) << " "
            << getGLSLSamplerName(sampler.name) << ";\n";
    }

    return result.str();
}

std::string ShaderCodeGenerator::generateSamplerGetters(
    const ShaderLayout& layout
) const {
    std::stringstream result;

    for (const auto& sampler : layout.getSamplers()) {
        if (layout.isBindless()) {
            result << "#define get_" << sampler.name
                << "() (sampler2D(shaderParams." << sampler.name << "))\n";
        } else {
            result << "#define get_" << sampler.name << "() ("
                << getGLSLSamplerName(sampler.name) << ")\n";
        }
    }

    return result.str();
}

std::string ShaderCodeGenerator::generatePropertyUnpack(
    const ShaderLayout::Property& prop,
    const uint32_t inArrayOffset,
    const uint32_t indentLevels
) const {
    const auto& info =
        ShaderCodeGeneratorData::typesUnpackInfo[static_cast<size_t>(prop.type)];
    std::stringstream converterFunc;
    converterFunc << info.vecFunc;
    
    std::vector<uint32_t> indexes;
    if (info.customIndexesUsed) {
        indexes = info.customIndexes;
    } else {
        indexes.resize(info.endOffset - info.startOffset + 1);
        std::iota(indexes.begin(), indexes.end(), 0);
    }

    for (uint32_t i = info.startOffset; i <= info.endOffset; ++i) {
        converterFunc << info.unpackFunc << "b_MaterialData[base + "
            << indexes[i - info.startOffset] + inArrayOffset << "])";
        if (i < info.endOffset) converterFunc << ", ";
    }

    std::stringstream result;
    result << generateIndentString(indentLevels) << "shaderParams."
        << prop.name << " = " << converterFunc.str() << ");\n";

    return result.str();
}

std::string ShaderCodeGenerator::generateUnpack(
    const ShaderLayout& layout
) const {
    std::stringstream result;
    for (const auto& prop : layout.getProperties()) {
        result << generatePropertyUnpack(
            prop, prop.offset / 4, 1
        );
    }

    return result.str();
}

std::string ShaderCodeGenerator::generateCommentMessage() const {
    std::stringstream result;
    result << "Engine version: " << PROJECT_VERSION;
    return result.str();
}

std::string ShaderCodeGenerator::generateShaderParams(
    const ShaderLayout& layout
) const {
    TemplateArguments args;
    if (!layout.getProperties().empty()) {
        args.set(
            "shader_params_struct",
            generateShaderParamsStruct(layout, 1)
        );
    } else {
        args.set(
            "shader_params_struct",
            "// Structure generation omitted due to its emptiness."
        );
    }
    if (layout.isBindless()) {
        args.set("sampler_uniforms", "// Bindless mode enabled.");
    } else {
        args.set("sampler_uniforms", generateSamplerUniforms(layout));
    }
    args.set("sampler_getters", generateSamplerGetters(layout));
    args.set("unpack_lines", generateUnpack(layout));
    return templateEngine.render(
        "shaders/shaderParams.glsl", args
    );
}

std::string ShaderCodeGenerator::generateShader(
    const ShaderLayout& layout,
    const std::string& userCode,
    const std::string& userFunc,
    const bool loadParams
) const {
    TemplateArguments engineGlobalsArgs;
    engineGlobalsArgs.set(
        "shader_params", generateShaderParams(layout)
    );
    std::string engineGlobals = templateEngine.render(
        "shaders/components/engineGlobals.glsl", engineGlobalsArgs
    );

    TemplateArguments headerArgs;
    headerArgs.set("user_func", userFunc);
    std::string header;
    if (loadParams) {
        header = templateEngine.render(
            "shaders/headers/loaderHeader.glsl", 
            headerArgs
        );
    } else {
        header = templateEngine.render(
            "shaders/headers/emptyHeader.glsl", 
            headerArgs
        );
    }

    TemplateArguments args;
    args.set("message", generateCommentMessage());
    args.set("engine_globals", engineGlobals);
    args.set("user_code", userCode);
    args.set("generated_header", header);

    return templateEngine.render("shaders/shaderTemplate.glsl", args);
}
