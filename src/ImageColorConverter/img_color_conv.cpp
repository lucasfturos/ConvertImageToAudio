#include "img_color_conv.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

ImageColorConverter::ImageColorConverter(const std::string &filename)
    : m_filename(filename), m_pixels(nullptr), m_resize_pixels(nullptr) {
    processImageColor();
}

ImageColorConverter::~ImageColorConverter() {
    if (m_pixels) {
        delete[] m_pixels;
    }
    if (m_resize_pixels) {
        delete[] m_resize_pixels;
    }
}

std::vector<Color> ImageColorConverter::getColors() { return colors; }

void ImageColorConverter::processImageColor() {
    int width, height;

    m_pixels = reinterpret_cast<uint32_t *>(
        stbi_load(m_filename.c_str(), &width, &height, NULL, 4));
    assert(m_pixels != nullptr);

    int resize_width = 128;
    int resize_height = height * resize_width / width;
    m_resize_pixels = static_cast<uint32_t *>(
        malloc(sizeof(uint32_t) * resize_width * resize_height));
    assert(m_resize_pixels != nullptr);

    stbir_resize_uint8(
        reinterpret_cast<u_char *>(m_pixels), width, height,
        sizeof(uint32_t) * width, reinterpret_cast<u_char *>(m_resize_pixels),
        resize_width, resize_height, sizeof(uint32_t) * resize_width, 4);

    for (int y = 0; y < resize_height; y++) {
        for (int x = 0; x < resize_width; x++) {
            uint32_t pixel = m_resize_pixels[y * resize_width + x];
            double r = ((pixel >> 8 * 0) & 0xFF) / 255.0;
            double g = ((pixel >> 8 * 1) & 0xFF) / 255.0;
            double b = ((pixel >> 8 * 2) & 0xFF) / 255.0;
            double a = ((pixel >> 8 * 3) & 0xFF) / 255.0;
            r *= a;
            g *= a;
            b *= a;
            colors.push_back({.r = r, .g = g, .b = b});
        }
    }
}