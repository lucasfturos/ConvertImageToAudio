#include "alsa_player.hpp"

ALSAPlayer::ALSAPlayer() : handle(nullptr), stream(SND_PCM_STREAM_PLAYBACK) {}

ALSAPlayer::~ALSAPlayer() {
    if (handle) {
        snd_pcm_close(handle);
    }
}

int ALSAPlayer::initialize(int samplerate, int channels) {
    int erro = snd_pcm_open(&handle, "default", stream, 0);
    if (erro < 0) {
        std::cerr << "Error opening ALSA device: " << snd_strerror(erro)
                  << '\n';
        return erro;
    }

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate(handle, params, samplerate, 0);
    snd_pcm_hw_params_set_channels(handle, params, channels);

    erro = snd_pcm_hw_params(handle, params);
    if (erro < 0) {
        std::cerr << "Error configuring ALSA parameters: " << snd_strerror(erro)
                  << '\n';
        return erro;
    }

    snd_pcm_sw_params_malloc(&swParams);
    snd_pcm_sw_params_current(handle, swParams);
    snd_pcm_sw_params_set_start_threshold(handle, swParams, 0U);
    snd_pcm_sw_params_set_stop_threshold(handle, swParams, 0U);

    erro = snd_pcm_sw_params(handle, swParams);
    if (erro < 0) {
        std::cerr << "Error configuring ALSA software parameters: "
                  << snd_strerror(erro) << '\n';
        return erro;
    }

    return 0;
}

int ALSAPlayer::play(char *buffer, int size) {
    int written = snd_pcm_writei(handle, buffer, size);
    if (written < 0) {
        written = snd_pcm_recover(handle, written, 0);
        if (written < 0) {
            std::cerr << "Error during playback: " << snd_strerror(written)
                      << '\n';
            return written;
        }
    }
    return written;
}

int ALSAPlayer::stop() {
    if (handle) {
        snd_pcm_drop(handle);
    }
    return 0;
}
