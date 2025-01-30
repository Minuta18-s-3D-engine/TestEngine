#include "Shader.hpp"

Shader::Shader(const std::string& name) : name(name) {
    this->compileShaders(
        "./assets/shaders/" + name + ".vertex.glsl",
        "./assets/shaders/" + name + ".fragment.glsl"
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
    const std::string& vertex_filename, 
    const std::string& fragment_filename
) {
    const char* vShaderCode = readShaderFile(vertex_filename).c_str();
    const char* vFragmentCode = readShaderFile(fragment_filename).c_str();

    uint vertex_id, fragment_id;
    int success;
    char infoLog[ERROR_BUFFER_SIZE];

    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vShaderCode, nullptr);
    glCompileShader(vertex_id);
    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_id, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << name << " compilation failed: ";
        std::cerr << vertex_filename << ":" << std::endl;
        std::cerr << infoLog << std::endl;
        throw std::runtime_error("Shader " + name + " compilation failed");
    }

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &vFragmentCode, nullptr);
    glCompileShader(fragment_id);
    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_id, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << name << " compilation failed: ";
        std::cerr << fragment_filename << ":" << std::endl;
        std::cerr << infoLog << std::endl;
        throw std::runtime_error("Shader " + name + " compilation failed");
    }

    gl_id = glCreateProgram();
    glAttachShader(gl_id, vertex_id);
    glAttachShader(gl_id, fragment_id);
    glLinkProgram(gl_id);
    glGetProgramiv(gl_id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(gl_id, ERROR_BUFFER_SIZE, nullptr, infoLog);
        std::cerr << "Shader " << name << " linking failed: ";
        std::cerr << infoLog << std::endl;
        throw std::runtime_error("Shader " + name + " linking failed");
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}

void Shader::use() {
    glUseProgram(gl_id);
}

uint Shader::getUniformLocation(const std::string& name) {
    auto found = uniformLocations.find(name);
    if (found == uniformLocations.end()) {
        uint location = glGetUniformLocation(gl_id, name.c_str());
        uniformLocations.try_emplace(name, location);
        return location;
    }
    return found->second;
}

void Shader::setUniform1i(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
} 

Shader::~Shader() {
    glDeleteProgram(gl_id);
}

