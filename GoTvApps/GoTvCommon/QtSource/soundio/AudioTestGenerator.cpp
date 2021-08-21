//============================================================================
// Copyright (C) 2018 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include <app_precompiled_hdr.h>
#include "AudioTestGenerator.h"
#include <CoreLib/VxDebug.h>
#include <CoreLib/IsBigEndianCpu.h>

#include <qmath.h>
#include <qendian.h>

//============================================================================
AudioTestGenerator::AudioTestGenerator( const QAudioFormat &format,
                                        qint64 durationUs,
                                        int sampleRate,
                                        QObject *parent )
: QIODevice( parent )
, m_pos( 0 )
{
    if( format.isValid() )
    {
        generateData( format, durationUs, sampleRate );
    }
}

//============================================================================
void AudioTestGenerator::start()
{
    open( QIODevice::ReadOnly );
}

//============================================================================
void AudioTestGenerator::stop()
{
    m_pos = 0;
    close();
}

//============================================================================
void AudioTestGenerator::generateData( const QAudioFormat &format, qint64 durationUs, int sampleRate )
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const int channelBytes = format.sampleSize() / 8;
    
    const int sampleBytes = format.channelCount() * channelBytes;

    qint64 length = ( ( format.sampleRate() * format.channelCount() * channelBytes ) * durationUs ) / 1000000;
    LogMsg( LOG_DEBUG, "generate audio len %lld rate %d channels %d channel bytes %d ms %lld", length, format.sampleRate(), format.channelCount(), channelBytes, durationUs / 1000 );


    Q_ASSERT( length % sampleBytes == 0 );
    Q_UNUSED( sampleBytes ) // suppress warning in release builds

    m_buffer.resize( length );
    unsigned char *ptr = reinterpret_cast< unsigned char * >( m_buffer.data() );

    int sampleIndex = 0;

    while( length ) 
    {
        const qreal x = qSin( 2 * M_PI * sampleRate * qreal( sampleIndex % format.sampleRate() ) / format.sampleRate() );
        for( int i = 0; i < format.channelCount(); ++i ) 
        {
            if( format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt ) 
            {
                const quint8 value = static_cast< quint8 >( ( 1.0 + x ) / 2 * 255 );
                *reinterpret_cast< quint8* >( ptr ) = value;
            }
            else if( format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt ) 
            {
                const qint8 value = static_cast< qint8 >( x * 127 );
                *reinterpret_cast< quint8* >( ptr ) = value;
            }
            else if( format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt ) 
            {
                quint16 value = static_cast< quint16 >( ( 1.0 + x ) / 2 * 65535 );
                if( format.byteOrder() == QAudioFormat::LittleEndian )
                    qToLittleEndian<quint16>( value, ptr );
                else
                    qToBigEndian<quint16>( value, ptr );
            }
            else if( format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt ) 
            {
                qint16 value = static_cast< qint16 >( x * 32767 );
                if( format.byteOrder() == QAudioFormat::LittleEndian )
                    qToLittleEndian<qint16>( value, ptr );
                else
                    qToBigEndian<qint16>( value, ptr );

                //LogMsg( LOG_DEBUG, "gen s16 idx %d val %d", sampleIndex, value );
            }
            else if( format.sampleSize() == 32 && format.sampleType() == QAudioFormat::Float )
            {
                float value = static_cast<float>( x );
                if( format.byteOrder() == QAudioFormat::LittleEndian )
                    qToLittleEndian<float>( value, ptr );
                else
                    qToBigEndian<float>( value, ptr );
                //LogMsg( LOG_DEBUG, "gen float idx %d val %3.3f", sampleIndex, value );
            }

            ptr += channelBytes;
            length -= channelBytes;
        }

        ++sampleIndex;
    }

    if( format.sampleType() == QAudioFormat::Float )
    {
        float * audioData = reinterpret_cast<float *>( m_buffer.data() );
        int sampleCnt = (int)( m_buffer.size() / sizeof( float ) );
        if( audioData && sampleCnt > 2 )
        {
            LogMsg( LOG_DEBUG, "type float first %3.3f second %3.3f last %3.3f ", audioData[ 0 ], audioData[ 2 ], audioData[ sampleCnt - 1 ] );
            sampleCnt = 0;
        }
    }
    else if( ( format.sampleType() == QAudioFormat::SignedInt ) && ( format.sampleSize() == 16 ) )
    {
        qint16 * audioData = reinterpret_cast<qint16 *>( m_buffer.data() );
        int sampleCnt = (int)( m_buffer.size() / sizeof( qint16 ) );
        if( audioData && sampleCnt > 1 )
        {
            LogMsg( LOG_DEBUG, "type int16 first %d second %d last %d ", audioData[ 0 ], audioData[ 1 ], audioData[ sampleCnt - 1 ] );
        }
    }

