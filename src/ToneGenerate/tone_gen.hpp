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
    const double PI = M_PI;
    const double AMPLITUDE = 1.0;
    const double FREQUENCY = 60.0;
    const double AMP_NORMALIZED = 32767.0;

  private:
    std::string m_filename;
    int m_format;
    int m_channels;

  private:
    std::size_t m_spectrumSize;
    std::shared_ptr<FFT<std::uint16_t>> fft_ptr;
    std::vector<std::complex<double>> m_spectrum;

    std::vector<std::int16_t> generateWaveform();

  public:
    void saveAudio();
    void setSpectrumData(std::vector<std::complex<double>> spectrum,
                         std::size_t size);

    ToneGenerate(const std::string &filename, int format, int channels);
};