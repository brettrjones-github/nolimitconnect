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

#include "HostListXferDb.h"
#include "HostListInfo.h"

#include <GoTvInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class HostListRxSession;
class HostListTxSession;
class HostListMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktHostListSendReq;
class PktHostListSendReply;
class PktHostListChunkReq;
class PktHostListSendCompleteReq;
class PktHostListListReply;

class HostListXferMgr
{
public:
	typedef std::map<VxGUID, HostListRxSession *>::iterator HostListRxIter;
	typedef std::vector<HostListTxSession *>::iterator HostListTxIter;

	HostListXferMgr( PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr );
	virtual ~HostListXferMgr();

	VxMutex&					getHostListQueMutex( void )					{ return m_HostListSendQueMutex; }
	void						lockHostListQue( void )						{ m_HostListSendQueMutex.lock(); }
	void						unlockHostListQue( void )						{ m_HostListSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiSendHostList( HostListInfo& assetInfo );
	virtual void				fromGuiCancelDownload( VxGUID& lclSessionId );
	virtual void				fromGuiCancelUpload( VxGUID& lclSessionId );

	void						fileAboutToBeDeleted( std::string& fileName );
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	virtual void				onPktHostListSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktHostListSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktHostListChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktHostListChunkReply		    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktHostListSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktHostListSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktHostListXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onHostListReceived( HostListRxSession * xferSession, HostListInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onHostListSent( HostListTxSession * xferSession, HostListInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				updateHostListMgrSendState( VxGUID& assetUniqueId, EHostListSendState sendState, int param );

	virtual HostListRxSession *	findRxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual HostListRxSession *	findRxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual HostListRxSession *	findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual HostListRxSession *	findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual HostListTxSession *	findTxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual HostListTxSession *	findTxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual HostListTxSession *	createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual HostListTxSession *	findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual HostListTxSession *	findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );

	virtual EXferError			beginHostListReceive( HostListRxSession * xferSession, PktHostListSendReq * poPkt, PktHostListSendReply& pktReply );
	virtual EXferError			beginHostListSend( HostListTxSession * xferSession );

	virtual void				endHostListXferSession( HostListRxSession * xferSession, bool pluginIsLocked );
	virtual void				endHostListXferSession( HostListTxSession * xferSession, bool pluginIsLocked, bool requeHostList );

	virtual EXferError			rxHostListChunk( bool pluginIsLocked, HostListRxSession * xferSession, PktHostListChunkReq * poPkt );
	virtual EXferError			txNextHostListChunk( HostListTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				finishHostListReceive( HostListRxSession * xferSession, PktHostListSendCompleteReq * poPkt, bool pluginIsLocked );

	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreHostListsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						assetSendComplete( HostListTxSession * xferSession );
	void						queHostList( HostListInfo& assetInfo );
	EXferError					createHostListTxSessionAndSend( bool pluginIsLocked, HostListInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
	bool						requireFileXfer( EHostListType assetType );

	//=== vars ===//
	bool						m_Initialized;
	std::map<VxGUID, HostListRxSession *>	m_RxSessions;
	std::vector<HostListTxSession *>		m_TxSessions;
	VxMutex						m_TxSessionsMutex;

	PluginMessenger&			m_Plugin;	
	PluginSessionMgr&			m_PluginSessionMgr;
	PluginMgr&					m_PluginMgr;
	P2PEngine&					m_Engine;					
	
	HostListMgr&				m_HostListMgr;
	HostListXferDb				m_HostListXferDb;

	VxMutex						m_HostListSendQueMutex;
	std::vector<HostListInfo>	m_HostListSendQue;
	VxThread					m_WorkerThread;
};



