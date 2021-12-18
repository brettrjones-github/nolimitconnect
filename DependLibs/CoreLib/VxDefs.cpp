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
// http://www.nolimitconnect.com
//============================================================================

#include <AppDependLibrariesConfig.h>

#if 0 // moved to VxTime.cpp
#ifdef TARGET_OS_ANDROID
#include "VxDefs.h"

#include <time.h>
// android doesn't have millisecond sleep like most linux distributions but does have nano sleep
void VxSleep( int iMilliSec )
{
	struct timespec ts;
	ts.tv_sec = iMilliSec/1000;
	ts.tv_nsec = (iMilliSec%1000)*1000000;
	nanosleep(&ts, NULL);
};	

#endif // TARGET_OS_ANDROID
#endif //0 