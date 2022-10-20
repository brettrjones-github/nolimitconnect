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
// http://www.nolimitconnect.org
//============================================================================

#include "SessionMgrBase.h"
#include "PluginSessionBase.h"
#include "TxSession.h"
#include "RxSession.h"
#include "P2PSession.h"

#include <map>

class OfferBaseInfo;

class PluginSessionMgr : public SessionMgrBase
{
public:
	PluginSessionMgr( P2PEngine& engine, PluginBase& plugin, PluginMgr& pluginMgr );
	virtual ~PluginSessionMgr();

	std::map<VxGUID, PluginSessionBase*>&	getSessions( void )			{ return m_aoSessions; }
	size_t						getSessionCount( void )					{ return m_aoSessions.size(); }

	virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt );
	virtual void				onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual	void				onConnectionLost( VxSktBase* sktBase );
	virtual void				cancelSessionByOnlineId( VxGUID& onlineId );

    virtual bool				fromGuiIsPluginInSession( bool pluginIsLocked, VxNetIdent* netIdent, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual void				fromGuiStopPluginSession( bool pluginIsLocked, VxNetIdent* netIdent, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual bool				fromGuiMakePluginOffer(	bool pluginIsLocked, VxNetIdent* netIdent, OfferBaseInfo& offerInfo, VxGUID& lclSessionId );
	virtual bool				fromGuiOfferReply( bool pluginIsLocked, VxNetIdent* netIdent, OfferBaseInfo& offerInfo, VxGUID& lclSessionId, EOfferResponse offerResponse );

	virtual void				onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPluginOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktSessionStopReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	void						addSession( VxGUID& sessionId, PluginSessionBase* session, bool pluginIsLocked );

	PluginSessionBase*			findPluginSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked );
	PluginSessionBase*			findPluginSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked );

	P2PSession *				findP2PSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked );
	P2PSession *				findP2PSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked );
	P2PSession *				findOrCreateP2PSessionWithSessionId( VxGUID& sessionId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked);
	P2PSession *				findOrCreateP2PSessionWithOnlineId( VxGUID& onlineId, VxSktBase* sktBase, VxNetIdent* netIdent, 
																		bool pluginIsLocked, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	TxSession *					findTxSessionBySessionId( bool pluginIsLocked, VxGUID& sessionId );
	TxSession *					findTxSessionByOnlineId( bool pluginIsLocked, VxGUID& onlineId );
	TxSession *					findOrCreateTxSessionWithSessionId( VxGUID& sessionId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked );
	TxSession *					findOrCreateTxSessionWithOnlineId( VxGUID& onlineId, VxSktBase* sktBase, VxNetIdent* netIdent, 
																		bool pluginIsLocked, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	int							getTxSessionCount( bool pluginIsLocked = false );

	RxSession *					findRxSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked );
	RxSession *					findRxSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked );
	RxSession *					findOrCreateRxSessionWithSessionId( VxGUID& sessionId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked );
	RxSession *					findOrCreateRxSessionWithOnlineId( VxGUID& onlineId, VxSktBase* sktBase, VxNetIdent* netIdent, 
																	bool pluginIsLocked, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	void						endPluginSession( PluginSessionBase* session, bool pluginIsLocked );
	void						endPluginSession( VxGUID& sessionId, bool pluginIsLocked );

	void						removeTxSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked );
	void						removeTxSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked );
	void						removeRxSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked );
	void						removeRxSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked );

	// returns true if found and removed session
	bool						removeSessionBySessionId( bool pluginIsLocked, VxGUID& sessionId, EOfferResponse offerResponse = eOfferResponseEndSession );
	bool						removeSession( bool pluginIsLocked, VxNetIdent* netIdent, VxGUID& sessionId, EOfferResponse offerResponse, bool fromGui = false );
	void						removeAllSessions( bool testSessionsOnly = false );

	typedef std::map<VxGUID, PluginSessionBase*>::iterator SessionIter;

protected:
	void						doEndAndEraseSession( PluginSessionBase* sessionBase, EOfferResponse offerResponse, bool pluginIsLocked );

	//=== vars ===//
	std::map<VxGUID, PluginSessionBase*>	m_aoSessions;

private:
	PluginSessionMgr(); // don't allow default constructor
	PluginSessionMgr(const PluginSessionMgr&); // don't allow copy constructor
};

