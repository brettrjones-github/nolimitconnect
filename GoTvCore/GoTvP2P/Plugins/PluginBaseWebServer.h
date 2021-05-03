#pragma once
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

#include "PluginBase.h"

#include <GoTvCore/GoTvP2P/WebServer/RcWebServer.h>

#include <map>

class PktVideoFeedPic;
class VxSktBase;

class WebRxSession
{
public:
	WebRxSession( VxNetIdent * netIdent, VxSktBase *	sktBase )
        : m_u32PicTotalDataLen(0)
		, m_u32PicDataIdx(0)
		, m_paPicData(0)
        , m_Ident(netIdent)
        , m_Skt(sktBase)
    {
	}

	void						setIdent( VxNetIdent * ident )		{ m_Ident = ident; }
	VxNetIdent *				getIdent( void )					{ return m_Ident; }

	void						setSkt( VxSktBase * skt )			{ m_Skt = skt; }
	VxSktBase *					getSkt( void )						{ return m_Skt; }

	uint32_t					m_u32PicTotalDataLen;
	uint32_t					m_u32PicDataIdx;
	uint8_t *					m_paPicData;
protected:
	VxNetIdent *				m_Ident;
	VxSktBase *					m_Skt;
};

class WebTxSession
{
public:
	WebTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
        : m_iOutstandingAckCnt(0)
		, m_hMyPicFile(NULL)
		, m_u32MyPicLen(0)
		, m_u32MyPicOffset(0)
        , m_Ident(netIdent)
        , m_Skt(sktBase)
    {
	}

	void						setIdent( VxNetIdent * ident )		{ m_Ident = ident; }
	VxNetIdent *				getIdent( void )					{ return m_Ident; }

	void						setSkt( VxSktBase * skt )			{ m_Skt = skt; }
	VxSktBase *					getSkt( void )						{ return m_Skt; }


	//=== vars ===//
	int							m_iOutstandingAckCnt; // how many receive acks are outstanding
	FILE *						m_hMyPicFile;
	uint32_t					m_u32MyPicLen;
	uint32_t					m_u32MyPicOffset;
protected:
	VxNetIdent *				m_Ident;
	VxSktBase *					m_Skt;
};

class WebProxySession
{
public:
	WebProxySession( VxNetIdent * netIdent, VxSktBase * sktBase, RcWebSkt * poWebSkt )
        : m_WebSkt(poWebSkt)
		, m_iOutstandingAckCnt(0)
        , m_Ident(netIdent)
        , m_Skt(sktBase)
    {
	}

	void						setIdent( VxNetIdent * ident )		{ m_Ident = ident; }
	VxNetIdent *				getIdent( void )					{ return m_Ident; }

	void						setSkt( VxSktBase * skt )			{ m_Skt = skt; }
	VxSktBase *					getSkt( void )						{ return m_Skt; }

	//=== vars ===//
	RcWebSkt *					m_WebSkt;
	int							m_iOutstandingAckCnt; // how many receive acks are outstanding
protected:
	VxNetIdent *				m_Ident;
	VxSktBase *					m_Skt;
};

//! implements WebServer plugin
class PluginBaseWebServer : public PluginBase, public RcWebServer
{
public:
	PluginBaseWebServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginBaseWebServer();

	virtual void				pluginShutdown( void );

	virtual void				fromGuiUserLoggedOn( void );


	VxSktBase *					makeNewSkt( void ) { return NULL; }
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase );
	void						onConnectionLost( VxSktBase * sktBase );

	void						onPktPluginOfferReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktPluginOfferReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						onPktSessionStartReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktSessionStartReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktSessionStopReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktSessionStopReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						onPktMyPicSendReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktMyPicSendReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktWebServerPicChunkTx		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktWebServerPicChunkAck		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktWebServerGetChunkTx		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktWebServerGetChunkAck		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktWebServerPutChunkTx		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	void						onPktWebServerPutChunkAck		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );


	RCODE						handleDataFromLocalBrowser( RcWebSkt * sktBase );

	RCODE						sendHttpErr(	bool					bIsGetData,				// if true use PktWebServerGetChunkTx else use PktWebServerPutChunkTx
												VxNetIdent *			netIdent, 
												VxSktBase *				sktBase, 
												int						iErrIdx );

	RCODE						sendWebServerData(	bool			bIsGetData,				// if true use PktWebServerGetChunkTx else use PktWebServerPutChunkTx
													VxNetIdent *	netIdent,
													VxSktBase *		sktBase,							// web socket
													const char *	pData,							// data to send
													int				iDataLen,						// length of data	
													bool			bDisconnectAfterSend = false,	// if true disconnect after data is sent
													int				iSendTimeoutSeconds = 3 );		// timeout attempt to send

	//! que up a file to be sent
	RCODE						queWebsiteFile( RcWebSkt *		sktBase,			// skt
												const char *	pFileName,			// file name or password
												uint64_t				fileLen,			// file length
												int				iFileOffset,		// file offset to begin sending at
												bool			bCloseConnection,
												EWbMyFiles		eFileType,			// file type if alias for shared file
												EHttpReqType	eHttpReqType,		// http request type
												const char *	pPwd );				// password

	RCODE						doSendFile( VxNetIdent *	netIdent,
											VxSktBase *		sktBase,		
											const char *	pFileName,
											uint64_t				fileLen,			// file length
											int				iFileOffset,
											bool			bCloseAfterSend );

	RCODE						handleHttpConnection( VxSktBase * sktBase, VxNetIdent * netIdent );
	bool						searchActionProfilePic( VxNetIdent * netIdent, VxSktBase * sktBase );

protected:
	WebRxSession *				findRxSession( VxNetIdent * netIdent );
	WebRxSession *				findOrCreateRxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	WebTxSession *				findTxSession( VxNetIdent * netIdent );
	WebTxSession *				findOrCreateTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );

	WebProxySession *			findWebProxySession( VxNetIdent * netIdent );

	void						getMyPicFileName( std::string& strRetFileName );
	void						sendNextPicChunk( VxSktBase * sktBase, VxNetIdent * netIdent, WebTxSession * poSession );

	//=== constants ===//
	typedef std::map<VxNetIdent *, WebRxSession *>::iterator WebRxIter;
	typedef std::map<VxNetIdent *, WebTxSession *>::iterator WebTxIter;
	typedef std::map<VxNetIdent *, WebProxySession *>::iterator WebProxyIter;
	typedef long (PluginBaseWebServer::*WEB_SERVER_PKT_FUNCTION)( VxSktBase *, VxPktHdr *, VxNetIdent * );  

	//=== vars ===//
	std::map<VxNetIdent *, WebRxSession *>		m_WebRxSessions;
	std::map<VxNetIdent *, WebTxSession *>		m_WebTxSessions;

	std::map<VxNetIdent *, WebProxySession *>	m_WebProxySessions;

	int							m_iMaxWebServerPktType;
	WEB_SERVER_PKT_FUNCTION *	m_paWebServerPktFuncTable;

	uint64_t					m_u64FileLen;
	uint64_t					m_u64LenSent;
	FILE *						m_File;
};



