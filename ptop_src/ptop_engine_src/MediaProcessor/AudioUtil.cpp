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

#include "AudioUtil.h"

//============================================================================
void AudioUtil::PcmS16ToFloats( int16_t * pcmData, uint32_t pcmLenInBytes, float * retFloatBuf )
{
	int numSamples = pcmLenInBytes >> 1;
	for( int i = 0; i < numSamples; i++ )
	{
		retFloatBuf[i] = PcmS16ToFloat( pcmData[i] );
	}
}

//============================================================================
void AudioUtil::FloatsToPcmS16( float * floatBuf, int16_t * pcmRetData, uint32_t pcmLenInBytes )
{
	int numSamples = pcmLenInBytes >> 1;
	for( int i = 0; i < numSamples; i++ )
	{
		pcmRetData[i] = FloatToPcmS16( floatBuf[i] );
	}
}

//=============================================================================
static int16_t MixPcmSample( int a, int b ) // int16_t sample1, int16_t sample2 ) 
{
    // from stack overflow

#if 0
    // averaging algorithum
    const int32_t result( (static_cast<int32_t>(sample1) + static_cast<int32_t>(sample2)) / 2 );
    typedef std::numeric_limits<int16_t> Range;
    if( Range::max() < result )
        return Range::max();
    else if( Range::min() > result )
        return Range::min();
    else
        return result;
#else
    int m; // mixed result will go here
    // Make both samples unsigned (0..65535)
    a += 32768;
    b += 32768;

    // Pick the equation
    if( (a < 32768) || (b < 32768) ) {
        // Viktor's first equation when both sources are "quiet"
        // (i.e. less than middle of the dynamic range)
        m = a * b / 32768;
    }
    else {
        // Viktor's second equation when one or both sources are loud
        m = 2 * (a + b) - (a * b) / 32768 - 65536;
    }

    // Output is unsigned (0..65536) so convert back to signed (-32768..32767)
    if( m == 65536 ) m = 65535;
    m -= 32768;

    return (int16_t)m;
#endif // 0
}

//=============================================================================
void AudioUtil::mixPcmAudio( int16_t* pcmData, int16_t* outData, int toMixSampleCnt )
{
    if( toMixSampleCnt )
    {
        for( int i = 0; i < toMixSampleCnt; i++ )
        {
            outData[ i ] = MixPcmSample( pcmData[ i ], outData[ i ] );
        }
    }
}

