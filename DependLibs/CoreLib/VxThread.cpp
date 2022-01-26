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
// http://www.nolimitconnect.com
//============================================================================
#include "config_corelib.h"

#include "VxThread.h"
#include "VxMutex.h"
#include "VxGlobals.h"
#include "VxTimeUtil.h"
#include "VxDebug.h"
#include "VxParse.h"
#include "AppErr.h"

#include "memory.h"

#ifndef TARGET_OS_WINDOWS
	#include <sys/types.h> 
	#include <sys/syscall.h> 
	#include <unistd.h>
    #include <stdlib.h>
#endif // TARGET_OS_WINDOWS

//#define ENABLE_THREAD_INFO 1

class VxThreadInfo
{
public:
    VxThreadInfo( unsigned int threadId, const char * threadName )
        : m_ThreadId( threadId )
        , m_ThreadName( threadName )
    {
        if( m_ThreadName.empty() )
        {
            m_ThreadName = "unknown";
        }
    }

    // overload assignment operator
    VxThreadInfo( const VxThreadInfo & rhs )
    {
        *this = rhs;
    }

    VxThreadInfo &operator=( const VxThreadInfo & rhs )
    {
        if( this != &rhs )
        {
            m_ThreadId      = rhs.m_ThreadId;
            m_ThreadName    = rhs.m_ThreadName;
        }

        return *this;
    }

    //============================================================================
    void                        dumpThreadInfo( void )
    {
        LogMsg( LOG_INFO, "ThreadsInfo id 0x%X thread name %s", m_ThreadId, m_ThreadName.c_str() );
    }

protected:

private:
    unsigned int				m_ThreadId;
    std::string                 m_ThreadName;
};

namespace
{
    int							g_ThreadCreateCnt{ 0 };
	unsigned int				g_GuiThreadId{ 0 };
    VxMutex						g_DebugThreadsMutex;
    std::vector<VxThread *>		g_RuningThreadList;
    std::vector<VxThreadInfo>   g_RuningThreadInfo;
}

//============================================================================
void VxThreadDefaultStartCallback( unsigned int threadId, const char * threadName )
{
#ifdef ENABLE_THREAD_INFO
    if( threadName )
    {
        g_DebugThreadsMutex.lock();
        VxThreadInfo threadInfo( threadId, threadName );
        g_RuningThreadInfo.push_back( threadInfo );
        g_DebugThreadsMutex.unlock();
        LogMsg( LOG_DEBUG, "VxThreadStartCallback: thread id 0x%X %s total count%d", threadId, threadName, g_RuningThreadInfo.size() );
   }
    else
    {
        LogMsg( LOG_DEBUG, "ERROR VxThreadStartCallback: thread id 0x%X has no name" );
    }
#endif //ENABLE_THREAD_INFO
}

//============================================================================
void VxThreadDefaultStopCallback( unsigned int threadId, bool iIsExitCallbackLocked, const char * threadName )
{
#ifdef ENABLE_THREAD_INFO
    if( threadName )
    {
        LogMsg( LOG_DEBUG, "VxThreadStopCallback: thread id 0x%X %s Locked = %d", threadId, threadName, iIsExitCallbackLocked );
    }
    else
    {
        LogMsg( LOG_DEBUG, "ERROR VxThreadStopCallback: thread id 0x%X has no name" );
    }
#endif //ENABLE_THREAD_INFO
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
//! set the thread id of qt application using current thread id
void VxSetGuiThreadId( void )
{
	g_GuiThreadId = VxGetCurrentThreadTid();
}

//============================================================================
//! get the thread id of qt application
unsigned int VxGetGuiThreadId( void )
{
	return g_GuiThreadId;
}

//============================================================================
std::string		VxGetThreadInfo( void )
{
	std::string threadInfo;
#ifdef TARGET_OS_WINDOWS
	StdStringFormat( threadInfo, " @[tid=%d] ",   VxGetCurrentThreadId() );
#else
	StdStringFormat( threadInfo, " @[tid=%d] ",   VxGetCurrentThreadTid() );
#endif // TARGET_OS_WINDOWS
	return threadInfo;
}


//============================================================================
VX_THREAD_START_CALLBACK_T VxThread::m_funcStartCallback = VxThreadDefaultStartCallback;
VX_THREAD_EXIT_CALLBACK_T VxThread::m_funcExitCallback = VxThreadDefaultStopCallback;

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
, m_ThreadStartTimeGmtMs( 0 )
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
		
        std::vector<VxThreadInfo>::iterator infoIter;
        for( infoIter = g_RuningThreadInfo.begin(); infoIter != g_RuningThreadInfo.end(); ++infoIter )
        {
            ( *infoIter ).dumpThreadInfo();
        }

		g_DebugThreadsMutex.unlock();
	}
