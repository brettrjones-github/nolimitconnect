//============================================================================
// Copyright (C) 2016 Brett R. Jones
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

#include "EchoUtil.h"

//============================================================================
void PcmS16ToFloats( int16_t * pcmData, int sampleCnt, float * retFloatBuf )
{
	for( int i = 0; i < sampleCnt; i++ )
	{
		retFloatBuf[i] = PcmS16ToFloat( pcmData[i] );
	}
}

//============================================================================
void FloatsToPcmS16( float * floatBuf, int16_t * pcmRetData, int sampleCnt )
{
	for( int i = 0; i < sampleCnt; i++ )
	{
		pcmRetData[i] = FloatToPcmS16( floatBuf[i] );
	}
}