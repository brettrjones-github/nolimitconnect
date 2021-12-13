#pragma once
static const int AUDIO_BUF_SIZE_48000_2 = 15360;    // ( 48000 * 2 bytes per sample * 2 channels ) * 0.08 = 80 ms of audio data
static const int AUDIO_BUF_SIZE_8000_1 = 1280;      // ( 8000 * 2 bytes per sample * 1 channels ) * 0.08 = 80 ms of audio data
static const int AUDIO_BUF_MS = 80;                 // 80 ms = 0.08 sec of audio data
static const int AUDIO_BUF_COUNT = 4;               // number of buffers for audio cache ( NOTE: one buffer will not be available as it is being written to )
static const int AUDIO_SIZE_DIV_TO_MS_48000_2 = AUDIO_BUF_SIZE_48000_2 / AUDIO_BUF_MS;
static const int AUDIO_OUT_CACHE_SIZE = AUDIO_BUF_SIZE_48000_2 * ( AUDIO_BUF_COUNT - 1 );
static const int AUDIO_SAMPLE_TOTAL_SIZE = 4; // 2 bytes per sample * 2 channels;

