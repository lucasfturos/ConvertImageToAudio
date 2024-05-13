#pragma once

#include "../ImageToAudio/img_to_audio.hpp"
#include "../ToneGenerate/tone_gen.hpp"
#include <iostream>
#include <memory>

class CApp {
  private:
    std::shared_ptr<ToneGenerate> m_toneGen;
    std::shared_ptr<ImageToAudio> m_imgAudio;

  private:
    std::string filenameAudio;

  public:
    void run();
    CApp(int argc, char **argv);
};