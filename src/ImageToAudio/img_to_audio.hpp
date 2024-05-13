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
    std::vector<Complex<double>> imageData;

  public:
    std::size_t getImageSize();
    std::vector<Complex<double>> getImageData();
    void processImage();

    ImageToAudio(const std::string &filename);
    ~ImageToAudio();
};