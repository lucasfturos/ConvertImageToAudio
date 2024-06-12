#pragma once

#include "../ImageData/img_data.hpp"
#include "../ImageToAudio/img_audio.hpp"
#include <iostream>
#include <memory>

class CApp {
  private:
    std::shared_ptr<ImageToAudio> m_toneGen;
    std::shared_ptr<ImageData> m_imgAudio;

  private:
    std::string filenameAudio;

  public:
    void run();
    CApp(int argc, char **argv);
};