#endif // ENABLE_THREAD_INFO 
}

//============================================================================
void VxThread::dumpThreadInfo( void )
{
	std::string timeStart = VxTimeUtil::formatTimeStampIntoHoursAndMinutesAndSeconds( m_ThreadStartTimeGmtMs, true );
    LogModule( eLogThread, LOG_INFO, "Thrd: tid %d id 0x%x %s started %s", m_ThreadTid, m_uiThreadId, m_strThreadName.c_str(), timeStart.c_str() );
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
#if defined(DEBUG_THREADS)
    LogMsg( LOG_INFO, "$$$VxThread::abortThreadRun %s\n", getThreadName() );
    dumpRunningThreads();
#endif // DEBUG_THREADS
}

//============================================================================
//! set function that will be called when any thread is started
void VxThread::setThreadStartCallback( VX_THREAD_START_CALLBACK_T func )
{
#if defined(DEBUG_THREADS)
	    LogMsg( LOG_INFO, "$$$VxThread::setThreadStartCallback\n" );
#endif // DEBUG_THREADS

	m_funcStartCallback = func;
}

//============================================================================
//! set function that will be called when any thread is about to exit
void VxThread::setThreadExitCallback( VX_THREAD_EXIT_CALLBACK_T func )
{
#if defined(DEBUG_THREADS)
	    LogMsg( LOG_INFO, "$$$VxThread::setThreadExitCallback\n" );
#endif // DEBUG_THREADS

	m_funcExitCallback = func;
}

//============================================================================
RCODE VxThread::startThread(	VX_THREAD_FUNCTION_T	pfuncThreadFunc,	// function that thread calls
								void *					pvUserParam, 		// caller defined param
								const char *			pThreadName,		// thread name
								int						iExtraStackSpace )	// will be added to minimum stack size	
{
    if( this->isThreadRunning() )
    {
        LogMsg( LOG_FATAL, "ERROR VxThread::startThread thread %s still running", m_strThreadName.c_str() );
        vx_assert( false == this->isThreadRunning() );
        return -1;
    }

	g_ThreadCreateCnt++;
	m_pvUserParam = pvUserParam;
	if( pThreadName )
	{
		m_strThreadName = pThreadName;
	}
	else
	{
		StdStringFormat( m_strThreadName, "WorkerThread0x%x-%d", this, g_ThreadCreateCnt );
	}

	abortThreadRun( false );
	setIsThreadDone( false );
	setIsThreadRunning( true, false );	
    setIsThreadCreated( false );

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
			AppErr( eAppErrThreadCreateFailed, "VxThread:: ERROR.. FAILED TO CREATE THREAD\n" );
			abort();
			return -1;
		}

        setIsThreadCreated( true );
        LogModule( eLogThread, LOG_INFO, "VxThread:Created Thread %s thread id 0x%x", m_strThreadName.c_str(), m_uiThreadId );

	#else // LINUX

		int iErr;
		if( 0 != ( iErr = pthread_attr_init( &m_ThreadAttr ) ) )
		{
            AppErr( eAppErrThreadCreateFailed, "VxThread:pthread_attr_init FAILED %d %d", VxGetLastError(), iErr );
		}
		else
		{
			if( iExtraStackSpace )
			{
				size_t sizetStackSize = 0;
				iErr = pthread_attr_getstacksize( &m_ThreadAttr, &sizetStackSize );
				if( iErr )
				{
                    LogMsg( LOG_FATAL, "VxThread:pthread_attr_getstacksize error %d", iErr );
				}
				else
				{
                    #if defined(DEBUG_THREADS)
                        LogMsg( LOG_INFO, "VxThread:Created Thread %s min stack size %d\n", m_strThreadName.c_str(), sizetStackSize );
                    #endif // DEBUG_THREADS
					sizetStackSize += iExtraStackSpace;
					iErr = pthread_attr_setstacksize( &m_ThreadAttr, sizetStackSize );
					if( iErr )
					{
                        LogMsg( LOG_FATAL, "VxThread:pthread_attr_setstacksize error %d", iErr );
					}
				}
			}

			iErr = pthread_attr_setdetachstate( &m_ThreadAttr, PTHREAD_CREATE_DETACHED );
			if( iErr )
			{
                LogMsg( LOG_FATAL, "VxThread:pthread_attr_setdetachstate error %d", iErr );
			}
		}

		memset( &m_ThreadInfo, 0, sizeof( m_ThreadInfo ) );
		if( 0 < ( iErr = pthread_create(	&m_ThreadInfo,		// thread structure
											&m_ThreadAttr,		// thread attributes
											pfuncThreadFunc,	// thread function
											(void *)this) ) )	// parameter to pass to thread function
		{
			m_u8ThreadFlags = 0;
            AppErr( eAppErrThreadCreateFailed, "VxThread:pthread_create FAILED %d %d", VxGetLastError(), iErr );
#ifdef TARGET_OS_ANDROID
			// android doesn't seem to have a abort
			exit(99);
#else
			abort();
#endif // TARGET_OS_ANDROID
			return -1;
		}
