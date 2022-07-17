#pragma once
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

#include <CoreLib/VxDefs.h>

static NLC_INLINE int16_t FloatToPcmS16( float v ) 
{
	if (v > 0)
		return v >= 1.0f ? S16_MAXVAL
		: static_cast<int16_t>(v * S16_MAXVAL + 0.5f);
	return v <= -1.0f ? S16_MINVAL
		: static_cast<int16_t>(-v * S16_MINVAL - 0.5f);
}

static NLC_INLINE float PcmS16ToFloat( int16_t v ) 
{
	static const float kMaxInt16Inverse = 1.f / S16_MAXVAL;
	static const float kMinInt16Inverse = 1.f / S16_MINVAL;
	return v * (v > 0 ? kMaxInt16Inverse : -kMinInt16Inverse);
}

void PcmS16ToFloats( int16_t * pcmData, uint32_t pcmLenInBytes, float * retFloatBuf ); 
void FloatsToPcmS16( float * floatBuf, int16_t * pcmRetData, uint32_t pcmLenInBytes );
