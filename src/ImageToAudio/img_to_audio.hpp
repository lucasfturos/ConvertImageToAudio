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
    std::size_t imageSize;
    std::uint32_t *m_pixels;
    std::uint32_t *m_resizePixels;
    std::vector<Complex<double>> spectrum;
    std::shared_ptr<FFT<double>> fft_ptr;

  public:
    std::size_t getSpectrumSize();
    std::vector<Complex<double>> getSpectrum();
    void processImage();

    ImageToAudio(const std::string &filename);
    ~ImageToAudio();
};