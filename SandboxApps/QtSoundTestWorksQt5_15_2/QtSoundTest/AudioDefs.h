#pragma once
// input from microphone
static const int AUDIO_CHANNEL_BYTES_MICROPHONE = 2;    // 2 bytes per sample (sizeof int16)
static const int AUDIO_SAMPLE_BYTES_MICROPHONE = 2;     // 2 bytes per sample * 1 channels
static const int AUDIO_MS_MICROPHONE = 80;              // 80 ms = 0.08 sec of audio data
static const int AUDIO_BUF_SIZE_8000_1_S16 = 1280;      // ( 8000 * 2 bytes per sample * 1 channels ) * 0.08 = 80 ms of audio data
static const double BYTES_TO_MS_MULTIPLIER_MICROPHONE = ( (double)AUDIO_MS_MICROPHONE / (double)AUDIO_BUF_SIZE_8000_1_S16 );

// output to speakers
static const int AUDIO_CHANNEL_BYTES_SPEAKERS = 2;      // 2 bytes per sample (sizeof int16)
static const int AUDIO_SAMPLE_BYTES_SPEAKERS = 4;       // 2 bytes per sample * 2 channels
static const int AUDIO_MS_SPEAKERS = 80;                // 80 ms = 0.08 sec of audio data
static const int AUDIO_BUF_SIZE_48000_2_S16 = 15360;    // ( 48000 * 2 bytes per sample * 2 channels ) * 0.08 = 80 ms of audio data
static const double BYTES_TO_MS_MULTIPLIER_SPEAKERS = ((double)AUDIO_MS_SPEAKERS / (double)AUDIO_BUF_SIZE_48000_2_S16);

// kodi output (float) 960 frames 
static const int AUDIO_CHANNEL_BYTES_KODI = 4;          // 4 bytes per sample (sizeof float)
static const int AUDIO_SAMPLE_BYTES_KODI = 8;           // 4 bytes per sample * 2 channels
static const int AUDIO_MS_KODI = 20;                    // 80 ms = 0.08 sec of audio data
static const int AUDIO_BUF_SIZE_48000_2_FLOAT = 7680;   //19200;    // ( 48000 * 4 bytes per sample * 2 channels ) * 0.02 = 20 ms of audio data

// we hold one buffer in reserve to work with before available to speakers
static const int AUDIO_OUT_CACHE_USABLE_BUF_COUNT = 4;
static const int AUDIO_OUT_CACHE_USABLE_SIZE = ( AUDIO_OUT_CACHE_USABLE_BUF_COUNT * AUDIO_BUF_SIZE_48000_2_S16 );

static const int AUDIO_OUT_CACHE_TOTAL_BUF_COUNT = AUDIO_OUT_CACHE_USABLE_BUF_COUNT + 1;
static const int AUDIO_OUT_CACHE_TOTAL_SIZE = ( AUDIO_OUT_CACHE_TOTAL_BUF_COUNT * AUDIO_BUF_SIZE_48000_2_S16 );

