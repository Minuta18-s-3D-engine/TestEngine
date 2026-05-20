#include "ShaderCodeGenerator.hpp"

ShaderCodeGenerator::ShaderCodeGenerator() : 
    templateEngine("core://templates", parser) {}

std::string ShaderCodeGenerator::generateEngineGlobals() const {
    return templateEngine.render(
        "core://templates/shaders/components/engineGlobals.glsl"
    );
}

std::string ShaderCodeGenerator::generateMaterialProperties(
    const Material& mat, uint32_t indent = 4
) const {
    const auto& layout = mat.getLayout();
    const auto& props = layout.getPropertyOrder();
    
    std::string indentStr = "";
    for (int i = 0; i < indent; ++i) indentStr += " ";

    std::stringstream result;
    for (const std::string& name : props) {
        const auto& propertyInfo = layout.getPropertyInfo(name);

        result << indentStr;
        result << MaterialLayout::getGLSLType(propertyInfo.type) << " ";
        result << name + ";\n";
    }

    return result.str();
}

std::string ShaderCodeGenerator::generateMaterialUnpacker(
    const Material& mat, uint32_t indent = 4
) const {
    
}

std::string ShaderCodeGenerator::getGLSLSamplerName(
    const std::string& name
) const {
    return "u_GeneratedSampler_" + name;
}

std::string ShaderCodeGenerator::getGLSLSamplerType(
    SamplerType type
) const {
    switch (type) {
        case SamplerType::Texture2D: return "sampler2D";
        case SamplerType::CubeMap2D: return "samplerCube";
    } 
    return "";
}

std::string ShaderCodeGenerator::generateSamplerDefinitions(
    const Material& mat
) const {
    const auto& samplers = mat.getSamplerDefinitions();

    std::stringstream result;
    for (const auto& sampler : samplers) {
        result << "uniform " << getGLSLSamplerType(sampler.type) << " ";
        result << getGLSLSamplerName(sampler.name) << "\n";
    }

    return result.str();
}

std::string ShaderCodeGenerator::generateSamplerGetter(
    const Material& mat, const std::string& name
) const {
    std::stringstream result;
    result << "inline " << getGLSLSamplerType(mat.getSampler(name).type);
    result << " get_" << name << "() {\n";
    result << "    return ";
    if (GL_ARB_bindless_texture) {
        result << mat.getName() + "." << name << ";\n";
    } else {
        result << getGLSLSamplerName(name) << ";\n";
    }
    result << "}\n\n";
    return result.str();
}

std::string ShaderCodeGenerator::generateSamplerGetters(
    const Material& mat
) const {
    const auto& samplers = mat.getSamplerDefinitions();
    
    std::stringstream result;
    for (const auto& sampler : samplers) {
        result << generateSamplerGetter(mat, sampler.name);
    }

    return result.str();
}
