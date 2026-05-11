#ifndef ENGINE_MATERIALS_MATERIALBUILDER_H_
#define ENGINE_MATERIALS_MATERIALBUILDER_H_

#include <string>

#include "Material.hpp"

class MaterialBuilder {
public:
    MaterialBuilder(const std::string& name);

    template <typename T>
    MaterialBuilder& addProperty(const std::string& name);
    
    template <typename T>
    MaterialBuilder& addProperty(
        const std::string& name, const T& defaultValue
    );

    MaterialBuilder& addSampler(const std::string& name);

    Material finalize();
};

#endif // ENGINE_MATERIALS_MATERIALBUILDER_H_
