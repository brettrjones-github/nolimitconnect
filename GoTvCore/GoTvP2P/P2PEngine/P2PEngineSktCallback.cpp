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
#include "P2PConnectList.h"

#include <GoTvCore/GoTvP2P/Network/NetworkDefs.h> 
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginServiceRelay.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsRelay.h>
#include <NetLib/VxSktCrypto.h>

#include <string.h>

//============================================================================
void P2PEngine::handleTcpData( VxSktBase * sktBase )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	// NOTE: TODO check if is in our ip address ignore list
	int	iDataLen =	sktBase->getSktBufDataLen();
	if( iDataLen < 16 )
	{
		return;
	}

	if( sktBase->isPluginSpecificSkt() )
	{
		m_PluginMgr.handlePluginSpecificSkt( sktBase );
	}

	uint32_t u32UsedLen = 0;
	// app should get the buffer ( this also locks it from being modified by threads )
	// then read the data then call Amount read
    uint8_t * pSktBuf = (uint8_t *)sktBase->getSktReadBuf();
	VxPktHdr * pktHdr = (VxPktHdr *)pSktBuf;
	if( sktBase->isFirstRxPacket() )
	{
		if( false == sktBase->isRxEncryptionKeySet() )
		{
			if( iDataLen < 32 )
			{
				return;
			}

			// check if all ascii
			bool bIsAscii = true;
			for( int i = 0; i < 32; i++ )
			{
				if( (pSktBuf[i] < 2) || (pSktBuf[i] > 127) )
				{
					bIsAscii = false;
					break;
				}
			}

			if( bIsAscii )
			{
				pSktBuf[ iDataLen ] = 0;
				sktBase->sktBufAmountRead( 0 ); // release mutex
                LogMsg( LOG_VERBOSE, "Rxed ascii %s", pSktBuf );
				if( m_PluginMgr.handleFirstWebPageConnection( sktBase ) )
				{
					// was either profile or storyboard web page
					return;
				} 
				
                if( ( iDataLen > 7 ) && ( 0 == strncmp( ( char * )pSktBuf, "http://", 7 ) ) )
				{
                    LogMsg( LOG_VERBOSE, "Rxed net %s", pSktBuf );
					// probably net services
					if( NetServiceUtils::verifyAllDataArrivedOfNetServiceUrl( sktBase ) )
					{
						m_PluginMgr.handleFirstNetServiceConnection( sktBase );
					}
				}

				return;
			}			
		}
		else
		{
			sktBase->decryptReceiveData();
			iDataLen = sktBase->getRxDecryptedLen();
		}

		// not http.. better be a announce packet
		if( (false == sktBase->isAcceptSocket()) 
			&& (false == sktBase->isRxEncryptionKeySet() ) )
		{
			// we connected out but never set our key
			vx_assert( false );
		}

		if( false == sktBase->isRxEncryptionKeySet() )
		{
			// this data has not been decrypted.. set encryption key and
			// decrypt it
			//m_PktAnn.DebugDump();
			//vx_assert( sktBase->getRxDecryptedLen() == 0 );
			GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_NetworkMgr.getNetworkKey() );
			sktBase->decryptReceiveData();
			iDataLen = sktBase->getRxDecryptedLen();
		}

		if( ( PKT_TYPE_ANNOUNCE != pktHdr->getPktType() ) ||
			 ( pktHdr->getPktLength() < 288 ) ||  // currently length of pkt announce
			 ( pktHdr->getPktLength() > sizeof( PktAnnounce ) + 100 ) ) // leave room for expanding pkt announce in the future
		{
			// somebody tried to send crap .. this may be a hack attack or it may be that our ip and port is same as someone else or network key has changed
			LogMsg( LOG_INFO, "First packet is not Announce pkt skt %d type %d length %d ip %s:%d id %s\n", sktBase->getSktId(), 
																							pktHdr->getPktType(),  
																							pktHdr->getPktLength(),
                                                                                            sktBase->getRemoteIp().c_str(),
																							sktBase->getRemotePort(),
																							pktHdr->getSrcOnlineId().toHexString().c_str() );

			//hackerOffense( NULL, 1, sktBase->getRemoteIpBinary(), "Hacker no announcement attack from ip %s\n", sktBase->getRemoteIp() );
			// release the mutex
			sktBase->sktBufAmountRead( 0 );
            hackerOffense( eHackerLevelMedium, eHackerReasonPktAnnNotFirstPacket, NULL, sktBase->getRemoteIpBinary(), "Hacker no announcement attack from ip %s\n", sktBase->getRemoteIp().c_str() );
			sktBase->closeSkt( eSktClosePktAnnNotFirstPacket );
			return;
		}

		if( ( true == sktBase->isAcceptSocket() ) 
			&& ( false == this->isP2POnline() ) )
		{
            LogMsg( LOG_INFO, "Not Online Yet so cannot accept skt id %d ip %s\n", sktBase->getSktId(), sktBase->getRemoteIp().c_str() );
			sktBase->sktBufAmountRead( 0 );
			sktBase->closeSkt( eSktCloseP2PNotReadyForAcceptSkt );
			return;
		}

		if( false == pktHdr->isPktAllHere(iDataLen) )
		{
			// not all of packet arrived
			return;
		}

		// pkt announce has arrived
		if( false == pktHdr->isValidPkt() )
		{
			// invalid announcement packet
			sktBase->setIsFirstRxPacket( false ); 
			// release the mutex
			sktBase->sktBufAmountRead( 0 );
            LogMsg( LOG_ERROR, "Invalid Packet announce from ip %s\n", sktBase->getRemoteIp().c_str() );
			// disconnect
			sktBase->closeSkt( eSktClosePktAnnInvalid );
		}

		//NOTE: TODO check if is in our Ident ignore list

		//LogMsg( LOG_INFO, "Got Ann on Skt %d\n", sktBase->m_iSktId );

		u32UsedLen = pktHdr->getPktLength();

		sktBase->setIsFirstRxPacket( false );
		onPktAnnounce( sktBase, pktHdr );
		// success
		// fall thru in case there are more packets
	}

	while( true )
	{
		//LogMsg( LOG_INFO, "AppRcpCallback.. 3 Skt %d num %d Total Len %d Used Len %d decrypted Len %d\n", 
		//	sktBase->GetSocketId(),
		//	sktBase->m_iSktId,
		//	u32Len,
		//	u32UsedLen,
		//	sktBase->m_u32DecryptedLen );
		if( false == sktBase->isConnected() )
		{
			//socket has been closed
			LogMsg( LOG_INFO, "P2PEngine::handleTcpData: callback was for data but socket is not connected\n" );
			return;
		}

		if( sktBase->getRxDecryptedLen() <= u32UsedLen )
		{
			//all decrypted data used up
			break;
		}

		if( 16 > ( sktBase->getRxDecryptedLen() - u32UsedLen ) )
		{
			//not enough for a valid packet
			break;
		}

		pktHdr = (VxPktHdr *)&pSktBuf[ u32UsedLen ];
		if( false == pktHdr->isValidPkt() )
		{
			// invalid data
			hackerOffense( eHackerLevelMedium, eHackerReasonPktHdrInvalid, nullptr, sktBase->getRemoteIpBinary(), "Invalid VxPktHdr" );
			// release the mutex
			sktBase->sktBufAmountRead( 0 );
			sktBase->closeSkt( eSktClosePktHdrInvalid );
			return;
		}

		uint16_t u16PktLen = pktHdr->getPktLength();

		if( u32UsedLen + u16PktLen > sktBase->getRxDecryptedLen() )
		{
			//not all of packet is here
			//LogMsg( LOG_INFO,  "AppRcpCallback.. Skt %d num %d Not all of packet arrived\n", 
			//		sktBase->GetSocketId(),
			//		sktBase->m_iSktId );
			break;
		}

		if( pktHdr->getDestOnlineId() == m_PktAnn.getMyOnlineId() )
		{
			PktHandlerBase::handlePkt( sktBase, pktHdr );
		}
		else
		{
			handleIncommingRelayData( sktBase, pktHdr );
		}

		//we used up this packet
		u32UsedLen += u16PktLen;
		//LogMsg( LOG_INFO, "AppRcpCallback.. skt %d num %d Used Len %d of %d \n", 
		//			sktBase->GetSocketId(),
		//			sktBase->m_iSktId,
		//			u32UsedLen, 
		//			sktBase->m_u32DecryptedLen );

	}			

	sktBase->sktBufAmountRead( u32UsedLen );
}

