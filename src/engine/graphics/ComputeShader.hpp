#ifndef ENGINE_GRAPHICS_COMPUTE_SHADER_H_
#define ENGINE_GRAPHICS_COMPUTE_SHADER_H_

#include "Shader.hpp"

class ComputeShader : public Shader {
protected:
    void compileComputeShader(const std::string& filename);
public:
    ComputeShader() = default;
    ComputeShader(const std::string& name);
    virtual ~ComputeShader();
};

#endif // ENGINE_GRAPHICS_COMPUTE_SHADER_H_
