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
// brettjones1900@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "stdafx.h"
#include "AppAll.h"
#include "VxThread.h"

#include <sys/stat.h>
#include "memory.h"

#ifndef TARGET_OS_WINDOWS
	#include <sys/types.h> 
	#include <sys/syscall.h> 
	#include <unistd.h>
    #include <stdlib.h>
#endif // TARGET_OS_WINDOWS

//#define ENABLE_THREAD_INFO 1
//#define DEBUG_THREADS 1

bool VxIsAppShuttingDown(void)
{
	return false;
}

namespace
{
	int							g_ThreadCreateCnt	= 0;
	std::vector<VxThread *>		g_RuningThreadList;
}

//============================================================================
//! get the thread id of current thread that is executing
unsigned int VxGetCurrentThreadId( void )
{
	#ifdef TARGET_OS_WINDOWS
		return GetCurrentThreadId();
	#else // LINUX
        return pthread_self();
    #endif
}

//============================================================================
//! get the thread tid of current thread that is executing
unsigned int VxGetCurrentThreadTid( void )
{
#ifdef TARGET_OS_WINDOWS
	return GetCurrentThreadId();
#else // LINUX
	#ifdef TARGET_OS_ANDROID
		return ( unsigned int )gettid();
	#else
		return pthread_self();
	#endif // TARGET_OS_ANDROID
#endif
}

//============================================================================
std::string		VxGetThreadInfo( void )
{
	std::string threadInfo;
	threadInfo = "No Info";
	//#ifdef TARGET_OS_WINDOWS
//	StdStringFormat( threadInfo, " @[tid=%d] ",   VxGetCurrentThreadId() );
//#else
//	StdStringFormat( threadInfo, " @[tid=%d] ",   VxGetCurrentThreadTid() );
//#endif // TARGET_OS_WINDOWS
	return threadInfo;
}

//============================================================================
VX_THREAD_START_CALLBACK_T VxThread::m_funcStartCallback = NULL;
VX_THREAD_EXIT_CALLBACK_T VxThread::m_funcExitCallback = NULL;

//============================================================================
VxThread::VxThread( void )
: m_bIsExitCallbackLocked(false)
, m_pvUserParam(0)
, m_u8ThreadFlags(0)
, m_strThreadName("UnknownThread")
, m_uiThreadId(0)		// thread ID
#ifdef TARGET_OS_WINDOWS
, m_hThread( 0 )		    // handle to thread
#endif
, m_ThreadTid( 0 )
, m_ThreadStartTime( 0 )
{
}

//============================================================================
VxThread::~VxThread( void )
{
	killThread();
}

//============================================================================
int VxThread::getThreadsRunningCount( void )
{
	return (int)g_RuningThreadList.size();
}

//============================================================================
void VxThread::dumpRunningThreads( void )
{
#ifdef ENABLE_THREAD_INFO
	//if ( false == VxIsAppShuttingDown() )
	{
		LogMsg( LOG_INFO, "Threads running %d time now %s\n", g_RuningThreadList.size(), VxTimeUtil::getLocalDateAndTimeWithNumberMonths(true).c_str() );

		std::vector<VxThread *>::iterator iter;
		g_DebugThreadsMutex.lock();
		for( iter = g_RuningThreadList.begin(); iter != g_RuningThreadList.end(); ++iter )
		{
			(*iter)->dumpThreadInfo();
		}	
		
		g_DebugThreadsMutex.unlock();
	}
#endif // ENABLE_THREAD_INFO 
}

//============================================================================
void VxThread::dumpThreadInfo( void )
{
	//std::string timeStart = VxTimeUtil::formatTimeStampIntoHoursAndMinutesAndSeconds( m_ThreadStartTime, true );
	//LogMsg( LOG_INFO, "Thrd: tid %d id %d %s started %s\n", m_ThreadTid, m_uiThreadId, m_strThreadName.c_str(), timeStart.c_str() );
}

//============================================================================
//! thread calls this to see if should exit
bool VxThread::isAborted( void )
{ 
	return ( (0 != (m_u8ThreadFlags & VX_FLAG_THREAD_ABORT)) || VxIsAppShuttingDown());
}

//============================================================================
void VxThread::abortThreadRun( bool bAbort )
{
	if( bAbort || VxIsAppShuttingDown() )
		m_u8ThreadFlags |= VX_FLAG_THREAD_ABORT;
	else
		m_u8ThreadFlags &= ~VX_FLAG_THREAD_ABORT;
}

