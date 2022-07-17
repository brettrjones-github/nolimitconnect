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

#include "AudioInputFrame.h"
#include "AudioUtil.h"

#include <CoreLib/VxDebug.h>

//============================================================================
void AudioInputFrame::resetFrame( void )
{
	m_LenWrote = 0;
	m_PeakAmplitude0to100 = 0;
	m_WriteTimestamp = 0;
	m_WriteDelayMs = 0;
}
