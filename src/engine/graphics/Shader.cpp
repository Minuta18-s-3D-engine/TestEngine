#include "Shader.hpp"

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "engine/utils/exc/ShaderExceptions.hpp"
#include "engine/utils/exc/GeneralExceptions.hpp"

uint32_t Shader::compileStage(const GLenum stage, const std::string& source) const {
    const char* code = source.c_str();
    uint32_t shaderId = glCreateShader(stage);
    glShaderSource(shaderId, 1, &code, nullptr);
    glCompileShader(shaderId);

    GLuint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[ERROR_BUFFER_SIZE];
        glGetShaderInfoLog(
            shaderId,
            static_cast<GLsizei>(ERROR_BUFFER_SIZE),
            nullptr,
            infoLog
        );
        glDeleteShader(shaderId);
        throw exc::shader_exceptions::compilation_failed(
            "Shader compilation failed: " + std::string(infoLog)
        );
    }
    return shaderId;
}

Shader::Shader(const ShaderSources& sources) {
    glId = glCreateProgram();
    std::vector<uint32_t> attachedShaders;

    try {
        if (!sources.vertex.empty() && !sources.fragment.empty()) {
            const uint32_t vertexId = compileStage(GL_VERTEX_SHADER, sources.vertex);
            const uint32_t fragmentId = compileStage(GL_FRAGMENT_SHADER, sources.fragment);
            glAttachShader(glId, vertexId);
            glAttachShader(glId, fragmentId);
            attachedShaders.push_back(vertexId);
            attachedShaders.push_back(fragmentId);
        } else if (!sources.compute.empty()) {
            const uint32_t computeId = compileStage(GL_COMPUTE_SHADER, sources.compute);
            glAttachShader(glId, computeId);
            attachedShaders.push_back(computeId);
        } else {
            throw exc::invalid_argument("Invalid shader configuration");
        }

        glLinkProgram(glId);

        GLuint success;
        glGetProgramiv(glId, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[ERROR_BUFFER_SIZE];
            glGetProgramInfoLog(glId, ERROR_BUFFER_SIZE, nullptr, infoLog);
            throw exc::shader_exceptions::linking_failed(
                "Shader linking failed: " + std::string(infoLog)
            );
        }
    } catch (...) {
        if (glId != NO_SHADER) glDeleteProgram(glId);
        throw;
    }

    for (const uint32_t shaderStageId : attachedShaders) {
        glDeleteShader(shaderStageId);
    }
}

Shader::Shader(Shader&& other) noexcept 
    : uniformLocations(std::move(other.uniformLocations)),
      glId(other.glId) 
{
    other.glId = NO_SHADER;
}

Shader& Shader::operator=(Shader&& other) noexcept 
{
    if (this != &other) {
        if (glId != NO_SHADER) {
            glDeleteProgram(glId); 
        }
        glId = other.glId;
        uniformLocations = std::move(other.uniformLocations);
        other.glId = NO_SHADER;
    }
    return *this;
}

Shader::~Shader() {
    if (glId == NO_SHADER) return;
    glDeleteProgram(glId);
}


void Shader::use() const {
    glUseProgram(glId);
}

GLint Shader::getUniformLocation(const std::string& name) {
    const auto found = uniformLocations.find(name);
    if (found == uniformLocations.end()) {
        GLint location = glGetUniformLocation(glId, name.c_str());
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
