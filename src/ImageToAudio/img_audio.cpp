#include "img_audio.hpp"
#include "AlsaPlayer/alsa_player.hpp"
#include "Common/constants.hpp"
#include "Common/threadpool.hpp"
#include <chrono>

ImageToAudio::ImageToAudio(const std::string &filename, int format,
                           int channels, double gain, double duration)
    : m_filename(filename), m_format(format), m_channels(channels),
      m_gain(gain), m_duration(duration) {}

void ImageToAudio::setup() {
    std::string formatName = (m_format == SF_FORMAT_WAV)    ? "WAV"
                             : (m_format == SF_FORMAT_FLAC) ? "FLAC"
                             : (m_format == SF_FORMAT_OGG)  ? "OGG"
                                                            : "Unknown";
    if (formatName.compare("Unknown") == 0) {
        throw std::invalid_argument("Invalid audio format.");
    }

    std::string channelName = (m_channels == 1)   ? "MONO"
                              : (m_channels == 2) ? "STEREO"
                                                  : "Invalid";
    if (channelName.compare("Invalid") == 0) {
        throw std::invalid_argument("Number of channels must be 1 or 2.");
    }

    if (m_gain == 0) {
        throw std::invalid_argument("The gain cannot be zero");
    }

    if (m_duration > 100) {
        throw std::invalid_argument("Maximum audio duration is 100s");
    }
    if (m_duration == 0) {
        m_duration = 1;
    }
}

void ImageToAudio::setImageData(std::vector<double> imageData,
                                Dimension imageSize) {
    m_imageData = std::move(imageData);
    m_imageWidth = imageSize.width;
    m_imageHeight = imageSize.height;
    if (static_cast<int>(m_imageData.size()) != m_imageWidth * m_imageHeight) {
        throw std::invalid_argument(
            "ImageData size does not match image dimensions.");
    }
}

std::vector<double> ImageToAudio::calculateFrequencies() const {
    std::vector<double> frequencies(m_imageHeight);
    double stepSize =
        (MAX_FREQ - MIN_FREQ) / static_cast<double>(m_imageHeight - 1);
    std::iota(frequencies.begin(), frequencies.end(), 0);
    std::transform(
        frequencies.begin(), frequencies.end(), frequencies.begin(),
        [stepSize](double index) { return MIN_FREQ + index * stepSize; });
    return frequencies;
}

void ImageToAudio::processSamples(std::vector<std::int16_t> &samples, int start,
                                  int end,
                                  const std::vector<double> &frequencies) {
    for (int frame = start; frame < end; ++frame) {
        double t = static_cast<double>(frame) / SAMPLE_RATE;
        double signalValue = 0.0;
        int col = (frame * m_imageWidth) / samples.size();
        for (int row = 0; row < m_imageHeight; ++row) {
            int invertedRow = m_imageHeight - 1 - row;
            double intensity =
                m_imageData[invertedRow * m_imageWidth + col] * m_gain;
            double frequency = frequencies[row];
            signalValue += intensity * std::sin(2.0 * PI * frequency * t);
        }
        samples[frame] = static_cast<int16_t>((signalValue / m_imageHeight) *
                                              AMP_NORMALIZED);
    }
}

std::vector<std::int16_t> ImageToAudio::createAudioSamples() {
    int numSamples = m_duration * SAMPLE_RATE;
    auto frequencies = calculateFrequencies();
    std::vector<std::int16_t> samples(numSamples, 0);

    const int numThreads = std::thread::hardware_concurrency();
    auto threadpool = std::make_unique<ThreadPool>(numThreads);
    int blockSize = numSamples / numThreads;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < numThreads; ++i) {
        int start = i * blockSize;
        int end = i == numThreads ? numSamples : start + blockSize;
        futures.emplace_back(threadpool->enqueue(
            std::bind(&ImageToAudio::processSamples, this, std::ref(samples),
                      start, end, std::cref(frequencies))));
    }
    for (auto &future : futures) {
        future.get();
    }
    return samples;
}

void ImageToAudio::saveAudio() {
    auto samples = createAudioSamples();

    SF_INFO info;
    info.samplerate = SAMPLE_RATE;
    info.channels = m_channels;
    info.format = m_format | SF_FORMAT_PCM_16;
    if (m_filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty.");
    }
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

void ImageToAudio::playAudio() {
    ALSAPlayer player;
    player.initialize(SAMPLE_RATE, m_channels);

    auto samples = createAudioSamples();
    std::vector<char> buffer(samples.size() * sizeof(int16_t));

    std::copy(reinterpret_cast<char *>(samples.data()),
              reinterpret_cast<char *>(samples.data()) + buffer.size(),
              buffer.data());

    std::size_t bufferSize = 4096;
    std::size_t offset = 0;

    while (offset < buffer.size()) {
        std::size_t chunkSize = std::min(bufferSize, buffer.size() - offset);
        player.play(buffer.data() + offset, chunkSize);
        offset += chunkSize;

        std::this_thread::sleep_for(std::chrono::milliseconds(
            (chunkSize / sizeof(int16_t)) * 1000 / SAMPLE_RATE));
    }
    player.stop();
}
