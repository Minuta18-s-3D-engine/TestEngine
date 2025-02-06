#include "./PngCoder.hpp"

const std::string ERR_BASE = "Failed to read image ";

const std::string ERR_LIBPNG_TIP = "It's probably pnglib-related issue. ";
const std::string ERR_PCRS = "Failed to initialize png_structp. ";
const std::string ERR_PCIS = "Failed to init png_infop. ";
const std::string ERR_IHDR = "Failed to read IHDR. ";

const std::string ERR_UNKNOWN = "Unknown error. "; 

struct InMemoryReader {
    const uint8_t* bytes;
    size_t size;
    size_t offset;
};

void user_error_fn(png_structp png_ptr, png_const_charp msg) {
    std::cout << "Libpng error: " << msg << std::endl;
}

void user_warning_fn(png_structp png_ptr, png_const_charp msg) {
    std::cout << "Libpng warning: " << msg << std::endl;
}

// https://github.com/MihailRis/VoxelEngine-Cpp/blob/main/src/coders/png.cpp
// line 101
static void read_in_memory(
    png_structp pngPtr, png_bytep dst, png_size_t toread
) {
    png_voidp ioPtr = png_get_io_ptr(pngPtr);
    if (ioPtr == nullptr) {
        throw std::runtime_error("png_get_io_ptr(...) -> NULL");
    }
    auto& reader = *reinterpret_cast<InMemoryReader*>(ioPtr);
    if (reader.offset + toread > reader.size) {
        throw std::runtime_error("buffer underflow");
    }
    std::memcpy(dst, reader.bytes + reader.offset, toread);
    reader.offset += toread;
}

std::shared_ptr<ImageData> PngCodec::load_image(
    const uint8_t* bytes, size_t size, std::string name
) {
    png_structp pngPtr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    if (pngPtr == nullptr)
        throw std::runtime_error(
            ERR_BASE + name + ": " + ERR_PCRS + ERR_LIBPNG_TIP
        );
    
    png_infop infoPtr = nullptr;
    infoPtr = png_create_info_struct(pngPtr);
    if (infoPtr == nullptr) {
        png_destroy_read_struct(&pngPtr, nullptr, nullptr);

        throw std::runtime_error(
            ERR_BASE + name + ": " + ERR_PCIS + ERR_LIBPNG_TIP
        );
    }

    if (setjmp(png_jmpbuf(pngPtr))) {
        throw std::runtime_error(
            ERR_BASE + name + ": " + ERR_UNKNOWN + ERR_LIBPNG_TIP
        );
    }

    png_set_error_fn(
        pngPtr, nullptr, user_error_fn, user_warning_fn
    );

    InMemoryReader reader {bytes, size, 0};

    png_set_read_fn(pngPtr, &reader, read_in_memory);
    png_read_info(pngPtr, infoPtr);

    uint width = 0, height = 0;
    int bitDepth = 0, colorType = -1;
    uint retval = png_get_IHDR(
        pngPtr, infoPtr, &width, &height, &bitDepth, &colorType, 
        nullptr, nullptr, nullptr
    );

    if (retval != 1) {
        png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
        throw std::runtime_error(ERR_BASE + name + ": " + ERR_IHDR);
    }

    if (bitDepth == 16) png_set_strip_16(pngPtr);

    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(pngPtr);
    }
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
        png_set_expand_gray_1_2_4_to_8(pngPtr);
    }
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngPtr);
    }
    if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(pngPtr, 0xFF, PNG_FILLER_AFTER);
    }
    if (colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(pngPtr);
    }
    png_read_update_info(pngPtr, infoPtr);

    int rowBytes = png_get_rowbytes(pngPtr, infoPtr);
    colorType = 6;
    bitDepth = png_get_bit_depth(pngPtr, infoPtr);

    auto imageData = std::make_unique<png_byte[]>(rowBytes * height);
    auto rowPointers = std::make_unique<png_byte*[]>(height);
    for (int i = 0; i < height; ++i) {
        rowPointers[height - 1 - i] = imageData.get() + i * rowBytes;
    }
    png_read_image(pngPtr, rowPointers.get());

    ImageFormat format = ImageFormat::rgba;
    switch (colorType) {
        case PNG_COLOR_TYPE_RGBA:
            format = ImageFormat::rgba;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = ImageFormat::rgb;
            break;
        default:
            png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
            throw std::runtime_error(
                "color type " +  
                std::to_string(colorType) + " is not supported!"
            );
    }
    auto image = std::make_shared<ImageData>(
        format, width, height, std::move(imageData)
    );
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return image;
}

std::shared_ptr<Texture> PngCodec::load_texture(
    const uint8_t* bytes, size_t size, std::string name
) {
    std::shared_ptr<ImageData> image = PngCodec::load_image(bytes, size, name);
    auto texture = Texture::create(image.get());
    
    return texture;
}
