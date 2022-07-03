
#include <QAudioFormat>
#include <QtEndian>
#include "AudioUtils.h"
#include "IsBigEndianCpu.h"
#include "VxDebug.h"

//=============================================================================
qint64 AudioUtils::audioDurationUs(const QAudioFormat &format, qint64 bytes)
{
    return (bytes * 1000000) /
        (format.sampleRate() * format.channelCount() * format.bytesPerSample());
}

//=============================================================================
qint64 AudioUtils::audioLength(const QAudioFormat &format, qint64 microSeconds)
{
    qint64 result = (format.sampleRate() * format.channelCount() * format.bytesPerSample())
        * microSeconds / 1000000;
    result -= result % (format.channelCount() * format.bytesPerSample() * 8);
    return result;
}

//=============================================================================
qreal AudioUtils::nyquistFrequency(const QAudioFormat &format)
{
    return format.sampleRate() / 2;
}

//=============================================================================
QString AudioUtils::formatToString(const QAudioFormat &format)
{
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
}

//=============================================================================
bool AudioUtils::isPCM(const QAudioFormat &format)
{
    return format.sampleFormat() == QAudioFormat::Int16;
}

//=============================================================================
bool AudioUtils::isPCMS16LE(const QAudioFormat &format)
{
    return isPCM(format) && !IsBigEndianCpu();
}

const qint16  PCMS16MaxValue     =  32767;
const quint16 PCMS16MaxAmplitude =  32768; // because minimum is -32768

//=============================================================================
qreal AudioUtils::pcmToReal(qint16 pcm)
{
    return qreal(pcm) / PCMS16MaxAmplitude;
}

//=============================================================================
float AudioUtils::pcmToFloat( int16_t pcm )
{
    return float(pcm) / PCMS16MaxAmplitude;
}

//=============================================================================
qint16 AudioUtils::realToPcm(qreal real)
{
    return real * PCMS16MaxValue;
}

//=============================================================================
int16_t AudioUtils::floatToPcm( float val )
{
    return val * PCMS16MaxValue;
}

//=============================================================================
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

//=============================================================================
void AudioUtils::mixPcmAudio( int16_t* pcmData, int16_t* outData, int toMixBytes )
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

//=============================================================================
void AudioUtils::upsamplePcmAudioLerpPrev( int16_t* srcSamples, int srcSampleCnt, int upResampleMultiplier, int16_t prevFrameSample, int16_t* destSamples )
{
    int16_t firstSample = prevFrameSample;
    int16_t secondSample;
    float sampleUpMult = (float)upResampleMultiplier;
    float sampleStep;
    int iDestIdx = 0;
    for( int i = 0; i < srcSampleCnt; i++ )
    {
        secondSample = srcSamples[ i ];
        if( secondSample >= firstSample )
        {
            // ramp up
            sampleStep = ((secondSample - firstSample) / sampleUpMult);
        }
        else
        {
            // ramp down
            sampleStep = -((firstSample - secondSample) / sampleUpMult);
        }

        if( 0.0f == sampleStep )
        {
            for( int j = 0; j < upResampleMultiplier; ++j )
            {
                destSamples[ iDestIdx ] = firstSample;
                iDestIdx++;
            }
        }
        else
        {
            float sampleOffs = sampleStep;
            for( int j = 0; j < upResampleMultiplier; ++j )
            {
                destSamples[ iDestIdx ] = (int16_t)(firstSample + sampleOffs);
                iDestIdx++;
                sampleOffs += sampleStep;
            }
        }

        firstSample = secondSample;
    }
}

