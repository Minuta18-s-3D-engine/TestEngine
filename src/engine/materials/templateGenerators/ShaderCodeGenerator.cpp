#include "ShaderCodeGenerator.hpp"

ShaderCodeGenerator::ShaderCodeGenerator() : 
    templateEngine("core://assets/templates", parser) {}

std::string ShaderCodeGenerator::generateEngineGlobals() const {
    return templateEngine.render(
        "shaders/components/engineGlobals.glsl"
    );
}

std::string ShaderCodeGenerator::getIndentStr(uint32_t indent) const {
    return std::string(indent, ' ');
}

std::string ShaderCodeGenerator::generateMaterialProperties(
    const Material& mat, uint32_t indent
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
        result << getGLSLSamplerName(sampler.name) << ";\n";
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
        result << getGLSLSamplerType(mat.getSampler(name).type) << 
            "(currentMaterial." << name << ");\n";
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
    const std::string& type, const std::vector<int>& values,
    const std::string& wrapFunc
) const {
    std::vector<std::string> strValues;
    strValues.reserve(values.size());
    for (int index : values) {
        strValues.push_back(wrapFunc + "(" + generateBase(index) + ")");
    }
    return unpackVec(type, strValues);
}

std::string ShaderCodeGenerator::unpackVecIndexes(
    const std::string& type, int minValue, int maxValue,
    const std::string& wrapFunc
) const {
    std::vector<int> values(maxValue - minValue + 1);
    for (int i = minValue; i <= maxValue; ++i) {
        values[i - minValue] = i;
    }
    return unpackVecIndexes(type, values, wrapFunc);
}

std::string ShaderCodeGenerator::generateMaterialUnpacker(
    const Material& mat, const std::string& prop, uint32_t indent
) const {
    const auto& propInfo = mat.getLayout().getPropertyInfo(prop);
    size_t inArrayOffset = propInfo.offset / 4;
    int matOffset = static_cast<int>(inArrayOffset);
    std::string base = generateBase(inArrayOffset);

    std::string converterFunc = "";
    switch (propInfo.type) {
        case MaterialLayout::PropertyType::Int:
            converterFunc = unpackInt(base);
            break;
        case MaterialLayout::PropertyType::Uint:
            converterFunc = unpackUint(base);
            break;
        case MaterialLayout::PropertyType::Int64:
            converterFunc = unpackInt64(inArrayOffset);
            break;
        case MaterialLayout::PropertyType::Uint64:
            converterFunc = unpackUint64(inArrayOffset);
            break;
        case MaterialLayout::PropertyType::Float:
            converterFunc = unpackFloat(base);
            break;
        case MaterialLayout::PropertyType::Bool:
            converterFunc = unpackBool(base);
            break;
        case MaterialLayout::PropertyType::Vec2:
            converterFunc = unpackVecIndexes(
                "vec2", inArrayOffset, inArrayOffset + 1,
                "uintBitsToFloat"
            );
            break;
        case MaterialLayout::PropertyType::IVec2:
            converterFunc = unpackVecIndexes(
                "ivec2", inArrayOffset, inArrayOffset + 1,
                "int"
            );
            break;
        case MaterialLayout::PropertyType::UVec2:
            converterFunc = unpackVecIndexes(
                "uvec2", inArrayOffset, inArrayOffset + 1
            );
            break;
        case MaterialLayout::PropertyType::Vec3:
            converterFunc = unpackVecIndexes(
                "vec3", inArrayOffset, inArrayOffset + 2,
                "uintBitsToFloat"
            );
            break;
        case MaterialLayout::PropertyType::IVec3:
            converterFunc = unpackVecIndexes(
                "ivec3", inArrayOffset, inArrayOffset + 2, "int"
            );
            break;
        case MaterialLayout::PropertyType::UVec3:
            converterFunc = unpackVecIndexes(
                "uvec3", inArrayOffset, inArrayOffset + 2
            );
            break;
        case MaterialLayout::PropertyType::Vec4:
            converterFunc = unpackVecIndexes(
                "vec4", inArrayOffset, inArrayOffset + 3,
                "uintBitsToFloat"
            );
            break;
        case MaterialLayout::PropertyType::IVec4:
            converterFunc = unpackVecIndexes(
                "ivec4", inArrayOffset, inArrayOffset + 3, "int"
            );
            break;
        case MaterialLayout::PropertyType::UVec4:
            converterFunc = unpackVecIndexes(
                "uvec4", inArrayOffset, inArrayOffset + 3
            );
            break;
        case MaterialLayout::PropertyType::Mat2:
            converterFunc = unpackVecIndexes(
                "mat2", inArrayOffset, inArrayOffset + 3,
                "uintBitsToFloat"
            ); 
            break;
        case MaterialLayout::PropertyType::Mat3:
            converterFunc = unpackVecIndexes(
                "mat3", { 
                    matOffset,     matOffset + 1, matOffset + 2,
                    matOffset + 4, matOffset + 5, matOffset + 6, 
                    matOffset + 8, matOffset + 9, matOffset + 10
                },
                "uintBitsToFloat"
            ); 
            break;
        case MaterialLayout::PropertyType::Mat4:
            converterFunc = unpackVecIndexes(
                "mat4", inArrayOffset, inArrayOffset + 15,
                "uintBitsToFloat"
            ); 
            break;
        default:
            converterFunc = "(0 /* Unsupported type */)";
            break;
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
    if (GL_ARB_bindless_texture) {
        args.set("sampler_definitions", "");
    } else {
        args.set("sampler_definitions", generateSamplerDefinitions(mat));
    }
    args.set("sampler_getters", generateSamplerGetters(mat));
    args.set("unpack_lines", generateUnpackerFunc(mat));
    args.set("material_properties", generateMaterialProperties(mat));

    return templateEngine.render(
        "shaders/materialDefinition.glsl", args
    );
}

std::string ShaderCodeGenerator::generateShader(
    const Material& mat, const std::string& type, const std::string& userCode,
    const std::string& userFunc
) const {
    TemplateArguments engineGlobalsArgs;
    engineGlobalsArgs.set(
        "material_definition", generateMaterialDefinition(mat)
    );
    std::string engineGlobals = templateEngine.render(
        "shaders/components/engineGlobals.glsl", engineGlobalsArgs
    );

    TemplateArguments materialLoaderHeaderArgs;
    materialLoaderHeaderArgs.set(
        "user_func", userFunc
    );
    std::string materialLoaderHeader = templateEngine.render(
        "shaders/headers/materialLoaderHeader.glsl", 
        materialLoaderHeaderArgs
    );

    TemplateArguments args;
    args.set("engine_globals", engineGlobals);
    args.set(
        "shader_specific_globals", 
        templateEngine.render("shaders/components/" + type + "Globals.glsl")
    );
    args.set(
        "generated_header", 
        materialLoaderHeader
    );
    args.set("user_code", userCode);

    return templateEngine.render("shaders/generalShaderTemplate.glsl", args);
}
