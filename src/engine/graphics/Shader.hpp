#ifndef ENGINE_GRAPHICS_SHADER_H_
#define ENGINE_GRAPHICS_SHADER_H_

#include "../utils/EngineTypes.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class Shader {
    uint glId;
    std::string name;
    const uint ERROR_BUFFER_SIZE = 512;
    std::unordered_map<std::string, uint> uniformLocations;

    std::string readShaderFile(const std::string& filename);
    void compileShaders(
        const std::string& vertexFilename, 
        const std::string& fragmentFilename
    );

    uint getUniformLocation(const std::string& name);
public:
    Shader(const std::string& name);
    ~Shader();

    void use();
    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniform4mat(const std::string& name, const glm::mat4& value);
    void setUniform3f(const std::string& name, float v1, float v2, float v3);
    void setUniform3f(const std::string& name, const glm::vec3& value);
    std::string get_name();
};


#endif // ENGINE_GRAPHICS_SHADER_H_
