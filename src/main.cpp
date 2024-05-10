#include "ImageToAudio/img_to_audio.hpp"
#include "common/util.hpp"

using namespace util;
using namespace utilMain;

int main(int argc, char **argv) {
    try {
        inputCLIMessageError(argc);
        std::string filenameImage = argv[1];
        std::string filenameAudio = argv[2];
        int channel = std::stoi(argv[3]);
        std::string extAudio = getAudioExtension(filenameAudio);
        int typeAudio = getAudioType(toUppercase(extAudio));
        std::vector<std::string> filename{filenameAudio, filenameImage};
        auto imgToAudio =
            std::make_shared<ImageToAudio>(filename, typeAudio, channel);
        imgToAudio->run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}