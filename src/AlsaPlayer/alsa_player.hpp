#pragma once

#include <alsa/asoundlib.h>
#include <iostream>
#include <sndfile.h>

class ALSAPlayer {
  public:
    ALSAPlayer();
    ~ALSAPlayer();

    int initialize(int samplerate, int channels);
    int play(char *buffer, int size);
    int stop();

  private:
    snd_pcm_t *handle;
    snd_pcm_stream_t stream;
    snd_pcm_hw_params_t *params;
    snd_pcm_sw_params_t *swParams;
};
