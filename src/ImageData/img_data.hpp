#pragma once

#include "../Common/constants.hpp"
#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class ImageData {
  private:
    std::string m_filename;

  private:
    std::uint32_t *m_pixels;
    std::uint32_t *m_resizePixels;
    Dimension imageSize;
    std::vector<double> imageData;

    void resizeImage(int *w, int *h);
    double rgbToGrayscale(std::uint32_t pixel);

  public:
    Dimension getImageSize() { return imageSize; }
    std::vector<double> getImageData() { return imageData; }

  public:
    void processImagePixels();

    ImageData(const std::string &filename);
    ~ImageData();
};