#endif

#if defined(DEBUG_THREADS)
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
		vx_assert( false ); // should never happen. TODO remove bIsRunning
		m_u8ThreadFlags &= ~VX_FLAG_THREAD_RUNNING;
	}

	if( calledFromStartedThread && bIsRunning )
	{

		m_uiThreadId		    = VxGetCurrentThreadId();
		m_ThreadTid			    = VxGetCurrentThreadTid();
		m_ThreadStartTimeGmtMs	= GetGmtTimeMs();

        LogModule( eLogThread, LOG_ERROR,
            "setIsThreadRunning started ? %d Thread %s id 0x%X tid %d total running %d created %d",
			(uint32_t)calledFromStartedThread,
			m_strThreadName.c_str(),
		    m_uiThreadId,
			m_ThreadTid,
			g_RuningThreadList.size(),
			g_ThreadCreateCnt );

		if( m_funcStartCallback )
		{
			m_funcStartCallback( m_ThreadTid, m_strThreadName.c_str() );
		}
#ifdef TARGET_OS_ANDROID
		else
		{
            LogMsg( LOG_ERROR, "setIsThreadRunning %s MUST HAVE CALLBACK", m_strThreadName.c_str() );
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
    LogModule( eLogThread, LOG_DEBUG, "threadAboutToExit %s tid %d start", m_strThreadName.c_str(), thisThreadId );

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
       LogModule( eLogThread, LOG_ERROR, "Thread %s 0x%x tid %d exiting with callback total running %d", m_strThreadName.c_str(), m_uiThreadId, getThreadTid(), g_RuningThreadList.size() );

		// if android cannot log after thread is detached in exit callback 
		m_funcExitCallback( m_ThreadTid, m_bIsExitCallbackLocked, m_strThreadName.c_str() );
	}
	else
	{

        LogModule( eLogThread, LOG_VERBOSE, "threadAboutToExit %s %u tid %d", m_strThreadName.c_str(), m_uiThreadId, getThreadTid() );

        #ifdef TARGET_OS_ANDROID
            LogMsg( LOG_ERROR, "threadAboutToExit %s tid %d MUST HAVE CALLBACK", m_strThreadName.c_str(), getThreadTid() );
        #endif // TARGET_OS_ANDROID
    }

	setIsThreadCreated( false );
	abortThreadRun( false ); // reset abort flag
    m_u8ThreadFlags &= ~VX_FLAG_THREAD_RUNNING;
    m_u8ThreadFlags |= VX_FLAG_THREAD_DONE;

   LogModule( eLogThread, LOG_DEBUG, "threadAboutToExit %s tid %d done", m_strThreadName.c_str(), thisThreadId );

    #ifndef TARGET_OS_WINDOWS
		if( bExitThreadNow )
		{
            // thread about to exit really is thread exit in linux and android
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

    LogModule( eLogThread, LOG_ERROR, "VxThread: killThread %s id 0x%x tid %d", getThreadName(), getThreadId(), getThreadTid() );

    abortThreadRun( true ); // tell thread to abort
	if( ( m_u8ThreadFlags & VX_FLAG_THREAD_RUNNING ) 
		&& ( 0 == ( m_u8ThreadFlags & VX_FLAG_THREAD_DONE ) ) )
	{
		int iCnt = 0;
		while( isThreadRunning() )
		{
			VxSleep( 10 );
			iCnt++;
			if( iCnt > 600 )
			{
				// timeout and exit
                LogMsg( LOG_ERROR, "VxThread: timeout of Thread %s tid %d", getThreadName(), getThreadTid() );
				return -1;
			}
        }
	}

	return 0;
}


