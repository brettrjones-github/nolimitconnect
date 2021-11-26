//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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
#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetworkMonitor.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>

#include "P2PConnectList.h"

#include <ptop_src/ptop_engine_src/Network/NetworkDefs.h> 
#include <ptop_src/ptop_engine_src/Network/ConnectRequest.h> 
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <NetLib/VxSktBase.h>
#include <PktLib/PktsRelay.h>
#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
bool P2PEngine::connectToContact(	VxConnectInfo&		connectInfo, 
									VxSktBase **		ppoRetSkt,
									bool&				retIsNewConnection,
									EConnectReason		connectReason )
{
	bool result = m_NetConnector.connectToContact( connectInfo, ppoRetSkt, retIsNewConnection, connectReason );
	if(  true == result )
	{
		if( eConnectReasonRelaySearch == connectReason )
		{
			m_NetConnector.handlePossibleRelayConnect( connectInfo, *ppoRetSkt, retIsNewConnection, connectReason );
			return true;
		}
		else if( retIsNewConnection )
		{
			if( eConnectReasonRelaySearch == connectReason )
			{
			}

			// handle success connect
			BigListInfo * bigListInfo = 0;
			int retryCnt = 0;
			while( ( 0 == bigListInfo )
					&& ( retryCnt < 3 ) )
			{
				// wait for announce packet that was sent when connected to be rxed so we get big list info
				retryCnt++;
				bigListInfo = getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
				if( 0 == bigListInfo )
				{
					VxSleep( 1000 );
				}
			}

			if( bigListInfo )
			{
#ifdef DEBUG_CONNECTIONS
				LogMsg( LOG_INFO, "P2PEngine::connectToContact: success  %s\n", bigListInfo->getOnlineName() );
#endif // DEBUG_CONNECTIONS
				m_NetConnector.handleConnectSuccess( bigListInfo, *ppoRetSkt, retIsNewConnection, connectReason );
			}
#ifdef DEBUG_CONNECTIONS
			else
			{
				LogMsg( LOG_INFO, "NetConnector::doConnectRequest: No BigList for connected  %s\n", netIdent->getOnlineName() );
			}
#endif // DEBUG_CONNECTIONS
		}
	}

	return result;
}

//============================================================================
bool P2PEngine::txSystemPkt(	const VxGUID&		destOnlineId,
								VxSktBase *			sktBase, 
								VxPktHdr *			poPkt, 
								bool				bDisconnectAfterSend )
{
	bool bSendSuccess = false;
	poPkt->setSrcOnlineId( m_PktAnn.getMyOnlineId() );

	if( 0 == (poPkt->getPktLength() & 0xf ) )
	{
		if( sktBase->isConnected() && sktBase->isTxEncryptionKeySet() )
		{
			sktBase->m_u8TxSeqNum++;
			poPkt->setPktSeqNum( sktBase->m_u8TxSeqNum );
			RCODE rc = sktBase->txPacket( destOnlineId, poPkt, bDisconnectAfterSend );
			if( 0 == rc )
			{
				bSendSuccess = true;
			}
#ifdef DEBUG_PKTS
			else
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: skt %d error %d\n", sktBase->m_iSktId, sktBase->m_rcLastError );
			}
#endif // DEBUG_PKTS
		}
#ifdef DEBUG_PKTS
		else
		{
			if( false == sktBase->isConnected() )
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d not connected\n", sktBase->m_iSktId );
			}
			else
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d has no encryption key\n", sktBase->m_iSktId );
			}
		}
#endif // DEBUG_PKTS
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::txPluginPkt: Invalid system Packet length %d type %d\n", poPkt->getPktLength(), poPkt->getPktType() );
		vx_assert( false );
	}

	return bSendSuccess;
}

//============================================================================
bool P2PEngine::txImAlivePkt(	VxGUID&				destOnlineId, 
								VxSktBase *			sktBase )
{
	return txSystemPkt( destOnlineId, sktBase, &m_PktImAliveReq, false );
}

//============================================================================
void P2PEngine::broadcastSystemPkt( VxPktHdr * pkt, bool onlyIncludeMyContacts )
{
	m_ConnectionList.broadcastSystemPkt( pkt, onlyIncludeMyContacts );
}

//============================================================================
void P2PEngine::broadcastSystemPkt( VxPktHdr * pkt, VxGUIDList& retIdsSentPktTo )
{
	m_ConnectionList.broadcastSystemPkt( pkt, retIdsSentPktTo );
}

