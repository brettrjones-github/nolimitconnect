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

#include "PluginBaseWebServer.h"
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>


#include <GoTvCore/GoTvP2P/Search/RcScan.h>

#include <PktLib/PktsWebServer.h>
#include <CoreLib/VxFileUtil.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>

#define MAX_OUTSTANDING_PIC_CHUNK_PACKETS 3

//============================================================================
void PluginBaseWebServer::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	WebTxIter iter;
	AutoPluginLock( this );
	for( iter = m_WebTxSessions.begin(); iter != m_WebTxSessions.end(); ++iter )
	{
		WebTxSession * poSession = iter->second;
		if( poSession->getSkt() == poOldSkt )
		{
			poSession->setSkt( poNewSkt );
		}
	}
}

//============================================================================
void PluginBaseWebServer::onContactWentOffline(  VxNetIdent * netIdent, VxSktBase * sktBase )
{
	AutoPluginLock( this );
	bool bErased = true;
	bool sktIsDisconnected = !sktBase->isConnected();
	while( bErased ) 
	{
		bErased = false;
		WebTxIter iter = m_WebTxSessions.begin();
		while( iter != m_WebTxSessions.end() )
		{
			WebTxSession * poSession = iter->second;
			if( ( sktIsDisconnected && ( poSession->getSkt() == sktBase ) ) 
				|| ( poSession->getIdent()->getMyOnlineId() == netIdent->getMyOnlineId() ) )
			{
				delete poSession;
				m_WebTxSessions.erase( iter );
				bErased = true;
				break;
			}
			else
			{
				++iter;
			}
		}
	}
}

//============================================================================
void PluginBaseWebServer::onConnectionLost( VxSktBase * sktBase )
{
	AutoPluginLock( this );
	bool bErased = true;
	while( bErased ) 
	{
		bErased = false;
		WebTxIter iter = m_WebTxSessions.begin();
		while( iter != m_WebTxSessions.end() )
		{
			WebTxSession * poSession = iter->second;
			if( poSession->getSkt() == sktBase )
			{
				delete poSession;
				m_WebTxSessions.erase( iter );
				bErased = true;
				break;
			}
			else
			{
				++iter;
			}
		}
	}
}

//============================================================================
RCODE PluginBaseWebServer::handleDataFromLocalBrowser( RcWebSkt * sktBase )
{
	RCODE rc = 0;

	int iDataLen = sktBase->getSktBufDataLen();
	if( iDataLen < 16 )
	{
		// not enough to work with
		return 0;
	}

	int iOrigDataLen = iDataLen;
	LogMsg( LOG_INFO, "handleDataFromLocalBrowser total data Len %d\n", iDataLen );
	char *			pSktBuf				= (char *)sktBase->getSktReadBuf();
	VxNetIdent *	netIdent			= sktBase->getIdentity();
	if( netIdent == m_WebPlugin->m_MyIdent )
	{
		char *			curParseBuf			= pSktBuf;
		char *			endParseBuf			= 0;
		int				iTotalDataUsed		= 0;
		int				iFileOffset;
		EWbMyFiles		eFileType;
		EHttpReqType	eHttpReqType; 
		std::string		strPwd;

		while( 0 != ( endParseBuf = strstr( curParseBuf, "\r\n\r\n" ) ) )
		{
			endParseBuf[2] = 0;
			std::string		strDebug = curParseBuf;
			endParseBuf[2] = '\r';
			LogMsg( LOG_INFO, "handleDataFromLocalBrowser %s", strDebug.c_str() );

			int				iRetDataUsed		= 0;
			int				iHttpError			= 0;
			bool			bCloseConnection	= false;
			std::string		strFileName			= "";
			uint64_t				fileLen			= 0;
			parseWebRxData(	curParseBuf, 
							iDataLen, 
							iRetDataUsed, 
							iHttpError, 
							strFileName,
							fileLen,
							iFileOffset,
							bCloseConnection,
							&eFileType,
							&eHttpReqType,
							&strPwd );

			if( iHttpError )
			{
				sendHttpErr( true, netIdent, sktBase, iHttpError );
			}
			else if( 0 != strFileName.length() )
			{
				m_WebPlugin->queWebsiteFile( sktBase, strFileName.c_str(), fileLen, iFileOffset, bCloseConnection, eFileType, eHttpReqType, strPwd.c_str() );
			}

			if( 0 != iRetDataUsed )
			{
				iDataLen		-=	iRetDataUsed;
				iTotalDataUsed	+=	iRetDataUsed;
				if( 0 >= iDataLen )
				{
					break;
				}
				else
				{
					curParseBuf		=	endParseBuf + 4;
				}
			}
			else
			{
				break;
			}
		}

		if( iTotalDataUsed != iOrigDataLen )
		{
			LogMsg( LOG_INFO, "Failed to use all data orig %d used %d\n", iOrigDataLen, iTotalDataUsed );
		}

		sktBase->sktBufAmountRead( iTotalDataUsed );
	}
	else
	{
		// assume relay of data ?
		rc = sendWebServerData(		true, 
									netIdent,
									sktBase,				// web socket
									pSktBuf,				// data to send
									iDataLen );				// length of data	

		sktBase->sktBufAmountRead( iDataLen );
	}

	return rc;
}

