//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
#include "AppCompilerConfig.h"

#include "VxTime.h"

#include "VxTimer.h"

#ifdef TARGET_OS_WINDOWS
#include "windows.h"

# define VxGetLastError		GetLastError
#else
//VxSleep.. sleep specified milliseconds
# if defined( TARGET_OS_ANDROID )
int VxSleep( int iMilliSec )
{
    struct timespec ts;
    ts.tv_sec = iMilliSec/1000;
    ts.tv_nsec = (iMilliSec%1000)*1000000;
    nanosleep(&ts, NULL);
    return 0;
};
# else
#  ifndef TARGET_OS_WINDOWS
// normal Linux
#   include <unistd.h>
#   define VxSleep(exp)		usleep(exp * 1000)
#  endif // TARGET_OS_WINDOWS
# endif // TARGET_OS_ANDROID
#endif // TARGET_OS_WINDOWS

//#define TEST_TIMER 1 //define this to use the test program

//============================================================================
VxTimer::VxTimer()
: m_TimeTillDoneMs(0.0)
{
    m_StartTickMs = GetGmtTimeMs();
}

//============================================================================
void VxTimer::startTimerMs( double milliSec )
{
    m_StartTickMs = GetGmtTimeMs();
	m_TimeTillDoneMs = m_StartTickMs + milliSec;
}

//============================================================================
void VxTimer::waitTimeMs( double milliSec )
{
	//===for all other modes just loop and wait for time to run out ===//
   startTimerMs( milliSec  );
   int iFirstTime = true;
   while(! timerDone())
   {
	   // we want to sleep as much as possible so we do not waist cpu cycles but we
	   // want to be awake when timer is done
		if( iFirstTime )
		{
			iFirstTime = false;
			if( milliSec > 10 )
			{
				VxSleep( milliSec - 10 ); //should wake up 10 ms before timer is done
			}

			iFirstTime = 0;
		}
        else
        {
            VxSleep( 1 );
        }
   }
}

//============================================================================
double VxTimer::elapsedMs( void )
{
     return GetGmtTimeMs() - m_StartTickMs;
}

//============================================================================
#ifdef TEST_TIMER
int main(int argc, char *argv[])
{

	VxTimer gTimer;
	double dDelay;
	long lLoopCnt;
	long i;
	while(true)
	{
		printf("\n Enter Time To Delay in Milli Seconds ");
		scanf("%lf", &dDelay);
		printf("\n Enter The Number of Times to Delay ");
		scanf("%ld", &lLoopCnt);
		printf("\nNow Executing using timerDone function");
		for(i = 0; i < lLoopCnt; i++)
		{
			gTimer.startTimerMs(dDelay);
			while(false == gTimer.timerDone())
				;
		}
		printf("\nNow Executing using Elapsed Time function");

		for(i = 0; i < lLoopCnt; i++)
		{
			gTimer.startTimerMs(0);
			while(dDelay > gTimer.elapsedMs())
				;
		}
		printf("\n*****************Finished******************");
		printf("\nPress X to exit or any other key to do again \n");
		fflush(stdin);
		int val = getch();
		if('X' == val || 'x' == val)
			return 0;
	}
}

#endif //TEST_TIMER


