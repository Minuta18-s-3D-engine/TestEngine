#include "Shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shader::Shader() {}

Shader::Shader(
    const VirtualPath& _vertexFilename,
    const VirtualPath& _fragmentFilename
) : vertexFilename(_vertexFilename), fragmentFilename(_fragmentFilename) {
    this->compileShaders(
        vertexFilename.resolve(),
        fragmentFilename.resolve()
    );
}

std::string Shader::readShaderFile(const std::string& filename) {
    std::string code;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        shaderFile.open(filename);

        std::stringstream fileStream;
        fileStream << shaderFile.rdbuf();

        shaderFile.close();

        code = fileStream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "Reading " << filename << " failed" << std::endl;
        throw std::runtime_error("Reading " + filename + " failed");
    }

    return code;
}

void Shader::compileShaders(
    const std::string& _vertexFilename, 
    const std::string& _fragmentFilename
) {
    std::string cShaderCode = readShaderFile(_vertexFilename);
    std::string cFragmentCode = readShaderFile(_fragmentFilename);
    const char* vShaderCode = cShaderCode.c_str();
    const char* vFragmentCode = cFragmentCode.c_str();

    uint vertexId, fragmentId;
    int success;
    char infoLog[ERROR_BUFFER_SIZE];

    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vShaderCode, nullptr);
    glCompileShader(vertexId);
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << vertexFilename.resolve();
        std::cerr << " compilation failed: ";
        std::cerr << _vertexFilename << ":" << std::endl;
        std::cerr << infoLog << std::endl;
        glDeleteShader(vertexId);
        throw std::runtime_error(
            "Shader " + vertexFilename.resolve() + " compilation failed"
        );
    }

    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &vFragmentCode, nullptr);
    glCompileShader(fragmentId);
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << fragmentFilename.resolve();
        std::cerr << " compilation failed: ";
        std::cerr << _fragmentFilename << ":" << std::endl;
        std::cerr << infoLog << std::endl;
        glDeleteShader(vertexId);
        glDeleteShader(fragmentId);
        throw std::runtime_error(
            "Shader " + fragmentFilename.resolve() + " compilation failed");
    }

    glId = glCreateProgram();
    glAttachShader(glId, vertexId);
    glAttachShader(glId, fragmentId);
    glLinkProgram(glId);
    glGetProgramiv(glId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(glId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << vertexFilename.resolve() << " and ";
        std::cerr << fragmentFilename.resolve() << " linking failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(vertexId);
        glDeleteShader(fragmentId);
        glUseProgram(glId);
        throw std::runtime_error(
            "Shader " + vertexFilename.resolve() + " linking failed"
        );
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Shader::use() {
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

void Shader::setUniform1i(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1ui(const std::string& name, uint value) {
    glUniform1ui(getUniformLocation(name), value);
}

void Shader::setUniform2i(const std::string& name, glm::vec2 value) {
    glUniform2i(getUniformLocation(name), value.x, value.y);
}

void Shader::setUniform2i(const std::string& name, int v1, int v2) {
    glUniform2i(getUniformLocation(name), v1, v2);
}

void Shader::setUniform2ui(const std::string& name, glm::uvec2 value) {
    glUniform2ui(getUniformLocation(name), value.x, value.y);
}

void Shader::setUniform2ui(const std::string& name, uint v1, uint v2) {
    glUniform2ui(getUniformLocation(name), v1, v2);
}

void Shader::setUniform3i(const std::string& name, glm::vec3 value) {
    glUniform3i(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setUniform3i(const std::string& name, int v1, int v2, int v3) {
    glUniform3i(getUniformLocation(name), v1, v2, v3);
}

void Shader::setUniform3ui(const std::string& name, glm::uvec3 value) {
    glUniform3ui(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setUniform3ui(const std::string& name, uint v1, uint v2, uint v3) {
    glUniform3ui(getUniformLocation(name), v1, v2, v3);
}
    
void Shader::setUniform1f(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
} 

void Shader::setUniform4mat(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(
        getUniformLocation(name), 1, GL_FALSE, 
        glm::value_ptr(value)
    );
}

void Shader::setUniform3f(
    const std::string& name, float v1, float v2, float v3
) {
    glUniform3f(getUniformLocation(name), v1, v2, v3);
}

void Shader::setUniform3f(
    const std::string& name, const glm::vec3& v
) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::setUniform1b(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), value);
}

Shader::~Shader() {
    glDeleteProgram(glId);
}
