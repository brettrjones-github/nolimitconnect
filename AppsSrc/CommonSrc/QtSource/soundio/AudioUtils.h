
#pragma once

#include <QtCore/qglobal.h>
#include <QDebug>

#include <stdint.h>

class QAudioFormat;

namespace AudioUtils
{
    qint64                      audioDurationUs(const QAudioFormat &format, qint64 bytes);
    int                         audioDurationMs( const QAudioFormat& format, int bytes );
    qint64                      audioLength(const QAudioFormat &format, qint64 microSeconds);
    int                         audioSamplesRequiredForGivenMs( const QAudioFormat& format, qint64 milliSeconds );

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

    //  expand pcm Audio data to desired number of samples using a multiplier.. typically PCM 8000Hz Mono Channel to PCM 48000Hz Stereo Channel
    void                        upsamplePcmAudioLerpPrev( int16_t* srcSamples, int srcSampleCnt, int upResampleMultiplier, int16_t prevFrameSample, int16_t* destSamples );
    //  expand pcm Audio data to desired number of samples using a multiplier.. typically PCM 8000Hz Mono Channel to PCM 48000Hz Stereo Channel
    void                        upsamplePcmAudioLerpNext( int16_t* srcSamples, int srcSampleCnt, int upResampleMultiplier, int16_t nextFrameSample, int16_t* destSamples );

    //  contract pcm Audio data to disired number of samples using a divider.. typically PCM 48000Hz Stereo Channel to PCM 8000Hz Mono Channel 
    void                        dnsamplePcmAudio( int16_t* srcSamples, int resampledCnt, int dnResampleDivider, int16_t* destSamples );
    // apply volume to pcm audio
    void                        applyPcmVolume( float volume, uchar* data, int datalen );
    // get peak amplitude of pcm audio (returns 0-100)
    int                         getPeakPcmAmplitude0to100( int16_t* srcSamples, int datalen );

    int                         hasSomeSilence( int16_t* srcSamples, int datalen );
    int                         countConsecutiveValues( int16_t* srcSamples, int datalen, int minConsecutiveToMatch );

    // interperlate between samples for up sample audio to a higher sample rate
    int16_t                     lerpPcm( int16_t samp1, int16_t samp2, float totalSteps, int thisLerpIdx );

} // namespace AudioUtils
