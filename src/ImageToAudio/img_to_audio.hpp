#pragma once

#include "../ImageColorConverter/img_color_conv.hpp"
#include "../ToneGenerate/tone_gen.hpp"
#include <memory>
#include <vector>

class ImageToAudio {
  private:
    std::shared_ptr<ToneGenerate> m_toneGen;
    std::shared_ptr<ImageColorConverter> m_imgColorConv;

  private:
    std::string filenameAudio;

  public:
    void run();
    ImageToAudio(const std::vector<std::string> &filename, int format,
                 int channels);
};