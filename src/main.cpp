#include "ImageToAudio/img_to_audio.hpp"

int main(int argc, char **argv) {
    try {
        auto imgToAudio = std::make_shared<ImageToAudio>(argc, argv);
        imgToAudio->run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}