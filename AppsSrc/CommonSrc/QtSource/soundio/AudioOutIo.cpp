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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppCommon.h"
#include "AudioOutIo.h"
#include "AudioIoMgr.h"
#include "AudioUtils.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <QDebug>
#include <QtEndian>
#include <QTimer>
#include <QMessageBox>

#include <math.h>

//============================================================================
AudioOutIo::AudioOutIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent )
: QIODevice( parent )
, m_AudioIoMgr( mgr )
, m_MyApp( mgr.getMyApp() )
, m_AudioBufMutex( audioOutMutex )
, m_AudioOutState( QAudio::StoppedState )
, m_SpeakerOutputEnableTimer( new QTimer( this ) )
{
    m_SpeakerOutputEnableTimer->setInterval( 200 );
    connect( m_SpeakerOutputEnableTimer, SIGNAL(timeout()), this, SLOT(slotUpdateSpeakerOutputEnable()) );
}

//============================================================================
bool AudioOutIo::initAudioOut( QAudioFormat& audioFormat, const QAudioDevice& deviceInfo )
{
    if( !m_initialized )
    {
        m_AudioFormat = audioFormat;
        setUpsampleMultiplier( (m_AudioFormat.sampleRate() * m_AudioFormat.channelCount()) / 8000 );
        int deviceIndex = 0;
        m_AudioIoMgr.getSoundOutDeviceIndex( deviceIndex );
        m_initialized = soundOutDeviceChanged( deviceIndex );
    }

    return m_initialized;
}

//============================================================================
bool AudioOutIo::soundOutDeviceChanged( int deviceIndex )
{
    if( !m_AudioOutputDevice.isNull() )
    {
        m_AudioOutputDevice->disconnect( this );
    }

    std::vector< std::pair<QString, QAudioDevice> > outDeviceList;
    m_AudioIoMgr.getSoundOutDevices( outDeviceList );
    if( outDeviceList.empty() )
    {
        QMessageBox::information( nullptr, QObject::tr( "Sound Out Device" ), QObject::tr( "No Sound Output Devices Avalable" ), QMessageBox::Ok );
        return false;
    }

    if( deviceIndex >= (int)outDeviceList.size() )
    {
        QMessageBox::information( nullptr, QObject::tr( "Sound Out Device" ), QObject::tr( "Sound Output Device Index Out Of Range. Will Use Default Device" ), QMessageBox::Ok );
        deviceIndex = 0;
    }

    QString deviceDesc = outDeviceList[ deviceIndex ].first;
    const QAudioDevice& deviceInfo = outDeviceList[ deviceIndex ].second;

    QAudioFormat format = deviceInfo.preferredFormat();

    format.setSampleRate( m_AudioFormat.sampleRate() );
    format.setChannelCount( m_AudioFormat.channelCount() );
    format.setSampleFormat( QAudioFormat::Int16 ); // only pcm is allowed

    if( !deviceInfo.isFormatSupported( m_AudioFormat ) )
    {
        LogMsg( LOG_DEBUG, "AudioOutIo Format not supported rate %d channels %d size %d", m_AudioFormat.sampleRate(), m_AudioFormat.channelCount(), m_AudioFormat.bytesPerSample() );
        QMessageBox::information( nullptr, QObject::tr( "Sound Out Format Not supported" ), QObject::tr( "Audio Out Format Not supported" ), QMessageBox::Ok );
        return false;
    }

    m_AudioOutputDevice.reset( new QAudioSink( deviceInfo, m_AudioFormat ) );

    // do not call setBufferSize. Qt does do read sizes of fixed size (on windows 16384) but qt does not call read with consistent time.. mostly 85 ms with size 16384
    // but occasionally 160 ms with size of 0
    // where as if not setBufferSize called the length of read periodically changes from  24000  to  24960  and back but the read interval is consistent at elapsed appox 251 ms
    // of the 2 options the second option is better because there is no remainder when upsampling from 8000Hz to 48000Hz and the time is consistent
    // when using option 2 use only 1 channel because using 2 channels just doubles the buffer size of read requests without affecting timing
    if( m_AudioIoMgr.useFixedAudioOutBufferSize() )
    {
        m_AudioOutputDevice->setBufferSize( format.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16 );
    }
    
    m_initialized = true;

    connect( m_AudioOutputDevice.data(), SIGNAL( stateChanged( QAudio::State ) ), SLOT( onAudioDeviceStateChanged( QAudio::State ) ) );
    LogMsg( LOG_VERBOSE, "AudioInIo Format supported rate %d channels %d size %d", m_AudioFormat.sampleRate(), m_AudioFormat.channelCount(), m_AudioFormat.bytesPerSample() );

    this->open( QIODevice::ReadOnly );

    if( m_AudioIoMgr.isSpeakerOutputWanted() )
    {
        wantSpeakerOutput( true );
    }

    return true;
}

