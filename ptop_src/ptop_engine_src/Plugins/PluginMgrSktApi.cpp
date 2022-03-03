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
// http://www.nolimitconnect.com
//============================================================================
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktBase.h>
#include <NetLib/VxPeerMgr.h>

//============================================================================
bool PluginMgr::pluginApiSktConnectTo(		EPluginType			ePluginType,	// plugin id
											VxNetIdentBase *	netIdent,		// identity of contact to connect to
											int					pvUserData,		// plugin defined data
											VxSktBase **		ppoRetSkt, 		// returned Socket
											EConnectReason		connectReason )
{
	VxSktBase * sktBase = nullptr;
	* ppoRetSkt = nullptr;
	bool newConnection = false;
	if( true == m_Engine.connectToContact( netIdent->getConnectInfo(), &sktBase, newConnection, connectReason ) )
	{
		* ppoRetSkt = sktBase;
		return true;
	}

	return false;
}

//============================================================================
//! close socket connection
void PluginMgr::pluginApiSktClose( ESktCloseReason closeReason, VxSktBase * sktBase )
{
	sktBase->closeSkt(closeReason);
}

//============================================================================
//! close socket immediate.. don't bother to flush buffer
void PluginMgr::pluginApiSktCloseNow( ESktCloseReason closeReason, VxSktBase * sktBase )
{
	sktBase->closeSkt(closeReason,  false);
}

//============================================================================
bool PluginMgr::pluginApiTxPacket(  EPluginType			ePluginType,
                                    const VxGUID&       onlineId,
                                    VxSktBase*          sktBase,
                                    VxPktHdr*           pktHdr,
                                    bool				bDisconnectAfterSend,
                                    EPluginType         overridePlugin )
{
    // when sending packets they are typically from plugin to the same remote plugin
    // for host/client we convert host to client and client to hot
    EPluginType hostClientType = ePluginTypeInvalid;
    switch( ePluginType )
    {
    case ePluginTypeClientChatRoom:
        hostClientType = ePluginTypeHostChatRoom;
        break;
    case ePluginTypeHostChatRoom:
        hostClientType = ePluginTypeClientChatRoom;
        break;
    case ePluginTypeClientConnectTest:
        hostClientType = ePluginTypeHostConnectTest;
        break;
    case ePluginTypeHostConnectTest:
        hostClientType = ePluginTypeClientConnectTest;
        break;
    case ePluginTypeClientGroup:
        hostClientType = ePluginTypeHostGroup;
        break;
    case ePluginTypeHostGroup:
        hostClientType = ePluginTypeClientGroup;
        break;
    case ePluginTypeClientPeerUser:
        hostClientType = ePluginTypeHostPeerUser;
        break;
    case ePluginTypeHostPeerUser:
        hostClientType = ePluginTypeClientPeerUser;
        break;
    case ePluginTypeClientRandomConnect:
        hostClientType = ePluginTypeHostRandomConnect;
        break;
    case ePluginTypeHostRandomConnect:
        hostClientType = ePluginTypeClientRandomConnect;
        break;
    case ePluginTypeAboutMePageServer:
        hostClientType = ePluginTypeAboutMePageClient;
        break;
    case ePluginTypeAboutMePageClient:
        hostClientType = ePluginTypeAboutMePageServer;
        break;
    case ePluginTypeStoryboardServer:
        hostClientType = ePluginTypeStoryboardClient;
        break;
    case ePluginTypeStoryboardClient:
        hostClientType = ePluginTypeStoryboardServer;
        break;
    default:
        break;
    }

    if( overridePlugin != ePluginTypeInvalid )
    {
        pktHdr->setPluginNum( ( uint8_t )overridePlugin );
    }
    else if( hostClientType != ePluginTypeInvalid )
    {
        pktHdr->setPluginNum( ( uint8_t )hostClientType );
    }
    else
    {
        pktHdr->setPluginNum( ( uint8_t )ePluginType );
    }

    pktHdr->setSrcOnlineId( m_Engine.getMyOnlineId() );

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        // destination is ourself
        pktHdr->setDestOnlineId( onlineId );
        handleNonSystemPackets( sktBase, pktHdr );
        return true;
    }

#ifdef DEBUG
    // loopback flag is only for development convenience and should never be used for production
    if( pktHdr->getIsLoopback() )
    {
        pktHdr->setDestOnlineId( m_Engine.getMyOnlineId() );
        pktHdr->setSrcOnlineId( onlineId );
        handleNonSystemPackets( sktBase, pktHdr );
        return true;
    }
#endif // DEBUG

    return m_Engine.getPeerMgr().txPacket( sktBase, onlineId, pktHdr, bDisconnectAfterSend );
}

