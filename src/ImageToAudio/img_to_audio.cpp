#include "img_to_audio.hpp"
#include <iomanip>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

ImageToAudio::ImageToAudio(const std::string &filename)
    : m_filename(filename), m_pixels(nullptr), m_resizePixels(nullptr),
      fft_ptr(std::make_shared<FFT<double>>()) {}

ImageToAudio::~ImageToAudio() {
    if (m_pixels) {
        delete[] m_pixels;
    }
    if (m_resizePixels) {
        delete[] m_resizePixels;
    }
}

std::vector<std::complex<double>> ImageToAudio::getSpectrum() {
    return spectrum;
}

std::size_t ImageToAudio::getSpectrumSize() { return spectrum.size(); }

void ImageToAudio::processImage() {
    int width, height;

    m_pixels = reinterpret_cast<std::uint32_t *>(
        stbi_load(m_filename.c_str(), &width, &height, NULL, 4));
    assert(m_pixels != nullptr && "Error loading image!");

    int resizeWidth = 128;
    int resizeHeight = height * resizeWidth / width;
    m_resizePixels = static_cast<std::uint32_t *>(
        malloc(sizeof(std::uint32_t) * resizeWidth * resizeHeight));
    assert(m_resizePixels != nullptr &&
           "Error allocating memory for resized pixels!");

    stbir_resize_uint8(reinterpret_cast<u_char *>(m_pixels), width, height,
                       sizeof(std::uint32_t) * width,
                       reinterpret_cast<u_char *>(m_resizePixels), resizeWidth,
                       resizeHeight, sizeof(std::uint32_t) * resizeWidth, 4);

    imageSize = resizeWidth * resizeHeight;
    for (int y = 0; y < resizeHeight; y++) {
        for (int x = 0; x < resizeWidth; x++) {
            std::uint32_t pixel = m_resizePixels[y * resizeWidth + x];
            double r = (pixel >> 8 * 0) & 0xFF;
            double g = (pixel >> 8 * 1) & 0xFF;
            double b = (pixel >> 8 * 2) & 0xFF;
            double a = (pixel >> 8 * 3) & 0xFF;
            r *= a / 255.0 / 255.0;
            g *= a / 255.0 / 255.0;
            b *= a / 255.0 / 255.0;
            double intensity = 0.2126 * r + 0.7152 * g + 0.0722 * b;
            spectrum.push_back(intensity);
        }
    }
}