//============================================================================
void AudioOutIo::wantSpeakerOutput( bool enableOutput )
{
    m_SpeakerOutputEnabled = enableOutput;
    // if we start stop start to fast windows crashes with Exception thrown at 0x00007FFC0C4C4FD9 (KernelBase.dll) in NoLimitConnectD.exe: WinRT originate error - 0xC00D36B3 : 'The stream number provided was invalid.'.
    // so fire a timer
    m_SpeakerOutputEnableTimer->start();
}

//============================================================================
void AudioOutIo::slotUpdateSpeakerOutputEnable( void )
{
    m_SpeakerOutputEnableTimer->stop();
    if( m_SpeakerOutputEnabled )
    {
        if (m_AudioOutputDevice->state() == QAudio::SuspendedState)
        {
            m_AudioOutputDevice->resume();
        }
        else if (m_AudioOutputDevice->state() == QAudio::ActiveState)
        {
            // already enabled
        }
        else if (m_AudioOutputDevice->state() == QAudio::IdleState)
        {
            // no-op
        }
        else if( m_AudioOutputDevice->state() == QAudio::StoppedState )
        {
            startAudioOut();
        }
    }
    else
    {
        if (m_AudioOutputDevice->state() == QAudio::SuspendedState || m_AudioOutputDevice->state() == QAudio::StoppedState)
        {
            // already stopped
        }
        else if (m_AudioOutputDevice->state() == QAudio::ActiveState)
        {
            stopAudioOut();
        }
        else if (m_AudioOutputDevice->state() == QAudio::IdleState)
        {
            // no-op
        }
    }
}

//============================================================================
void AudioOutIo::startAudioOut( void )
{
    // start in pull mode.. qt will call readData as needed for sound output
    m_AudioOutputDevice->start( this );
    if( m_AudioIoMgr.useFixedAudioOutBufferSize() )
    {
        m_AudioOutputDevice->setBufferSize( m_AudioFormat.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16 );
    }

    m_AudioIoMgr.speakerDeviceEnabled( true );
}

//============================================================================
void AudioOutIo::stopAudioOut( void )
{
    m_AudioOutputDevice->stop();
    m_AudioIoMgr.speakerDeviceEnabled( false );
}

//============================================================================
qint64 AudioOutIo::bytesAvailable() const
{
    // LINUX seems to be the only os that calls this
    // just always reture a frame size even if there is no audio available.. it will get filled with silence
    return m_AudioIoMgr.getAudioOutMixer().getMixerFrameSize();
}

//============================================================================
qint64 AudioOutIo::size() const
{
    return m_AudioIoMgr.getAudioOutMixer().getMixerFrameSize();
}

//============================================================================
void AudioOutIo::flush()
{
    // Flushing buffers is a bit tricky...
    // Don't modify this unless you're sure
    this->stopAudioOut();
	if( m_AudioOutputDevice )
	{
		m_AudioOutputDevice->reset();
	}

    startAudioOut();
}

