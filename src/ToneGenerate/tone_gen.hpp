#pragma once

#include <cmath>
#include <complex>
#include <cstdint>
#include <sndfile.h>
#include <stdexcept>
#include <string>
#include <vector>

class ToneGenerate {
  private:
    const int SAMPLERATE = 44100;
    const double PI = M_PI;
    const double AMPLITUDE = 0.5;
    const double FREQUENCY = 60.0;
    const double AMP_NORMALIZED = 32767.0;

  private:
    std::string m_filename;
    int m_format;
    int m_channels;

  private:
    std::complex<double> color;

    std::vector<int16_t> generateWaveform();

  public:
    void setComplexNumber(std::complex<double> z_);
    void saveAudio();

    ToneGenerate(const std::string &filename, int format, int channels);
};