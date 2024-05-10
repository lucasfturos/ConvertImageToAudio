#include "img_color_conv.hpp"
#include <cmath>
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

void ImageColorConverter::RGBToHSV(Color color) {
    double eps = std::numeric_limits<double>::epsilon();
    double cmax = std::max(color.r, std::max(color.g, color.b));
    double cmin = std::min(color.r, std::min(color.g, color.b));
    double delta = cmax - cmin;

    double tempH = 0.0;
    if (delta < eps) {
        tempH = 0.0;
    } else if (cmax == color.r) {
        tempH = 60.0 * std::fmod((color.g - color.b) / delta, 6.0);
    } else if (cmax == color.g) {
        tempH = 60.0 * ((color.b - color.r) / delta + 2.0);
    } else if (cmax == color.b) {
        tempH = 60.0 * ((color.r - color.g) / delta + 4.0);
    } else {
        assert(0 && "Inaccessible");
    }

    double tempL = (cmax + cmax) / 2.0;
    double tempS;
    if (delta < eps) {
        tempS = 0.0;
    } else {
        tempS = delta / (1.0 - std::fabs(2.0 * tempL - 1.0));
    }

    color.h = std::fmod(std::fmod(tempH, 360.0) + 360.0, 360.0);
    color.s = (tempS * 100.0);
    color.v = (tempL * 100.0);
    colors.push_back(color);
}

std::complex<double> ImageColorConverter::HSVToComplex(Color color) {
    double x = color.v;
    double y = color.s;
    double theta = color.h * M_PI / 180.0;
    double r = std::sqrt((x * x) + (y * y));
    return std::complex<double>(r * std::sin(theta), r * std::cos(theta));
}

void ImageColorConverter::processImageColor() {
    Color color;
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

    for (int y = 0; y < resize_height; ++y) {
        for (int x = 0; x < resize_width; ++x) {
            uint32_t pixel = m_resize_pixels[y * resize_width + x];
            double r = ((pixel >> 8 * 0) & 0xFF) / 255.0;
            double g = ((pixel >> 8 * 1) & 0xFF) / 255.0;
            double b = ((pixel >> 8 * 2) & 0xFF) / 255.0;
            double a = ((pixel >> 8 * 3) & 0xFF) / 255.0;
            r *= a;
            g *= a;
            b *= a;
            double gray_value = (r + g + b) / 3.0;
            r = gray_value;
            g = gray_value;
            b = gray_value;
            color = {.r = r, .g = g, .b = b};
            RGBToHSV(color);
        }
    }
}