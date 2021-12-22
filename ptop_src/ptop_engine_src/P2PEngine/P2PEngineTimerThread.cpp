//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "P2PEngine.h"

#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetworkMonitor.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxTime.h>

namespace
{
    //============================================================================
    void * P2PEngineTimerThreadFunc( void * pvContext )
    {
        VxThread * poThread = ( VxThread * )pvContext;
        poThread->setIsThreadRunning( true );
        P2PEngine * p2pEngine = ( P2PEngine * )poThread->getThreadUserParam();
        if( p2pEngine && false == poThread->isAborted() )
        {
            p2pEngine->executeTimerThreadFunctions();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }
}

//============================================================================
void P2PEngine::enableTimerThread( bool enable )
{
    if( enable && !VxIsAppShuttingDown() )
    {
        static int threadStartCnt = 0;
        threadStartCnt++;
        std::string timerThreadName;
        StdStringFormat( timerThreadName, "TimerThread_%d", threadStartCnt );
        m_TimerThread.killThread();
        m_TimerThread.startThread( ( VX_THREAD_FUNCTION_T )P2PEngineTimerThreadFunc, this, timerThreadName.c_str() );

        LogModule( eLogThread, LOG_VERBOSE, "timer thread %d started", threadStartCnt );
    }
    else
    {
        m_TimerThreadSemaphore.signal();
        m_TimerThread.killThread();
    }
}

//============================================================================
void P2PEngine::executeTimerThreadFunctions( void )
{
    while( !m_TimerThread.isAborted() && !VxIsAppShuttingDown() )
    {
        m_TimerThreadSemaphore.wait();
        if( !m_TimerThread.isAborted() && !VxIsAppShuttingDown() )
        {
            onOncePerSecond();
        }
        else
        {
            break;
        }
    }
}

//============================================================================
void P2PEngine::fromGuiOncePerSecond( void )
{
    m_TimerThreadSemaphore.signal();
}

//============================================================================
void P2PEngine::onOncePerSecond( void )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    m_NetworkMonitor.onOncePerSecond();
    m_NetworkMgr.onOncePerSecond();
    m_PluginMgr.onOncePerSecond();

    static int thirtySecCntInSeconds = 31;
    thirtySecCntInSeconds--;
    if( 0 >= thirtySecCntInSeconds )
    {
        thirtySecCntInSeconds = 30;
        onOncePer30Seconds();
        onOncePer15Minutes(); // BRJ for testing only.. remove me
    }

    static int minuteCntInSeconds = 62;
    minuteCntInSeconds--;
    if( 0 >= minuteCntInSeconds )
    {
        minuteCntInSeconds = 60;
        onOncePerMinute();
    }

    static int minute15CntSeconds = 60 * 15 + 3;
    minute15CntSeconds--;
    if( 0 >= minute15CntSeconds )
    {
        minute15CntSeconds = 60 * 15;
        onOncePer15Minutes();
    }

    static int minute30CntSeconds = 60 * 30 + 4;
    minute30CntSeconds--;
    if( 0 >= minute30CntSeconds )
    {
        minute30CntSeconds = 60 * 30;
        onOncePer30Minutes();
    }

    static int hourCntInSeconds = 3606;
    hourCntInSeconds--;
    if( 0 >= hourCntInSeconds )
    {
        hourCntInSeconds = 3600;
        onOncePerHour();
    }
}

//============================================================================
void P2PEngine::onOncePer30Seconds( void )
{
    m_RcScan.onOncePer30Seconds();
}

//============================================================================
void P2PEngine::onOncePerMinute( void )
{
    m_ConnectionList.broadcastSystemPkt( &m_PktImAliveReq, false );
    m_RcScan.onOncePerMinute();

//#ifdef _DEBUG
//	VxThread::dumpRunningThreads();
//#endif // _DEBUG
}

//============================================================================
void P2PEngine::onOncePer15Minutes( void )
{
    m_PluginMgr.onThreadOncePer15Minutes();
}

//============================================================================
void P2PEngine::onOncePer30Minutes( void )
{
}

//============================================================================
void P2PEngine::onOncePerHour( void )
{
    getNetworkStateMachine().onOncePerHour();
}
