#ifndef ENGINE_GRAPHICS_SHADERSTORAGEBUFFER_H_
#define ENGINE_GRAPHICS_SHADERSTORAGEBUFFER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdint.h>
#include <vector>

enum SSBOBindings {
    CLUSTER_BINDING = 0,
    LIGHT_BINDING = 1,
    LIGHT_INDICIES_BINDING = 2,
    BVH_NODES = 3,
    BVH_INDICES = 4,
    MATERIALS = 5,
};

class ShaderStorageBuffer {
    uint32_t glId;
    SSBOBindings binding;
    GLenum SSBOtype;
public:
    ShaderStorageBuffer(
        SSBOBindings _binding, GLenum _SSBOtype
    );
    ~ShaderStorageBuffer();
    ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
    ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
    ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept;
    ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept;

    void bind();
    void bindData();
    void unbind();
    void setData(const void* data, size_t size);
    void setSubData(const void* data, size_t offset, size_t size);

    uint32_t getId() const { return glId; }
};

#endif // ENGINE_GRAPHICS_SHADERSTORAGEBUFFER_H_
