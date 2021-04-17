#pragma once
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

#include "config_gotvapps.h"

// A lot of assumptions are made
// General Requirements
// Must run in thread

// Requirements for P2PEngine
// 1.) buffer size 1280 bytes of 8000hz 16Bit PCM data ( 80 milliseconds )
// 2.) Engine must be notified when new buffer is available
// 3.) Will be resampled into 2 channel 48000 Hz PCM data on arrival

// Simplified Requirements for Kodi
// 1.) 2 channel 48000 Hz PCM data ( this seems to be supported by all platforms )
// 2.) all other formats will be resampled into the 2 channel 48000 Hz PCM data 
// 3.) Kodi calls getCachedDataLength and getAudioDelayMs to determine when to send more audio data 


#include "AudioTestGenerator.h"
#include "AudioOutIo.h"
#include "AudioBuffer.h"

#include <GuiInterface/IGoTvDefs.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxBuffer.h>

#include <QThread>
#include <QAudioOutput>
#include <QBuffer>
#include <QQueue>

class AppCommon;

class AudioOutQt : public QObject
{
	Q_OBJECT

public:
    AudioOutQt( AppCommon& myApp ) ;
	virtual ~AudioOutQt();

    // volume is from 0.0 to 1.0
    void						setVolume( float volume );

    // add audio data to play.. convert to s16 pcm data before calling writeData
    virtual int				    toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes );

    // write audio data to queue in s16 pcm format
    //qint64						writeData( const char * data, qint64 len );

    // read audio data from queue for output speakers
    qint64						speakerOutReadData( char * data, qint64 maxlen );

    // delay of audio calculated from amount of data in queue
    double                      getAudioDelaySeconds( );

    // maximum queue cache size in seconds 
    double                      toGuiGetAudioCacheTotalSeconds();

    // amount of free queue space in bytes
    virtual int				    toGuiGetAudioCacheFreeSpace( EAppModule appModule );

    void                        pauseAudioOut( );
    void                        resumeAudioOut( );

signals:
    void                        signalNeedMoreAudioData( int requiredLen );

public slots:
    void                        slotNeedMoreAudioData( int  requiredLen );
    void                        playerStateChanged( QAudio::State state );

protected:
    void						initAudioOut( );
    void						stopAudioOut( );
    //void                        addToQue( const char * data, int len );

    //void                        run( ) override; // thread override

    int                         getCachedDataLength( bool requireLock = true );
    int                         getCachedMaxLength( ) { return AUDIO_OUT_CACHE_SIZE; }

    void                        checkAudioState( int lastLenWrote );


	//=== vars ===//
	AppCommon&				    m_MyApp;
    QAudioFormat                m_AudioFormat;
    AudioOutIo                  m_AudioOutIo;
    AudioTestGenerator *        m_AudioGen = nullptr;
    QAudioDeviceInfo            m_AudioDeviceInfo;
    bool                        m_IsPaused = false;
    bool                        m_IsTestMode = true;
    int                         m_CacheAuidioLen = 0;
    VxMutex                     m_AudioQueueMutex;
    int                         m_OutWriteCount = 0;
    int16_t                     m_MyLastAudioOutSample = 0;

//    AudioBuffer                 m_AudioBuf;
//    std::vector<unsigned char *>  m_AudioDataQue;
//    VxBuffer                    m_PartialInBuf;
//    VxBuffer                    m_PartialOutBuf;



};
