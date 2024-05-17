#include "img_data.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

// #include "../common/util.hpp"
// using namespace test;

ImageData::ImageData(const std::string &filename)
    : m_filename(filename), m_pixels(nullptr), m_resizePixels(nullptr) {}

ImageData::~ImageData() {
    if (m_pixels) {
        stbi_image_free(m_pixels);
    }
    if (m_resizePixels) {
        delete[] m_resizePixels;
    }
}

std::vector<Complex<>> ImageData::getImageData() { return imageData; }

double ImageData::rgbToGrayscale(std::uint32_t pixel) {
    double r = ((pixel >> 8 * 0) & 0xFF) / 255.0;
    double g = ((pixel >> 8 * 1) & 0xFF) / 255.0;
    double b = ((pixel >> 8 * 2) & 0xFF) / 255.0;
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

void ImageData::resizeImage(int *width, int *height) {
    m_pixels = reinterpret_cast<std::uint32_t *>(
        stbi_load(m_filename.c_str(), width, height, NULL, 4));
    assert(m_pixels != nullptr && "Error loading image!");

    int resizeWidth = 64 * 2;
    int resizeHeight = *height * resizeWidth / *width;
    m_resizePixels = static_cast<std::uint32_t *>(
        malloc(sizeof(std::uint32_t) * resizeWidth * resizeHeight));
    assert(m_resizePixels != nullptr &&
           "Error allocating memory for resized pixels!");

    stbir_resize_uint8(reinterpret_cast<u_char *>(m_pixels), *width, *height,
                       sizeof(std::uint32_t) * (*width),
                       reinterpret_cast<u_char *>(m_resizePixels), resizeWidth,
                       resizeHeight, sizeof(std::uint32_t) * resizeWidth, 4);
    *width = resizeWidth;
    *height = resizeHeight;
}

void ImageData::processImagePixels() {
    int width, height;
    resizeImage(&width, &height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::uint32_t pixel = m_resizePixels[y * width + x];
            imageData.push_back(rgbToGrayscale(pixel)/ 255.0);
        }
    }

    // Teste de cores
    // std::vector<Color> colors(width * height);
    // for (int y = 0; y < height; ++y) {
    //     for (int x = 0; x < width; ++x) {
    //         double color = std::abs(imageData[y * width + x]);
    //         colors[y * width + x] = {
    //             .r = color /*  / 255.0 */,
    //             .g = color /*  / 255.0 */,
    //             .b = color /*  / 255.0 */,
    //         };
    //     }
    // }
    // printPPM(colors, width, height);
}
