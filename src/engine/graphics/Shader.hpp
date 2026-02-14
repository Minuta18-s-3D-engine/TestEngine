#ifndef ENGINE_GRAPHICS_SHADER_H_
#define ENGINE_GRAPHICS_SHADER_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <glad/glad.h>

#include "../utils/EngineTypes.h"
#include "../project/VirtualPath.hpp"

class Shader {
protected:
    VirtualPath vertexFilename;
    VirtualPath fragmentFilename;
    
    const uint ERROR_BUFFER_SIZE = 512;
    std::unordered_map<std::string, uint> uniformLocations;

    std::string readShaderFile(const std::string& filename);
    void compileShaders(
        const std::string& vertexFilename, 
        const std::string& fragmentFilename
    );

    uint getUniformLocation(const std::string& name);
public:
    uint glId;
    Shader();
    Shader(
        const VirtualPath& vertexFilename,
        const VirtualPath& fragmentFilename
    );
    virtual ~Shader();

    void use();

    void setUniform(const std::string& name, int32_t value);
    void setUniform(const std::string& name, uint32_t value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::ivec2& value);
    void setUniform(const std::string& name, const glm::uvec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::ivec3& value);
    void setUniform(const std::string& name, const glm::uvec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::ivec4& value);
    void setUniform(const std::string& name, const glm::uvec4& value);
    void setUniform(const std::string& name, const glm::mat2& value);
    void setUniform(const std::string& name, const glm::mat3& value);
    void setUniform(const std::string& name, const glm::mat4& value);
};


#endif // ENGINE_GRAPHICS_SHADER_H_
