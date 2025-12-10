#include "JpgCoder.hpp"

std::shared_ptr<ImageData> JpgCoder::load_image(
    const uint8_t* bytes, size_t size, std::string name
) {
    struct jpeg_decompress_struct imageInfo;
    struct jpeg_error_mgr errors;

    imageInfo.err = jpeg_std_error(&errors);
    jpeg_create_decompress(&imageInfo);

    jpeg_mem_src(&imageInfo, const_cast<uint8_t*>(bytes), size);

    jpeg_read_header(&imageInfo, TRUE);
    jpeg_start_decompress(&imageInfo);

    int row_stride = imageInfo.image_width * imageInfo.output_components;
    auto imageData = std::make_unique<uint8_t[]>(
        imageInfo.image_height * imageInfo.image_width * 
        imageInfo.output_components
    );

    while (imageInfo.output_scanline < imageInfo.output_height) {
        uint8_t *rowPtr = &imageData[imageInfo.output_scanline * row_stride];
        jpeg_read_scanlines(&imageInfo, &rowPtr, 1);
    }

    jpeg_finish_decompress(&imageInfo);
    jpeg_destroy_decompress(&imageInfo);

    ImageFormat imgFormat;
    if (imageInfo.out_color_space == JCS_RGB && 
        imageInfo.output_components == 3) {
        imgFormat = ImageFormat::rgb;
    } else if (imageInfo.out_color_space == JCS_GRAYSCALE &&
        imageInfo.output_components == 1) {
        imgFormat = ImageFormat::grayscale;
    } else {
        imgFormat = ImageFormat::rgb;
    }

    auto image = std::make_shared<ImageData>(
        imgFormat, imageInfo.image_width, imageInfo.image_height, 
        std::move(imageData)
    );

    return image;
}

std::shared_ptr<Texture> JpgCoder::load_texture(
    const uint8_t* bytes, size_t size, std::string name
) {
    std::shared_ptr<ImageData> image = JpgCoder::load_image(bytes, size, name);
    auto texture = Texture::creatE(image.get());

    return texture;
}
