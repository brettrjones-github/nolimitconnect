#pragma once
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

#include <config_appcorelibs.h>

#include "OfferBaseXferDb.h"
#include "OfferBaseInfo.h"

#include <GuiInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class OfferBaseRxSession;
class OfferBaseTxSession;
class OfferBaseMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktOfferSendReq;
class PktOfferSendReply;
class PktOfferChunkReq;
class PktOfferSendCompleteReq;
class PktOfferListReply;

class OfferBaseXferMgr
{
public:
	typedef std::map<VxGUID, OfferBaseRxSession *>::iterator OfferBaseRxIter;
	typedef std::vector<OfferBaseTxSession *>::iterator OfferBaseTxIter;

	OfferBaseXferMgr( P2PEngine& engine, OfferBaseMgr& offerMgr, PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr, const char * stateDbName, EOfferMgrType offerMgrType );
	virtual ~OfferBaseXferMgr();

	VxMutex&					getOfferBaseQueMutex( void )					{ return m_OfferBaseSendQueMutex; }
	void						lockOfferBaseQue( void )						{ m_OfferBaseSendQueMutex.lock(); }
	void						unlockOfferBaseQue( void )						{ m_OfferBaseSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiSendOfferBase( OfferBaseInfo& assetInfo );
	virtual void				fromGuiCancelDownload( VxGUID& lclSessionId );
	virtual void				fromGuiCancelUpload( VxGUID& lclSessionId );

	void						fileAboutToBeDeleted( std::string& fileName );
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	virtual void				onPktOfferSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktOfferSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktOfferChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktOfferChunkReply		    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktOfferSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktOfferSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktOfferBaseXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onOfferBaseReceived( OfferBaseRxSession * xferSession, OfferBaseInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onOfferBaseSent( OfferBaseTxSession * xferSession, OfferBaseInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onTxFailed( VxGUID& assetOfferId, bool pluginIsLocked );
	virtual void				onTxSuccess( VxGUID& assetOfferId, bool pluginIsLocked );
	virtual void				updateOfferMgrSendState( VxGUID& assetOfferId, EOfferSendState sendState, int param );

	virtual OfferBaseRxSession *	    findRxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual OfferBaseRxSession *	    findRxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual OfferBaseRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual OfferBaseRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual OfferBaseTxSession *	    findTxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual OfferBaseTxSession *	    findTxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual OfferBaseTxSession *	    createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual OfferBaseTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual OfferBaseTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );

	virtual EXferError			beginOfferBaseReceive( OfferBaseRxSession * xferSession, PktOfferSendReq * poPkt, PktOfferSendReply& pktReply );
	virtual EXferError			beginOfferBaseSend( OfferBaseTxSession * xferSession );

	virtual void				endOfferBaseXferSession( OfferBaseRxSession * xferSession, bool pluginIsLocked );
	virtual void				endOfferBaseXferSession( OfferBaseTxSession * xferSession, bool pluginIsLocked, bool requeOffer );

	virtual EXferError			rxOfferBaseChunk( bool pluginIsLocked, OfferBaseRxSession * xferSession, PktOfferChunkReq * poPkt );
	virtual EXferError			txNextOfferBaseChunk( OfferBaseTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				finishOfferBaseReceive( OfferBaseRxSession * xferSession, PktOfferSendCompleteReq * poPkt, bool pluginIsLocked );

	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreOffersToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						assetSendComplete( OfferBaseTxSession * xferSession );
	void						queOffer( OfferBaseInfo& assetInfo );
	EXferError					createOfferTxSessionAndSend( bool pluginIsLocked, OfferBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
	bool						requireFileXfer( EOfferType assetType );

	//=== vars ===//
	bool						m_Initialized;
	std::map<VxGUID, OfferBaseRxSession *>	m_RxSessions;
	std::vector<OfferBaseTxSession *>		m_TxSessions;
	VxMutex						m_TxSessionsMutex;

    P2PEngine&					m_Engine;	
    OfferBaseMgr&				m_OfferMgr;
    PluginMgr&					m_PluginMgr;
	PluginMessenger&			m_Plugin;	
	PluginSessionMgr&			m_PluginSessionMgr;				
	
	OfferBaseXferDb				m_OfferBaseXferDb;

	VxMutex						m_OfferBaseSendQueMutex;
	std::vector<OfferBaseInfo>	m_OfferBaseSendQue;
	VxThread					m_WorkerThread;

};



