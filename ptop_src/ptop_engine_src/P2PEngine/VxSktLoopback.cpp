//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxTime.h>

namespace
{
    //============================================================================
    void* VxSktLoopbackThreadFunc( void* pvContext )
    {
        VxThread* poThread = ( VxThread* )pvContext;
        poThread->setIsThreadRunning( true );
        VxSktLoopback* sktLoopback = ( VxSktLoopback* )poThread->getThreadUserParam();
        if( sktLoopback && false == poThread->isAborted() )
        {
            sktLoopback->executeSktLoopbackRxThreadFunc();
        }

        poThread->threadAboutToExit();
        sktLoopback->checkForMorePacketsAfterThreadExit();
        return nullptr;
    }
} // namespace

//============================================================================
VxSktLoopback::VxSktLoopback( P2PEngine& engine )
    : VxSktBase()
    , m_Engine( engine )
{
	m_eSktType = eSktTypeLoopback;
    m_ConnectionId = m_LoopbackConnectId;
}

//============================================================================
RCODE VxSktLoopback::txPacketWithDestId( VxPktHdr * pktHdrIn, bool bDisconnect )
{
    if( !VxIsAppShuttingDown() && pktHdrIn )
    {
        vx_assert( pktHdrIn->getDestOnlineId() == m_Engine.getMyOnlineId() );

        VxPktHdr* pktHdr = pktHdrIn->makeCopy();

        lockPktList();
        m_PktList.push_back( pktHdr );
        unlockPktList();

        enableSktLoopbackThread( true );
    }

    return 0;
}

//============================================================================
void VxSktLoopback::enableSktLoopbackThread( bool enable )
{
    if( enable && !VxIsAppShuttingDown() && m_PktList.size() <= 1 && !m_SktLoopbackThread.isThreadRunning() )
    {
        // if the multiple threads run for a bit it should not matter
        std::string threadName{ "SktLoopbackThread" };
        m_SktLoopbackThread.startThread( ( VX_THREAD_FUNCTION_T )VxSktLoopbackThreadFunc, this, threadName.c_str() );
    }
    else if( !enable )
    {
        m_SktLoopbackThread.killThread();
    }
}

//============================================================================
void VxSktLoopback::executeSktLoopbackRxThreadFunc( void )
{
    while( !m_SktLoopbackThread.isAborted() && !VxIsAppShuttingDown() && !m_PktList.empty() )
    {
        VxPktHdr* pktHdr{ nullptr };
        lockPktList();
        auto iter = m_PktList.begin();
        if( iter != m_PktList.end() )
        {
            pktHdr = *iter;
            m_PktList.erase( iter );
        }
        
        unlockPktList();
        if( pktHdr )
        {
            m_Engine.handlePkt( this, pktHdr );
            delete pktHdr;
        }
    }
}

//============================================================================
void VxSktLoopback::checkForMorePacketsAfterThreadExit( void )
{
    if( !m_PktList.empty() )
    {
        enableSktLoopbackThread( true );
    }
}