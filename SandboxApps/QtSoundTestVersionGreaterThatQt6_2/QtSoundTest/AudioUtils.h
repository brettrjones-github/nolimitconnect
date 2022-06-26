
#pragma once

#include <QtCore/qglobal.h>
#include <QDebug>

#include <stdint.h>

class QAudioFormat;

namespace AudioUtils
{
    qint64                      audioDurationUs(const QAudioFormat &format, qint64 bytes);
    qint64                      audioLength(const QAudioFormat &format, qint64 microSeconds);

    QString                     formatToString(const QAudioFormat &format);

    qreal                       nyquistFrequency(const QAudioFormat &format);

    // Scale PCM value to [-1.0, 1.0]
    qreal                       pcmToReal(qint16 pcm);
    float                       pcmToFloat( int16_t pcm );

    // Scale real value in [-1.0, 1.0] to PCM
    qint16                      realToPcm( qreal real );
    int16_t                     floatToPcm( float val );

    // Check whether the audio format is PCM
    bool                        isPCM(const QAudioFormat &format);

    // Check whether the audio format is signed, little-endian, 16-bit PCM
    bool                        isPCMS16LE(const QAudioFormat &format);

//    // Compile-time calculation of powers of two

//    template<int N> class PowerOfTwo
//    { public: static const int Result = PowerOfTwo<N-1>::Result * 2; };

//    template<> class PowerOfTwo<0>
//    { public: static const int Result = 1; };

    void                        mixPcmAudio( int16_t * pcmData,  int16_t * outData, int toMixBytes );

    //  expand pcm Audio data to disired number of samples using a multiplier.. typically PCM 8000Hz Mono Channel to PCM 48000Hz Stereo Channel
    void                        upsamplePcmAudio( int16_t* srcSamples, int16_t prevFrameSample, int origPcmDataLenInBytes, int upResampleMultiplier, int16_t* destSamples, int16_t& lastSampleOfFrame );

    //  contract pcm Audio data to disired number of samples using a divider.. typically PCM 48000Hz Stereo Channel to PCM 8000Hz Mono Channel 
    void                        dnsamplePcmAudio( int16_t* srcSamples, int resampledCnt, int dnResampleDivider, int16_t* destSamples );
    // apply volume to pcm audio
    void                        applyPcmVolume( float volume, uchar* data, int datalen );
    // get peak amplitude of pcm audio (returns 0-100)
    int                         getPeakPcmAmplitude( int16_t* srcSamples, int datalen );

    int                         hasSomeSilence( int16_t* srcSamples, int datalen );

} // namespace AudioUtils
