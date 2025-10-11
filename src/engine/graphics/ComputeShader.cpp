#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const std::string& name) {
    this->compileComputeShader(
        "./assets/shaders/" + name + ".comp.glsl"
    );
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
        std::cerr << "Shader " << name << " compilation failed: ";
        std::cerr << filename << ":" << std::endl;
        std::cerr << infoLog << std::endl;
        throw std::runtime_error("Shader " + name + " compilation failed");
    }

    glId = glCreateProgram();
    glAttachShader(glId, computeId);
    glLinkProgram(glId);
    glGetProgramiv(glId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(glId, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << name << " linking failed: ";
        std::cerr << infoLog << std::endl;
        throw std::runtime_error("Shader " + name + " linking failed");
    }

    glDeleteShader(computeId);
}