//============================================================================
void P2PEngine::handleIncommingRelayData( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	bool dataWasRelayed = false;
	bool relayAvailable = false;
	uint16_t pktType = pktHdr->getPktType();
	std::string strSrcId;
	pktHdr->getSrcOnlineId().getVxGUID( strSrcId );
	std::string strDestId;
	pktHdr->getDestOnlineId().getVxGUID( strDestId );
	uint16_t pktLen = pktHdr->getPktLength();
	LogMsg( LOG_ERROR, "handleIncommingRelayData pkt type %d len %d src id %s dest id %s\n", 
									pktType, pktLen, strSrcId.c_str(), strDestId.c_str() );
	if( false == pktHdr->getDestOnlineId().isVxGUIDValid() )
	{
		LogMsg( LOG_ERROR, "handleIncommingRelayData invalid dest id pkt type %d len %d\n", pktHdr->getPktType(), pktHdr->getPktLength() );
		return;
	}

	RCODE rc = 0;
	if( getPluginServiceRelay().isUserRelayOk( pktHdr->getSrcOnlineId(), pktHdr->getDestOnlineId() ) )
	{
		relayAvailable = true;
		m_ConnectionList.connectListLock();
		RcConnectInfo * connectInfo = m_ConnectionList.findConnection( pktHdr->getDestOnlineId() );
		if( 0 != connectInfo )
		{
			rc = connectInfo->getSkt()->txPacket( pktHdr->getDestOnlineId(), pktHdr );
			if( 0 == rc )
			{
				dataWasRelayed = true;
			}
		}

		m_ConnectionList.connectListUnlock();
	}

	if( false == dataWasRelayed )
	{
		LogMsg( LOG_INFO, "Not Relayed data pkt type %d len %d relayAvail %d\n", pktHdr->getPktType(), pktHdr->getPktLength(), relayAvailable );
		PktRelayUserDisconnect		relayReply;
		relayReply.m_UserId			= pktHdr->getDestOnlineId();
		relayReply.setSrcOnlineId( m_PktAnn.getMyOnlineId() );
		sktBase->txPacket( pktHdr->getSrcOnlineId(), &relayReply );	
	}
}

