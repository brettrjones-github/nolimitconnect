//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginConnectionTestClient.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>

#include <NetLib/VxSktBase.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER


//============================================================================
PluginConnectionTestClient::PluginConnectionTestClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBaseHostClient( engine, pluginMgr, myIdent, pluginType )
, m_NetServicesMgr( engine.getNetServicesMgr() )
{
    setPluginType( ePluginTypeClientConnectTest );
}

//============================================================================
void PluginConnectionTestClient::testIsMyPortOpen( void )
{
    m_NetServicesMgr.addNetActionIsMyPortOpenToQueue();
}

//============================================================================
RCODE PluginConnectionTestClient::handlePtopConnection( VxSktBase* sktBase, NetServiceHdr& netServiceHdr )
{
    //if( false == m_NetServiceUtil.isAllHttpContentArrived( sktBase ) )
    //{
    //	LogMsg( LOG_ERROR, "PluginNetServices::handlePtopConnection: not all of http content arrived\n" );
    //	return 0; // don't error.. we should get more later
    //}
    RCODE rc = 0;
    if( sktBase->isConnected() )
    {
        rc = internalHandlePtopConnection( sktBase, netServiceHdr );

        // flush the socket
        sktBase->getSktReadBuf();// so lock.. will unlock with sktBufAmountRead
        sktBase->sktBufAmountRead( netServiceHdr.m_TotalDataLen );
    }

    return rc;
}

//============================================================================
RCODE PluginConnectionTestClient::internalHandlePtopConnection( VxSktBase* sktBase, NetServiceHdr& netServiceHdr )
{
    switch( netServiceHdr.m_NetCmdType )
    {
    //case eNetCmdAboutMePage:
    //    LogMsg( LOG_ERROR, "PluginNetServices::handlePtopConnection: invalid cmd  eNetCmdAboutMePage\n" );
    //    return -1;

    //case eNetCmdStoryboardPage:
    //    LogMsg( LOG_ERROR, "PluginNetServices::handlePtopConnection: invalid cmd  eNetCmdStoryboardPage\n" );
    //    return -1;

    case eNetCmdPing:
        //LogMsg( LOG_INFO, "PluginNetServices::handlePtopConnection: eNetCmdPing\n" );
        return m_NetServicesMgr.handleNetCmdPing( sktBase, netServiceHdr );

    case eNetCmdPong:
        //LogMsg( LOG_INFO, "PluginNetServices::handlePtopConnection: eNetCmdPong\n" );
        return m_NetServicesMgr.handleNetCmdPong( sktBase, netServiceHdr );

    case eNetCmdIsMyPortOpenReq:
        return m_NetServicesMgr.handleNetCmdIsMyPortOpenReq( sktBase, netServiceHdr );

    case eNetCmdIsMyPortOpenReply:
        return m_NetServicesMgr.handleNetCmdIsMyPortOpenReply( sktBase, netServiceHdr );

    //case eNetCmdHostReq:
    //    return m_NetServicesMgr.handleNetCmdHostReq( sktBase, netServiceHdr );

    //case eNetCmdHostReply:
    //    return m_NetServicesMgr.handleNetCmdHostReply( sktBase, netServiceHdr );

    case eNetCmdUnknown:
    default:
        LogMsg( LOG_ERROR, "PluginNetServices::handlePtopConnection: unknown cmd type\n" );
        return -1;
    }

    return 0;
}
