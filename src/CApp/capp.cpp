#include "capp.hpp"
#include "Common/util.hpp"

using namespace util;

CApp::CApp(int argc, char **argv) : m_Argc(argc), m_Argv(argv) {}

void CApp::setup() {
    CLIMessageError(m_Argc);

    std::string filenameImage = m_Argv[1];
    m_FilenameAudio = m_Argv[2];

    int channel = std::stoi(m_Argv[3]);
    double gain = std::stod(m_Argv[4]);
    double duration = std::stod(m_Argv[5]);

    std::string extAudio = getAudioExtension(m_FilenameAudio);
    int format = getAudioType(toUppercase(extAudio));

    m_ImageToAudio = std::make_shared<ImageToAudio>(m_FilenameAudio, format,
                                                    channel, gain, duration);
    m_ImageToAudio->setup();

    m_ImageData = std::make_shared<ImageData>(filenameImage);
}

void CApp::run() {
    setup();

    if (!createDirectory(m_FilenameAudio)) {
        throw std::runtime_error(std::string(
            "Failed to create directory for audio file: " + m_FilenameAudio));
    }

    m_ImageData->processImagePixels();
    m_ImageToAudio->setImageData(m_ImageData->getImageData(),
                                 m_ImageData->getImageSize());

    std::string mode = m_Argv[6];
    audioModeSavePlay(mode, {[this]() { m_ImageToAudio->saveAudio(); },
                             [this]() { m_ImageToAudio->playAudio(); }});

    std::cout << "Done\n";
}