//============================================================================
//! set function that will be called when any thread is started
void VxThread::setThreadStartCallback( VX_THREAD_START_CALLBACK_T func )
{
	//LogMsg( LOG_INFO, "$$$VxThread::setThreadStartCallback\n" );
	m_funcStartCallback = func;
}

//============================================================================
//! set function that will be called when any thread is about to exit
void VxThread::setThreadExitCallback( VX_THREAD_EXIT_CALLBACK_T func )
{
	//LogMsg( LOG_INFO, "$$$VxThread::setThreadExitCallback\n" );
	m_funcExitCallback = func;
}

//============================================================================
RCODE VxThread::startThread(	VX_THREAD_FUNCTION_T	pfuncThreadFunc,	// function that thread calls
								void *					pvUserParam, 		// caller defined param
								const char *			pThreadName,		// thread name
								int						iExtraStackSpace )	// will be added to minimum stack size	
{
	//vx_assert( false == this->isThreadRunning() );
	g_ThreadCreateCnt++;
	m_pvUserParam = pvUserParam;
	if( pThreadName )
	{
		m_strThreadName = pThreadName;
	}
	else
	{
		//StdStringFormat( m_strThreadName, "WorkerThread0x%x-%d", this, g_ThreadCreateCnt );
		m_strThreadName = "Unknown Thread";
	}

	if( m_strThreadName == "VxSktBaseUDP3" )
	{
		m_pvUserParam = pvUserParam;		
	}
	
	abortThreadRun( false );
	setIsThreadCreated( true );
	setIsThreadDone( false );
	setIsThreadRunning( true, false );	

	#ifdef TARGET_OS_WINDOWS
		// start a windows thread
		m_hThread = CreateThread(	0,					// security attributes
									0,					// stack size
									(LPTHREAD_START_ROUTINE)pfuncThreadFunc, // thread function
									(void *)this,  		// parameter to pass to thread function
									0,					// thread creation flags
									&m_uiThreadId );	// return thread id
		if( NULL == m_hThread )
		{
			m_u8ThreadFlags = 0;
			//AppErr( eAppErrThreadCreateFailed, "VxThread:: ERROR.. FAILED TO CREATE THREAD\n" );
			abort();
			return -1;
		}
#ifdef DEBUG_THREADS
		LogMsg( LOG_INFO, "VxThread:Created Thread %s thread id %d\n", m_strThreadName.c_str(), m_uiThreadId );
#endif // DEBUG_THREADS
	#else // LINUX

		int iErr;
		if( 0 != ( iErr = pthread_attr_init( &m_ThreadAttr ) ) )
		{
			AppErr( eAppErrThreadCreateFailed, "VxThread:pthread_attr_init FAILED %d %d\n", VxGetLastError(), iErr );
		}
		else
		{
			if( iExtraStackSpace )
			{
				size_t sizetStackSize = 0;
				iErr = pthread_attr_getstacksize( &m_ThreadAttr, &sizetStackSize );
				if( iErr )
				{
					LogMsg( LOG_FATAL, "VxThread:pthread_attr_getstacksize error %d\n", iErr );
				}
				else
				{
					LogMsg( LOG_INFO, "VxThread:Created Thread %s min stack size %d\n", m_strThreadName.c_str(), sizetStackSize );
					sizetStackSize += iExtraStackSpace;
					iErr = pthread_attr_setstacksize( &m_ThreadAttr, sizetStackSize );
					if( iErr )
					{
						LogMsg( LOG_FATAL, "VxThread:pthread_attr_setstacksize error %d\n", iErr );
					}
				}
			}

			iErr = pthread_attr_setdetachstate( &m_ThreadAttr, PTHREAD_CREATE_DETACHED );
			if( iErr )
			{
				LogMsg( LOG_FATAL, "VxThread:pthread_attr_setdetachstate error %d\n", iErr );
			}
		}

		memset( &m_ThreadInfo, 0, sizeof( m_ThreadInfo ) );
		if( 0 < ( iErr = pthread_create(	&m_ThreadInfo,		// thread structure
											&m_ThreadAttr,		// thread attributes
											pfuncThreadFunc,	// thread function
											(void *)this) ) )	// parameter to pass to thread function
		{
			m_u8ThreadFlags = 0;
			AppErr( eAppErrThreadCreateFailed, "VxThread:pthread_create FAILED %d %d\n", VxGetLastError(), iErr );
#ifdef TARGET_OS_ANDROID
			// android doesn't seem to have a abort
			exit(99);
#else
			abort();
#endif // TARGET_OS_ANDROID
			return -1;
		}
#endif
#ifdef DEBUG_THREADS
	LogMsg( LOG_VERBOSE, "VxThread: created thread %s with id %d\n", getThreadName(), getThreadTid() );
#endif // DEBUG_THREADS
	g_ThreadCreateCnt++;

	return 0;
}

