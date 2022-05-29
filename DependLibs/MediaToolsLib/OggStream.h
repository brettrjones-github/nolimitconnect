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

#include "OggBuffer.h"

#include <libogg/include/ogg/ogg.h>

#include <CoreLib/VxDefs.h>

#include <string>
#include <stdio.h>

class MyOpusHeader;

class OggStream
{
public:
	OggStream();
	~OggStream();

	void						setTitle( const char * title );
	void						setArtist( const char * artist );
	void						setAlbum( const char * album );
	void						setDate( const char * date );
	void						setGenre( const char * genre );
	void						setUserComment( const char * userComment );

	bool						openOggStream( FILE * fileHandle, int streamSerialNumber = 0 ); // if streamSerialNumber is zero then generate random serial number
	int							writeHeader( MyOpusHeader& opusHeader, uint8_t * packetBuf, int packetBufLen );
	int							writeEncodedFrame( uint8_t * encodedFrame, int32_t encodedLen );
	uint64_t					closeOggStream( void ); // returns total bytes written

	void						setOpusHeader( MyOpusHeader& opusHeader )	{ m_OpusHeader = &opusHeader; }

protected:
	int							flushStreamToFile( void );
	int							writePageToOutput( ogg_page * page );

	bool						m_StreamInitialized;
	FILE *						m_FileHandle;
	OggBuffer					m_OggBuffer; 
	ogg_stream_state			m_StreamState;		// os
	ogg_packet					m_OggPkt;			// op;
	ogg_page					m_OggPage;			// og
	ogg_int64_t					m_LastGranulePos;	// ogg_int64_t last_granulepos=0;
	ogg_int64_t					m_EncGranulePos;
	ogg_int64_t					m_OriginalSamples;
	ogg_int32_t					m_id;
	int							m_LastSegments;
	int							m_Eos;
	uint64_t					m_TotalBytesWritten;	
	int							m_PagesOut;	
	std::string					m_Title;
	std::string					m_Artist;
	std::string					m_Album;
	std::string					m_Date;
	std::string					m_Genre;
	std::string					m_UserTag;
	int							m_LastSizeSegments;
	int							m_MaxOggDelay;
	MyOpusHeader *				m_OpusHeader;

};