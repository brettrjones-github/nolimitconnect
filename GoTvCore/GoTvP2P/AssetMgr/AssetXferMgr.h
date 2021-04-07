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

#include "AssetXferDb.h"
#include "AssetInfo.h"

#include <GoTvInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginBaseMultimedia;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class AssetRxSession;
class AssetTxSession;
class AssetMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktAssetSendReq;
class PktAssetSendReply;
class PktAssetChunkReq;
class PktAssetSendCompleteReq;
class PktAssetListReply;

class AssetXferMgr
{
public:
	typedef std::map<VxGUID, AssetRxSession *>::iterator AssetRxIter;
	typedef std::vector<AssetTxSession *>::iterator AssetTxIter;

	AssetXferMgr( P2PEngine& engine, PluginBaseMultimedia& plugin, PluginSessionMgr&	pluginSessionMgr );
	virtual ~AssetXferMgr();

	VxMutex&					getAssetQueMutex( void )					{ return m_AssetSendQueMutex; }
	void						lockAssetQue( void )						{ m_AssetSendQueMutex.lock(); }
	void						unlockAssetQue( void )						{ m_AssetSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiSendAsset( AssetInfo& assetInfo );
	virtual void				fromGuiCancelDownload( VxGUID& lclSessionId );
	virtual void				fromGuiCancelUpload( VxGUID& lclSessionId );

	void						fileAboutToBeDeleted( std::string& fileName );
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

	virtual void				onPktAssetSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetChunkReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onAssetReceived( AssetRxSession * xferSession, AssetInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onAssetSent( AssetTxSession * xferSession, AssetInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				updateAssetMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param );

	virtual AssetRxSession *	findRxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual AssetRxSession *	findRxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual AssetRxSession *	findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetRxSession *	findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetTxSession *	findTxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual AssetTxSession *	findTxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual AssetTxSession *	createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetTxSession *	findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual AssetTxSession *	findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );

	virtual EXferError			beginAssetReceive( AssetRxSession * xferSession, PktAssetSendReq * poPkt, PktAssetSendReply& pktReply );
	virtual EXferError			beginAssetSend( AssetTxSession * xferSession );

	virtual void				endAssetXferSession( AssetRxSession * xferSession, bool pluginIsLocked );
	virtual void				endAssetXferSession( AssetTxSession * xferSession, bool pluginIsLocked, bool requeAsset );

	virtual EXferError			rxAssetChunk( bool pluginIsLocked, AssetRxSession * xferSession, PktAssetChunkReq * poPkt );
	virtual EXferError			txNextAssetChunk( AssetTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				finishAssetReceive( AssetRxSession * xferSession, PktAssetSendCompleteReq * poPkt, bool pluginIsLocked );

	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreAssetsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						assetSendComplete( AssetTxSession * xferSession );
	void						queAsset( AssetInfo& assetInfo );
	EXferError					createAssetTxSessionAndSend( bool pluginIsLocked, AssetInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
	bool						requireFileXfer( EAssetType assetType );

	//=== vars ===//
	bool						m_Initialized;
	std::map<VxGUID, AssetRxSession *>	m_RxSessions;
	std::vector<AssetTxSession *>		m_TxSessions;
	VxMutex						m_TxSessionsMutex;

    P2PEngine&                  m_Engine;
    PluginBaseMultimedia&		m_Plugin;
	PluginSessionMgr&			m_PluginSessionMgr;
	PluginMgr&					m_PluginMgr;				
	
	AssetMgr&					m_AssetMgr;
	AssetXferDb					m_AssetXferDb;

	VxMutex						m_AssetSendQueMutex;
	std::vector<AssetInfo>		m_AssetSendQue;
	VxThread					m_WorkerThread;
};



