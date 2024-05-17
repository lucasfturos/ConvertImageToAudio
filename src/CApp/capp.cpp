#include "capp.hpp"
#include "../common/util.hpp"

using namespace util;

CApp::CApp(int argc, char **argv) {
    CLIMessageError(argc);
    filenameAudio = argv[2];
    std::string filenameImage = argv[1];
    int channel = std::stoi(argv[3]);

    std::string extAudio = getAudioExtension(filenameAudio);
    int format = getAudioType(toUppercase(extAudio));

    m_toneGen = std::make_shared<ToneGenerate>(filenameAudio, format, channel);
    m_imgAudio = std::make_shared<ImageData>(filenameImage);
}

void CApp::run() {
    if (!createDirectory(filenameAudio)) {
        throw std::runtime_error(std::string(
            "Failed to create directory for audio file: " + filenameAudio));
    }
    m_imgAudio->processImagePixels();
    m_toneGen->setImageData(m_imgAudio->getImageData(),
                            m_imgAudio->getImageSize().width,
                            m_imgAudio->getImageSize().height);
    m_toneGen->saveAudio();
    std::cout << "Done\n";
}