//============================================================================
//! send standard http error to browser
RCODE PluginBaseWebServer::sendHttpErr(		bool				bIsGetData,				// if true use PktWebServerGetChunkTx else use PktWebServerPutChunkTx
										VxNetIdent *		netIdent, 
										VxSktBase *			sktBase, 
										int					iErrIdx )
{
	//send error
	RCODE rc = iErrIdx;
	static char szMsg[512];
	static const char *szStatMsgs [] = {
		"200 OK",
		"201 Created",
		"202 Accepted",
		"204 No Content",
		"301 Moved Permanently",
		"302 Moved Temporarily",
		"304 Not Modified",
		"400 Bad Request",
		"401 Unauthorized",
		"403 Forbidden",
		"404 Not Found",
		"500 Internal Server Error",
		"501 Not Implemented",
		"502 Bad Gateway",
		"503 Service Unavailable"
	};

	#define NUMSTATMSGS (int)(sizeof(szStatMsgs) / sizeof(szStatMsgs[0]))
	if( rc < 0 || rc > NUMSTATMSGS)
	{
		vx_assert( false );
		rc = HTTP_STATUS_SERVERERROR;
	}

	sprintf( szMsg, "<body><h1>%s</h1></body>", szStatMsgs[rc] );
	return sendWebServerData( bIsGetData, netIdent, sktBase, szMsg, (int)strlen( szMsg ), true );
}

//============================================================================
RCODE PluginBaseWebServer::sendWebServerData(	bool					bIsGetData,				// if true use PktWebServerGetChunkTx else use PktWebServerPutChunkTx
											VxNetIdent *			netIdent, 
											VxSktBase *				poSktIn,				// web socket
											const char *			pData,					// data to send
											int						iDataLen,				// length of data	
											bool					bDisconnectAfterSend,	// if true disconnect after data is sent
											int						iSendTimeoutSeconds )	// timeout attempt to send
{
	RCODE rc = 0;
	if( poSktIn->isWebSkt() )
	{
		RcWebSkt * sktBase = (RcWebSkt *)poSktIn;
		netIdent =  sktBase->getIdentity();
		if( ( 0 == netIdent ) || 
			(m_MyIdent == netIdent) )
		{
			// viewing our own profile.. send data directly to web browser
			rc = sktBase->sendData( pData, iDataLen, bDisconnectAfterSend );
		}
		else if( 0 != netIdent )
		{
			// viewing remote user profile.. package browser data into pkt and send to user
			VxSktBase * poRmtSkt = (VxSktBase *)sktBase->getUserExtraData();
			if( poRmtSkt && poRmtSkt->isConnected() )
			{
				if( bIsGetData )
				{
					uint16_t u16PktLen = ROUND_TO_16BYTE_BOUNDRY( sizeof( PktWebServerGetChunkTx ) + iDataLen );
					PktWebServerGetChunkTx * poPkt = (PktWebServerGetChunkTx *) new char[ u16PktLen ];
					poPkt->init();
					poPkt->setPktLength( u16PktLen );
					poPkt->setThisDataLen( iDataLen );
					memcpy( poPkt->getDataPayload(), pData, iDataLen );
					rc = ( txPacket( netIdent, poRmtSkt, poPkt ) ) ? 0 : -1;
					delete poPkt;
				}
				else
				{
					uint16_t u16PktLen = ROUND_TO_16BYTE_BOUNDRY( sizeof( PktWebServerPutChunkTx ) + iDataLen );
					PktWebServerPutChunkTx * poPkt = (PktWebServerPutChunkTx *) new char[ u16PktLen ];
					poPkt->init();
					poPkt->setPktLength( u16PktLen );
					poPkt->setThisDataLen( iDataLen );
					memcpy( poPkt->getDataPayload(), pData, iDataLen );
					rc = ( txPacket( netIdent, poRmtSkt, poPkt ) ) ? 0 : -1;
					delete poPkt;
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "sendWebServerData ERROR skt %d\n", poRmtSkt );
			}
		}
	}
	else
	{
		VxSktBase * poRmtSkt = poSktIn;
		if( poRmtSkt && poRmtSkt->isConnected() )
		{
			if( bIsGetData )
			{
				uint16_t u16PktLen = ROUND_TO_16BYTE_BOUNDRY( sizeof( PktWebServerGetChunkTx ) + iDataLen );
				PktWebServerGetChunkTx * poPkt = (PktWebServerGetChunkTx *) new char[ u16PktLen ];
				poPkt->init();
				poPkt->setPktLength( u16PktLen );
				poPkt->setThisDataLen( iDataLen );
				memcpy( poPkt->getDataPayload(), pData, iDataLen );
				rc = ( txPacket( netIdent, poRmtSkt, poPkt ) ) ? 0 : -1;
				delete poPkt;
			}
			else
			{
				uint16_t u16PktLen = ROUND_TO_16BYTE_BOUNDRY( sizeof( PktWebServerPutChunkTx ) + iDataLen );
				PktWebServerPutChunkTx * poPkt = (PktWebServerPutChunkTx *) new char[ u16PktLen ];
				poPkt->init();
				poPkt->setPktLength( u16PktLen );
				poPkt->setThisDataLen( iDataLen );
				memcpy( poPkt->getDataPayload(), pData, iDataLen );
				rc = ( txPacket( netIdent, poRmtSkt, poPkt ) ) ? 0 : -1;
				delete poPkt;
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "sendWebServerData ERROR skt %d\n", poRmtSkt );
		}
	}

	return rc;
}

