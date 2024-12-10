#pragma once

#include "ImageData/img_data.hpp"
#include "ImageToAudio/img_audio.hpp"
#include <iostream>
#include <memory>

class CApp {
  private:
    std::shared_ptr<ImageToAudio> m_ImageToAudio;
    std::shared_ptr<ImageData> m_ImageData;

    int m_Argc;
    char **m_Argv;

  private:
    std::string m_FilenameAudio;

    void setup();

  public:
    void run();
    CApp(int argc, char **argv);
};