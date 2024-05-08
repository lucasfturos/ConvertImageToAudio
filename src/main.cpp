#include "ToneGenerate/tone_gen.hpp"
#include "common/util.hpp"

using namespace util;
using namespace utilMain;

int main(int argc, char **argv) {
    try {
        inputCLIMessageError(argc);

        // User input
        std::string filepathImage = argv[1];
        std::string filepathAudio = argv[2];
        int channel = std::stoi(argv[3]);

        // Create directory if it doesn't exist
        createDirectory(filepathAudio);

        // Convert param audio
        std::string extAudio = getAudioExtension(filepathAudio);
        int typeAudio = getAudioType(toUppercase(extAudio));
        auto tone_gen =
            std::make_shared<ToneGenerate>(filepathAudio, typeAudio, channel);
        tone_gen->saveAudio();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}