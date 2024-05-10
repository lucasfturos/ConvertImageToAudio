#include "img_to_audio.hpp"
#include "../common/util.hpp"

using namespace util;

ImageToAudio::ImageToAudio(const std::vector<std::string> &filename, int format,
                           int channels)
    : m_toneGen(std::make_shared<ToneGenerate>(filename[0], format, channels)),
      m_imgColorConv(std::make_shared<ImageColorConverter>(filename[1])) {
    filenameAudio = filename[0];
}

void ImageToAudio::run() {
    createDirectory(filenameAudio);
    m_toneGen->setComplexNumber(std::complex<double>(0, 2 * M_PI * 60.0));
    m_toneGen->saveAudio();
}