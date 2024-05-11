#include "img_to_audio.hpp"
#include "../common/util.hpp"

using namespace util;
using namespace test;

ImageToAudio::ImageToAudio(int argc, char **argv) {
    CLIMessageError(argc);
    filenameAudio = argv[2];
    std::string filenameImage = argv[1];
    int channel = std::stoi(argv[3]);
    std::string extAudio = getAudioExtension(filenameAudio);
    int format = getAudioType(toUppercase(extAudio));

    m_toneGen = std::make_shared<ToneGenerate>(filenameAudio, format, channel);
    m_imgColorConv = std::make_shared<ImageColorConverter>(filenameImage);
}

void ImageToAudio::run() {
    if (!createDirectory(filenameAudio)) {
        throw std::runtime_error(std::string(
            "Failed to create directory for audio file: " + filenameAudio));
    }
    std::vector<Color> colors = m_imgColorConv->getColors();
    for (const auto &color : colors) {
        m_toneGen->setComplexNumber(m_imgColorConv->HSVToComplex(color));
    }
    // m_toneGen->setComplexNumber(std::complex<double>(0, 2 * M_PI * 60.0));
    m_toneGen->saveAudio();
}