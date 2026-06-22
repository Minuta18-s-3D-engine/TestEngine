#include "ImageData.hpp"

ImageData::ImageData(ImageFormat format, uint width, uint height, 
    std::unique_ptr<uint8_t[]> data
) : format(format), width(width), height(height), data(std::move(data)) {

}

ImageData::~ImageData() = default;

uint ImageData::getWidth() const { return width; }
uint ImageData::getHeight() const { return height; }
ImageFormat ImageData::getFormat() const { return format; }

uint8_t* ImageData::getData() const {
    return data.get();
}