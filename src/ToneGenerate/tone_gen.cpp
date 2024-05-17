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

void ToneGenerate::setImageData(std::vector<Complex<>> imageData, int width,
                                int height) {
    m_imageData = imageData;
    m_imageWidth = width;
    m_imageHeight = height;
}

std::vector<std::int16_t> ToneGenerate::generateWaveform() {
    int numSamples = SAMPLE_RATE;
    double gain = 8.0;
    double stepSize =
        (MAX_FREQ - MIN_FREQ) / static_cast<double>(m_imageHeight);

    std::vector<std::int16_t> samples(numSamples, 0);
    for (int frame = 0; frame < numSamples; ++frame) {
        double t = static_cast<double>(frame) / SAMPLE_RATE;
        double signalValue = 0.0;
        for (int row = 0; row < m_imageHeight; ++row) {
            int invertedRow = m_imageHeight - 1 - row;
            int col = (frame * m_imageWidth) / numSamples;
            double intensity =
                std::abs(m_imageData[invertedRow * m_imageWidth + col]) * gain;
            double frequency = row * stepSize;
            signalValue += intensity * std::sin(2.0 * pi * frequency * t);
        }
        samples[frame] = static_cast<int16_t>((signalValue / m_imageHeight) *
                                              AMP_NORMALIZED);
    }
    return samples;
}

void ToneGenerate::saveAudio() {
    std::vector<std::int16_t> samples = generateWaveform();

    SF_INFO info;
    info.samplerate = SAMPLE_RATE;
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