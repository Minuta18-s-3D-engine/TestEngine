#ifndef ENGINE_GRAPHICS_SHADERLAYOUT_HPP
#define ENGINE_GRAPHICS_SHADERLAYOUT_HPP

#include "ShaderPropertyTypes.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class ShaderLayout {
public:
    using PropertyType = shader_layout::PropertyType;
    using SamplerType = shader_layout::SamplerType;

    static constexpr uint32_t NO_HANDLE = 0;

    struct Property {
        std::string name;
        PropertyType type;
        uint32_t offset = 0;
    };

    struct Sampler {
        std::string name;
        SamplerType type;
        uint32_t binding = 0;
        uint32_t handleOffset = NO_HANDLE;
    };
private:
    bool isFinalized = false;

    uint32_t dataBlockSize = 0;

    std::vector<Property> properties;
    std::vector<Sampler> samplers;

    std::unordered_map<std::string, size_t> propertyIndex;
    std::unordered_map<std::string, size_t> samplerIndex;
public:
    explicit ShaderLayout() = default;

    void addField(const std::string& name, PropertyType type);
    void addSampler(const std::string& name, SamplerType type);
    void finalize();

    [[nodiscard]] bool isFinalized() noexcept { return isFinalized; }
    [[nodiscard]] bool hasProperty(const std::string& name) const;
    [[nodiscard]] bool hasSampler(const std::string& name) const;
    [[nodiscard]] uint32_t dataBlockSize() const noexcept { return dataBlockSize; };

    [[nodiscard]] const Property& getProperty(const std::string& name) const;
    [[nodiscard]] const Sampler& getSampler(const std::string& name) const;

    [[nodiscard]] const std::vector<Property>& getProperties() const { return properties; };
    [[nodiscard]] const std::vector<Sampler>& getSamplers() const { return samplers; };
};

#endif // ENGINE_GRAPHICS_SHADERLAYOUT_HPP