//============================================================================
void P2PEngine::handleMulticastData( VxSktBase * sktBase )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	int	iDataLen		= sktBase->getSktBufDataLen();
	char * pSktBuf		= (char *)sktBase->getSktReadBuf();
	if( false == VxIsEncryptable( iDataLen ) )
	{
		//LogMsg( LOG_INFO, "RcSysSktMgr::HandleUdpData: Skt %d not encryptable len %d\n", sktBase->m_iSktId, iDataLen );
		sktBase->sktBufAmountRead( iDataLen );
		return;
	}

	// decrypt
	VxPktHdr * poPkt = (VxPktHdr *)pSktBuf;
	VxSymDecrypt( &sktBase->m_RxKey, pSktBuf, iDataLen );
	if( poPkt->isValidPkt() )
	{
		//LogMsg( LOG_INFO, "RcSysSktMgr::HandleUdpData: Skt %d valid packet\n", sktBase->m_iSktId );
		// valid pkt
		if( PKT_TYPE_ANNOUNCE == poPkt->getPktType() )
		{
			//LogMsg( LOG_INFO, "RcSysSktMgr::HandleUdpData: Skt %d from %s port %d PktAnnounce\n", sktBase->m_iSktId, sktBase->m_strRmtIp.c_str(), sktBase->m_u16RmtPort );
			//LogMsg( LOG_INFO, "RcSysSktMgr::HandleUdpData: ProcessingSktEvent\n" );
			onPktAnnounce( sktBase, poPkt );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::handleMulticastData: Invalid Packet\n" );
	}

	sktBase->sktBufAmountRead( iDataLen );
}
