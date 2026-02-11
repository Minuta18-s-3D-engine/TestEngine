#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const VirtualPath& name) {
    this->compileComputeShader(name.resolve());
}

ComputeShader::~ComputeShader() {
    glDeleteProgram(glId);
}

void ComputeShader::compileComputeShader(const std::string& filename) {
    std::string cShaderCode = this->readShaderFile(filename);
    const char* vShaderCode = cShaderCode.c_str();

    uint computeId;
    int success;
    char infoLog[ERROR_BUFFER_SIZE];

    computeId = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeId, 1, &vShaderCode, nullptr);
    glCompileShader(computeId);
    glGetShaderiv(computeId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(computeId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << filename << " compilation failed: ";
        std::cerr << filename << ":" << std::endl;
        std::cerr << infoLog << std::endl;
        glDeleteShader(computeId);
        throw std::runtime_error("Shader " + filename + " compilation failed");
    }

    glId = glCreateProgram();
    glAttachShader(glId, computeId);
    glLinkProgram(glId);
    glGetProgramiv(glId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(glId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << filename << " linking failed: ";
        std::cerr << infoLog << std::endl;
        glDeleteShader(computeId);
        glDeleteProgram(glId);
        throw std::runtime_error("Shader " + filename + " linking failed");
    }

    glDeleteShader(computeId);
}
