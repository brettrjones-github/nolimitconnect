
#pragma once

#include <QtCore/qglobal.h>
#include <QDebug>

#include <stdint.h>

class QAudioFormat;

namespace AudioUtils
{
    qint64                      audioDuration(const QAudioFormat &format, qint64 bytes);
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

}