//============================================================================
const char * VxThread::getThreadName( void )
{
	return m_strThreadName.c_str();
}

//============================================================================
//! get the user specified parameter
void * VxThread::getThreadUserParam( void )
{
	return m_pvUserParam;
}

//============================================================================
bool VxThread::isThreadCreated( void )			
{ 
	return (m_u8ThreadFlags & VX_FLAG_THREAD_CREATED) ? true : false;
}

//============================================================================
bool VxThread::isThreadRunning( void )			
{ 
	return ( m_u8ThreadFlags & VX_FLAG_THREAD_RUNNING ) ? true : false;
}

//============================================================================
void VxThread::setIsThreadCreated( bool bIsCreated )
{ 
	if( bIsCreated ) m_u8ThreadFlags |= VX_FLAG_THREAD_CREATED;
	else m_u8ThreadFlags &= ~VX_FLAG_THREAD_CREATED;
}

//============================================================================
//! thread calls this when thread done with exit
void VxThread::setIsThreadDone( bool bIsDoneExit )
{
	if( bIsDoneExit ) m_u8ThreadFlags |= VX_FLAG_THREAD_DONE;
	else m_u8ThreadFlags &= ~VX_FLAG_THREAD_DONE;
}

//============================================================================
bool VxThread::isThreadDone( void )
{
	return ( m_u8ThreadFlags & VX_FLAG_THREAD_DONE ) ? true : false;
}

//============================================================================
//! thread calls this when first starts running
void VxThread::setIsThreadRunning( bool bIsRunning, bool calledFromStartedThread )
{ 
	if( bIsRunning) 
	{
		m_u8ThreadFlags |= VX_FLAG_THREAD_RUNNING;
	}
	else
	{
		//vx_assert( false ); // should never happen. TODO remove bIsRunning
		m_u8ThreadFlags &= ~VX_FLAG_THREAD_RUNNING;
	}

	if( calledFromStartedThread && bIsRunning )
	{

		m_uiThreadId		= VxGetCurrentThreadId();
		m_ThreadTid			= VxGetCurrentThreadTid();
		//m_ThreadStartTime	= VxGetSysTimeSec();
		m_ThreadStartTime = time(NULL);
		#ifdef DEBUG_THREADS
		LogMsg( LOG_ERROR,
			"setIsThreadRunning started ? %d Thread %s id %d tid %d total running %d created %d\n",
			(U32)calledFromStartedThread,
			m_strThreadName.c_str(),
		    m_uiThreadId,
			m_ThreadTid,
			g_RuningThreadList.size(),
			g_ThreadCreateCnt );
		#endif // DEBUG_THREADS

		if( m_funcStartCallback )
		{
			m_funcStartCallback( m_ThreadTid, m_strThreadName.c_str() );
		}
#ifdef TARGET_OS_ANDROID
		else
		{
			LogMsg( LOG_ERROR, "setIsThreadRunning %s MUST HAVE CALLBACK\n", m_strThreadName.c_str() );
		}
#endif // TARGET_OS_ANDROID

#ifdef ENABLE_THREAD_INFO
		//if ( false == VxIsAppShuttingDown() )
		{
			std::vector<VxThread *>::iterator iter;
			g_DebugThreadsMutex.lock();
			bool foundOurself = false;
			for( iter = g_RuningThreadList.begin(); iter != g_RuningThreadList.end(); ++iter )
			{
				if( (*iter) == this )
				{
					LogMsg( LOG_ERROR, "Running Thread %s already in info list\n", m_strThreadName.c_str() );
					foundOurself = true;
					break;
				}
			}	

			if( !foundOurself )
			{
				g_RuningThreadList.push_back( this );
			}

			g_DebugThreadsMutex.unlock();
		}
#endif // ENABLE_THREAD_INFO 
	}
}

