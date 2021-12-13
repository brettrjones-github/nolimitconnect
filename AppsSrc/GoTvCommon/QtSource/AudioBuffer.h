#pragma once

#include <CoreLib/VxMutex.h>

#include "AudioDefs.h"


class AudioBuffer
{
public:
    AudioBuffer();

    // write to buffer.. return amount written
    int writeData( char * data, int datalen );

    // read from buffer.. return amount read
    int readData( char * data, int maxReadlen );

    // total amount of buffer space in bytes.. leave one buffer free to avoid possible overwrite
    int totalBufferSize( ) { return (AUDIO_BUF_COUNT - 1) * AUDIO_BUF_SIZE_48000_2; }

    // amount of data available in bytes
    int bytesAvailable();

    // amount of space free
    int bufSpaceFree() { return bytesAvailable() >= totalBufferSize() ? 0 : bytesAvailable(); }

private:
    // increment index.. return true if tail == head index
    bool incrementIndex( int& bufIdx );
    // increment head index.. do not allow head to reach tail.. return true if incremented
    bool incrementHeadIndex( int& bufIdx );
    void autoIncrementHeadIndex();
    // increment tail index.. do not allow tail to reach head.. return true if incremented
    bool incrementTailIndex( int& bufIdx );
    void autoIncrementTailIndex();

    // return bytes available at given index
    int getBytesAvailable( int bufIdx );
    // return bytes free at given index
    int getBytesFree( int bufIdx );
    // write into buffer.. return amount wrote
    int writeBufData( char * data, int dataLen );
    // read from buffer.. return amount read
    int readBufData( char * data, int dataLen );

    char m_RingBuf[ AUDIO_BUF_COUNT ][ AUDIO_BUF_SIZE_48000_2 ];
    int m_DataSize[ AUDIO_BUF_COUNT ];
    int m_HeadIdx = 0;
    int m_TailIdx = AUDIO_BUF_COUNT - 1;
    VxMutex m_BufMutex;
};