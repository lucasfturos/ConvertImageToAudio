#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <functional>
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
    if (argc < 7) {
        throw std::runtime_error(
            std::string("Enter an image, directory and audio name, and the "
                        "number of channels, gain of wave, duration of "
                        "audio in seconds and mode (save or play).\nEx: "
                        "./src/ConvertImageToAudio assets/img/image.png "
                        "assets/audio/out.wav 1 500.0 60 save"));
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

inline void audioModeSavePlay(const std::string &mode,
                      const std::vector<std::function<void()>> &func) {
    if (mode == "save") {
        if (!func.empty()) {
            func[0]();
        } else {
            throw std::runtime_error("No function provided for 'save' mode.");
        }
    } else if (mode == "play") {
        if (func.size() > 1) {
            func[1]();
        } else {
            throw std::runtime_error("No function provided for 'play' mode.");
        }
    } else {
        throw std::runtime_error(
            "Please provide the correct mode (save or play).");
    }
}

} // namespace util

namespace test {

struct Color {
    double r = 0.0, g = 0.0, b = 0.0;
    double h = 0.0, s = 0.0, v = 0.0;
};

inline std::vector<int> histogramEqualization(std::vector<double> intensities) {
    const int num_bins = 256;
    std::vector<int> histogram(num_bins, 0);
    for (double intensity : intensities) {
        int bin = std::min<int>(intensity * num_bins, num_bins - 1);
        histogram[bin]++;
    }

    int count = 0;
    int number_of_pixels = intensities.size();
    std::vector<double> cdf(num_bins, 0.0);
    for (int i = 0; i < num_bins; ++i) {
        count += histogram[i];
        cdf[i] = static_cast<double>(count) / number_of_pixels;
    }

    std::vector<int> equalized(num_bins, 0);
    for (int i = 0; i < num_bins; ++i) {
        equalized[i] = std::round(cdf[i] * (num_bins - 1));
    }

    std::vector<int> result;
    result.reserve(intensities.size());
    for (double intensity : intensities) {
        int bin = std::min<int>(intensity * num_bins, num_bins - 1);
        result.push_back(equalized[bin]);
    }
    return result;
}

inline double applySobelFilter(const std::vector<double> &grayscaleImage, int x,
                               int y, int width, int height) {
    std::vector<std::vector<int>> sobelX = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1},
    };
    std::vector<std::vector<int>> sobelY = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };
    double gx = 0.0, gy = 0.0;
    for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
            int pixelX = std::clamp(x + kx, 0, width - 1);
            int pixelY = std::clamp(y + ky, 0, height - 1);
            double pixel = grayscaleImage[pixelY * width + pixelX];
            gx += pixel * sobelX[ky + 1][kx + 1];
            gy += pixel * sobelY[ky + 1][kx + 1];
        }
    }
    return std::sqrt(gx * gx + gy * gy);
}

inline void printPPM(std::vector<Color> colors, int width, int height) {
    std::cout << "P3\n";
    std::cout << width << " " << height << '\n';
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