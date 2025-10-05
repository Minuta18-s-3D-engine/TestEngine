#ifndef ENGINE_GRAPHICS_GBUFFER_H
#define ENGINE_GRAPHICS_GBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <exception>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

#include "../utils/EngineTypes.h"

class GBuffer {
public:
    enum BufferType {
        BUFFER_POSITION,
        BUFFER_NORMAL,
        BUFFER_ALBEDO,
        NUM_BUFFERS
    };

    GBuffer(uint _width, uint _height);
    virtual ~GBuffer();
    void resize(uint _width, uint _height);

    void bind();
    void unbind();
    void bindBufffers();
private:
    uint width, height;
    uint buffersId[BufferType::NUM_BUFFERS];
    uint fboId;
    uint depthBufferId = 0;

    void createFBO();
    void createBuffer(uint ind, GLuint format, GLenum type);
    void createDepthBuffer();
    void createBuffers();
};

#endif // ENGINE_GRAPHICS_GBUFFER_H
