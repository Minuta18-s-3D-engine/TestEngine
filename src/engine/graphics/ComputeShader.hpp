#ifndef ENGINE_GRAPHICS_COMPUTE_SHADER_H_
#define ENGINE_GRAPHICS_COMPUTE_SHADER_H_

#include "Shader.hpp"

#include "../project/VirtualPath.hpp"

class ComputeShader : public Shader {
protected:
    void compileComputeShader(const std::string& source);
public:
    ComputeShader(const std::string& source);
    virtual ~ComputeShader();
};

#endif // ENGINE_GRAPHICS_COMPUTE_SHADER_H_
