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

#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>

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
        m_TimerThread.killThread();
    }
}

//============================================================================
void P2PEngine::executeTimerThreadFunctions( void )
{
    static uint64_t timeLast = GetGmtTimeMs();
    while( !m_TimerThread.isAborted() && !VxIsAppShuttingDown() )
    {
        uint64_t timeNow = GetGmtTimeMs();
        if( timeNow - timeLast >= 1000 )
        {
            timeNow = timeLast;
            onThreadOncePerSecond();
        }

        VxSleep( 400 );
    }
}

//============================================================================
void P2PEngine::onThreadOncePerSecond( void )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

//    m_NetworkMonitor.onOncePerSecond();
//    m_NetworkMgr.onOncePerSecond();
//    m_PluginMgr.onOncePerSecond();

    //static int announceCntInSeconds = 60;
    //	announceCntInSeconds--;
    //	if( 0 >= announceCntInSeconds )
    //	{
    //		announceCntInSeconds = 60;
    //		if( isP2POnline() && ( false == m_EngineSettings.getIsThisNodeAnNetHost() ) )
    //		{
    //			m_NetServicesMgr.announceToHost( m_NetworkStateMachine.getHostIp(), m_NetworkStateMachine.getHostPort() );
    //		}
    //	}
    static int thirtySecCntInSeconds = 31;
    thirtySecCntInSeconds--;
    if( 0 >= thirtySecCntInSeconds )
    {
        thirtySecCntInSeconds = 30;
        onThreadOncePer30Seconds();
    }

    static int minuteCntInSeconds = 62;
    minuteCntInSeconds--;
    if( 0 >= minuteCntInSeconds )
    {
        minuteCntInSeconds = 60;
        onThreadOncePerMinute();
    }

    static int minute15CntSeconds = 60 * 15 + 3;
    minute15CntSeconds--;
    if( 0 >= minute15CntSeconds )
    {
        minute15CntSeconds = 60 * 15;
        onThreadOncePer15Minutes();
    }

    static int minute30CntSeconds = 60 * 30 + 4;
    minute30CntSeconds--;
    if( 0 >= minute30CntSeconds )
    {
        minute30CntSeconds = 60 * 30;
        onThreadOncePer30Minutes();
    }


    static int hourCntInSeconds = 3606;
    hourCntInSeconds--;
    if( 0 >= hourCntInSeconds )
    {
        hourCntInSeconds = 3600;
        onThreadOncePerHour();
    }
}

//============================================================================
void P2PEngine::onThreadOncePer30Seconds( void )
{
    //m_RcScan.onOncePer30Seconds();
}

//============================================================================
void P2PEngine::onThreadOncePerMinute( void )
{
    //m_ConnectionList.broadcastSystemPkt( &m_PktImAliveReq, false );
    //m_RcScan.onOncePerMinute();
    //LogMsg( LOG_INFO, "Sockets current in memory %d total created %d\n", VxSktBase::getCurrentSktCount(), VxSktBase::getTotalCreatedSktCount() );

//#ifdef _DEBUG
//	VxThread::dumpRunningThreads();
//#endif // _DEBUG
}

//============================================================================
void P2PEngine::onThreadOncePer15Minutes( void )
{
    //m_RcScan.onOncePer30Seconds();
    m_PluginMgr.onThreadOncePer15Minutes();
}

//============================================================================
void P2PEngine::onThreadOncePer30Minutes( void )
{
    //m_RcScan.onOncePer30Seconds();
}

//============================================================================
void P2PEngine::onThreadOncePerHour( void )
{
    //getNetworkStateMachine().onOncePerHour();
}
