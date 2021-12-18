


#include <QAudioFormat>
#include "AudioUtils.h"
#include <CoreLib/IsBigEndianCpu.h>

qint64 AudioUtils::audioDuration(const QAudioFormat &format, qint64 bytes)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    return (bytes * 1000000) /
        (format.sampleRate() * format.channelCount() * format.bytesPerSample());
#else
    return (bytes * 1000000) /
        (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8));
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

qint64 AudioUtils::audioLength(const QAudioFormat &format, qint64 microSeconds)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    qint64 result = (format.sampleRate() * format.channelCount() * format.bytesPerSample())
        * microSeconds / 1000000;
    result -= result % (format.channelCount() * format.bytesPerSample() * 8);
    return result;
#else
   qint64 result = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
       * microSeconds / 1000000;
   result -= result % (format.channelCount() * format.sampleSize());
   return result;
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

qreal AudioUtils::nyquistFrequency(const QAudioFormat &format)
{
    return format.sampleRate() / 2;
}

QString AudioUtils::formatToString(const QAudioFormat &format)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QString result;

    if (QAudioFormat() != format) {

        Q_ASSERT(format.sampleFormat() == QAudioFormat::Int16);

        const QString formatEndian = !IsBigEndianCpu()
            ? QString("LE") : QString("BE");

        QString formatType;
        switch (format.sampleFormat()) {
        case QAudioFormat::Int16:
        case QAudioFormat::Int32:
            formatType = "signed";
            break;
        case QAudioFormat::UInt8:
            formatType = "unsigned";
            break;
        case QAudioFormat::Float:
            formatType = "float";
            break;
        case QAudioFormat::Unknown:
        default:
            formatType = "unknown";
            break;
        }

        QString formatChannels = QString("%1 channels").arg(format.channelCount());
        switch (format.channelCount()) {
        case 1:
            formatChannels = "mono";
            break;
        case 2:
            formatChannels = "stereo";
            break;
        }

        result = QString("%1 Hz %2 bytes %3 %4 %5")
            .arg(format.sampleRate())
            .arg(format.bytesPerSample())
            .arg(formatType)
            .arg(formatEndian)
            .arg(formatChannels);
    }

    return result;
#else
    QString result;

    if (QAudioFormat() != format) {
        if (format.codec() == "audio/pcm") {
            Q_ASSERT(format.sampleType() == QAudioFormat::SignedInt);

            const QString formatEndian = (format.byteOrder() == QAudioFormat::LittleEndian)
                ?   QString("LE") : QString("BE");

            QString formatType;
            switch (format.sampleType()) {
            case QAudioFormat::SignedInt:
                formatType = "signed";
                break;
            case QAudioFormat::UnSignedInt:
                formatType = "unsigned";
                break;
            case QAudioFormat::Float:
                formatType = "float";
                break;
            case QAudioFormat::Unknown:
                formatType = "unknown";
                break;
            }

            QString formatChannels = QString("%1 channels").arg(format.channelCount());
            switch (format.channelCount()) {
            case 1:
                formatChannels = "mono";
                break;
            case 2:
                formatChannels = "stereo";
                break;
            }

            result = QString("%1 Hz %2 bit %3 %4 %5")
                .arg(format.sampleRate())
                .arg(format.sampleSize())
                .arg(formatType)
                .arg(formatEndian)
                .arg(formatChannels);
        } else {
            result = format.codec();
        }
    }

    return result;
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

bool AudioUtils::isPCM(const QAudioFormat &format)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    return format.sampleFormat() == QAudioFormat::Int16;
#else
    return (format.codec() == "audio/pcm");
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}


bool AudioUtils::isPCMS16LE(const QAudioFormat &format)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    return isPCM(format) && !IsBigEndianCpu();
#else
    return isPCM(format) &&
           format.sampleType() == QAudioFormat::SignedInt &&
           format.sampleSize() == 16 &&
           format.byteOrder() == QAudioFormat::LittleEndian;
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

const qint16  PCMS16MaxValue     =  32767;
const quint16 PCMS16MaxAmplitude =  32768; // because minimum is -32768

qreal AudioUtils::pcmToReal(qint16 pcm)
{
    return qreal(pcm) / PCMS16MaxAmplitude;
}

float AudioUtils::pcmToFloat( int16_t pcm )
{
    return float(pcm) / PCMS16MaxAmplitude;
}


qint16 AudioUtils::realToPcm(qreal real)
{
    return real * PCMS16MaxValue;
}

int16_t AudioUtils::floatToPcm( float val )
{
    return val * PCMS16MaxValue;
}

int16_t MixPcmSample( int a, int b ) // int16_t sample1, int16_t sample2 ) 
{
    // from stack overflow

#if 0
    // averaging algorithum
    const int32_t result( ( static_cast<int32_t>( sample1 ) + static_cast<int32_t>( sample2 ) ) / 2 );
    typedef std::numeric_limits<int16_t> Range;
    if( Range::max() < result )
        return Range::max();
    else if( Range::min() > result )
        return Range::min();
    else
        return result;
#else
    int m; // mixed result will go here
    // Make both samples unsigned (0..65535)
    a += 32768;
    b += 32768;

    // Pick the equation
    if( ( a < 32768 ) || ( b < 32768 ) ) {
        // Viktor's first equation when both sources are "quiet"
        // (i.e. less than middle of the dynamic range)
        m = a * b / 32768;
    }
    else {
        // Viktor's second equation when one or both sources are loud
        m = 2 * ( a + b ) - ( a * b ) / 32768 - 65536;
    }

    // Output is unsigned (0..65536) so convert back to signed (-32768..32767)
    if( m == 65536 ) m = 65535;
    m -= 32768;

    return (int16_t)m;
#endif // 0
}

void AudioUtils::mixPcmAudio( int16_t * pcmData, int16_t * outData, int toMixBytes )
{
    int sampleCnt = toMixBytes / 2;
    if( sampleCnt )
    {
        for( int i = 0; i < sampleCnt; i++ )
        {
            outData[i] = MixPcmSample( pcmData[ i ], outData[ i ] );
        }
    }
}
