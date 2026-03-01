#ifndef ENGINE_MATERIALS_TEMPLATEGENERATORS_GEOMSHADERGENERATOR_HPP
#define ENGINE_MATERIALS_TEMPLATEGENERATORS_GEOMSHADERGENERATOR_HPP

#include <string>

#include "../Material.hpp"
#include "../../stringProcessing/templateEngine/TemplateEngine.hpp"

class GeomShaderGenerator {
    const TemplateEngine& templateEngine;

    std::string generateMaterialDefinition(const Material& material) const;
public:
    GeomShaderGenerator(const TemplateEngine& _engine);

    std::string generate(const Material& material) const;
};

#endif // ENGINE_MATERIALS_TEMPLATEGENERATORS_GEOMSHADERGENERATOR_HPP
