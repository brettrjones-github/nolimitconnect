
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
bool AudioEchoCancel::microphoneWroteSamples( int16_t* mono8000WriteBuf, int mono8000WriteSampleCnt, int16_t* echoCanceledData )
{
	return m_AudioEchoCancelImpl.microphoneWroteSamples( mono8000WriteBuf, mono8000WriteSampleCnt, echoCanceledData );
}

//============================================================================
bool AudioEchoCancel::speakerReadSamples( int16_t* mono8000ReadBuf, int mono8000ReadSampleCnt )
{
	return m_AudioEchoCancelImpl.speakerReadSamples( mono8000ReadBuf, mono8000ReadSampleCnt );
}