//=============================================================================
void AudioUtils::upsamplePcmAudioLerpNext( int16_t* srcSamples, int srcSampleCnt, int upResampleMultiplier, int16_t nextFrameSample, int16_t* destSamples )
{
    int16_t firstSample = srcSamples[ 0 ];
    int16_t secondSample;
    float sampleUpMult = (float)upResampleMultiplier;
    float sampleStep;
    int iDestIdx = 0;
    for( int i = 1; i <= srcSampleCnt; i++ )
    {
        secondSample = i == srcSampleCnt ? nextFrameSample : srcSamples[ i ];

        if( secondSample >= firstSample )
        {
            // ramp up
            sampleStep = ((secondSample - firstSample) / sampleUpMult);
        }
        else
        {
            // ramp down
            sampleStep = -((firstSample - secondSample) / sampleUpMult);
        }

        if( 0.0f == sampleStep )
        {
            for( int j = 0; j < upResampleMultiplier; ++j )
            {
                destSamples[ iDestIdx ] = firstSample;
                iDestIdx++;
            }
        }
        else
        {
            float sampleOffs = sampleStep;
            for( int j = 0; j < upResampleMultiplier; ++j )
            {
                destSamples[ iDestIdx ] = (int16_t)(firstSample + sampleOffs);
                iDestIdx++;
                sampleOffs += sampleStep;
            }
        }

        firstSample = secondSample;
    }
}

//=============================================================================
void AudioUtils::dnsamplePcmAudio( int16_t* srcSamples, int resampledCnt, int dnResampleDivider, int16_t* destSamples )
{
    int16_t* pcmData = (int16_t*)srcSamples;
    for( int i = 0; i < resampledCnt; i++ )
    {
        destSamples[ i ] = pcmData[ i * dnResampleDivider ];
    }
}

//============================================================================
void AudioUtils::applyPcmVolume( float volume, uchar *data, int datalen )
{
    volume = volume / 100;
    int samples = datalen / 2;
    float mult = pow( 10.0, 0.05*volume );

    for( int i = 0; i < samples; i++ ) 
    {
        qint16 val = qFromLittleEndian<qint16>( data + i * 2 )*mult;
        qToLittleEndian<qint16>( val, data + i * 2 );
    }
}

//============================================================================
int AudioUtils::getPeakPcmAmplitude( int16_t* srcSamples, int datalen )
{
    int samples = datalen / 2;
    int peakValue{ 0 };
    for( int i = 0; i < samples; i++ )
    {
        if( srcSamples[ i ] > peakValue )
        {
            peakValue = srcSamples[ i ];
        }
    }

    if( peakValue )
    {
        return (int)(((float)peakValue / 32768.0f ) * 100);
    }
    
    return 0;
}

//============================================================================
int AudioUtils::hasSomeSilence( int16_t* srcSamples, int datalen )
{
    int samples = datalen / 2;
    int lastSample = 0;
    int silenceSamples = 0;
    for( int i = 0; i < samples; i++ )
    {
        if( srcSamples[ i ] == 0  && lastSample == 0 )
        {
            silenceSamples++;
        }

        lastSample = srcSamples[ i ];
    }

    return silenceSamples > 4 ? silenceSamples : 0;
}


//============================================================================
int AudioUtils::countConsecutiveValues( int16_t* srcSamples, int datalen, int minConsecutiveToMatch )
{
    int samples = datalen / 2;
    int lastSample = srcSamples[ 0 ];
    int consecutiveInARow = 0;
    int consecutiveTotalCnt = 0;
    for( int i = 1; i < samples; i++ )
    {
        if( srcSamples[ i ] == lastSample )
        {
            consecutiveInARow++;
            if( consecutiveInARow >= minConsecutiveToMatch )
            {
                consecutiveTotalCnt++;
            }
        }
        else
        {
            consecutiveInARow = 0;
        }

        lastSample = srcSamples[ i ];
    }

    return consecutiveTotalCnt;
}


//============================================================================
// interperlate between samples for up sample audio to a higher sample rate
// example of lerped samples for 12 as upsample multiplier and sample 1 = 0 and sample 2 = 1200 
// (first number is index and second is the lerped value)
// 0 (0), 1 (100), 2 (200), 3 (300), 4 (400), 5 (500), 6 (600), 7 (700), 8 (800), 9 (900), 10 (1000), 11 (1100), 12 (1200)
int16_t AudioUtils::lerpPcm( int16_t samp1, int16_t samp2, float totalSteps, int thisLerpIdx )
{
    float sampleStep = samp1 >= samp2 ? ((samp2 - samp1) / totalSteps) : -((samp1 - samp2) / totalSteps);

    if( 0.0f == sampleStep )
    {
        return samp1;
    }
    else
    {
        return (int16_t)(samp1 + thisLerpIdx * sampleStep);
    }
}
