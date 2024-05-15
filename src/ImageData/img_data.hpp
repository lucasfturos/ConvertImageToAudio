#pragma once

#include "../FFT/fft.hpp"
#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

class ImageData {
  private:
    std::string m_filename;

  private:
    std::uint32_t *m_pixels;
    std::uint32_t *m_resizePixels;
    std::vector<Complex<>> imageData;

    void resizeImage(int *width, int *height);
    double rgbToGrayscale(std::uint32_t pixel);
    std::vector<int> histogramEqualization(std::vector<double> intensities);
    double applySobelFilter(const std::vector<double> &grayscaleImage, int x,
                            int y, int width, int height);

  public:
    std::vector<Complex<>> getImageData();
    void processImage();

    ImageData(const std::string &filename);
    ~ImageData();
};