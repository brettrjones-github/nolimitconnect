#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include <GuiInterface/IDefs.h>
#include <memory.h>

class RawAudio
{
public:
	RawAudio( int16_t *	pcmData, uint16_t pcmDataLen, EAppModule appModule )
		: m_PcmData( (int16_t*)new uint8_t[ pcmDataLen ] )
		, m_PcmDataLen( pcmDataLen )
		, m_AppModule( appModule )
	{
		memcpy( m_PcmData, pcmData, pcmDataLen );
	}

	RawAudio( int16_t pcmDataLen, bool clearData, EAppModule appModule )
		: m_PcmData( (int16_t*)new uint8_t[ pcmDataLen ] )
		, m_PcmDataLen( pcmDataLen )
		, m_AppModule( appModule )
	{
		if( clearData )
		{
			muteAudioContents();
		}
	}

	~RawAudio()
	{
		delete m_PcmData;
	}

	EAppModule					getAppModule( void )		{ return m_AppModule; };
	int16_t *					getDataBuf( void )			{ return m_PcmData; }
	bool						getIsSilence( void )		{ return m_IsSilence; }

	void						muteAudioContents( void )	{ memset( m_PcmData, 0, m_PcmDataLen ); m_IsSilence = true; }

	//=== vars ===//
	int16_t *					m_PcmData;
	uint16_t					m_PcmDataLen; 
	EAppModule					m_AppModule{ eAppModuleInvalid };
	bool						m_IsSilence{ false };
};
