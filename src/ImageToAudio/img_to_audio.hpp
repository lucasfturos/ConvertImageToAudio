#pragma once

#include "../ImageColorConverter/img_color_conv.hpp"
#include "../ToneGenerate/tone_gen.hpp"
#include <iostream>
#include <memory>

class ImageToAudio {
  private:
    std::shared_ptr<ToneGenerate> m_toneGen;
    std::shared_ptr<ImageColorConverter> m_imgColorConv;

  private:
    std::string filenameAudio;

  public:
    void run();
    ImageToAudio(int argc, char **argv);
};