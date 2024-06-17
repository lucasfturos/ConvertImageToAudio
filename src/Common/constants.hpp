#pragma once

#include <cmath>
#include <numbers>

struct Dimension {
    int width, height;
};

const int MIN_FREQ = 0;
const int MAX_FREQ = 20000;
const int SAMPLE_RATE = 44100;

const double PI = std::numbers::pi;
const double AMP_NORMALIZED = 32767.0;
