// #include "ToneGenerate/tone_gen.hpp"
#include "ImageColorConverter/img_color_conv.hpp"
#include "common/util.hpp"

using namespace util;
using namespace utilMain;

int main(int argc, char **argv) {
    try {
        inputCLIMessageError(argc);

        // User input
        std::string filepathImage = argv[1];
        // std::string filepathAudio = argv[2];
        // int channel = std::stoi(argv[3]);

        // Create directory if it doesn't exist
        // createDirectory(filepathAudio);

        // Converts the audio file format to the type used by the audio library
        // std::string extAudio = getAudioExtension(filepathAudio);
        // int typeAudio = getAudioType(toUppercase(extAudio));
        // auto tone_gen =
        //     std::make_shared<ToneGenerate>(filepathAudio, typeAudio,
        //     channel);
        // tone_gen->saveAudio();
        auto color_conv = std::make_shared<ImageColorConverter>(filepathImage);
        std::vector<Color> colors = color_conv->getColors();
        for (const auto &color : colors) {
            std::cout << "(R: " << std::min(color.r, 1.0)
                      << ", G: " << std::min(color.g, 1.0)
                      << " B: " << std::min(color.b, 1.0) << ")\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}