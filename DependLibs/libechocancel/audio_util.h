/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_COMMON_AUDIO_INCLUDE_AUDIO_UTIL_H_
#define WEBRTC_COMMON_AUDIO_INCLUDE_AUDIO_UTIL_H_

#include <CoreLib/VxDefs.h>

#include <limits>
#include <cstring>

#include "checks.h"
#include "WebRtcTypeDefs.h"

typedef std::numeric_limits<int16_t> limits_int16;
namespace webrtc
{
// The conversion functions use the following naming convention:
// int16_t:      int16_t [-32768, 32767]
// Float:    float   [-1.0, 1.0]
// FloatS16: float   [-32768.0, 32767.0]
    int16_t   FloatToS16SndSample(float v);
    float     S16ToFloatSndSample(int16_t v);
    int16_t   FloatS16ToS16SndSample(float v);
    float     FloatToFloatS16SndSample(float v);
    float     FloatS16ToFloatSndSample(float v);

    void FloatToS16SndArrayCopy( const float * src, int size, int16_t * dest );
    void S16ToFloatSndArrayCopy( const int16_t * src, int size, float * dest );
    void FloatS16ToS16SndArrayCopy( const float * src, int size, int16_t * dest );
    void FloatToFloatS16SndArrayCopy( const float * src, int size, float * dest );
    void FloatS16ToFloatSndArrayCopy( const float * src, int size, float * dest );

// Copy audio from |src| channels to |dest| channels unless |src| and |dest|
// point to the same address. |src| and |dest| must have the same number of
// channels, and there must be sufficient space allocated in |dest|.
template <typename T>
void CopyAudioIfNeeded(const T* const* src,
                       int num_frames,
                       int num_channels,
                       T* const* dest) {
  for (int i = 0; i < num_channels; ++i) {
    if (src[i] != dest[i]) {
      std::copy(src[i], src[i] + num_frames, dest[i]);
    }
  }
}

// Deinterleave audio from |interleaved| to the channel buffers pointed to
// by |deinterleaved|. There must be sufficient space allocated in the
// |deinterleaved| buffers (|num_channel| buffers with |samples_per_channel|
// per buffer).
template <typename T>
void Deinterleave(const T* interleaved,
                  size_t samples_per_channel,
                  size_t num_channels,
                  T* const* deinterleaved) {
  for (size_t i = 0; i < num_channels; ++i) {
    T* channel = deinterleaved[i];
    size_t interleaved_idx = i;
    for (size_t j = 0; j < samples_per_channel; ++j) {
      channel[j] = interleaved[interleaved_idx];
      interleaved_idx += num_channels;
    }
  }
}

// Interleave audio from the channel buffers pointed to by |deinterleaved| to
// |interleaved|. There must be sufficient space allocated in |interleaved|
// (|samples_per_channel| * |num_channels|).
template <typename T>
void Interleave(const T* const* deinterleaved,
                size_t samples_per_channel,
                size_t num_channels,
                T* interleaved) {
  for (size_t i = 0; i < num_channels; ++i) {
    const T* channel = deinterleaved[i];
    size_t interleaved_idx = i;
    for (size_t j = 0; j < samples_per_channel; ++j) {
      interleaved[interleaved_idx] = channel[j];
      interleaved_idx += num_channels;
    }
  }
}

// Copies audio from a single channel buffer pointed to by |mono| to each
// channel of |interleaved|. There must be sufficient space allocated in
// |interleaved| (|samples_per_channel| * |num_channels|).
template <typename T>
void UpmixMonoToInterleaved(const T* mono,
                            int num_frames,
                            int num_channels,
                            T* interleaved) {
  int interleaved_idx = 0;
  for (int i = 0; i < num_frames; ++i) {
    for (int j = 0; j < num_channels; ++j) {
      interleaved[interleaved_idx++] = mono[i];
    }
  }
}

template <typename T, typename Intermediate>
void DownmixToMono(const T* const* input_channels,
                   size_t num_frames,
                   int num_channels,
                   T* out) {
  for (size_t i = 0; i < num_frames; ++i) {
    Intermediate value = input_channels[0][i];
    for (int j = 1; j < num_channels; ++j) {
      value += input_channels[j][i];
    }
    out[i] = value / num_channels;
  }
}

// Downmixes an interleaved multichannel signal to a single channel by averaging
// all channels.
template <typename T, typename Intermediate>
void DownmixInterleavedToMonoImpl(const T* interleaved,
                                  size_t num_frames,
                                  int num_channels,
                                  T* deinterleaved) {
  RTC_DCHECK_GT(num_channels, 0);
  RTC_DCHECK_GT(num_frames, 0u);

  const T* const end = interleaved + num_frames * num_channels;

  while (interleaved < end) {
    const T* const frame_end = interleaved + num_channels;

    Intermediate value = *interleaved++;
    while (interleaved < frame_end) {
      value += *interleaved++;
    }

    *deinterleaved++ = value / num_channels;
  }
}

template <typename T>
void DownmixInterleavedToMono(const T* interleaved,
                              size_t num_frames,
                              int num_channels,
                              T* deinterleaved);

template <>
void DownmixInterleavedToMono<int16_t>(const int16_t* interleaved,
                                       size_t num_frames,
                                       int num_channels,
                                       int16_t* deinterleaved);

};  // namespace webrtc

#endif  // WEBRTC_COMMON_AUDIO_INCLUDE_AUDIO_UTIL_H_
