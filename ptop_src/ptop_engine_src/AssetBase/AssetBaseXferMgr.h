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

#include "AssetBaseXferDb.h"
#include "AssetBaseInfo.h"
#include "BaseXferInterface.h"

#include <GuiInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxGuidPairList.h>

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

class PktBaseGetReq;
class PktBaseGetReply;
class PktBaseSendReq;
class PktBaseSendReply;
class PktBaseChunkReq;
class PktBaseChunkReply;
class PktBaseGetCompleteReq;
class PktBaseGetCompleteReply;
class PktBaseSendCompleteReq;
class PktBaseSendCompleteReply;
class PktBaseXferErr;
class PktBaseListReq;
class PktBaseListReply;

class AssetBaseXferMgr
{
public:
	AssetBaseXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName );
	virtual ~AssetBaseXferMgr();

	VxMutex&					getAssetBaseQueMutex( void )					{ return m_AssetBaseSendQueMutex; }
	void						lockAssetBaseQue( void )						{ m_AssetBaseSendQueMutex.lock(); }
	void						unlockAssetBaseQue( void )						{ m_AssetBaseSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual bool				fromGuiSendAssetBase( AssetBaseInfo& assetInfo );
    virtual bool				fromGuiRequestAssetBase( AssetBaseInfo& assetInfo, VxSktBase * sktBase = nullptr );
    virtual bool				fromGuiRequestAssetBase(  VxNetIdent * netIdent, AssetBaseInfo& assetInfo, VxSktBase* sktBase = nullptr );
	virtual void				fromGuiCancelDownload( VxGUID& lclSessionId );
	virtual void				fromGuiCancelUpload( VxGUID& lclSessionId );

	virtual void				sendToGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 );

	void						fileAboutToBeDeleted( std::string& fileName );
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual void				onConnectionLost( VxSktBase * sktBase );
	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

    virtual void				onPktAssetBaseGetReq			    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktAssetBaseGetReply			    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseSendReq			    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseSendReply			    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseChunkReq			    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseChunkReply		    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktAssetBaseGetCompleteReq	    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktAssetBaseGetCompleteReply	    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseSendCompleteReq	    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseSendCompleteReply	    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetBaseXferErr			    ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onAssetBaseReceived( AssetBaseRxSession * xferSession, AssetBaseInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onAssetBaseSent( VxNetIdent * netIdent, VxSktBase * sktBase, AssetBaseInfo& assetInfo, EXferError error, bool pluginIsLocked );
    virtual void				onRequestAssetFailed( VxNetIdent * netIdent, AssetBaseInfo& assetInfo, VxGUID& sktConnectId, bool pluginIsLocked );
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

	virtual EXferError			        beginAssetBaseReceive( AssetBaseRxSession * xferSession, AssetBaseInfo& assetInfo, PktBaseSendReq * poPkt, PktBaseSendReply& pktReply );
    virtual EXferError			        beginAssetBaseReceive( AssetBaseRxSession * xferSession, AssetBaseInfo& assetInfo, VxGUID& lclSessionId, int64_t startOffset = 0 );

	virtual EXferError			        beginAssetBaseSend( AssetBaseTxSession * xferSession );

	virtual void				        endAssetBaseXferSession( AssetBaseRxSession * xferSession, bool pluginIsLocked );
	virtual void				        endAssetBaseXferSession( AssetBaseTxSession * xferSession, bool pluginIsLocked, bool requeAsset );

	virtual EXferError			        rxAssetBaseChunk( bool pluginIsLocked, AssetBaseRxSession * xferSession, PktBaseChunkReq * poPkt );
	virtual EXferError			        txNextAssetBaseChunk( AssetBaseTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				        finishAssetBaseReceive( AssetBaseRxSession * xferSession, PktBaseSendCompleteReq * poPkt, bool pluginIsLocked );

	void						        clearRxSessionsList( void );
	void						        clearTxSessionsList( void );
	void						        checkQueForMoreAssetsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						        assetSendComplete( AssetBaseTxSession * xferSession );
	void						        queAsset( AssetBaseInfo& assetInfo );
	EXferError					        createAssetTxSessionAndSend( bool pluginIsLocked, AssetBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
    EXferError                          createAssetRxSessionAndReceive( bool pluginIsLocked, AssetBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );

	bool						        requireFileXfer( EAssetType assetType );

    virtual void                        onAssetBaseBeginUpload( VxNetIdent *netIdent, AssetBaseInfo& assetInfo ) {};
    virtual void                        onAssetBaseUploadError( VxNetIdent *netIdent, AssetBaseInfo& assetInfo, EXferError xferErr ) {};

    virtual PktBaseGetReq*			    createPktBaseGetReq( void );
    virtual PktBaseGetReply*			createPktBaseGetReply( void );
    virtual PktBaseSendReq*			    createPktBaseSendReq( void );
    virtual PktBaseSendReply*			createPktBaseSendReply( void );
    virtual PktBaseChunkReq*			createPktBaseChunkReq( void );
    virtual PktBaseChunkReply*			createPktBaseChunkReply( void );
    virtual PktBaseGetCompleteReq*		createPktBaseGetCompleteReq( void );
    virtual PktBaseGetCompleteReply*	createPktBaseGetCompleteReply( void );
    virtual PktBaseSendCompleteReq*		createPktBaseSendCompleteReq( void );
    virtual PktBaseSendCompleteReply*	createPktBaseSendCompleteReply( void );
    virtual PktBaseXferErr*			    createPktBaseXferErr( void );

    virtual PktBaseListReq*			    createPktBaseListReq( void );
    virtual PktBaseListReply*			createPktBaseListReply( void );

	virtual bool						isAssetRequested( VxGUID& assetId, VxGUID& sktConnectId );
	virtual void						assetXferComplete( VxGUID& assetId, VxGUID& sktConnectId );

	//=== vars ===//
	bool						        m_Initialized;
	std::map<VxGUID, AssetBaseRxSession *>	m_RxSessions;
	std::vector<AssetBaseTxSession *>		m_TxSessions;
	VxMutex						        m_TxSessionsMutex;

    P2PEngine&					        m_Engine;	
    AssetBaseMgr&				        m_AssetBaseMgr;
    BaseXferInterface&                  m_XferInterface;
    PluginMgr&					        m_PluginMgr;
	
	AssetBaseXferDb				        m_AssetBaseXferDb;

	VxMutex						        m_AssetBaseSendQueMutex;
	std::vector<AssetBaseInfo>	        m_AssetBaseSendQue;
	VxThread					        m_WorkerThread;
    std::string                         m_WorkerThreadName;

	VxMutex						        m_AssetRequestedListMutex;
	VxGuidPairList						m_AssetRequestedList;
};



