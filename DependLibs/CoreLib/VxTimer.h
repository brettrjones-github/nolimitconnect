#pragma once
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

#include "VxDefs.h"

//=== VxTimer Class object ===//
//there are a couple of ways to use the timer
//		any call to StartTime with any value sets the start time
//		so that calls to ElapsedTime return the Time since last called startTimer or timer was constructed,

//		The second mode is startTimer with a time till done
//		Subsequent calls to TimeDone return FALSE if the specified TimeTillDone has not been
//		reached or TRUE if the time has expired
//
//		NOTE: even after the timerDone has expired, calls to ElapsedTimer are still valid

class VxTimer
{
public:
	VxTimer();
    virtual ~VxTimer() = default;

    double						elapsedMs( void );       // return the elapsed time in milliseconds since timer was started or created 
    double						elapsedSec( void )  { return ( elapsedMs() / 1000 ); }      // return the elapsed time in seconds since
									                                                        // startTimer was called or VxTimer was constructed...it doesn't mater if the time
									                                                        // specified in the startTimer parameter has elapsed or not
    // same as start timer.. start at 0 then read elapsed time since reset or start timer
    void                        resetTimer( void )                          { startTimerMs( 0 ); }

	// just sets start time for elapsed time
    void						startTimer( void )							{ startTimerMs(0); }

    // starts a timer and sets time in milliseconds till timerDone returns true
    void						startTimerMs( int milliSec );

	// starts a timer and sets time till timerDone returns true
    void						startTimerSec( int dSec  )                  { startTimerMs( dSec * 1000 ); }

    // returns true when time specified has expired
	bool						timerDone( void )							{ return elapsedMs() > m_TimeTillDoneMs ? true : false; }

    void						waitTimeMs( int milliSec );
    void						waitTimeSec( int dSec )                     { waitTimeMs( dSec * 1000 ); }

private:
    int64_t						m_TimeTillDoneMs{0};
    int64_t						m_StartTickMs{0};
};


int64_t GetHighResolutionTimeUs( void ); //  time in usec
int64_t GetHighResolutionTimeMs( void ); //  time in millisec