#pragma once

#include "../FFT/fft.hpp"
#include <algorithm>
#include <memory>
#include <sndfile.h>
#include <stdexcept>
#include <string>
#include <vector>

class ToneGenerate {
  private:
    const int SAMPLERATE = 44100;
    const double AMP_NORMALIZED = 32767.0;

  private:
    std::string m_filename;
    int m_format;
    int m_channels;

  private:
    std::size_t m_imageSize;
    std::shared_ptr<FFT<>> fft_ptr;
    std::vector<Complex<>> m_imageData;
    std::vector<Complex<>> m_frequencyData;

    std::vector<std::int16_t> generateWaveform();

  public:
    void saveAudio();
    void setImageData(std::vector<Complex<>> imageData);

    ToneGenerate(const std::string &filename, int format, int channels);
};