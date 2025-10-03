#ifndef ENGINE_GRAPHICS_GBUFFER_H
#define ENGINE_GRAPHICS_GBUFFER_H

#include "../utils/EngineTypes.h"

class GBuffer {
    uint width, height;

    void createBuffers();
public:
    GBuffer(uint _width, uint _height);
    virtual ~GBuffer();

    void bindForWriting();
    void bindForReading();
}

#endif // ENGINE_GRAPHICS_GBUFFER_H
