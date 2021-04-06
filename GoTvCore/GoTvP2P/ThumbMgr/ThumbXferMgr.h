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

#include "ThumbXferDb.h"
#include "ThumbInfo.h"

#include <GoTvInterface/IDefs.h>

#include <PktLib/VxCommon.h>
#include <CoreLib/VxThread.h>

#include <map>

class PluginMessenger;
class PluginSessionMgr;
class PluginMgr;
class VxPktHdr;
class ThumbRxSession;
class ThumbTxSession;
class ThumbMgr;
class IToGui;
class P2PEngine;
class VxSha1Hash;

class PktAssetSendReq;
class PktAssetSendReply;
class PktAssetChunkReq;
class PktAssetSendCompleteReq;
class PktThumbListReply;

class ThumbXferMgr
{
public:
	typedef std::map<VxGUID, ThumbRxSession *>::iterator ThumbRxIter;
	typedef std::vector<ThumbTxSession *>::iterator ThumbTxIter;

	ThumbXferMgr( PluginMessenger& plugin, PluginSessionMgr&	pluginSessionMgr );
	virtual ~ThumbXferMgr();

	VxMutex&					getAssetQueMutex( void )					{ return m_ThumbSendQueMutex; }
	void						lockThumbQue( void )						{ m_ThumbSendQueMutex.lock(); }
	void						unlockThumbQue( void )						{ m_ThumbSendQueMutex.unlock(); }

	virtual void				fromGuiUserLoggedOn( void );

	virtual void				fromGuiSendThumb( ThumbInfo& assetInfo );
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
	virtual void				onPktThumbXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	//virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	void						assetXferThreadWork( VxThread * workThread );
protected:
	virtual void				onThumbReceived( ThumbRxSession * xferSession, ThumbInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onThumbSent( ThumbTxSession * xferSession, ThumbInfo& assetInfo, EXferError error, bool pluginIsLocked );
	virtual void				onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked );
	virtual void				updateThumbMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param );

	virtual ThumbRxSession *	    findRxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual ThumbRxSession *	    findRxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual ThumbRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual ThumbRxSession *	    findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual ThumbTxSession *	    findTxSession( bool pluginIsLocked, VxNetIdent * netIdent );
	virtual ThumbTxSession *	    findTxSession( bool pluginIsLocked, VxGUID& lclSessionId );
	virtual ThumbTxSession *	    createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual ThumbTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase );
	virtual ThumbTxSession *	    findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase );

	virtual EXferError			beginThumbReceive( ThumbRxSession * xferSession, PktAssetSendReq * poPkt, PktAssetSendReply& pktReply );
	virtual EXferError			beginThumbSend( ThumbTxSession * xferSession );

	virtual void				endThumbXferSession( ThumbRxSession * xferSession, bool pluginIsLocked );
	virtual void				endThumbXferSession( ThumbTxSession * xferSession, bool pluginIsLocked, bool requeThumb );

	virtual EXferError			rxThumbChunk( bool pluginIsLocked, ThumbRxSession * xferSession, PktAssetChunkReq * poPkt );
	virtual EXferError			txNextThumbChunk( ThumbTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked );

	virtual void				finishThumbReceive( ThumbRxSession * xferSession, PktAssetSendCompleteReq * poPkt, bool pluginIsLocked );

	void						clearRxSessionsList( void );
	void						clearTxSessionsList( void );
	void						checkQueForMoreThumbsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase );

	void						assetSendComplete( ThumbTxSession * xferSession );
	void						queThumb( ThumbInfo& assetInfo );
	EXferError					createThumbTxSessionAndSend( bool pluginIsLocked, ThumbInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase );
	bool						requireFileXfer( EAssetType assetType );

	//=== vars ===//
	bool						m_Initialized;
	std::map<VxGUID, ThumbRxSession *>	m_RxSessions;
	std::vector<ThumbTxSession *>		m_TxSessions;
	VxMutex						m_TxSessionsMutex;

	PluginMessenger&			m_Plugin;	
	PluginSessionMgr&			m_PluginSessionMgr;
	PluginMgr&					m_PluginMgr;
	P2PEngine&					m_Engine;					
	
	ThumbMgr&				    m_ThumbMgr;
	ThumbXferDb				    m_ThumbXferDb;

	VxMutex						m_ThumbSendQueMutex;
	std::vector<ThumbInfo>	    m_ThumbSendQue;
	VxThread					m_WorkerThread;
};



