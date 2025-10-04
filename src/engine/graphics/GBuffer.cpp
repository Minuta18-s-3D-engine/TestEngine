#include "GBuffer.hpp"

GBuffer::GBuffer(uint _width, uint _height) : width(_width), height(_height) {
    createFBO();
    createBuffers();

    GLenum attachments[BufferType::NUM_BUFFERS] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(BufferType::NUM_BUFFERS, attachments);
  
    createDepthBuffer();

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("GBuffer is not complete: " + status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer() {
    glDeleteFramebuffers(1, &fboId);
    glDeleteRenderbuffers(1, &depthBufferId);
    for (int i = 0; i < BufferType::NUM_BUFFERS; ++i) {
        glDeleteTextures(1, &buffersId[i]);
    }
}

void GBuffer::createFBO() {
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
}

void GBuffer::createBuffer(uint ind, GLuint format, GLenum type) {
    if (!buffersInitialized)
        glGenTextures(1, &buffersId[ind]);
    glBindTexture(GL_TEXTURE_2D, buffersId[ind]);
    glTexImage2D(
        GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, type, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ind, GL_TEXTURE2, 
        buffersId[ind], 0
    );
}

void GBuffer::createDepthBuffer() {
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId
    );
}

void GBuffer::createBuffers() {
    createBuffer(0, GL_RGBA16F, GL_FLOAT);
    createBuffer(0, GL_RGBA16F, GL_FLOAT);
    createBuffer(0, GL_RGBA, GL_UNSIGNED_BYTE);
    buffersInitialized = true;
}

void GBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::bindBufffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < BufferType::NUM_BUFFERS; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, buffersId[i]);
    }
}

void GBuffer::resize(uint _width, uint _height) {
    if (width == _width && height == _height) return;
    this->width = width;
    this->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    createBuffers();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
