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
#include "config_corelib.h"

#include "VxTime.h"
#include "VxTimer.h"
#include "VxDebug.h"

//#define TEST_TIMER 1 //define this to use the test program


#ifdef TARGET_OS_WINDOWS
# include <WinSock2.h>
# include <windows.h>    /* timer */
#else
# include <sys/time.h>
#endif

#ifdef TARGET_OS_WINDOWS
//============================================================================
int64_t GetHighResolutionTimeUs( void ) //  time in usec
{
	/* Returns a time counter in microsec   */
	/* the resolution is platform dependent */
	/* but is typically 1.62 us resolution  */
	LARGE_INTEGER lpPerformanceCount;
	LARGE_INTEGER lpFrequency;
	QueryPerformanceCounter( &lpPerformanceCount );
	QueryPerformanceFrequency( &lpFrequency );
	return (int64_t)((1000000 * (lpPerformanceCount.QuadPart)) / lpFrequency.QuadPart);
}
#else   // Linux or Mac or Android
int64_t GetHighResolutionTimeUs( void ) /* O  time in usec*/
{
	// if the posix os has CLOCK_MONOTONIC defined then is better to use clock_gettime
	// clock_gettime will not jump if user changes the current time. going backwards in time could cause issues
#if defined(CLOCK_MONOTONIC)
	// even if does not highlight in qt this is active for android
	struct timespec tspec;
	tspec.tv_sec = tspec.tv_nsec = 0;
	clock_gettime( CLOCK_MONOTONIC, &tspec );
# if defined(TARGET_OS_ANDROID) || defined(TARGET_OS_LINUX)
	int64_t timeNow = ((int64_t)tspec.tv_sec * 1000000) + ((int64_t)tspec.tv_nsec / 1000);
# else
	int64_t timeNow = ((int64_t)tspec.tv_sec * 1000000) + ((int64_t)tspec.tv_usec);
# endif // defined(TARGET_OS_ANDROID)
	static int64_t lastTime = 0;
	if( timeNow < lastTime )
	{
		LogMsg( LOG_ERROR, "ERROR MONOTONIC GetHighResolutionTimeMs() Went backwards from %" PRId64 " to %" PRId64 "", lastTime, timeNow );
	}

	lastTime = timeNow;
	return timeNow;
#else
	struct timeval tv;
	if( gettimeofday( &tv, NULL ) != 0 )
	{
		LogMsg( LOG_ERROR, "GetHighResolutionTimeMs() Failed gettimeofday" );
		return 0;
	}

	int64_t timeNow = ((int64_t)tv.tv_sec * 1000LL) + ((int64_t)tv.tv_usec / 1000L);
	static int64_t lastTime = 0;
	if( timeNow < lastTime )
	{
		LogMsg( LOG_ERROR, "ERROR GetHighResolutionTimeMs() Went backwards from %" PRId64 " to %" PRId64 "", lastTime, timeNow );
		lastTime = timeNow;
	}

	return timeNow;
#endif // CLOCK_MONOTONIC

	struct timeval tv;
	gettimeofday( &tv, 0 );
	return((tv.tv_sec * 1000000) + (tv.tv_usec));
}
#endif

#ifdef TARGET_OS_WINDOWS
//============================================================================
int64_t GetHighResolutionTimeMs( void ) //  time in millisec
{
	/* Returns a time counter in millisec   */
	/* the resolution is platform dependent */
	/* but is typically 1.62 us resolution  */
	LARGE_INTEGER lpPerformanceCount;
	LARGE_INTEGER lpFrequency;
	QueryPerformanceCounter( &lpPerformanceCount );
	QueryPerformanceFrequency( &lpFrequency );
	return (int64_t)((1000 * (lpPerformanceCount.QuadPart)) / lpFrequency.QuadPart);
}
#else   // Linux or Mac or Android
int64_t GetHighResolutionTimeMs( void ) /* O  time in usec*/
{
	// if the posix os has CLOCK_MONOTONIC defined then is better to use clock_gettime
	// clock_gettime will not jump if user changes the current time. going backwards in time could cause issues
#if defined(CLOCK_MONOTONIC)
	// even if does not highlight in qt this is active for android
	struct timespec tspec;
	tspec.tv_sec = tspec.tv_nsec = 0;
	clock_gettime( CLOCK_MONOTONIC, &tspec );
# if defined(TARGET_OS_ANDROID) || defined(TARGET_OS_LINUX)
	int64_t timeNow = ((int64_t)tspec.tv_sec * 1000LL) + ((int64_t)tspec.tv_nsec / 1000000L);
# else
	int64_t timeNow = ((int64_t)tspec.tv_sec * 1000LL) + ((int64_t)tspec.tv_usec / 1000L);
# endif // defined(TARGET_OS_ANDROID)
	static int64_t lastTime = 0;
	if( timeNow < lastTime )
	{
		LogMsg( LOG_ERROR, "ERROR MONOTONIC GetHighResolutionTimeMs() Went backwards from %" PRId64 " to %" PRId64 "", lastTime, timeNow );
	}

	lastTime = timeNow;
	return timeNow;
#else
	struct timeval tv;
	if( gettimeofday( &tv, NULL ) != 0 )
	{
		LogMsg( LOG_ERROR, "GetHighResolutionTimeMs() Failed gettimeofday" );
		return 0;
	}

	int64_t timeNow = ((int64_t)tv.tv_sec * 1000LL) + ((int64_t)tv.tv_usec / 1000L);
	static int64_t lastTime = 0;
	if( timeNow < lastTime )
	{
		LogMsg( LOG_ERROR, "ERROR GetHighResolutionTimeMs() Went backwards from %" PRId64 " to %" PRId64 "", lastTime, timeNow );
		lastTime = timeNow;
	}

	return timeNow;
#endif // CLOCK_MONOTONIC
}
#endif

//============================================================================
VxTimer::VxTimer()
{
    m_StartTickMs = GetHighResolutionTimeMs();
}

//============================================================================
void VxTimer::startTimerMs( int milliSec )
{
    m_StartTickMs = GetHighResolutionTimeMs();
	m_TimeTillDoneMs = m_StartTickMs + milliSec;
}

//============================================================================
void VxTimer::waitTimeMs( int milliSec )
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
     return (double)(GetHighResolutionTimeMs() - m_StartTickMs);
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


