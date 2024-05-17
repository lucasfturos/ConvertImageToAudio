#pragma once

#include "../common/constants.hpp"
#include <algorithm>
#include <memory>
#include <sndfile.h>
#include <stdexcept>
#include <string>
#include <vector>

class ToneGenerate {
  private:
    std::string m_filename;
    int m_format;
    int m_channels;

  private:
    int m_imageWidth;
    int m_imageHeight;
    std::vector<double> m_imageData;

    std::vector<double> calculateFrequencies();
    std::vector<std::int16_t> createAudioSamples();

  public:
    void saveAudio();
    void setImageData(std::vector<double> imageData, Dimension imageSize);

    ToneGenerate(const std::string &filename, int format, int channels);
};