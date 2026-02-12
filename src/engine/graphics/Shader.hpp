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
    void setUniform1i(const std::string& name, int value);
    void setUniform1ui(const std::string& name, uint value);
    void setUniform2i(const std::string& name, glm::vec2 value);
    void setUniform2i(const std::string& name, int v1, int v2);
    void setUniform2ui(const std::string& name, glm::uvec2 value);
    void setUniform2ui(const std::string& name, uint v1, uint v2);
    void setUniform3i(const std::string& name, glm::vec3 value);
    void setUniform3i(const std::string& name, int v1, int v2, int v3);
    void setUniform3ui(const std::string& name, glm::uvec3 value);
    void setUniform3ui(const std::string& name, uint v1, uint v2, uint v3);
    void setUniform1f(const std::string& name, float value);
    void setUniform4mat(const std::string& name, const glm::mat4& value);
    void setUniform3f(const std::string& name, float v1, float v2, float v3);
    void setUniform3f(const std::string& name, const glm::vec3& value);
    void setUniform1b(const std::string& name, bool value);
};


#endif // ENGINE_GRAPHICS_SHADER_H_
