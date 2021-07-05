/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "audio_util.h"
//============================================================================
int16_t webrtc::FloatToS16SndSample(float v)
{
    if (v > 0)
        return v >= 1 ? 32767
                      : static_cast<int16_t>(v * 32767 + 0.5f);
    return v <= -1 ? -32768
                   : static_cast<int16_t>(-v * -32768 - 0.5f);
}

//============================================================================
float webrtc::S16ToFloatSndSample(int16_t v)
{
    static const float kMaxInt16Inverse = 1.f / 32767;
    static const float kMinInt16Inverse = 1.f / -32768;
    return v * (v > 0 ? kMaxInt16Inverse : -kMinInt16Inverse);
}

//============================================================================
int16_t webrtc::FloatS16ToS16SndSample(float v)
{
    static const float kMaxRound = 32767 - 0.5f;
    static const float kMinRound = -32768 + 0.5f;
    if (v > 0)
        return v >= kMaxRound ? 32767
                              : static_cast<int16_t>(v + 0.5f);
    return v <= kMinRound ? -32768 : static_cast<int16_t>(v - 0.5f);
}

//============================================================================
float webrtc::FloatToFloatS16SndSample(float v)
{
    return v * (v > 0 ? 32767 : -32768);
}

//============================================================================
float webrtc::FloatS16ToFloatSndSample(float v)
{
    static const float kMaxInt16Inverse = 1.f / 32767;
    static const float kMinInt16Inverse = 1.f / -32768;
    return v * (v > 0 ? kMaxInt16Inverse : -kMinInt16Inverse);
}

//============================================================================
void webrtc::FloatToS16SndArrayCopy(const float* src, int size, int16_t * dest)
{
    for( int i = 0; i < size; i++ )
    {
        dest[i] = webrtc::FloatToS16SndSample( src[ i ] );
    }
}

//============================================================================
void webrtc::S16ToFloatSndArrayCopy(const int16_t * src, int size, float* dest)
{
    for( int i = 0; i < size; i++ )
    {
        dest[i] = webrtc::S16ToFloatSndSample( src[ i ] );
    }
}

//============================================================================
void webrtc::FloatS16ToS16SndArrayCopy( const float * src, int size, int16_t * dest )
{
    for( int i = 0; i < size; i++ )
    {
        dest[i] = webrtc::FloatS16ToS16SndSample( src[ i ] );
    }
}

//============================================================================
void webrtc::FloatToFloatS16SndArrayCopy(const float * src, int size, float * dest)
{
    for( int i = 0; i < size; i++ )
    {
        dest[i] = webrtc::FloatToFloatS16SndSample( src[ i ] );
    }
}

//============================================================================
void webrtc::FloatS16ToFloatSndArrayCopy(const float * src, int size, float * dest)
{
    for( int i = 0; i < size; i++ )
    {
        dest[i] = webrtc::FloatS16ToFloatSndSample( src[ i ] );
    }
}