//============================================================================
//! que up a file to be sent
RCODE PluginBaseWebServer::queWebsiteFile(		RcWebSkt *		poWebSkt,			//
											const char *	pFileName,			// file name or password
											uint64_t				fileLen,			// file length
											int				iFileOffset,		// file offset to begin sending at
											bool			bCloseConnection,	//
											EWbMyFiles		eFileType,			// file type if alias for shared file
											EHttpReqType	eHttpReqType,		// http request type
											const char *	pPwd )				// password
{
	if( NULL == poWebSkt->getIdentity() )
	{
		poWebSkt->queWebsiteFile( pFileName, fileLen, eFileType, eHttpReqType, pPwd );
	}
	else
	{
		doSendFile( poWebSkt->getIdentity(), poWebSkt, pFileName, fileLen, iFileOffset, bCloseConnection );
	}

	return 0;
}

//============================================================================
void PluginBaseWebServer::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{

}

//============================================================================
void PluginBaseWebServer::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void PluginBaseWebServer::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBaseWebServer::onPktSessionStartReq" );
}
//============================================================================
void PluginBaseWebServer::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBaseWebServer::onPktSessionStartReply" );
}
//============================================================================
void PluginBaseWebServer::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBaseWebServer::onPktSessionStopReq" );
}
//============================================================================
void PluginBaseWebServer::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginBaseWebServer::onPktSessionStopReply" );
}

//============================================================================
void PluginBaseWebServer::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	WebTxSession * txSession = 0;

	PktMyPicSendReply oPkt;
	oPkt.setError( isAccessAllowed( netIdent ) ? 0 : PIC_ERR_ACCESS_DENIED );
	if( 0 == oPkt.getError() )
	{
		std::string strMyPicFileName;
		getMyPicFileName( strMyPicFileName );

		uint32_t u32MyPicLen = (uint32_t)VxFileUtil::fileExists( strMyPicFileName.c_str() );
		if( 0 == u32MyPicLen )
		{
			oPkt.setError( PIC_ERR_FILE_NOT_FOUND );
		}
		else
		{
			oPkt.setTotalDataLen( u32MyPicLen );
			oPkt.setError( 0 );
			FILE * hMyPicFile = fopen( strMyPicFileName.c_str(), "rb" );
			if( 0 != hMyPicFile )
			{
				if( MAX_WEB_SERVER_CHUNK_LEN > u32MyPicLen )
				{
					// we can do the complete picture in single pkt so do all now
					PktWebServerPicChunkTx * poPkt = (PktWebServerPicChunkTx *)new char[ u32MyPicLen + sizeof( PktWebServerPicChunkTx ) + 16 ];
					poPkt->init();
					poPkt->setThisDataLen( u32MyPicLen );
					poPkt->setTotalDataLen( u32MyPicLen );
					uint32_t u32ReadLen = ( uint32_t )(fread( poPkt->getPayloadBuffer(), 1, u32MyPicLen, hMyPicFile ));
					if( u32ReadLen == u32MyPicLen )
					{
						txPacket( netIdent, sktBase, &oPkt );
						poPkt->calcPktLen();
						txPacket( netIdent, sktBase, poPkt );
						delete poPkt;
						return;
					}
					else
					{
						oPkt.setError( PIC_ERR_FILE_READ_ERROR );
					}
				}
				else
				{
					// create session to send picture packets
					txSession = findOrCreateTxSession( netIdent, sktBase );
					if( 0 != txSession->m_hMyPicFile )
					{
						fclose( txSession->m_hMyPicFile );
					}
					
					txSession->m_hMyPicFile		= hMyPicFile;
					txSession->m_u32MyPicLen	= u32MyPicLen;
					txSession->m_u32MyPicOffset = 0;
				}
			}
			else
			{
				oPkt.setError( PIC_ERR_FILE_OPEN_ERROR );
			}
		}
	}

	txPacket( netIdent, sktBase, &oPkt );
	if( ( 0 == oPkt.getError() ) && ( 0 != txSession ) )
	{
		sendNextPicChunk( sktBase, netIdent, txSession );
	}
}