//============================================================================
bool P2PEngine::txSystemPkt(	VxNetIdentBase *	netIdent, 
								VxSktBase *			sktBase, 
								VxPktHdr *			poPkt, 
								bool				bDisconnectAfterSend )
{
	return txSystemPkt( netIdent->getMyOnlineId(), sktBase, poPkt, bDisconnectAfterSend );
}

//============================================================================
bool P2PEngine::txPluginPkt( 	EPluginType			ePluginType, 
								VxNetIdentBase *	netIdent, 
								VxSktBase *			sktBase, 
								VxPktHdr *			poPkt, 
								bool				bDisconnectAfterSend )
{
	bool bSendSuccess = false;
	if( 0 == (poPkt->getPktLength() & 0xf ) )
	{
		LogModule( eLogPkt, LOG_VERBOSE, "skt %d txPluginPkt\n", sktBase->m_iSktId );
		if( sktBase->isConnected() && sktBase->isTxEncryptionKeySet() )
		{
			poPkt->setSrcOnlineId( m_PktAnn.getMyOnlineId() );
			poPkt->setPluginNum( (uint8_t)ePluginType );
			sktBase->m_u8TxSeqNum++;
			poPkt->setPktSeqNum( sktBase->m_u8TxSeqNum );
			RCODE rc = sktBase->txPacket( netIdent->getMyOnlineId(), poPkt, bDisconnectAfterSend );
			if( 0 == rc )
			{
				bSendSuccess = true;
			}
			else
			{
				LogMsg( LOG_ERROR, "P2PEngine::txPluginPkt: error %d\n", rc );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "skt %d P2PEngine::txPluginPkt: ERROR disconnected or no TxEncryption key\n", sktBase->m_iSktId );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::txPluginPkt: Invalid Packet length %d type %d from plugin %s\n", 
			poPkt->getPktLength(),
			poPkt->getPktType(),
			DescribePluginLclName( ePluginType ));
	}
	return bSendSuccess;
}

//============================================================================
void P2PEngine::attemptConnectionToRelayService( BigListInfo * poInfo )
{
	VxSktBase * sktBase = nullptr;
	m_NetConnector.directConnectTo( poInfo->getConnectInfo(), &sktBase, eConnectReasonRelayService );
}

//============================================================================
bool P2PEngine::isContactConnected( VxGUID& oOnlineId )
{
	return (nullptr != m_ConnectionList.findConnection( oOnlineId )) ? true : false;
}

//============================================================================
std::string P2PEngine::describeContact( BigListInfo * bigListInfo )
{
	VxConnectInfo& connectInfo = bigListInfo->getConnectInfo();

	std::string hexId;
	connectInfo.getMyOnlineId().toHexString( hexId );

	std::string strDesc;
	if( connectInfo.requiresRelay() )
	{
		StdStringFormat( strDesc, " %s Connected ? %d Online ID %s using relay ip %s ", 
			bigListInfo->getOnlineName(),
			bigListInfo->isConnected() ? 1 : 0,
			hexId.c_str(), 
			connectInfo.getRelayIpAddress().toStdString().c_str() );
	}
	else
	{
		StdStringFormat( strDesc, " %s Connected ? %d Online ID %s at ip %s ", 
			bigListInfo->getOnlineName(),
			bigListInfo->isConnected() ? 1 : 0,
			hexId.c_str(), 
			connectInfo.getMyOnlineIPv4().toStdString().c_str() );
	}

	return strDesc;
}

//============================================================================
std::string P2PEngine::describeContact( VxConnectInfo& connectInfo )
{
	BigListInfo * poBigListInfo = getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
	if( poBigListInfo )
	{
		return describeContact( poBigListInfo );
	}

	std::string hexId;
	connectInfo.getMyOnlineId().toHexString( hexId );

	std::string strDesc;
	if( connectInfo.requiresRelay() )
	{
		StdStringFormat( strDesc, " name %s ID %s using relay ip %s ", 
			(0 == strlen( connectInfo.getOnlineName() )) ? "UNKNOWN" : connectInfo.getOnlineName(),
			hexId.c_str(), 
			connectInfo.getRelayIpAddress().toStdString().c_str() );
	}
	else
	{
		StdStringFormat( strDesc, " name %s ID %s at ip %s ", 
			(0 == strlen( connectInfo.getOnlineName() )) ? "UNKNOWN" : connectInfo.getOnlineName(),
			hexId.c_str(), 
			connectInfo.getMyOnlineIPv4().toStdString().c_str() );
	}
	
	return strDesc;
}

//============================================================================
std::string P2PEngine::describeContact( ConnectRequest& connectRequest )
{
	return describeContact( connectRequest.getConnectInfo() );
}
