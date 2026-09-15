#include "ShaderLayout.hpp"

#include <glad/glad.h>

#include <stdexcept>

namespace {
    uint32_t alignUp(uint32_t v, uint32_t a) noexcept {
        return (v + a - 1) & ~(a - 1);
    }
}

void ShaderLayout::packProperties() {
    uint32_t cursor = 0, maxAlign = 1;
    for (auto& p : properties) {
        const auto& info = shader_layout::typeInfo(p.type);
        cursor = alignUp(cursor, info.std430Alignment);
        p.offset = cursor;
        cursor += info.std430Size;
        maxAlign = std::max(maxAlign, info.std430Alignment);
    }
    dataBlockSize = alignUp(cursor, maxAlign);

    for (auto& s : samplers) {
        const auto& it = propertyIndex.find(s.name);
        if (it != propertyIndex.end()) {
            s.handleOffset = properties[it->second].offset;
        }
    }
}

bool ShaderLayout::hasProperty(const std::string& name) const {
    return propertyIndex.contains(name);
}

bool ShaderLayout::hasSampler(const std::string& name) const {
    return samplerIndex.contains(name);
}

void ShaderLayout::addField(const std::string& name, PropertyType type) {
    if (finalized) throw std::logic_error("Already finalized");
    if (propertyIndex.contains(name))
        throw std::invalid_argument("Property already exists: " + name);
    if (type == PropertyType::Count)
        throw std::invalid_argument("Invalid property type for field: " + name);

    propertyIndex[name] = static_cast<uint32_t>(properties.size());
    properties.push_back({ name, type, 0 });
}

void ShaderLayout::addSampler(
    const std::string& name, SamplerType type
) {
    if (finalized) throw std::logic_error("Already finalized");
    if (samplerIndex.contains(name))
        throw std::invalid_argument("Sampler already exists: " + name);
    if (type == SamplerType::Count)
        throw std::invalid_argument("Invalid sapmler type for field: " + name);

    Sampler s;
    s.name = name;
    s.type = type;
    s.binding = static_cast<uint32_t>(samplers.size());
    samplerIndex[name] = static_cast<uint32_t>(samplers.size());
    samplers.push_back(std::move(s));

    if (GL_ARB_bindless_texture) {
        addField(name, PropertyType::UVec2);
    }
}

void ShaderLayout::finalize() {
    if (finalized) return;

    packProperties();

    finalized = true;
}

const ShaderLayout::Property& ShaderLayout::getProperty(
    const std::string& name
) const {
    return properties.at(propertyIndex.at(name));
}

const ShaderLayout::Sampler& ShaderLayout::getSampler(
    const std::string& name
) const {
    return samplers.at(samplerIndex.at(name));
}
