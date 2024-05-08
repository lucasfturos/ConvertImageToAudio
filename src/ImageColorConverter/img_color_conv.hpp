#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

typedef struct {
    double r, g, b;
} Color;

class ImageColorConverter {
  private:
    std::string m_filename;
    uint32_t *m_pixels;
    uint32_t *m_resize_pixels;
    std::vector<Color> colors;

    void processImageColor();

  public:
    std::vector<Color> getColors();
    ImageColorConverter(const std::string &filename);
    ~ImageColorConverter();
};