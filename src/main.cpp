#include "CApp/capp.hpp"

int main(int argc, char **argv) {
    try {
        auto app = std::make_shared<CApp>(argc, argv);
        app->run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}