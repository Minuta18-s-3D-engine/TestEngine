#include "ShaderStorageBuffer.hpp"

ShaderStorageBuffer::ShaderStorageBuffer(
    SSBOBindings _binding, GLenum _SSBOtype
) : binding(_binding), SSBOtype(_SSBOtype) {
    glGenBuffers(1, &glId);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
    glDeleteBuffers(1, &glId);
}

ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept
    : glId(other.glId), binding(other.binding), SSBOtype(other.SSBOtype) {
    other.glId = 0;
}

ShaderStorageBuffer& ShaderStorageBuffer::operator=(
    ShaderStorageBuffer&& other
) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &glId);
        glId = other.glId;
        binding = other.binding;
        SSBOtype = other.SSBOtype;
        other.glId = 0;
    }
    return *this;
}

void ShaderStorageBuffer::bind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, glId);
}

void ShaderStorageBuffer::bindData() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, glId);
}

void ShaderStorageBuffer::unbind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::setData(const void* data, size_t size) {
    bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, SSBOtype);
    unbind();
}

void ShaderStorageBuffer::setSubData(
    const void* data, size_t size, size_t offset
) {
    bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
    unbind();
}
