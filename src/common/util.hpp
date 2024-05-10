#pragma once

#include "../ImageColorConverter/img_color_conv.hpp"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sndfile.h>
#include <stdexcept>
#include <unordered_map>

namespace util {
namespace fs = std::filesystem;

inline std::string toUppercase(const std::string &filenameAudio) {
    std::string uppercaseFilename = filenameAudio;
    std::transform(uppercaseFilename.begin(), uppercaseFilename.end(),
                   uppercaseFilename.begin(), ::toupper);
    return uppercaseFilename;
}

inline void createDirectory(const std::string &directoryPath) {
    fs::path path = fs::absolute(directoryPath);
    fs::path parentPath = path.parent_path();
    if (!fs::exists(parentPath)) {
        std::cout << "Parent directory does not exist: " + parentPath.string()
                  << '\n';
    }

    if (fs::exists(path) && fs::is_directory(path)) {
        std::cout << "Directory already exists: " << directoryPath << std::endl;
        return;
    }

    if (!fs::create_directory(parentPath)) {
        std::cerr << "Failed to create directory: " << parentPath << '\n';
    }

    std::cout << "Directory created: " << parentPath << '\n';
}

} // namespace util

namespace utilMain {

inline void inputCLIMessageError(int argc) {
    if (argc < 2) {
        throw std::runtime_error(
            std::string("Enter an image, directory and audio name, and the "
                        "number of channels.\nEx: "
                        "./src/ConvertImageToAudio assets/img/image.png "
                        "assets/audio/out.wav 1"));
    }
}

inline std::string getAudioExtension(const std::string &filenameAudio) {
    std::size_t extPos = filenameAudio.find_last_of('.');
    return filenameAudio.substr(extPos + 1);
}

inline int getAudioType(const std::string &extAudio) {
    std::unordered_map<std::string, int> typeAudioOption{
        {"WAV", SF_FORMAT_WAV},
        {"FLAC", SF_FORMAT_FLAC},
        {"OGG", SF_FORMAT_OGG},
    };

    auto it = typeAudioOption.find(extAudio);
    if (it == typeAudioOption.end()) {
        throw std::invalid_argument("Invalid audio file extension!");
    }

    return it->second;
}

inline void printPPM(std::vector<Color> colors) {
    std::cout << "P3\n";
    std::cout << "128 128\n";
    std::cout << "255\n";
    for (const auto &color : colors) {
        std::cout << static_cast<int>(color.r * 255) << ' '
                  << static_cast<int>(color.g * 255) << ' '
                  << static_cast<int>(color.b * 255) << '\n';
    }
}

inline void printRGB(std::vector<Color> colors) {
    for (const auto &color : colors) {
        std::cout << "(R: "  << std::setprecision(2)<< std::min(color.r, 1.0)
                  << ", G: " << std::setprecision(2)<< std::min(color.g, 1.0)
                  << " B: "  << std::setprecision(2)<< std::min(color.b, 1.0) << ")\n";
    }
}

inline void printHSV(std::vector<Color> colors) {
    for (const auto &color : colors) {
        std::cout << "(H: " << static_cast<int>(color.h)
                  << ", S: " << static_cast<int>(color.s)
                  << " V: " << static_cast<int>(color.v) << ")\n";
    }
}

} // namespace utilMain