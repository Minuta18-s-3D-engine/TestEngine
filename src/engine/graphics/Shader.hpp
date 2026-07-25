#ifndef ENGINE_GRAPHICS_SHADER_H_
#define ENGINE_GRAPHICS_SHADER_H_

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>
#include <glad/glad.h>

struct ShaderSources {
    std::string vertex;
    std::string fragment;
    std::string compute;
};

class Shader final {
    const uint32_t ERROR_BUFFER_SIZE = 4096;

    std::unordered_map<std::string, GLint> uniformLocations;
    GLint getUniformLocation(const std::string& name);

    uint32_t compileStage(GLenum stage, const std::string& source) const;

    const uint32_t NO_SHADER = 0;
    uint32_t glId = NO_SHADER;
public:
    explicit Shader(const ShaderSources& sources);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ~Shader();

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

    [[nodiscard]] uint32_t getGlId() const { return glId; }
};


#endif // ENGINE_GRAPHICS_SHADER_H_
