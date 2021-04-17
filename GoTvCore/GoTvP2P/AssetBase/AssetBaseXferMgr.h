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

#include "AssetBaseXferDb.h"
#include "AssetBaseInfo.h"

#include <GuiInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class AssetBaseRxSession;
class AssetBaseTxSession;
class AssetBaseMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktAssetSendReq;
class PktAssetSendReply;
class PktAssetChunkReq;
class PktAssetSendCompleteReq;
class PktAssetListReply;

class AssetBaseXferMgr
{
public:
	typedef std::map<VxGUID, AssetBaseRxSession *>::iterator AssetBaseRxIter;
	typedef std::vector<AssetBaseTxSession *>::iterator AssetBaseTxIter;

	AssetBaseXferMgr( P2PEngine& engine, PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr, const char * stateDbName );
	virtual ~AssetBaseXferMgr();

	VxMutex&					getAssetBaseQueMutex( void )					{ return m_AssetBaseSendQueMutex; }
	void						lockAssetBaseQue( void )						{ m_AssetBaseSendQueMutex.lock(); }
	void						unlockAssetBaseQue( void )						{ m_AssetBaseSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiSendAssetBase( AssetBaseInfo& assetInfo );
	virtual void				fromGuiCancelDownload( VxGUID& lclSessionId );
	virtual void				fromGuiCancelUpload( VxGUID& lclSessionId );

	void						fileAboutToBeDeleted( std::string& fileName );
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	virtual void				onPktAssetSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetChunkReply		    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onAssetBaseReceived( AssetBaseRxSession * xferSession, AssetBaseInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onAssetBaseSent( AssetBaseTxSession * xferSession, AssetBaseInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				updateAssetMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param );

	virtual AssetBaseRxSession *	    findRxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual AssetBaseRxSession *	    findRxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual AssetBaseRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetBaseRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetBaseTxSession *	    findTxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual AssetBaseTxSession *	    findTxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual AssetBaseTxSession *	    createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetBaseTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetBaseTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );

	virtual EXferError			beginAssetBaseReceive( AssetBaseRxSession * xferSession, PktAssetSendReq * poPkt, PktAssetSendReply& pktReply );
	virtual EXferError			beginAssetBaseSend( AssetBaseTxSession * xferSession );

	virtual void				endAssetBaseXferSession( AssetBaseRxSession * xferSession, bool pluginIsLocked );
	virtual void				endAssetBaseXferSession( AssetBaseTxSession * xferSession, bool pluginIsLocked, bool requeAsset );

	virtual EXferError			rxAssetBaseChunk( bool pluginIsLocked, AssetBaseRxSession * xferSession, PktAssetChunkReq * poPkt );
	virtual EXferError			txNextAssetBaseChunk( AssetBaseTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				finishAssetBaseReceive( AssetBaseRxSession * xferSession, PktAssetSendCompleteReq * poPkt, bool pluginIsLocked );

	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreAssetsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						assetSendComplete( AssetBaseTxSession * xferSession );
	void						queAsset( AssetBaseInfo& assetInfo );
	EXferError					createAssetTxSessionAndSend( bool pluginIsLocked, AssetBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
	bool						requireFileXfer( EAssetType assetType );

	//=== vars ===//
	bool						m_Initialized;
	std::map<VxGUID, AssetBaseRxSession *>	m_RxSessions;
	std::vector<AssetBaseTxSession *>		m_TxSessions;
	VxMutex						m_TxSessionsMutex;

    P2PEngine&					m_Engine;	
    AssetBaseMgr&				m_AssetBaseMgr;
    PluginMgr&					m_PluginMgr;
	PluginMessenger&			m_Plugin;	
	PluginSessionMgr&			m_PluginSessionMgr;				
	
	AssetBaseXferDb				m_AssetBaseXferDb;

	VxMutex						m_AssetBaseSendQueMutex;
	std::vector<AssetBaseInfo>	m_AssetBaseSendQue;
	VxThread					m_WorkerThread;
};



