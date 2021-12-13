
#include "AudioBuffer.h"
#include <CoreLib/VxDebug.h>
#include <algorithm>
#include <memory.h>

 AudioBuffer::AudioBuffer()
 {
     memset( m_DataSize, 0, sizeof( m_DataSize ) );
 }

// write to buffer.. return amount written
int AudioBuffer::writeData( char * data, int dataLen )
{
    if( !data || ( dataLen <= 0 ) )
    {
        LogMsg( LOG_ERROR, " AudioBuffer::writeData invalid dataLen %d\n", dataLen );
        return 0;
    }

    int lenLeftToWrite = dataLen;
    int totalWrote = 0;
    m_BufMutex.lock();
    while( totalWrote != dataLen )
    {
        autoIncrementHeadIndex();
        int bytesWritten = writeBufData( &data[totalWrote], lenLeftToWrite );
        totalWrote += bytesWritten;
        lenLeftToWrite -= bytesWritten;
        if( 0 == bytesWritten )
        {
            break;
        }
    }

    m_BufMutex.unlock();
    return totalWrote;
}

// read from buffer.. return amount read
int AudioBuffer::readData( char * data, int maxReadlen )
{
    if( !data || ( 0 >= maxReadlen) )
    {
        return 0;
    }

    int totalBytesRead = 0;
    m_BufMutex.lock();
    while( totalBytesRead != maxReadlen  )
    {
        int bytesRead = readBufData( &data[ totalBytesRead ], maxReadlen - totalBytesRead );
        if( 0 == bytesRead )
        {
            // there may be a bit left in current head buffer
            int leftOverAmount = m_DataSize[ m_HeadIdx ];
            if( leftOverAmount )
            {
                // see if tail is right behind head.. meaning buffers are about empty
                int tailIdx = m_TailIdx;
                incrementIndex( tailIdx );
                if( tailIdx == m_HeadIdx )
                {
                    // we can read out what is left in head buffer
                    int lenToRead = std::min( leftOverAmount, maxReadlen - totalBytesRead );
                    if( lenToRead )
                    {
                        memcpy( &data[ totalBytesRead ], &m_RingBuf[ m_HeadIdx ][ 0 ], lenToRead );
                        if( lenToRead != leftOverAmount )
                        {
                            // move remaining data to start of buffer
                            memcpy( &m_RingBuf[ m_HeadIdx ][ 0 ], &m_RingBuf[ m_HeadIdx ][ lenToRead ], leftOverAmount - lenToRead );
                        }

                        m_DataSize[ m_HeadIdx ] -= lenToRead;
                        bytesRead = lenToRead;
                    }
                }
            }

            if( 0 == bytesRead )
            {
                break;
            }
        }

        totalBytesRead += bytesRead;
    }

    m_BufMutex.unlock();
    return totalBytesRead;
}

// amount of data available in bytes
int AudioBuffer::bytesAvailable()
{
    int amountAvailable = 0;
    m_BufMutex.lock();
    autoIncrementTailIndex();
    int currIdx = m_TailIdx;
    for( int i = 0; i < AUDIO_BUF_COUNT; i++ )
    {
        int amountInBuf = getBytesAvailable( currIdx );
        if( 0 == amountInBuf )
        {
            break;
        }

        amountAvailable += amountInBuf;

        incrementIndex( currIdx );
        if( currIdx == m_HeadIdx )
        {
            // include amount in buffer currently being written t0
            amountAvailable += m_DataSize[ m_HeadIdx ];
            break;
        }
    }

    m_BufMutex.unlock(); 
    return amountAvailable;
}

// increment index.. return true if tail == head index
bool AudioBuffer::incrementIndex( int& bufIdx )
{
    if(  m_HeadIdx == m_TailIdx )
    {
        return true;
    }

    bufIdx++;
    if( bufIdx >= AUDIO_BUF_COUNT )
    {
        bufIdx = 0;
    }

    return m_HeadIdx == m_TailIdx;
}

// increment head index.. do not allow head to reach tail
bool AudioBuffer::incrementHeadIndex( int& bufIdx )
{
    bool incrementSuccess = false;
    int nextIdx = bufIdx;
    nextIdx++;
    if( nextIdx >= AUDIO_BUF_COUNT )
    {
        nextIdx = 0;
    }

    if( nextIdx != m_TailIdx )
    {
        bufIdx = nextIdx;
        incrementSuccess = true;
    }
 
    return incrementSuccess;
}

// increment head index if needed
void AudioBuffer::autoIncrementHeadIndex( )
{
    if( 0 == getBytesFree( m_HeadIdx ) )
    {
        incrementHeadIndex( m_HeadIdx );
    }
}

// increment tail index.. do not allow tail to reach head.. return true if incremented
bool AudioBuffer::incrementTailIndex( int& bufIdx )
{
    bool incrementSuccess = false;
    int nextIdx = bufIdx;
    nextIdx++;
    if( nextIdx >= AUDIO_BUF_COUNT )
    {
        nextIdx = 0;
    }

    if( nextIdx != m_HeadIdx )
    {
        bufIdx = nextIdx;
        incrementSuccess = true;
    }

    return incrementSuccess;
}

// increment head index if needed
void AudioBuffer::autoIncrementTailIndex()
{
    if( 0 == getBytesAvailable( m_TailIdx ) )
    {
        incrementTailIndex( m_TailIdx );
    }
}

// return bytes available at given index
int AudioBuffer::getBytesAvailable( int bufIdx )
{
    return m_DataSize[ bufIdx ];
}

// return bytes free at given index
int AudioBuffer::getBytesFree( int bufIdx )
{
    return AUDIO_BUF_SIZE_48000_2 - getBytesAvailable( bufIdx );
}

// write into buffer.. return amount wrote
int AudioBuffer::writeBufData( char * data, int dataLen )
{
    int lenWrote = 0;
    if( data && dataLen )
    {
        autoIncrementHeadIndex();
        int spaceAvailable = getBytesFree( m_HeadIdx );
        if( spaceAvailable )
        {
            int spaceUsed = getBytesAvailable( m_HeadIdx );
            int lenToWrite = std::min( spaceAvailable, dataLen );
            if( lenToWrite )
            {
                memcpy( &m_RingBuf[ m_HeadIdx ][ spaceUsed ], data, lenToWrite );
                m_DataSize[ m_HeadIdx ] += lenToWrite;
                lenWrote = lenToWrite;
                autoIncrementHeadIndex();
            }
        }
    }

    return lenWrote;
}

// read from buffer.. return amount read
int AudioBuffer::readBufData( char * data, int dataLen )
{
    int lenRead = 0;
    if( data && dataLen )
    {
        autoIncrementTailIndex();
        int bytesAvail = getBytesAvailable( m_TailIdx );
        if( 0 != bytesAvail )
        {
            int lenToRead = std::min( bytesAvail, dataLen );
            if( lenToRead )
            {
                memcpy( data, &m_RingBuf[ m_TailIdx ][ 0 ], lenToRead );
                if( lenToRead != bytesAvail )
                {
                    // move remaining data to start of buffer
                    memcpy( &m_RingBuf[ m_TailIdx ][ 0 ], &m_RingBuf[ m_TailIdx ][ lenToRead ], bytesAvail - lenToRead );
                }

                m_DataSize[ m_TailIdx ] -= lenToRead;
                lenRead = lenToRead;
                autoIncrementTailIndex();
            }
        }
    }

    return lenRead;
}
