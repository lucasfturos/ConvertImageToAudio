#pragma once

#include "../FFT/fft.hpp"
#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

struct Dimension {
    int width, height;
};

class ImageData {
  private:
    std::string m_filename;

  private:
    std::uint32_t *m_pixels;
    std::uint32_t *m_resizePixels;
    Dimension imageSize;
    std::vector<Complex<>> imageData;

    void resizeImage(int *w, int *h);
    double rgbToGrayscale(std::uint32_t pixel);

  public:
    Dimension getImageSize() { return imageSize; }
    std::vector<Complex<>> getImageData() { return imageData; }

  public:
    void processImagePixels();

    ImageData(const std::string &filename);
    ~ImageData();
};