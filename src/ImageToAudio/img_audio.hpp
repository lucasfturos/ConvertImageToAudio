#pragma once

#include "Common/constants.hpp"
#include <algorithm>
#include <memory>
#include <numeric>
#include <sndfile.h>
#include <string>
#include <vector>

class ImageToAudio {
  private:
    std::string m_filename;
    int m_format;
    int m_channels;
    double m_gain;
    double m_duration;

  private:
    int m_imageWidth;
    int m_imageHeight;
    std::vector<double> m_imageData;

    std::vector<double> calculateFrequencies() const;
    void processSamples(std::vector<std::int16_t> &samples, int start, int end,
                        const std::vector<double> &frequencies);
    std::vector<std::int16_t> createAudioSamples();

  public:
    void setup();
    void saveAudio();
    void playAudio();

    void setImageData(std::vector<double> imageData, Dimension imageSize);

    ImageToAudio(const std::string &filename, int format, int channels,
                 double gain, double duration);
};