//============================================================================
void PluginBaseWebServer::sendNextPicChunk( VxSktBase * sktBase, VxNetIdent * netIdent, WebTxSession * poSession )
{
	if( poSession && poSession->m_hMyPicFile )
	{
		uint32_t u32LenLeftToSend = poSession->m_u32MyPicLen - poSession->m_u32MyPicOffset;
		if( u32LenLeftToSend )
		{
			uint32_t u32LenThisSend = (u32LenLeftToSend <= MAX_WEB_SERVER_CHUNK_LEN) ? u32LenLeftToSend : MAX_WEB_SERVER_CHUNK_LEN;
			PktWebServerPicChunkTx * poPkt = (PktWebServerPicChunkTx *)new char[ u32LenThisSend + sizeof( PktWebServerPicChunkTx ) + 16 ];
			poPkt->init();
			poPkt->setThisDataLen( u32LenThisSend );
			poPkt->setTotalDataLen( poSession->m_u32MyPicLen );
			uint32_t u32ReadLen = ( uint32_t )( fread( poPkt->getPayloadBuffer(), 1, u32LenThisSend, poSession->m_hMyPicFile ));
			if( u32ReadLen == u32LenThisSend )
			{
				poPkt->calcPktLen();
				txPacket( netIdent, sktBase, poPkt );
				poSession->m_u32MyPicOffset += u32LenThisSend;
				delete poPkt;

				poSession->m_iOutstandingAckCnt++;
				if( 0 == (poSession->m_u32MyPicLen - poSession->m_u32MyPicOffset) )
				{
					fclose(poSession->m_hMyPicFile);
					poSession->m_hMyPicFile = NULL;
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "sendNextPicChunk attempted read %d only read %d\n", u32LenThisSend, u32ReadLen );
			}
		}
	}
}

//============================================================================
void PluginBaseWebServer::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktMyPicSendReply * poPkt = (PktMyPicSendReply *)pktHdr;
	if( 0 == poPkt->getError() )
	{
		WebRxSession * poSession = findOrCreateRxSession( netIdent, sktBase );
		delete poSession->m_paPicData;

		poSession->m_u32PicTotalDataLen		= (uint32_t)poPkt->getTotalDataLen();
		poSession->m_paPicData				= new uint8_t[ poPkt->getTotalDataLen() ];
		poSession->m_u32PicDataIdx			= 0;

	}
	else
	{
		m_Engine.getRcScan().onScanResultError( eScanTypeProfilePic, netIdent, sktBase, poPkt->getError() );
	}
}