//============================================================================
qint64 AudioOutIo::readData( char *data, qint64 maxlen )
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anything while being destroyed
        return 0;
    }

    if( !m_AudioIoMgr.isAudioInitialized() )
    {
        memset( data, 0, maxlen );
        return maxlen;
    }

    if( !maxlen )
    {
        // on windows it seems to be normal to get 0 as maxlen sometimes. so do nothing
        LogMsg( LOG_ERROR, "AudioOutIo::readData has 0 maxlen. Consider not calling setBufferSize " );
        return maxlen;
    }

    static int64_t lastTime = 0;
    int64_t timeNow = GetHighResolutionTimeMs();
    int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
    lastTime = timeNow;

    static qint64 lastSpeakerReadLen = 0;
    static int audioReadDurationUs = 0;
    if( maxlen != lastSpeakerReadLen )
    {
        // first time or device changed or read buffer len changed
        audioReadDurationUs = (int)AudioUtils::audioDurationUs( m_AudioFormat, maxlen );
        m_SpeakerReadTimeEstimator.setIntervalUs( audioReadDurationUs );
        lastSpeakerReadLen = maxlen;
    }

    int64_t speakerReadTimeMs = m_SpeakerReadTimeEstimator.estimateTime( timeNow );
    int speakerReqSampleCnt = maxlen / 2;

    if( m_AudioTestState != eAudioTestStateNone )
    {
        memset( data, 0, maxlen );
        if( m_AudioTestState == eAudioTestStateRun && !m_AudioTestSentTimeMs )
        {
            int16_t* sampleBuf = (int16_t*)data;
            // create a 480 hz square wave tone for 10 ms as a sound to be detected by microphone for delay timing test
            int maxSamplesToSet = AudioUtils::audioSamplesRequiredForGivenMs( m_AudioFormat, 10 );
            maxSamplesToSet = std::min( maxSamplesToSet, (int)maxlen / 2 );
            int samplesCycle = (m_AudioFormat.sampleRate() * m_AudioFormat.channelCount()) / (480 * 2) ;
            bool sampleIsMax{ true };
            for( int i = 0; i < maxSamplesToSet; i += samplesCycle )
            {
                int16_t sampVal = sampleIsMax ? 32767 : -32768;
                for( int j = 0; j < samplesCycle && ((j + i) < maxSamplesToSet); j++ )
                {
                    sampleBuf[ i + j ] = sampVal;
                }

                sampleIsMax = !sampleIsMax;
            }

            m_AudioTestSentTimeMs = speakerReadTimeMs;
        }

        m_AudioIoMgr.getAudioMasterClock().audioSpeakerReadDurationTime( audioReadDurationUs / 1000, speakerReadTimeMs );
        if( m_AudioIoMgr.getSampleCntDebugEnable() )
        {
            m_AudioIoMgr.getAudioMasterClock().audioMicWriteSampleCnt( speakerReqSampleCnt );
        }

        return maxlen;
    }

    qint64 readAmount;
    if( m_AudioIoMgr.getAudioLoopbackEnable() )
    {
        readAmount = m_AudioIoMgr.getAudioLoopback().readRequestFromSpeaker( data, maxlen );
    }
    else
    {
        readAmount = m_AudioIoMgr.getAudioOutMixer().readRequestFromSpeaker( data, maxlen );
    }
   
    if( readAmount != maxlen )
    {
        LogMsg( LOG_DEBUG, "AudioOutIo::readData mismatch with maxlen %d and read %d", maxlen, readAmount );
    }

    m_PeakAudioOutAmplitude = AudioUtils::peakPcmAmplitude0to100( (int16_t*)data, maxlen / 2 );

    if( m_AudioIoMgr.getPeakAmplitudeDebugEnable() )
    {
        static int64_t lastMixerPcmTime{ 0 };
        static int funcCallCnt{ 0 };
        funcCallCnt++;
        if( lastMixerPcmTime )
        {
            LogMsg( LOG_VERBOSE, "AudioOutIo::readData %d peak amplitude %d", funcCallCnt, m_PeakAudioOutAmplitude );
        }

        lastMixerPcmTime = timeNow;
    }

    if( m_AudioIoMgr.getIsEchoCancelEnabled() )
    {
        m_AudioIoMgr.getAudioEchoCancel().speakerReadSamples( m_EchoFarBuffer.getSampleBuffer(), m_EchoFarBuffer.getSampleCnt(), 
            speakerReadTimeMs + (audioReadDurationUs / 1000), m_SpeakerReadTimeEstimator.getHasMaxTimestamps() );
    }

    m_EchoFarBuffer.clear();
  
    // master clock is based on speaker read event/length
    // using audioReadDurationUs if using fixed buffer size  has rounding errors.. have to use elapsed time us
    //m_AudioIoMgr.getAudioMasterClock().audioSpeakerReadUs( thisCallTimeUs - lastCallTimeUs, false );
    //lastCallTimeUs = thisCallTimeUs;

    m_AudioIoMgr.getAudioMasterClock().audioSpeakerReadDurationTime( audioReadDurationUs / 1000, speakerReadTimeMs );

    return maxlen;
}

//============================================================================
// not used
qint64 AudioOutIo::writeData( const char *data, qint64 len )
{
    Q_UNUSED( data );
    Q_UNUSED( len );

    return 0;
}

//============================================================================
void AudioOutIo::onAudioDeviceStateChanged( QAudio::State state )
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return;
    }

    if( m_AudioOutState != state )
    {
        LogMsg( LOG_DEBUG, "AudioOutIo::onAudioDeviceStateChanged  from %s to %s ", m_AudioIoMgr.describeAudioState( m_AudioOutState ), m_AudioIoMgr.describeAudioState( state ) );
        m_AudioOutState = state;
        if( m_AudioOutState == QAudio::State::StoppedState )
        {
            // do not attempt anythig while stopped in slot
            return;
        }
    }
    else
    {
        return;
    }

    if( m_AudioOutputDevice )
	{
		// Start buffering again in case of underrun...
		// Required on Windows, otherwise it stalls idle
		if( state == QAudio::IdleState && m_AudioOutputDevice->error() == QAudio::UnderrunError )
		{
			// This check is required, because Mac OS X underruns often
			//m_AudioOutputDevice->suspend();

            LogMsg( LOG_DEBUG, "AudioOutIo::onAudioDeviceStateChanged speaker out suspending due to underrun " );

			return;
		}      
	}
}

//============================================================================
void AudioOutIo::setSpeakerVolume( int volume0to100 )
{
    qreal linearVolume = QAudio::convertVolume( volume0to100 / qreal( 100 ),
        QAudio::LogarithmicVolumeScale,
        QAudio::LinearVolumeScale );

    m_AudioOutputDevice->setVolume( linearVolume );
}

//============================================================================
void AudioOutIo::setAudioTestState( EAudioTestState audioTestState )
{
    switch( audioTestState )
    {
    case eAudioTestStateInit:
        m_AudioTestSentTimeMs = 0;
        break;
    case eAudioTestStateRun:
        break;
    case eAudioTestStateResult:
        break;
    case eAudioTestStateDone:
        break;
    case eAudioTestStateNone:
    default:
        break;
    }

    m_AudioTestState = audioTestState;
}

//============================================================================
void AudioOutIo::echoCancelSyncStateThreaded( bool inSync )
{

}
