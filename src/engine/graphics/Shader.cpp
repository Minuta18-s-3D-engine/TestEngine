#include "Shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shader::Shader() {}

Shader::Shader(
    const std::string& vertexSource,
    const std::string& fragmentSource
) {
    this->compileShaders(
        vertexSource,
        fragmentSource
    );
}

Shader::Shader(Shader&& other) noexcept 
    : uniformLocations(std::move(other.uniformLocations)),
      glId(other.glId) 
{
    other.glId = 0; 
}

Shader& Shader::operator=(Shader&& other) noexcept 
{
    if (this != &other) {
        if (glId != 0) {
            glDeleteProgram(glId); 
        }
        glId = other.glId;
        uniformLocations = std::move(other.uniformLocations);
        other.glId = 0;
    }
    return *this;
}

Shader::~Shader() {
    if (glId == 0) return;
    glDeleteProgram(glId);
}

void Shader::compileShaders(
    const std::string& vertexSource, 
    const std::string& fragmentSource
) {
    const char* vShaderCode = vertexSource.c_str();
    const char* vFragmentCode = fragmentSource.c_str();

    uint vertexId, fragmentId;
    GLint success;
    char infoLog[ERROR_BUFFER_SIZE];

    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vShaderCode, nullptr);
    glCompileShader(vertexId);
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader compilation failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(vertexId);
        throw std::runtime_error(
            "Shader compilation failed"
        );
    }

    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &vFragmentCode, nullptr);
    glCompileShader(fragmentId);
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader compilation failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(vertexId);
        glDeleteShader(fragmentId);
        throw std::runtime_error(
            "Shader compilation failed"
        );
    }

    glId = glCreateProgram();
    glAttachShader(glId, vertexId);
    glAttachShader(glId, fragmentId);
    glLinkProgram(glId);
    glGetProgramiv(glId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(glId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader linking failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(vertexId);
        glDeleteShader(fragmentId);
        throw std::runtime_error(
            "Shader linking failed"
        );
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Shader::use() {
    std::cout << "Use: " << this->glId << std::endl;
    glUseProgram(glId);
}

uint Shader::getUniformLocation(const std::string& name) {
    auto found = uniformLocations.find(name);
    if (found == uniformLocations.end()) {
        uint location = glGetUniformLocation(glId, name.c_str());
        uniformLocations.try_emplace(name, location);
        return location;
    }
    return found->second;
}

void Shader::setUniform(const std::string& name, int32_t value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, uint32_t value) {
    glUniform1ui(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, const glm::vec2& value) {
    glUniform2f(getUniformLocation(name), value.x, value.y);
}

void Shader::setUniform(const std::string& name, const glm::ivec2& value) {
    glUniform2i(getUniformLocation(name), value.x, value.y);
}

void Shader::setUniform(const std::string& name, const glm::uvec2& value) {
    glUniform2ui(getUniformLocation(name), value.x, value.y);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) {
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, const glm::ivec3& value) {
    glUniform3i(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, const glm::uvec3& value) {
    glUniform3ui(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) {
    glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const glm::ivec4& value) {
    glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const glm::uvec4& value) {
    glUniform4ui(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const glm::mat2& value) {
    glUniformMatrix2fv(
        getUniformLocation(name), 1, GL_FALSE, 
        glm::value_ptr(value)
    );
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(
        getUniformLocation(name), 1, GL_FALSE, 
        glm::value_ptr(value)
    );
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(
        getUniformLocation(name), 1, GL_FALSE, 
        glm::value_ptr(value)
    );
}
