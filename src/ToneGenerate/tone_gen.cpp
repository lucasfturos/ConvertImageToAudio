#include "tone_gen.hpp"
#include <cmath>

ToneGenerate::ToneGenerate(const std::string &filename, int format,
                           int channels)
    : m_filename(filename) {

    std::string formatName = (format == SF_FORMAT_WAV)    ? "WAV"
                             : (format == SF_FORMAT_FLAC) ? "FLAC"
                             : (format == SF_FORMAT_OGG)  ? "OGG"
                                                          : "Unknown";
    if (formatName.compare("Unknown") == 0) {
        throw std::invalid_argument("Invalid audio format.");
    }
    m_format = format;

    std::string channelName = (channels == 1)   ? "MONO"
                              : (channels == 2) ? "STEREO"
                                                : "Invalid";
    if (channelName.compare("Invalid") == 0) {
        throw std::invalid_argument("Number of channels must be 1 or 2.");
    }
    m_channels = channels;
}

void ToneGenerate::setComplexNumber(std::complex<double> z_) { color = z_; }

std::vector<int16_t> ToneGenerate::generateWaveform() {
    std::vector<int16_t> samples;
    for (int i = 0; i < SAMPLERATE; ++i) {
        double t = static_cast<double>(i) / SAMPLERATE;
        std::complex<double> zt = std::exp(color * t);
        int16_t sample =
            static_cast<int16_t>(AMPLITUDE * zt.real() * AMP_NORMALIZED);
        samples.push_back(sample);
    }
    return samples;
}

void ToneGenerate::saveAudio() {
    std::vector<int16_t> samples = generateWaveform();

    SF_INFO info;
    info.samplerate = SAMPLERATE;
    info.channels = m_channels;
    info.format = m_format | SF_FORMAT_PCM_16;

    SNDFILE *file = sf_open(m_filename.c_str(), SFM_WRITE, &info);
    if (!file) {
        std::string errorMessage = "Error creating audio file: ";
        errorMessage += sf_strerror(file);
        throw std::runtime_error(errorMessage);
    }

    sf_write_short(file, samples.data(),
                   static_cast<sf_count_t>(samples.size()));
    sf_close(file);
}