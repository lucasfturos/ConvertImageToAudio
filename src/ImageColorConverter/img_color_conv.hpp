#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

struct Color {
    double r = 0.0, g = 0.0, b = 0.0;
    double h = 0.0, s = 0.0, v = 0.0;
};

class ImageColorConverter {
  private:
    std::string m_filename;
    uint32_t *m_pixels;
    uint32_t *m_resize_pixels;
    std::vector<Color> colors;

    double g(double x);
    void processImageColor();
    void RGBToHSV(Color color);

  public:
    std::vector<Color> getColors();
    std::complex<double> HSVToComplex(Color color);
    ImageColorConverter(const std::string &filename);
    ~ImageColorConverter();
};