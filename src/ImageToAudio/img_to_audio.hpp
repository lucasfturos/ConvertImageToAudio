#pragma once

#include "../FFT/fft.hpp"
#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

class ImageToAudio {
  private:
    std::string m_filename;

  private:
    std::uint32_t *m_pixels;
    std::uint32_t *m_resizePixels;
    std::vector<Complex<>> imageData;

    double rgbToGrayscale(std::uint32_t pixel);
    std::vector<int> histogramEqualization(std::vector<double> intensities);
    double applySobelFilterAtPixel(const std::vector<double> &grayscaleImage,
                                   int x, int y, int width, int height);

  public:
    std::vector<Complex<>> getImageData();
    void processImage();

    ImageToAudio(const std::string &filename);
    ~ImageToAudio();
};