//============================================================================
void PluginBaseWebServer::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{	
	PktWebServerPicChunkTx * poPktPicChunk = ( PktWebServerPicChunkTx * )pktHdr;
	if( poPktPicChunk->getThisDataLen() == poPktPicChunk->getTotalDataLen() )
	{
		// all in one packet.. no need to do sessions
		m_Engine.getRcScan().onScanResultProfilePic( netIdent, sktBase, poPktPicChunk->getPayloadBuffer(), poPktPicChunk->getThisDataLen() ); 
	}
	else
	{
		WebRxIter iter = m_WebRxSessions.find(netIdent);
		if( iter != m_WebRxSessions.end() )
		{
			WebRxSession * poSession = iter->second;

			if( poSession->m_paPicData && 
				(poSession->m_u32PicTotalDataLen >= (poSession->m_u32PicDataIdx + poPktPicChunk->getThisDataLen() ) ) )
			{
				memcpy( &poSession->m_paPicData[ poSession->m_u32PicDataIdx ], 
								poPktPicChunk->getPayloadBuffer(), 
								poPktPicChunk->getThisDataLen() );

				poSession->m_u32PicDataIdx += poPktPicChunk->getThisDataLen();

				PktWebServerPicChunkAck oPkt;
				oPkt.setTotalDataLen( poPktPicChunk->getTotalDataLen() );
				oPkt.setThisDataLen( poPktPicChunk->getThisDataLen() );
				oPkt.setFileOffset( poPktPicChunk->getFileOffset() );

				if( false == txPacket(netIdent, sktBase, &oPkt))
				{
					LogMsg( LOG_ERROR, "PluginBaseWebServer::onPktWebServerPicChunkTx\n");
					m_Engine.getRcScan().onScanResultError( eScanTypeProfilePic, netIdent, sktBase, 5 );
				}

				if( poSession->m_u32PicDataIdx >= poSession->m_u32PicTotalDataLen )
				{
					// all of picture arrived
					m_Engine.getRcScan().onScanResultProfilePic( netIdent, sktBase, poSession->m_paPicData, poSession->m_u32PicDataIdx ); 
				}
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginBaseWebServer::onPktWebServerPicChunkTx Error\n");
			m_Engine.getRcScan().onScanResultError( eScanTypeProfilePic, netIdent, sktBase, 6 );
		}
	}
}

//============================================================================
void PluginBaseWebServer::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	WebTxIter iter = m_WebTxSessions.find(netIdent);
	if( iter != m_WebTxSessions.end() )
	{
		WebTxSession * poSession = iter->second;
		if( poSession->m_iOutstandingAckCnt )
		{
			poSession->m_iOutstandingAckCnt--;
		}

		if( poSession->m_iOutstandingAckCnt < MAX_OUTSTANDING_PIC_CHUNK_PACKETS )
		{
			sendNextPicChunk( sktBase, netIdent, poSession );
		}
	}
}

//============================================================================
void PluginBaseWebServer::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{	
	//PktWebServerGetChunkTx * poPktGetChunk = ( PktWebServerGetChunkTx * )pktHdr;

	//int iRetDataUsed = 0;
	//int iHttpError = 0;
	//int iFileOffset = 0;
	//std::string strFileName;
 //   //EWbMyFiles eFileType;
 //   //EHttpReqType eHttpReqType;
 //   //std::string strPwd;
	//bool bCloseConnection = false;

	//parseWebRxData(	(char *)poPktGetChunk->m_au8Data, 
	//				poPktGetChunk->m_u32ThisDataLen, 
	//				iRetDataUsed, 
	//				iHttpError, 
	//				strFileName, 
	//				iFileOffset,
	//				bCloseConnection );
	//if( iHttpError )
	//{
	//	sendHttpErr( false, netIdent, sktBase, iHttpError );
	//}
	//else
	//{
	//	doSendFile( netIdent, sktBase, strFileName.c_str(), iFileOffset, bCloseConnection );
	//}

}

//============================================================================
void PluginBaseWebServer::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	WebTxIter iter = m_WebTxSessions.find(netIdent);
	if( iter != m_WebTxSessions.end() )
	{
		WebTxSession * poSession = iter->second;
		if( poSession->m_iOutstandingAckCnt )
		{
			poSession->m_iOutstandingAckCnt--;
		}
	}
}

//============================================================================
void PluginBaseWebServer::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{	
	//WebProxySession * poProxySession = findWebProxySession( netIdent );
	//if( poProxySession )
	//{
	//	if( poProxySession->m_WebSkt->isConnected() )
	//	{
	//		 PktWebServerPutChunkTx * poPkt = (PktWebServerPutChunkTx *)pktHdr;
	//		 poProxySession->m_WebSkt->sendData( (const char *)poPkt->m_au8Data, poPkt->m_u32ThisDataLen );
	//	}
	//}
}

//============================================================================
void PluginBaseWebServer::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	WebTxIter iter = m_WebTxSessions.find(netIdent);
	if( iter != m_WebTxSessions.end() )
	{
		WebTxSession * poSession = iter->second;
		if( poSession->m_iOutstandingAckCnt )
		{
			poSession->m_iOutstandingAckCnt--;
		}
	}
}
