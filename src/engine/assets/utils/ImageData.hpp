#ifndef ENGINE_ASSETS_UTILS_IMAGEDATA_H_
#define ENGINE_ASSETS_UTILS_IMAGEDATA_H_

#include <memory>
#include "../../utils/EngineTypes.h"
#include "inttypes.h"

enum class ImageFormat {
    rgb,
    rgba,
};

class ImageData {
    ImageFormat format;
    uint width, height;
    std::unique_ptr<uint8_t[]> data;
public:
    ImageData(
        ImageFormat format, uint width, 
        uint height, std::unique_ptr<uint8_t[]> data
    );

    ~ImageData();

    uint getWidth() const;
    uint getHeight() const;
    uint8_t* getData() const;
    ImageFormat getFormat() const;
};

#endif // ENGINE_ASSETS_UTILS_IMAGEDATA_H_
