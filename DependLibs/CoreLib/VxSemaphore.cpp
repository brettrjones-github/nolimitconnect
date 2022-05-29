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

#include "VxSemaphore.h"
#include "VxDebug.h"

#include <time.h> 

#ifndef TARGET_OS_WINDOWS
#include <time.h>
#endif

//============================================================================
VxSemaphore::VxSemaphore()
{
#ifdef TARGET_OS_WINDOWS
	m_hAccessLock = CreateSemaphore( NULL, 0, 5000, NULL );
#else
	sem_init( &m_Semaphore, 0, 0 );
#endif
}

//============================================================================
VxSemaphore::~VxSemaphore()
{
#ifdef TARGET_OS_WINDOWS
	if( m_hAccessLock )
	{
		CloseHandle( m_hAccessLock );
		m_hAccessLock = NULL;
	}
#endif
}

//============================================================================
//! wait until another thread calls Signal
bool VxSemaphore::wait( int iTimeoutMilliseconds )
{
#ifdef TARGET_OS_WINDOWS
	DWORD result;
	if( 0 == iTimeoutMilliseconds )
	{
		result = WaitForSingleObject(	m_hAccessLock,
										INFINITE );
	}
	else
	{
		result = WaitForSingleObject(	m_hAccessLock,
										iTimeoutMilliseconds );
	}
	if( WAIT_OBJECT_0 == result )
	{
		return true;
	}
	return false;
#else
	if( 0 == iTimeoutMilliseconds )
	{
		return (0 == sem_wait( &m_Semaphore ) );
	}
	else
	{
		struct timespec ts;
		if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
			LogMsg( LOG_FATAL, "VxSemaphore::Wait: failed getting real time clock\n" );
		ts.tv_sec += iTimeoutMilliseconds/1000;
		ts.tv_nsec += (iTimeoutMilliseconds%1000) * 1000;

		return (0 == sem_timedwait( &m_Semaphore, &ts ) );
	}
#endif
}
//============================================================================
//! signal waiting thread to run
int VxSemaphore::signal( void )
{
#ifdef TARGET_OS_WINDOWS
	if (!ReleaseSemaphore(
			m_hAccessLock,  // handle to semaphore
			1,           // increase count by one
			NULL) )      // not interested in previous count
	{
		return -1;
	}
	else
		return 0;
#else
    //#ifdef TARGET_OS_ANDROID
    //    return sem_st( &m_Semaphore );
    //#else
        return sem_post( &m_Semaphore );
    //#endif //TARGET_OS_ANDROID
#endif //TARGET_OS_WINDOWS
}

