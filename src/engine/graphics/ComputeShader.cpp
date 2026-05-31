#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const std::string& source) {
    this->compileComputeShader(source);
}

void ComputeShader::compileComputeShader(const std::string& source) {
    const char* vShaderCode = source.c_str();

    uint computeId;
    int success;
    char infoLog[ERROR_BUFFER_SIZE];

    computeId = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeId, 1, &vShaderCode, nullptr);
    glCompileShader(computeId);
    glGetShaderiv(computeId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(computeId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader compilation failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(computeId);
        throw std::runtime_error("Shader compilation failed");
    }

    glId = glCreateProgram();
    glAttachShader(glId, computeId);
    glLinkProgram(glId);
    glGetProgramiv(glId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(glId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader linking failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(computeId);
        glDeleteProgram(glId);
        throw std::runtime_error("Shader linking failed");
    }

    glDeleteShader(computeId);
}
