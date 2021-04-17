#pragma once
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

#include <config_gotvcore.h>

#include "BlobXferDb.h"
#include "BlobInfo.h"

#include <GuiInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class BlobRxSession;
class BlobTxSession;
class BlobMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktBlobSendReq;
class PktBlobSendReply;
class PktBlobChunkReq;
class PktBlobSendCompleteReq;
class PktBlobListReply;

class BlobXferMgr
{
public:
	typedef std::map<VxGUID, BlobRxSession *>::iterator BlobRxIter;
	typedef std::vector<BlobTxSession *>::iterator BlobTxIter;

	BlobXferMgr( P2PEngine& engine, PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr, const char * stateDbName );
	virtual ~BlobXferMgr();

	VxMutex&					getBlobQueMutex( void )					{ return m_BlobSendQueMutex; }
	void						lockBlobQue( void )						{ m_BlobSendQueMutex.lock(); }
	void						unlockBlobQue( void )						{ m_BlobSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiSendBlob( BlobInfo& assetInfo );
	virtual void				fromGuiCancelDownload( VxGUID& lclSessionId );
	virtual void				fromGuiCancelUpload( VxGUID& lclSessionId );

	void						fileAboutToBeDeleted( std::string& fileName );
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	virtual void				onPktBlobSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktBlobSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktBlobChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktBlobChunkReply		    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktBlobSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktBlobSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktBlobXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onBlobReceived( BlobRxSession * xferSession, BlobInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onBlobSent( BlobTxSession * xferSession, BlobInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				updateBlobMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param );

	virtual BlobRxSession *	    findRxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual BlobRxSession *	    findRxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual BlobRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual BlobRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual BlobTxSession *	    findTxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual BlobTxSession *	    findTxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual BlobTxSession *	    createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual BlobTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual BlobTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );

	virtual EXferError			beginBlobReceive( BlobRxSession * xferSession, PktBlobSendReq * poPkt, PktBlobSendReply& pktReply );
	virtual EXferError			beginBlobSend( BlobTxSession * xferSession );

	virtual void				endBlobXferSession( BlobRxSession * xferSession, bool pluginIsLocked );
	virtual void				endBlobXferSession( BlobTxSession * xferSession, bool pluginIsLocked, bool requeBlob );

	virtual EXferError			rxBlobChunk( bool pluginIsLocked, BlobRxSession * xferSession, PktBlobChunkReq * poPkt );
	virtual EXferError			txNextBlobChunk( BlobTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				finishBlobReceive( BlobRxSession * xferSession, PktBlobSendCompleteReq * poPkt, bool pluginIsLocked );

	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreBlobsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						assetSendComplete( BlobTxSession * xferSession );
	void						queBlob( BlobInfo& assetInfo );
	EXferError					createBlobTxSessionAndSend( bool pluginIsLocked, BlobInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
	bool						requireFileXfer( EAssetType assetType );

	//=== vars ===//
	bool						m_Initialized;
	std::map<VxGUID, BlobRxSession *>	m_RxSessions;
	std::vector<BlobTxSession *>		m_TxSessions;
	VxMutex						m_TxSessionsMutex;

    P2PEngine&					m_Engine;
	PluginMessenger&			m_Plugin;	
	PluginSessionMgr&			m_PluginSessionMgr;
	PluginMgr&					m_PluginMgr;
					
	BlobMgr&				    m_BlobMgr;
	BlobXferDb				    m_BlobXferDb;

	VxMutex						m_BlobSendQueMutex;
	std::vector<BlobInfo>	    m_BlobSendQue;
	VxThread					m_WorkerThread;
};



