#include "img_to_audio.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

ImageToAudio::ImageToAudio(const std::string &filename)
    : m_filename(filename), m_pixels(nullptr), m_resizePixels(nullptr),
      fft_ptr(std::make_shared<FFT<std::uint32_t>>()) {}

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
    assert(m_pixels != nullptr);

    int resizeWidth = 128;
    int resizeHeight = height * resizeWidth / width;
    m_resizePixels = static_cast<std::uint32_t *>(
        malloc(sizeof(std::uint32_t) * resizeWidth * resizeHeight));
    assert(m_resizePixels != nullptr);

    stbir_resize_uint8(reinterpret_cast<u_char *>(m_pixels), width, height,
                       sizeof(std::uint32_t) * width,
                       reinterpret_cast<u_char *>(m_resizePixels), resizeWidth,
                       resizeHeight, sizeof(std::uint32_t) * resizeWidth, 4);

    imageSize = resizeWidth * resizeHeight;
    std::vector<Complex<double>> imageData;
    for (int y = 0; y < resizeHeight; y++) {
        for (int x = 0; x < resizeWidth; x++) {
            uint32_t pixel = m_resizePixels[y * resizeWidth + x];
            imageData.push_back(pixel);
        }
    }
    std::size_t fft_size = 1;
    while (fft_size < imageData.size()) {
        fft_size *= 2;
    }
    spectrum.resize(fft_size, 0);
    fft_ptr->fftAnalyze(imageData, 1, spectrum, fft_size);
}