#else
    const int channelBytes = format.bytesPerSample();
    const int sampleBytes = format.channelCount() * channelBytes;

    qint64 length = ((format.sampleRate() * format.channelCount() * channelBytes) * durationUs) / 1000000;
    LogMsg(LOG_DEBUG, "generate audio len %lld rate %d channels %d channel bytes %d ms %lld", length, format.sampleRate(), format.channelCount(), channelBytes, durationUs / 1000);


    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds

        m_buffer.resize(length);
    unsigned char* ptr = reinterpret_cast<unsigned char*>(m_buffer.data());

    int sampleIndex = 0;

    while (length)
    {
        const qreal x = qSin(2 * M_PI * sampleRate * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
        for (int i = 0; i < format.channelCount(); ++i)
        {
            if (format.bytesPerSample() == 1 && format.sampleFormat() == QAudioFormat::UInt8)
            {
                const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
                *reinterpret_cast<quint8*>(ptr) = value;
            }
            else if (format.bytesPerSample() == 1 && format.sampleFormat() == QAudioFormat::Int16)
            {
                LogMsg(LOG_DEBUG, "generate audio invalid format");

                const qint8 value = static_cast<qint8>(x * 127);
                *reinterpret_cast<quint8*>(ptr) = value;
            }
            else if (format.bytesPerSample() == 2 && format.sampleFormat() == QAudioFormat::Int16)
            {
                qint16 value = static_cast<qint16>(x * 32767);
                if (!IsBigEndianCpu())
                    qToLittleEndian<qint16>(value, ptr);
                else
                    qToBigEndian<qint16>(value, ptr);

                //LogMsg( LOG_DEBUG, "gen s16 idx %d val %d", sampleIndex, value );
            }
            /*
            else if (format.bytesPerSample() == 2 && format.sampleFormat() == QAudioFormat::UnSignedInt)
            {
                quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
                if (!IsBigEndianCpu())
                    qToLittleEndian<quint16>(value, ptr);
                else
                    qToBigEndian<quint16>(value, ptr);
            }
            */
            else if (format.bytesPerSample() == 4 && format.sampleFormat() == QAudioFormat::Int32)
            {
                qint32 value = static_cast<qint32>(x * 2147483647);
                if (!IsBigEndianCpu())
                    qToLittleEndian<qint32>(value, ptr);
                else
                    qToBigEndian<qint32>(value, ptr);

                //LogMsg( LOG_DEBUG, "gen s16 idx %d val %d", sampleIndex, value );
            }
            else if (format.bytesPerSample() == 4 && format.sampleFormat() == QAudioFormat::Float)
            {
                float value = static_cast<float>(x);
                if (!IsBigEndianCpu())
                    qToLittleEndian<float>(value, ptr);
                else
                    qToBigEndian<float>(value, ptr);
                //LogMsg( LOG_DEBUG, "gen float idx %d val %3.3f", sampleIndex, value );
            }

            ptr += channelBytes;
            length -= channelBytes;
        }

        ++sampleIndex;
    }

    if (format.sampleFormat() == QAudioFormat::Float)
    {
        float* audioData = reinterpret_cast<float*>(m_buffer.data());
        int sampleCnt = (int)(m_buffer.size() / sizeof(float));
        if (audioData && sampleCnt > 2)
        {
            LogMsg(LOG_DEBUG, "type float first %3.3f second %3.3f last %3.3f ", audioData[0], audioData[2], audioData[sampleCnt - 1]);
            sampleCnt = 0;
        }
    }
    else if ((format.sampleFormat() == QAudioFormat::Int16) && (format.bytesPerSample() == 2))
    {
        qint16* audioData = reinterpret_cast<qint16*>(m_buffer.data());
        int sampleCnt = (int)(m_buffer.size() / sizeof(qint16));
        if (audioData && sampleCnt > 1)
        {
            LogMsg(LOG_DEBUG, "type int16 first %d second %d last %d ", audioData[0], audioData[1], audioData[sampleCnt - 1]);
        }
    }

#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

//============================================================================
qint64 AudioTestGenerator::readData( char *data, qint64 len )
{
    qint64 total = 0;
    if( !m_buffer.isEmpty() ) 
    {
        while( len - total > 0 ) 
        {
            const qint64 chunk = qMin( ( m_buffer.size() - m_pos ), len - total );
            memcpy( data + total, m_buffer.constData() + m_pos, chunk );
            m_pos = ( m_pos + chunk ) % m_buffer.size();
            total += chunk;
        }
    }

    return total;
}

//============================================================================
qint64 AudioTestGenerator::readDataNoPositionUpdate( char * data, qint64 len )
{
    qint64 total = 0;
    if( !m_buffer.isEmpty() )
    {
        const qint64 chunk = qMin( ( qint64 )m_buffer.size(), len - total );
        memcpy( data + total, m_buffer.constData(), chunk );
        total = chunk;
    }

    return total;
}

//============================================================================
qint64 AudioTestGenerator::readData( QByteArray& retData, qint64 len )
{
    qint64 total = 0;
    if( !m_buffer.isEmpty() )
    {
        while( len - total > 0 )
        {
            const qint64 chunk = qMin( ( m_buffer.size() - m_pos ), len - total );
            memcpy( retData.data() + total, m_buffer.constData() + m_pos, chunk );
            m_pos = ( m_pos + chunk ) % m_buffer.size();
            total += chunk;
        }
    }

    return total;
}

//============================================================================
qint64 AudioTestGenerator::writeData( const char *data, qint64 len )
{
    Q_UNUSED( data );
    Q_UNUSED( len );

    return 0;
}

//============================================================================
qint64 AudioTestGenerator::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}
