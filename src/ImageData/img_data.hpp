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

    void resizeImage(int *w, int *h);
    double rgbToGrayscale(std::uint32_t pixel);

  public:
    void processImagePixels();
    std::vector<Complex<>> getImageData();

    ImageData(const std::string &filename);
    ~ImageData();
};