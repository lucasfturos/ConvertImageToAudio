#pragma once

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sndfile.h>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace util {

inline std::string toUppercase(const std::string &filenameAudio) {
    std::string uppercaseFilename = filenameAudio;
    std::transform(uppercaseFilename.begin(), uppercaseFilename.end(),
                   uppercaseFilename.begin(), ::toupper);
    return uppercaseFilename;
}

namespace fs = std::filesystem;
inline bool createDirectory(const std::string &directoryPath) {
    fs::path path = fs::absolute(directoryPath);
    fs::path parentPath = path.parent_path();
    fs::path audioDirectory = path.parent_path();
    while (audioDirectory.filename() != "audio" &&
           audioDirectory != audioDirectory.root_path()) {
        audioDirectory = audioDirectory.parent_path();
    }
    if (audioDirectory.filename() != "audio") {
        std::cerr << "Failed to find 'audio' directory in: " << parentPath
                  << '\n';
        return false;
    }
    if (!fs::exists(audioDirectory)) {
        if (!fs::create_directory(audioDirectory)) {
            std::cerr << "Failed to create directory: " << audioDirectory
                      << '\n';
            return false;
        }
        std::cout << "Directory created: " << audioDirectory << '\n';
    }
    return true;
}

inline void CLIMessageError(int argc) {
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

} // namespace util

namespace test {

struct Color {
    double r = 0.0, g = 0.0, b = 0.0;
    double h = 0.0, s = 0.0, v = 0.0;
};

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
        std::cout << "(R: " << std::setprecision(2) << std::min(color.r, 1.0)
                  << ", G: " << std::setprecision(2) << std::min(color.g, 1.0)
                  << " B: " << std::setprecision(2) << std::min(color.b, 1.0)
                  << ")\n";
    }
}

inline void printHSV(std::vector<Color> colors) {
    for (const auto &color : colors) {
        std::cout << "(H: " << static_cast<int>(color.h)
                  << ", S: " << static_cast<int>(color.s)
                  << ", V: " << static_cast<int>(color.v) << ")\n";
    }
}

} // namespace test