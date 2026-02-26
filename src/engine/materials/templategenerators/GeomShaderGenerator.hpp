#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_GEOMSHADERGENERATOR_HPP
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_GEOMSHADERGENERATOR_HPP

#include <string>

#include "../Material.hpp"

class GeomShaderGenerator {
    std::string generateMaterialDefinition(const Material& material) const;
public:
    GeomShaderGenerator() = default;

    std::string generate(const Material& material, std::string source) const;
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_GEOMSHADERGENERATOR_HPP
