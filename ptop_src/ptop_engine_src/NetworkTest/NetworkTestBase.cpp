//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "NetworkTestBase.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceUtils.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <NetLib/VxSktUtil.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktConnectSimple.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

namespace
{
    //============================================================================
    void * NetworkTestBaseThreadFunc( void * pvContext )
    {
        VxThread * poThread = ( VxThread * )pvContext;
        poThread->setIsThreadRunning( true );
        NetworkTestBase * netTest = ( NetworkTestBase * )poThread->getThreadUserParam();
        if( netTest && false == poThread->isAborted() )
        {
            netTest->threadRunNetworkTest();
            netTest->networkTestComplete();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }
}

//============================================================================
NetworkTestBase::NetworkTestBase( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils )
    : m_Engine( engine )
    , m_EngineSettings( engineSettings )
    , m_NetServicesMgr( netServicesMgr )
    , m_NetServiceUtils( netServiceUtils )
    , m_ClientTestName( "Client Test" )
    , m_HostTestName( "Host Test" )
{
}

//============================================================================
void NetworkTestBase::startNetworkTest( void )
{
    m_RunTestThread.abortThreadRun( true );
    while( m_RunTestThread.isThreadRunning() )
    {
        VxSleep( 200 );
    }

    m_RunTestThread.startThread( ( VX_THREAD_FUNCTION_T )NetworkTestBaseThreadFunc, this, "NetworkTestBaseThread" );
}

//============================================================================
void NetworkTestBase::stopNetworkTest( void )
{
    m_RunTestThread.abortThreadRun( true );
}

//============================================================================
void NetworkTestBase::sendRunTestStatus( ERunTestStatus eStatus, const char * msg, ... )
{
    char as8Buf[ 1024 ];
    va_list argList;
    va_start( argList, msg );
    vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );
    IToGui::getToGui().toGuiRunTestStatus( getTestName().c_str(), eStatus, as8Buf );
}

//============================================================================
void NetworkTestBase::sendTestLog( const char * msg, ... )
{
    char as8Buf[ 1024 ];
    va_list argList;
    va_start( argList, msg );
    vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );
    IToGui::getToGui().toGuiRunTestStatus( getTestName().c_str(), eRunTestStatusLogMsg, as8Buf );
}
