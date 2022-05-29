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
#include "VxDebug.h"

#ifdef TARGET_OS_WINDOWS
	#include <WinSock2.h>
	#include <windows.h>
#else
	#include <libpthread/pthread.h>
#endif // TARGET_OS_WINDOWS

//#define DEBUG_VX_MUTEX 1

#ifdef DEBUG_VX_MUTEX
#define MAX_MUTEX_THREADS 50
class VxMutexDebug
{
public:
	VxMutexDebug()
	{
		m_u32ThreadId = 0;
		m_pLastLockFile = "NOLOCKCALLED";
		m_iLastLockLine = 0;
		m_pLastUnlockFile = "NOUNLOCKCALLED";
		m_iLastUnlockLine = 0;
		m_iLockCnt = 0;
	}
	uint32_t		m_u32ThreadId;
	char *	m_pLastLockFile;
	int		m_iLastLockLine;
	char *	m_pLastUnlockFile;
	int		m_iLastUnlockLine;
	int		m_iLockCnt;
};
#endif // DEBUG_VX_MUTEX

class VxMutex
{
public:
	VxMutex();
	virtual ~VxMutex();

	int							lock( void );
	int							unlock( void );
	int							lock( int iInstance );
	int							unlock( int iInstance );
	int							lock( const char* file, int line );
	int							unlock( const char* file, int line );

#ifdef DEBUG_VX_MUTEX
	VxMutexDebug				m_agMutexThreadIds[ MAX_MUTEX_THREADS ];
	UINT						m_uiLastLockThreadId;
#endif //DEBUG_VX_MUTEX

	//=== vars ===//
	#ifdef TARGET_OS_WINDOWS
		HANDLE					m_hAccessLock;
	#else // LINUX
		pthread_mutex_t			m_Lock;
		pthread_mutexattr_t		m_MutexAttr;
	#endif
	bool						m_IsLocked;

private:
	VxMutex(const VxMutex& rhs);// don't allow copy constructor
	VxMutex& operator=(const VxMutex& rhs);// don't allow copy operation
};




