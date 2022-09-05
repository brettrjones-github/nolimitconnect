
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include "AudioEchoCancel.h"
#include "AudioEchoCancelImpl.h"
#include "AppCommon.h"
#include "AppSettings.h"

//============================================================================
AudioEchoCancel::AudioEchoCancel( AppCommon& appCommon, AudioIoMgr& audioIoMgr, QObject* parent )
	: QObject(parent)
	, m_MyApp( appCommon )
	, m_AudioIoMgr( audioIoMgr )
	, m_AudioEchoCancelImpl( *new AudioEchoCancelImpl(appCommon, audioIoMgr, this ))
{	
}

//============================================================================
void AudioEchoCancel::echoCancelStartup( void )
{
	m_AudioEchoCancelImpl.setEchoDelayMsConstant( m_MyApp.getAppSettings().getEchoDelayParam() );
	m_AudioEchoCancelImpl.echoCancelStartup();
}

//============================================================================
void AudioEchoCancel::enableEchoCancel( bool enable )
{
	m_AudioEchoCancelImpl.enableEchoCancel( enable );
}

//============================================================================
bool AudioEchoCancel::isEchoCancelEnabled( void )
{
	return m_AudioEchoCancelImpl.isEchoCancelEnabled();
}

//============================================================================
void AudioEchoCancel::setEchoDelayMsParam( int delayMs )
{
	m_AudioEchoCancelImpl.setEchoDelayMsConstant( delayMs );
}

//============================================================================
void AudioEchoCancel::speakerReadSamples( int16_t* speakerReadData, int sampleCnt, int64_t speakerReadTailTimeMs, bool stableTimestamp )
{
	m_AudioEchoCancelImpl.speakerReadSamples( speakerReadData, sampleCnt, speakerReadTailTimeMs, stableTimestamp );
}

//============================================================================
void AudioEchoCancel::micWroteSamples( int16_t* micWritedData, int sampleCnt, int64_t micWriteTailTimeMs, bool stableTimestamp )
{
	m_AudioEchoCancelImpl.micWroteSamples( micWritedData, sampleCnt, micWriteTailTimeMs, stableTimestamp );
}

//============================================================================
void AudioEchoCancel::processEchoCancelThreaded( AudioSampleBuf& speakerProcessed8000Buf, QMutex& speakerProcessedMutex )
{
	m_AudioEchoCancelImpl.processEchoCancelThreaded( speakerProcessed8000Buf, speakerProcessedMutex );
}

//============================================================================
void AudioEchoCancel::setEchoCancelerNeedsReset( bool needReset )
{
	m_AudioEchoCancelImpl.setEchoCancelerNeedsReset( needReset );
}

//============================================================================
bool AudioEchoCancel::getIsInSync( void )
{
	return m_AudioEchoCancelImpl.getIsInSync();
}

//============================================================================
void AudioEchoCancel::setPeakAmplitudeDebugEnable( bool enableDebug ) 
{ 
	m_AudioEchoCancelImpl.setPeakAmplitudeDebugEnable( enableDebug );
}

//============================================================================
bool AudioEchoCancel::getPeakAmplitudeDebugEnable( void ) 
{ 
	return m_AudioEchoCancelImpl.getPeakAmplitudeDebugEnable();
}