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
#include "VxMutex.h"

//#define SHOW_LOCKS 1

//============================================================================
VxMutex::VxMutex( void )
: m_IsLocked( false )
{
#ifdef TARGET_OS_WINDOWS
	// windows event
	m_hAccessLock = CreateEvent(	NULL,		// security attributes 
		FALSE,		// manual reset
		TRUE,		// initial state signaled 
		NULL );		// name of event
#else // LINUX
	pthread_mutexattr_init( &m_MutexAttr );
	pthread_mutexattr_settype( &m_MutexAttr,  PTHREAD_MUTEX_RECURSIVE );
	pthread_mutex_init( &m_Lock, &m_MutexAttr );
#endif
#ifdef DEBUG_VX_MUTEX
	m_uiLastLockThreadId = 0;
#endif // DEBUG_VX_MUTEX
}

//============================================================================
VxMutex::~VxMutex( void )
{
#ifdef TARGET_OS_WINDOWS
	if( m_hAccessLock )
	{
		CloseHandle( m_hAccessLock ); 
		m_hAccessLock = NULL;
	}
#else // LINUX
	pthread_mutex_destroy( &m_Lock );
#endif
}

//============================================================================
int VxMutex::lock( int iInstance )
{
#ifdef SHOW_LOCKS
	LogMsg( LOG_INFO, "Unlocking VxMutex ..instance %d\n", iInstance );
#endif
	return this->lock();
}

//============================================================================
int VxMutex::unlock( int iInstance )
{
#ifdef SHOW_LOCKS
	LogMsg( LOG_INFO, "Unlocking VxMutex ..instance %d\n", iInstance );
#endif
	return this->unlock();
}

//============================================================================
int VxMutex::lock( void )
{
#ifdef TARGET_OS_WINDOWS

#ifdef DEBUG_VX_MUTEX
	vx_assert( m_uiLastLockThreadId != VxGetCurrentThreadId() );
	// timeout after 10 minutes
	DWORD dwResult = WaitForSingleObject(	m_hAccessLock, 600000	);
#else
	// wait infinite time
	DWORD dwResult = WaitForSingleObject(	m_hAccessLock, INFINITE	);
#endif
	if( 0 == dwResult )
	{
		// object signaled
		m_IsLocked = true;
		return 0;
	}

	if( 0xFFFFFFFF == dwResult )
	{
		LogMsg( LOG_INFO, "VxMutex Lock failure %d\n", GetLastError() );
	}
	else
	{
		LogMsg( LOG_INFO, "VxMutex Lock failure %d result %d\n", GetLastError(), dwResult );
	}

	vx_assert( false );
	return 0;
#else // LINUX
#ifdef DEBUG_VX_MUTEX
	m_uiLastLockThreadId = 0;
#endif // DEBUG_VX_MUTEX
	int iError;
	if( 0 != (iError = pthread_mutex_lock( &m_Lock ) ) )
	{
#ifdef DEBUG_VX_MUTEX
		DoMutexError( this, pthread_self(), iError );
#endif// DEBUG_VX_MUTEX
		LogMsg( LOG_INFO, "VxMutex Lock failure %d\n", iError );
		if( iError != EDEADLK )
		{
			LogMsg( LOG_INFO, "VxMutex was not Deadlocked\n");
		}
		vx_assert( false );
	}

	m_IsLocked = true;
	return iError;
#endif
}

//============================================================================
int VxMutex::unlock( void )
{
	m_IsLocked = false;
#ifdef TARGET_OS_WINDOWS
	if( false == SetEvent( m_hAccessLock ) )
	{
		int iLastErr = GetLastError();
		LogMsg( LOG_INFO, "VxMutex Unlock failure %d\n", iLastErr );
		vx_assert( false );
		return iLastErr;
	}
	return 0;
#else // LINUX
	int err;
	if( 0 != (err = pthread_mutex_unlock( &m_Lock ) ) )
	{
		LogMsg( LOG_INFO, "VxMutex Unlock failure %d\n", err );
		vx_assert( false );
	}

	return err;
#endif
}

//============================================================================
int VxMutex::lock( const char* file, int line )
{
#ifdef DEBUG_VX_MUTEX
	uint32_t u32ThreadId = GetCurrentThreadId();
	bool bFound = false;
	for( int i = 0; i < MAX_MUTEX_THREADS; i++ )
	{
		if( m_agMutexThreadIds[ i ].m_u32ThreadId == u32ThreadId )
		{
			bFound = true;
			VxMutexDebug * pgMutDbg = &m_agMutexThreadIds[ i ];
			if( 0 != pgMutDbg->m_iLockCnt )
			{
				LogMsg( LOG_INFO, "VxMutex: LockCnt %d file %s line %d\n", pgMutDbg->m_iLockCnt, file, line );
				LogMsg( LOG_INFO, "VxMutex: Prev lock file %s line %d\n", pgMutDbg->m_pLastLockFile, pgMutDbg->m_iLastLockLine );
				LogMsg( LOG_INFO, "VxMutex: Prev Unock file %s line %d\n", pgMutDbg->m_pLastUnlockFile, pgMutDbg->m_iLastUnlockLine );
				vx_assert( false );
			}
			pgMutDbg->m_iLockCnt++;
		}
	}

	if( false == bFound )
	{
		for( int i = 0; i < MAX_MUTEX_THREADS; i++ )
		{
			if( m_agMutexThreadIds[ i ].m_u32ThreadId == 0 )
			{
				m_agMutexThreadIds[ i ].m_u32ThreadId = u32ThreadId;
				VxMutexDebug * pgMutDbg = &m_agMutexThreadIds[ i ];
				pgMutDbg->m_iLockCnt = 1;
				pgMutDbg->m_pLastLockFile = file;
				pgMutDbg->m_iLastLockLine = line;
				bFound = true;
				break;
			}
		}
	}

	if( false == bFound )
	{
		LogMsg( LOG_INFO, "VxMutex: Ran out of thread ids file %s line %d\n", file, line );
		vx_assert( false );
	}
#endif // DEBUG_VX_MUTEX

	return this->lock();

}
//============================================================================
int VxMutex::unlock( const char* file, int line )
{
#ifdef DEBUG_VX_MUTEX
	uint32_t u32ThreadId = GetCurrentThreadId();
	bool bFound = false;
	for( int i = 0; i < MAX_MUTEX_THREADS; i++ )
	{
		if( m_agMutexThreadIds[ i ].m_u32ThreadId == u32ThreadId )
		{
			bFound = true;
			VxMutexDebug * pgMutDbg = &m_agMutexThreadIds[ i ];
			if( 1 != pgMutDbg->m_iLockCnt )
			{
				LogMsg( LOG_INFO, "VxMutex: UnlockCnt %d file %s line %d\n", pgMutDbg->m_iLockCnt, file, line );
				LogMsg( LOG_INFO, "VxMutex: Prev lock file %s line %d\n", pgMutDbg->m_pLastLockFile, pgMutDbg->m_iLastLockLine );
				LogMsg( LOG_INFO, "VxMutex: Prev Unock file %s line %d\n", pgMutDbg->m_pLastUnlockFile, pgMutDbg->m_iLastUnlockLine );
				vx_assert( false );
			}
			pgMutDbg->m_u32ThreadId = 0;
			pgMutDbg->m_iLockCnt = 0;
			break;
		}
	}

	if( false == bFound )
	{
		LogMsg( LOG_INFO, "VxMutex: Unlock without lock attempted by thread 0x%x file %s line %d\n", u32ThreadId, file, line );
		vx_assert( false );
	}
#endif // DEBUG_VX_MUTEX
	return this->unlock();
}

