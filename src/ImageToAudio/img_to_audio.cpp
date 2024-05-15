#include "img_to_audio.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

ImageToAudio::ImageToAudio(const std::string &filename)
    : m_filename(filename), m_pixels(nullptr), m_resizePixels(nullptr) {}

ImageToAudio::~ImageToAudio() {
    if (m_pixels) {
        stbi_image_free(m_pixels);
    }
    if (m_resizePixels) {
        delete[] m_resizePixels;
    }
}

std::vector<Complex<>> ImageToAudio::getImageData() { return imageData; }

double ImageToAudio::rgbToGrayscale(std::uint32_t pixel) {
    double r = ((pixel >> 8 * 0) & 0xFF) / 255.0;
    double g = ((pixel >> 8 * 1) & 0xFF) / 255.0;
    double b = ((pixel >> 8 * 2) & 0xFF) / 255.0;
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

std::vector<int>
ImageToAudio::histogramEqualization(std::vector<double> intensities) {
    const int num_bins = 256;
    std::vector<int> histogram(num_bins, 0);
    std::vector<double> cdf(num_bins, 0.0);
    std::vector<int> equalized(num_bins, 0);

    for (double intensity : intensities) {
        int bin = std::min<int>(intensity * num_bins, num_bins - 1);
        histogram[bin]++;
    }

    int count = 0;
    int number_of_pixels = intensities.size();
    for (int i = 0; i < num_bins; ++i) {
        count += histogram[i];
        cdf[i] = static_cast<double>(count) / number_of_pixels;
    }

    for (int i = 0; i < num_bins; ++i) {
        equalized[i] = std::round(cdf[i] * (num_bins - 1));
    }

    std::vector<int> result;
    result.reserve(intensities.size());
    for (double intensity : intensities) {
        int bin = std::min<int>(intensity * num_bins, num_bins - 1);
        result.push_back(equalized[bin]);
    }

    return result;
}

double
ImageToAudio::applySobelFilterAtPixel(const std::vector<double> &grayscaleImage,
                                      int x, int y, int width, int height) {
    std::vector<std::vector<int>> sobelX = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1},
    };

    std::vector<std::vector<int>> sobelY = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    double gx = 0.0;
    double gy = 0.0;

    for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
            int pixelX = std::clamp(x + kx, 0, width - 1);
            int pixelY = std::clamp(y + ky, 0, height - 1);
            double pixel = grayscaleImage[pixelY * width + pixelX];
            gx += pixel * sobelX[ky + 1][kx + 1];
            gy += pixel * sobelY[ky + 1][kx + 1];
        }
    }

    return std::sqrt(gx * gx + gy * gy);
}

void ImageToAudio::processImage() {
    int width, height;

    m_pixels = reinterpret_cast<std::uint32_t *>(
        stbi_load(m_filename.c_str(), &width, &height, NULL, 4));
    assert(m_pixels != nullptr && "Error loading image!");

    int resizeWidth = 64;
    int resizeHeight = height * resizeWidth / width;
    m_resizePixels = static_cast<std::uint32_t *>(
        malloc(sizeof(std::uint32_t) * resizeWidth * resizeHeight));
    assert(m_resizePixels != nullptr &&
           "Error allocating memory for resized pixels!");

    stbir_resize_uint8(reinterpret_cast<u_char *>(m_pixels), width, height,
                       sizeof(std::uint32_t) * width,
                       reinterpret_cast<u_char *>(m_resizePixels), resizeWidth,
                       resizeHeight, sizeof(std::uint32_t) * resizeWidth, 4);

    std::vector<double> intensities;
    intensities.reserve(resizeWidth * resizeHeight);
    for (int y = 0; y < resizeHeight; ++y) {
        for (int x = 0; x < resizeWidth; ++x) {
            std::uint32_t pixel = m_resizePixels[y * resizeWidth + x];
            intensities[y * resizeWidth + x] = rgbToGrayscale(pixel);
        }
    }
    std::vector<double> edges(resizeWidth * resizeHeight);
    for (int y = 0; y < resizeHeight; ++y) {
        for (int x = 0; x < resizeWidth; ++x) {
            edges[y * resizeWidth + x] = applySobelFilterAtPixel(
                intensities, x, y, resizeWidth, resizeHeight);
        }
    }

    std::vector<int> histogram = histogramEqualization(edges);
    for (int intensity : histogram) {
        imageData.push_back(static_cast<double>(intensity));
    }
}