//============================================================================
bool VxThread::isThreadEndCallbackLocked( void )
{
	return m_bIsExitCallbackLocked;
}

//============================================================================
void VxThread::setIsThreadEndCallbackLocked( bool bIsLocked )
{
	m_bIsExitCallbackLocked = bIsLocked;
}

//============================================================================
//! Thread calls this just before exit
void VxThread::threadAboutToExit( bool bExitThreadNow )  
{
	unsigned int thisThreadId = getThreadTid();
	//LogMsg( LOG_DEBUG, "threadAboutToExit %s tid %d start\n", m_strThreadName.c_str(), thisThreadId );
#ifdef ENABLE_THREAD_INFO
	//if ( false == VxIsAppShuttingDown() )
	{
		if( thisThreadId != VxGetCurrentThreadTid() )
		{
			LogMsg( LOG_ERROR, "VxThread::threadAboutToExit Thread %s tid %d does not match current thread %d\n", 
				m_strThreadName.c_str(),
				thisThreadId,
				VxGetCurrentThreadTid() );
		}

		std::vector<VxThread *>::iterator iter;
		g_DebugThreadsMutex.lock();
		for( iter = g_RuningThreadList.begin(); iter != g_RuningThreadList.end(); ++iter )
		{
			if( this == (*iter) )
			{
				g_RuningThreadList.erase( iter );
				break;
			}
		}

		g_DebugThreadsMutex.unlock();
		LogMsg( LOG_INFO, "Thread %s tid %d exit done total running %d\n", m_strThreadName.c_str(), thisThreadId, g_RuningThreadList.size() );
	}
#endif // ENABLE_THREAD_INFO
	
	if ( m_funcExitCallback )
	{
#ifdef DEBUG_THREADS
		LogMsg( LOG_ERROR, "Thread %s %d tid %d exiting with callback total running %d\n", m_strThreadName.c_str(), m_uiThreadId, getThreadTid(), g_RuningThreadList.size() );
#endif // DEBUG_THREADS
		// if android cannot log after thread is detached in exit callback 
		m_funcExitCallback( m_ThreadTid, m_bIsExitCallbackLocked, m_strThreadName.c_str() );
	}
	else
	{
#ifdef DEBUG_THREADS
		LogMsg( LOG_THREAD, "threadAboutToExit %s %d tid %d\n", m_strThreadName.c_str(), m_uiThreadId, getThreadTid() );
#ifdef TARGET_OS_ANDROID
		LogMsg( LOG_ERROR, "threadAboutToExit %s tid %d MUST HAVE CALLBACK\n", m_strThreadName.c_str(), getThreadTid() );
#endif // TARGET_OS_ANDROID
#endif // DEBUG_THREADS
	}

	setIsThreadCreated( false );
	abortThreadRun( false ); // reset abort flag
	m_u8ThreadFlags |= VX_FLAG_THREAD_DONE; 
	m_u8ThreadFlags &= ~VX_FLAG_THREAD_RUNNING; 

	//LogMsg( LOG_DEBUG, "threadAboutToExit %s tid %d done\n", m_strThreadName.c_str(), thisThreadId );
	#ifndef TARGET_OS_WINDOWS
		if( bExitThreadNow )
		{
			// linux exit thread
			// cannot log at this point because would require access to native java class
				pthread_exit(0);
		}
	#endif // TARGET_OS_WINDOWS
}

//============================================================================
//! end a thread
RCODE VxThread::killThread( void )
{
	if( false == isThreadCreated() )
	{
		return 0;
	}

	//LogMsg( LOG_ERROR, "VxThread: killThread %s %d\n", getThreadName(), getThreadTid() );
    abortThreadRun( true ); // tell thread to abort
	if( ( m_u8ThreadFlags & VX_FLAG_THREAD_RUNNING ) 
		&& ( 0 == ( m_u8ThreadFlags & VX_FLAG_THREAD_DONE ) ) )
	{
		int iCnt = 0;
		while( isThreadRunning() )
		{
			//VxSleep( 10 );
			Sleep(10);
			iCnt++;
			if( iCnt > 600 )
			{
				// timeout and exit
				//LogMsg( LOG_ERROR, "VxThread: timeout of Thread %s %d\n", getThreadName(), getThreadTid() );
				return -1;
			}
		}
	}

	return 0;
}


