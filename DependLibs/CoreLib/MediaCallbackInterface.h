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

#include "VxDefs.h"

class PktVoiceReq;
class PktVideoFeedPic;
class PktVideoFeedPicChunk;
class VxGUID;

class MediaCallbackInterface
{
public:
	virtual void				callbackPcm( void * userData, VxGUID& feedId, int16_t * pcmData, uint16_t pcmDataLen ){};
	virtual void				callbackOpusEncoded( void * userData, uint8_t * encodedAudio, int32_t frame1Len, int32_t	frame2Len ){};
	virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio ){};
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen ){};

	virtual void				callbackVideoJpgBig( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen ){};
	virtual void				callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 ){};
	virtual void				callbackVideoPktPic( void * userData, VxGUID& onlineId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum ){};
	virtual void				callbackVideoPktPicChunk( void * userData, VxGUID& onlineId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum ){};
};
