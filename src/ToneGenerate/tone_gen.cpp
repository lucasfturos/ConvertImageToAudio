#include "tone_gen.hpp"

ToneGenerate::ToneGenerate(const std::string &filename, int format,
                           int channels)
    : m_filename(filename), fft_ptr(std::make_shared<FFT<>>()) {

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

void ToneGenerate::setImageData(std::vector<Complex<>> imageData) {
    m_imageData = imageData;
    m_frequencyData.resize(imageData.size(), 0);
    fft_ptr->fftAnalyze(imageData, m_frequencyData, imageData.size());
}

std::vector<std::int16_t> ToneGenerate::generateWaveform() {
    std::vector<std::int16_t> samples;
    for (const auto &value : m_frequencyData) {
        double sample = std::abs(value);
        samples.push_back(static_cast<std::int16_t>(sample * AMP_NORMALIZED));
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