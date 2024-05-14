#include "tone_gen.hpp"
#include <iostream>

ToneGenerate::ToneGenerate(const std::string &filename, int format,
                           int channels)
    : m_filename(filename), fft_ptr(std::make_shared<FFT<double>>()) {

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

void ToneGenerate::setImageData(std::vector<Complex<double>> imageData,
                                std::size_t size) {
    m_imageData = imageData;
    m_imageSize = size;

    m_frequencyData.resize(size, 0);
    fft_ptr->fftAnalyze(imageData, m_frequencyData, size);
}

std::vector<std::int16_t> ToneGenerate::generateWaveform() {
    std::size_t numSamples = static_cast<std::size_t>(2 * SAMPLERATE);
    std::vector<std::int16_t> samples(numSamples, 0);
    for (std::size_t i = 0; i < numSamples; ++i) {
        double t = static_cast<double>(i) / numSamples;
        double sample = 0.0;
        for (std::size_t j = 0; j < m_frequencyData.size(); ++j) {
            double frequency =
                (static_cast<double>(j) / m_frequencyData.size()) * numSamples;
            double amplitude = std::abs(m_frequencyData[j]);
            sample +=
                amplitude * AMPLITUDE * std::sin(2.0 * PI * frequency * t);
        }
        samples[i] = static_cast<std::int16_t>(sample * AMP_NORMALIZED);
        samples[i] = std::clamp<std::int16_t>(samples[i], -AMP_NORMALIZED,
                                              AMP_NORMALIZED);
    }
    return samples;
}

void ToneGenerate::saveAudio() {
    std::vector<std::int16_t> samples = generateWaveform();

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