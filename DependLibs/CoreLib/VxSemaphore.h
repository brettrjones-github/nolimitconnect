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

#include <CoreLib/config_corelib.h>

#ifdef TARGET_OS_WINDOWS
    #include <WinSock2.h>
    #include <windows.h>
#else
    #include <semaphore.h>
#endif

class VxSemaphore
{
public:
	VxSemaphore();
	~VxSemaphore();

	//! wait until another thread calls Signal
	//! if iTimeoutMilliseconds == 0 then wait infinite time
	//! return true if was signaled else return false if timed out
	bool						wait( int iTimeoutMilliseconds = 0 );
	//! signal waiting thread to run
	int							signal( void );

	//=== vars ===//
#ifdef TARGET_OS_WINDOWS 
	HANDLE						m_hAccessLock; // windows handle to semaphore
#else
	sem_t						m_Semaphore; // linux semaphore
#endif

private:
	VxSemaphore(const VxSemaphore& rhs);// don't allow copy constructor
	VxSemaphore& operator=(const VxSemaphore& rhs);// don't allow copy operation
};
