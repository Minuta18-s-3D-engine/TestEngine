#include "ShaderCodeGenerator.hpp"

ShaderCodeGenerator::ShaderCodeGenerator() : 
    templateEngine("core://assets/templates", parser) {}

std::string ShaderCodeGenerator::generateEngineGlobals() const {
    return templateEngine.render(
        "shaders/components/engineGlobals.glsl"
    );
}

std::string ShaderCodeGenerator::getIndentStr(uint32_t indent) const {
    std::string indentStr = "";
    for (int i = 0; i < indent; ++i) indentStr += " ";
    return indentStr;
}

std::string ShaderCodeGenerator::generateMaterialProperties(
    const Material& mat, uint32_t indent = 4
) const {
    const auto& layout = mat.getLayout();
    const auto& props = layout.getPropertyOrder();
    
    

    std::stringstream result;
    for (const std::string& name : props) {
        const auto& propertyInfo = layout.getPropertyInfo(name);

        result << getIndentStr(indent);
        result << MaterialLayout::getGLSLType(propertyInfo.type) << " ";
        result << name + ";\n";
    }

    return result.str();
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

std::string ShaderCodeGenerator::generateBase(uint32_t idx) const {
    return "b_MaterialData[u_CurrentMaterialStartId + " + std::to_string(idx) 
        + "]";
}

std::string ShaderCodeGenerator::unpackInt(const std::string& base) const {
    return "int(" + base + ")";
}

std::string ShaderCodeGenerator::unpackUint(const std::string& base) const {
    return "(" + base + ")";
}

std::string ShaderCodeGenerator::unpackInt64(int idx) const {
    return "packInt2x32(" + unpackVecIndexes("ivec2", idx, idx + 1) + ")";
}

std::string ShaderCodeGenerator::unpackUint64(int idx) const {
    return "packUint2x32(" + unpackVecIndexes("uvec2", idx, idx + 1) + ")";
}

std::string ShaderCodeGenerator::unpackFloat(const std::string& base) const {
    return "uintBitsToFloat(" + base + ")";
}

std::string ShaderCodeGenerator::unpackBool(const std::string& base) const {
    return "(" + base + " != 0U)";
}

std::string ShaderCodeGenerator::unpackVec(
    const std::string& type, const std::vector<std::string>& values
) const {
    std::stringstream result;
    result << type << "(";
    for (int i = 0; i < values.size() - 1; ++i) {
        result << values[i] << ", ";
    }
    result << values.back() << ")";
    return result.str();
}

std::string ShaderCodeGenerator::unpackVecIndexes(
    const std::string& type, const std::vector<int>& values
) const {
    std::vector<std::string> strValues;
    strValues.reserve(values.size());
    for (int index : values) {
        strValues.push_back(generateBase(index));
    }
    return unpackVec(type, strValues);
}

std::string ShaderCodeGenerator::unpackVecIndexes(
    const std::string& type, int minValue, int maxValue
) const {
    std::vector<int> values(maxValue - minValue + 1);
    for (int i = minValue; i <= maxValue; ++i) {
        values[i - minValue] = i;
    }
    return unpackVecIndexes(type, values);
}

std::string ShaderCodeGenerator::generateMaterialUnpacker(
    const Material& mat, const std::string& prop, uint32_t indent = 4
) const {
    const auto& propInfo = mat.getLayout().getPropertyInfo(prop);
    std::string base = generateBase(propInfo.offset);

    std::string converterFunc = "";
    switch (propInfo.type) {
        case MaterialLayout::PropertyType::Int:
            converterFunc = unpackInt(base);
        case MaterialLayout::PropertyType::Uint:
            converterFunc = unpackUint(base);
        case MaterialLayout::PropertyType::Int64:
            converterFunc = unpackInt64(propInfo.offset);
        case MaterialLayout::PropertyType::Uint64:
            converterFunc = unpackUint64(propInfo.offset);
        case MaterialLayout::PropertyType::Float:
            converterFunc = unpackFloat(base);
        case MaterialLayout::PropertyType::Bool:
            converterFunc = unpackBool(base);
        case MaterialLayout::PropertyType::Vec2:
            converterFunc = unpackVecIndexes(
                "vec2", propInfo.offset, propInfo.offset + 1
            );
        case MaterialLayout::PropertyType::IVec2:
            converterFunc = unpackVecIndexes(
                "ivec2", propInfo.offset, propInfo.offset + 1
            );
        case MaterialLayout::PropertyType::UVec2:
            converterFunc = unpackVecIndexes(
                "uvec2", propInfo.offset, propInfo.offset + 1
            );
        case MaterialLayout::PropertyType::Vec3:
            converterFunc = unpackVecIndexes(
                "vec3", propInfo.offset, propInfo.offset + 2
            );
        case MaterialLayout::PropertyType::IVec3:
            converterFunc = unpackVecIndexes(
                "ivec3", propInfo.offset, propInfo.offset + 2
            );
        case MaterialLayout::PropertyType::UVec3:
            converterFunc = unpackVecIndexes(
                "uvec3", propInfo.offset, propInfo.offset + 2
            );
        case MaterialLayout::PropertyType::Vec4:
            converterFunc = unpackVecIndexes(
                "vec4", propInfo.offset, propInfo.offset + 3
            );
        case MaterialLayout::PropertyType::IVec4:
            converterFunc = unpackVecIndexes(
                "ivec4", propInfo.offset, propInfo.offset + 3
            );
        case MaterialLayout::PropertyType::UVec4:
            converterFunc = unpackVecIndexes(
                "uvec4", propInfo.offset, propInfo.offset + 3
            );
        case MaterialLayout::PropertyType::Mat2:
            converterFunc = unpackVecIndexes(
                "mat2", propInfo.offset, propInfo.offset + 3
            ); 
        case MaterialLayout::PropertyType::Mat3:
            converterFunc = unpackVecIndexes(
                "mat3", propInfo.offset, propInfo.offset + 8
            ); 
        case MaterialLayout::PropertyType::Mat4:
            converterFunc = unpackVecIndexes(
                "mat4", propInfo.offset, propInfo.offset + 15
            ); 
        default:
            converterFunc = "(0 /* Unsupported type */)";
    }

    std::stringstream result;
    result << getIndentStr(indent) << "currentMaterial." << prop;
    result << " = " << converterFunc << ";\n";
    return result.str();
}

std::string ShaderCodeGenerator::generateUnpackerFunc(
    const Material& mat
) const {
    const auto& properties = mat.getLayout().getPropertyOrder();
    
    std::stringstream result;
    for (const auto& prop : properties) {
        result << generateMaterialUnpacker(
            mat, prop
        );
    }
    return result.str();
}

std::string ShaderCodeGenerator::generateMaterialDefinition(
    const Material& mat
) const {
    TemplateArguments args;
    args.set("material_name", mat.getName());
    args.set("sampler_definitions", generateSamplerDefinitions(mat));
    args.set("sampler_getters", generateSamplerGetters(mat));
    args.set("unpack_lines", generateUnpackerFunc(mat));

    return templateEngine.render(
        "shaders/materialDefinition.glsl", args
    );
}

std::string ShaderCodeGenerator::generateShader(
    const Material& mat, const std::string& type, const std::string& userCode
) const {
    TemplateArguments engineGlobalsArgs;
    engineGlobalsArgs.set(
        "material_definition", generateMaterialDefinition(mat)
    );
    std::string engineGlobals = templateEngine.render(
        "shaders/generalShaderTemplate.glsl", engineGlobalsArgs
    );

    TemplateArguments args;
    args.set("engine_globals", engineGlobals);
    args.set(
        "shader_specific_globals", 
        templateEngine.render("shaders/components" + type + "Globals.glsl")
    );
    args.set(
        "generated_header", 
        templateEngine.render("shaders/headers/materialLoaderHeader.glsl")
    );
    args.set("user_code", userCode);

    return templateEngine.render("shaders/generalShaderTemplate.glsl", args);
}
