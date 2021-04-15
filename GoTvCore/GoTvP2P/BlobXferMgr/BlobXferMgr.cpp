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
#include "BlobXferMgr.h"
#include "BlobInfo.h"
#include "BlobMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "BlobTxSession.h"
#include "BlobRxSession.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <PktLib/PktsBlobXfer.h>
#include <PktLib/VxCommon.h>
#include <NetLib/VxSktBase.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/AppErr.h>
#include <CoreLib/VxFileUtil.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//#define DEBUG_AUTOPLUGIN_LOCK 1

namespace
{
	//#define		MAX_ASSET_XFER_OUTSTANDING_PKTS 3
	//#define		MAX_ASSET_XFER_TX_SESSIONS		5;
	const char * ASSET_XFER_DB_NAME = "BlobXferDb.db3";

	//============================================================================
    static void * BlobXferMgrThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		BlobXferMgr * poMgr = (BlobXferMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetXferThreadWork( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}


//============================================================================
BlobXferMgr::BlobXferMgr( P2PEngine& engine, PluginMessenger& plugin, PluginSessionMgr&pluginSessionMgr, const char * stateDbName )
: m_Engine( engine )
, m_Plugin( plugin )
, m_PluginSessionMgr( pluginSessionMgr )
, m_PluginMgr( engine.getPluginMgr() )
, m_BlobMgr( m_Engine.getBlobMgr() )
, m_BlobXferDb( stateDbName )
{
}

//============================================================================
BlobXferMgr::~BlobXferMgr()
{
	clearRxSessionsList();
	clearTxSessionsList();
}

//============================================================================
void BlobXferMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_WorkerThread.startThread( (VX_THREAD_FUNCTION_T)BlobXferMgrThreadFunc, this, "BlobXferThrd" );			
	}
}

//============================================================================
void BlobXferMgr::assetXferThreadWork( VxThread * workThread )
{
	if( workThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += ASSET_XFER_DB_NAME; 
	lockBlobQue();
	m_BlobXferDb.dbShutdown();
	m_BlobXferDb.dbStartup( 1, dbName );
	unlockBlobQue();
	if( workThread->isAborted() )
		return;

	std::vector<VxGUID> assetToSendList;
	m_BlobXferDb.getAllBlobs( assetToSendList );
	if( 0 == assetToSendList.size() )
	{
		// nothing to do
		return;
	}

	while( ( false == m_BlobMgr.isAssetListInitialized() )
			&& ( false == workThread->isAborted() ) )
	{
		// waiting for assets to be available
		VxSleep( 500 );
	}

	if( workThread->isAborted() )
		return;

	std::vector<VxGUID>::iterator iter; 
	m_BlobMgr.lockResources();
	lockBlobQue();
	for( iter = assetToSendList.begin(); iter != assetToSendList.end(); ++iter )
	{
		BlobInfo * assetInfo = dynamic_cast<BlobInfo*>(m_BlobMgr.findAsset( *iter ));
		if( assetInfo )
		{
			m_BlobSendQue.push_back( *assetInfo );
		}
		else
		{
			LogMsg( LOG_ERROR, "assetXferThreadWork removing asset not found in list\n" );
			m_BlobXferDb.removeBlob( *iter );
		}
	}

	unlockBlobQue();
	m_BlobMgr.unlockResources();
}

//============================================================================
void BlobXferMgr::fromGuiCancelDownload( VxGUID& lclSessionId )
{
	std::map<VxGUID, BlobRxSession *>::iterator iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelDownload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelDownload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		BlobRxSession * xferSession = iter->second;
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_RxSessions.erase( iter );
			xferSession->cancelDownload( lclSessionId );
			delete xferSession;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelDownload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::fromGuiCancelUpload( VxGUID& lclSessionId )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelUpload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelUpload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	BlobTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		BlobTxSession * xferSession = ( *iter );
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( lclSessionId );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}


//============================================================================
void BlobXferMgr::clearRxSessionsList( void )
{
	std::map<VxGUID, BlobRxSession *>::iterator iter;

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::clearRxSessionsList AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::clearRxSessionsList AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		BlobRxSession * xferSession = iter->second;
		delete xferSession;
	}

	m_RxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::clearRxSessionsList AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::clearTxSessionsList( void )
{
	BlobTxIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		BlobTxSession * xferSession = (*iter);
		delete xferSession;
	}

	m_TxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::fileAboutToBeDeleted( std::string& fileName )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fileAboutToBeDeleted AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fileAboutToBeDeleted AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	BlobTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		BlobTxSession * xferSession = ( *iter );
		if( xferSession->getXferInfo().getLclFileName() == fileName )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( xferSession->getXferInfo().getLclSessionId() );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "BlobXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::onConnectionLost( VxSktBase * sktBase )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onConnectionLost AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onConnectionLost AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	bool erasedSession = true;
	BlobTxIter iter;
	while( erasedSession )
	{
		erasedSession = false;
		for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
		{
			BlobTxSession * xferSession = ( *iter );
			if( xferSession->getSkt() == sktBase )
			{
				m_TxSessions.erase( iter );
				xferSession->cancelUpload( xferSession->getXferInfo().getLclSessionId() );
				delete xferSession;
				erasedSession = true;
				break;
			}
		}
	}

	erasedSession = true;
	BlobRxIter oRxIter; 
	while( erasedSession )
	{
		erasedSession = false;
		for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
		{
			BlobRxSession * xferSession = oRxIter->second;
			if( xferSession->getSkt() == sktBase )
			{				
				m_RxSessions.erase( oRxIter );
				xferSession->cancelDownload( xferSession->getXferInfo().getLclSessionId() );
				delete xferSession;
				erasedSession = true;
				break;
			}
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onConnectionLost AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
bool BlobXferMgr::requireFileXfer( EAssetType assetType )
{
	return ( ( eAssetTypePhoto == assetType )
		|| ( eAssetTypeAudio == assetType )
		|| ( eAssetTypeVideo == assetType )
		|| ( eAssetTypeDocument == assetType )
		|| ( eAssetTypeArchives == assetType )
		|| ( eAssetTypeExe == assetType )
		|| ( eAssetTypeOtherFiles == assetType ) );

}

//============================================================================
void BlobXferMgr::onPktBlobSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock start\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock done\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK

	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq\n");
	PktBlobSendReq * poPkt = (PktBlobSendReq *)pktHdr;
	VxGUID& assetUniqueId = poPkt->getUniqueId();
    EAssetType assetType = (EAssetType)poPkt->getBlobType();
	bool needFileXfer = requireFileXfer( assetType );
	PktBlobSendReply pktReply;
	pktReply.setRequiresFileXfer( needFileXfer );
	pktReply.setError( 0 );
	pktReply.setRmtSessionId( poPkt->getLclSessionId() );
	pktReply.setLclSessionId( poPkt->getRmtSessionId() );
	pktReply.setUniqueId( assetUniqueId );
	if( false == netIdent->isHisAccessAllowedFromMe( m_Plugin.getPluginType() ) )
	{
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq: permission denied\n" );
		pktReply.setError( eXferErrorPermission );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	if( false == needFileXfer )
	{
		// all we need is in the send request
		BlobInfo assetInfo;
		poPkt->fillBlobFromPkt( assetInfo );
		// make history id his id
		assetInfo.setHistoryId( netIdent->getMyOnlineId() );
		assetInfo.setAssetSendState( eAssetSendStateRxSuccess );
		m_BlobMgr.addAsset( assetInfo );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		IToGui::getToGui().toGuiBlobAction( eAssetActionRxSuccess, assetInfo.getAssetUniqueId(), 100 );
		IToGui::getToGui().toGuiBlobAction( eAssetActionRxNotifyNewMsg, assetInfo.getCreatorId(), 100 );
	}
	else
	{
		BlobRxSession * xferSession = findOrCreateRxSession( true, poPkt->getRmtSessionId(), netIdent, sktBase );
		if( xferSession )
		{
			BlobInfo& assetInfo = xferSession->getBlobInfo();
			poPkt->fillBlobFromPkt( assetInfo );
			// make history id his id
			assetInfo.setHistoryId( netIdent->getMyOnlineId() );
			assetInfo.setAssetSendState( eAssetSendStateRxProgress );

			xferSession->setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setLclSessionId( xferSession->getLclSessionId() );
			EXferError xferErr = beginBlobReceive( xferSession, poPkt, pktReply );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateBlobDownload( xferSession->getLclSessionId(), 0, rc );
				endBlobXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg(LOG_ERROR, "PluginBlobOffer::onPktBlobSendReq: Could not create session\n");
			PktBlobSendReply pktReply;
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::assetSendComplete( BlobTxSession * xferSession )
{
	updateBlobMgrSendState( xferSession->getBlobInfo().getAssetUniqueId(), eAssetSendStateTxSuccess, 100 );
}

//============================================================================
void BlobXferMgr::onPktBlobSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReply\n");
	PktBlobSendReply * poPkt = (PktBlobSendReply *)pktHdr;
	VxGUID&	assetUniqueId =	poPkt->getUniqueId();
	BlobInfo * assetInfo = dynamic_cast<BlobInfo*>(m_BlobMgr.findAsset( assetUniqueId ));
	if( 0 == assetInfo )
	{
		LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobSendReply failed to find asset id\n");
		updateBlobMgrSendState( assetUniqueId, eAssetSendStateTxFail, 0 );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	bool isFileXfer = (bool)poPkt->getRequiresFileXfer();
	uint32_t rxedErrCode = poPkt->getError();
	BlobTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );

	if( xferSession )
	{
		xferSession->setRmtSessionId( poPkt->getLclSessionId() );
		if( 0 == rxedErrCode )
		{
			if( isFileXfer )
			{
				// we did txNextBlobChunk in begin file send
				//RCODE rc = txNextBlobChunk( xferSession );
				//if( rc )
				//{
				//	//IToGui::getToGui().toGuiUpdateBlobUpload( xferSession->getLclSessionId(), 0, rc );
				//	LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobSendReply beginBlobSend returned error %d\n", rc );
				//	endBlobXferSession( xferSession, true );
				//}
			}
			else
			{
				assetSendComplete( xferSession );
				endBlobXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobSendReply PktBlobSendReply returned error %d\n", poPkt->getError() );
			endBlobXferSession( xferSession, true, false );
			updateBlobMgrSendState( assetUniqueId, eAssetSendStateTxFail, rxedErrCode );
		}
	}
	else
	{
		if( isFileXfer )
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobSendReply failed to find session\n");
			updateBlobMgrSendState( assetUniqueId, eAssetSendStateTxFail, rxedErrCode );
		}
		else
		{
			updateBlobMgrSendState( assetUniqueId, rxedErrCode ? eAssetSendStateTxFail : eAssetSendStateTxSuccess, rxedErrCode );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::onPktBlobChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	BlobRxSession * xferSession = 0;
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChunkReq\n");
	PktBlobChunkReq * poPkt = (PktBlobChunkReq *)pktHdr;
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChunkReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChunkReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findRxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = rxBlobChunk( true, xferSession, poPkt );
			if( eXferErrorNone != xferErr )
			{

				PktBlobChunkReply pktReply;
				pktReply.setLclSessionId( xferSession->getLclSessionId() );
				pktReply.setRmtSessionId( poPkt->getLclSessionId() );
				pktReply.setDataLen(0);
				pktReply.setError( xferErr );
				m_Plugin.txPacket( netIdent, sktBase, &pktReply );

				IToGui::getToGui().toGuiBlobAction( eAssetActionRxError, xferSession->getBlobInfo().getAssetUniqueId(), xferErr );
				endBlobXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobChunkReq failed to find session\n");
			PktBlobChunkReply pktReply;
			pktReply.setLclSessionId( poPkt->getRmtSessionId() );
			pktReply.setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setDataLen(0);
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void BlobXferMgr::onPktBlobChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktBlobChunkReply * poPkt = (PktBlobChunkReply *)pktHdr;
	BlobTxSession * xferSession = 0;
static int cnt = 0;
	cnt++;
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChuckReply start %d\n", cnt );
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChunkReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChunkReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findTxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = txNextBlobChunk( xferSession, poPkt->getError(), true );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateBlobUpload( xferSession->getLclSessionId(), 0, rc );
				endBlobXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobChuckReply failed to find session\n" );
		}

		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChuckReply done %d\n", cnt );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobChunkReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void BlobXferMgr::onPktBlobSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReq\n");
	PktBlobSendCompleteReq * poPkt = (PktBlobSendCompleteReq *)pktHdr;
	BlobRxSession * xferSession = findRxSession( true, poPkt->getRmtSessionId() );
	//TODO check checksum
	if( xferSession )
	{
		finishBlobReceive( xferSession, poPkt, true );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::onPktBlobSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReply\n");
	PktBlobSendCompleteReply * poPkt = (PktBlobSendCompleteReply *)pktHdr;
	BlobTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );
	if( xferSession )
	{
		VxFileXferInfo xferInfo = xferSession->getXferInfo();
		LogMsg( LOG_INFO, "BlobXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		//m_PluginMgr.getToGui().toGuiBlobUploadComplete( xferInfo.getLclSessionId(), 0 );
		onBlobSent( xferSession, xferSession->getBlobInfo(), (EXferError)poPkt->getError(), true );
	}
	else
	{
		LogMsg( LOG_ERROR, "BlobXferMgr::onPktBlobSendCompleteReply failed to find session\n");
		updateBlobMgrSendState( poPkt->getAssetUniqueId(), eAssetSendStateTxSuccess, 100 );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobSendCompleteReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::onPktBlobXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "BlobXferMgr::onPktBlobXferErr\n");
	// TODO handle error
}

//============================================================================
void BlobXferMgr::endBlobXferSession( BlobRxSession * poSessionIn, bool pluginIsLocked )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::endBlobXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::endBlobXferSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	std::string fileName = xferInfo.getDownloadIncompleteFileName();
	if( fileName.length() )
	{
		VxFileUtil::deleteFile( fileName.c_str() );
	}

	BlobRxIter oRxIter = m_RxSessions.begin();
	while( oRxIter != m_RxSessions.end() )
	{
		BlobRxSession * xferSession = oRxIter->second;
		if( poSessionIn == xferSession )
		{
			m_RxSessions.erase( oRxIter );
			delete xferSession;
			break;
		}
		else
		{
			++oRxIter;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::endBlobXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
void BlobXferMgr::endBlobXferSession( BlobTxSession * poSessionIn, bool pluginIsLocked, bool requeBlob )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::endBlobXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::endBlobXferSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	//if( requeBlob )
	//{
	//	queBlob( poSessionIn->getBlobInfo() );
	//}

	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	BlobTxIter iter = m_TxSessions.begin();
	while( iter != m_TxSessions.end() )
	{
		BlobTxSession * xferSession = (*iter);
		if( xferSession == poSessionIn )
		{
			m_TxSessions.erase( iter );
			delete xferSession;
			break;
		}
		else
		{
			++iter;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::endBlobXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
BlobRxSession * BlobXferMgr::findRxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobRxIter iter;
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		BlobRxSession * xferSession = iter->second;
		if( xferSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return  xferSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
BlobRxSession * BlobXferMgr::findRxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobRxIter iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		BlobRxSession * rxSession = iter->second;
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return rxSession;
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
BlobRxSession *	BlobXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobRxSession * xferSession = findRxSession( true, netIdent );
	if( NULL == xferSession )
	{
		xferSession = new BlobRxSession( m_Engine, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
BlobRxSession *	BlobXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	if( false == lclSessionId.isVxGUIDValid() )
	{
		lclSessionId.initializeWithNewVxGUID();
	}

	BlobRxSession * xferSession = findRxSession( true, lclSessionId );
	if( NULL == xferSession )
	{
		xferSession = new BlobRxSession( m_Engine, lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
BlobTxSession * BlobXferMgr::findTxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		BlobTxSession * txSession = ( *iter );
		if( txSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
BlobTxSession * BlobXferMgr::findTxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		BlobTxSession * txSession = ( *iter );
		if( txSession->getLclSessionId() == lclSessionId )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
BlobTxSession *	BlobXferMgr::createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	BlobTxSession * txSession = new BlobTxSession( m_Engine, sktBase, netIdent );
	return txSession;
}

//============================================================================
BlobTxSession *	BlobXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobTxSession * xferSession = findTxSession( true, netIdent );
	if( NULL == xferSession )
	{
		xferSession = createTxSession( netIdent, sktBase );
		if( false == xferSession->getLclSessionId().isVxGUIDValid() )
		{
			xferSession->getLclSessionId().initializeWithNewVxGUID();
		}

		m_TxSessions.push_back( xferSession );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
BlobTxSession *	 BlobXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	BlobTxSession * xferSession = 0;
	if( lclSessionId.isVxGUIDValid() )
	{
		xferSession = findTxSession( true, lclSessionId );
	}
	else
	{
		xferSession = findTxSession( true, netIdent );
	}

	if( NULL == xferSession )
	{
		xferSession = new BlobTxSession( m_Engine, lclSessionId, sktBase, netIdent );
		if( false == xferSession->getLclSessionId().isVxGUIDValid() )
		{
			xferSession->getLclSessionId().initializeWithNewVxGUID();
		}

		m_TxSessions.push_back( xferSession );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
void BlobXferMgr::fromGuiSendBlob( BlobInfo& assetInfo )
{
	bool xferFailed = true;
	BigListInfo * hisInfo = m_Engine.getBigListMgr().findBigListInfo( assetInfo.getHistoryId() );
	if( hisInfo )
	{
		// first try to connect and send.. if that fails then que and will send when next connected
		VxSktBase * sktBase = 0;
		m_PluginMgr.pluginApiSktConnectTo( m_Plugin.getPluginType(), hisInfo, 0, &sktBase );
		if( 0 != sktBase )
		{
			EXferError xferError = createBlobTxSessionAndSend( false, assetInfo, hisInfo, sktBase );
			if( xferError == eXferErrorNone )
			{
				xferFailed = false;
			}
		}
		else
		{
			queBlob( assetInfo );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "BlobXferMgr::fromGuiSendBlob NetIdent not found\n" );
	}

	if( xferFailed )
	{
		onTxFailed( assetInfo.getAssetUniqueId(), false );
	}
}

//============================================================================
void BlobXferMgr::onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked )
{
	updateBlobMgrSendState( assetUniqueId, eAssetSendStateTxFail, 0 );
}

//============================================================================
void BlobXferMgr::onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked )
{
	updateBlobMgrSendState( assetUniqueId, eAssetSendStateTxSuccess, 0 );
}

//============================================================================
void BlobXferMgr::updateBlobMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param )
{
	m_BlobMgr.updateAssetXferState( assetUniqueId, sendState, param );
}

//============================================================================
void BlobXferMgr::queBlob( BlobInfo& assetInfo )
{
	m_BlobSendQueMutex.lock();
	bool foundBlob = false;
    std::vector<BlobInfo>::iterator iter;
	for( iter = m_BlobSendQue.begin(); iter != m_BlobSendQue.end(); ++iter )
	{
		if( (*iter).getAssetUniqueId() == assetInfo.getAssetUniqueId() )
		{
			foundBlob = true;
			break;
		}
	}

	if( false == foundBlob )
	{
		m_BlobSendQue.push_back( assetInfo );
	}

	m_BlobSendQueMutex.unlock();
}

//============================================================================
EXferError BlobXferMgr::createBlobTxSessionAndSend( bool pluginIsLocked, BlobInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::createBlobTxSessionAndSend pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::createBlobTxSessionAndSend pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	EXferError xferErr = eXferErrorNone;
	BlobTxSession * txSession = createTxSession( hisIdent, sktBase );
	if( false == txSession->getLclSessionId().isVxGUIDValid() )
	{
		txSession->getLclSessionId().initializeWithNewVxGUID();
	}

	if( false == txSession->getRmtSessionId().isVxGUIDValid() )
	{
		txSession->setRmtSessionId( txSession->getLclSessionId() );
	}

	txSession->setBlobInfo( assetInfo );
	VxFileXferInfo& xferInfo = txSession->getXferInfo();
	xferInfo.setLclSessionId( txSession->getLclSessionId() );
	xferInfo.setRmtSessionId( txSession->getRmtSessionId() );
	xferInfo.setXferDirection( eXferDirectionTx );

	m_TxSessionsMutex.lock();
	m_TxSessions.push_back( txSession );
	m_TxSessionsMutex.unlock();

	updateBlobMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxProgress, 0 );
	if( assetInfo.hasFileName() )
	{
		// need to do first so file handle is set before get asset send reply back
		xferErr = beginBlobSend( txSession );
	}
	else
	{
		// all data was in the request packet .. we just wait for reply
	}

	if( eXferErrorNone != xferErr )
	{
		// failed to open file
		updateBlobMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, xferErr );
		endBlobXferSession( txSession, true, false );
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "BlobXferMgr::createBlobTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return xferErr;
	}

	PktBlobSendReq sendReq;
	sendReq.fillPktFromBlob( assetInfo );
	sendReq.setLclSessionId( xferInfo.getLclSessionId() );
	sendReq.setRmtSessionId( xferInfo.getRmtSessionId() );
	if( false == m_PluginMgr.pluginApiTxPacket( m_Plugin.getPluginType(), hisIdent->getMyOnlineId(), sktBase, &sendReq ) )
	{
		xferErr = eXferErrorDisconnected;
	}	

	if( eXferErrorNone == xferErr )
	{
		// re que for try some other time
		if( requireFileXfer( assetInfo.getAssetType() ) )
		{
			xferErr = txNextBlobChunk( txSession, eXferErrorNone, true );
		}
	}

	if( eXferErrorNone != xferErr )
	{
		// re que for try some other time
		updateBlobMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, xferErr );
		endBlobXferSession( txSession, true, ( ( eXferErrorFileNotFound == xferErr ) || ( eXferErrorDisconnected == xferErr ) ) ? false : true );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "BlobXferMgr::createBlobTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferErr;
}

//============================================================================
EXferError BlobXferMgr::beginBlobSend( BlobTxSession * xferSession )
{
	EXferError xferErr = eXferErrorNone;
	xferSession->clearErrorCode();
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "BlobXferMgr::beginBlobSend: ERROR: Blob transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	if( eXferErrorNone == xferErr )
	{
		xferInfo.setXferDirection( eXferDirectionTx );
		xferInfo.setLclFileName( xferSession->getBlobInfo().getAssetName().c_str() );
		VxFileUtil::getJustFileName( xferSession->getBlobInfo().getAssetName().c_str(), xferInfo.getRmtFileName() );
		xferInfo.setLclSessionId( xferSession->getLclSessionId() );
		xferInfo.setRmtSessionId( xferSession->getRmtSessionId() );
		xferInfo.setFileHashId( xferSession->getFileHashId() );

		xferInfo.m_u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 == xferInfo.m_u64FileLen )
		{
			// no file found to send
			LogMsg( LOG_INFO, "BlobXferMgr::beginBlobSend: Blob %s not found to send", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileNotFound;
		}
		else if( false == xferInfo.getFileHashId().isHashValid() )
		{
			// see if we can get hash from shared files
			//if( !m_SharedBlobsMgr.getBlobHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
			//{
			//	// TODO.. que for hash
			//}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		xferInfo.m_hFile = fopen( xferInfo.getLclFileName().c_str(), "rb" ); 
		if( NULL == xferInfo.m_hFile )
		{
			// open file failed
			xferInfo.m_hFile = NULL;
			LogMsg( LOG_INFO, "BlobXferMgr::beginBlobSend: Could not open Blob %s", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileOpenError;
			xferSession->setErrorCode( VxGetLastError() );
		}
	}

	if( eXferErrorNone == xferErr )
	{
		if( 0 != xferInfo.m_u64FileOffs )
		{
			if( xferInfo.m_u64FileLen < xferInfo.m_u64FileOffs )
			{
				fclose( xferInfo.m_hFile );
				xferInfo.m_hFile = NULL;
				LogMsg( LOG_INFO, "BlobXferMgr::beginBlobSend: Blob %s could not be resumed because too short", 
					(const char *)xferInfo.getLclFileName().c_str() );
				xferErr  = eXferErrorFileSeekError;
			}

			if( eXferErrorNone == xferErr )
			{
				RCODE rc = -1;
				// we have valid file so seek to end so we can resume if partial file exists
				if( 0 != (rc = VxFileUtil::fileSeek( xferInfo.m_hFile, xferInfo.m_u64FileOffs )) )
				{
					// seek failed
					fclose( xferInfo.m_hFile );
					xferInfo.m_hFile = NULL;
					LogMsg( LOG_INFO, "BlobXferMgr::beginBlobSend: could not seek to position %d in file %s",
						xferInfo.m_u64FileOffs,
						(const char *)xferInfo.getLclFileName().c_str() );
					xferErr  = eXferErrorFileSeekError;
					xferSession->setErrorCode( rc );
				}
			}
		}
	}

	return xferErr;
}

//============================================================================
EXferError BlobXferMgr::beginBlobReceive( BlobRxSession * xferSession, PktBlobSendReq * poPkt, PktBlobSendReply& pktReply )
{
	if( NULL == xferSession )
	{
		LogMsg( LOG_ERROR, "BlobXferMgr::beginBlobReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	uint64_t u64FileLen;
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( poPkt->getError() )
	{
		//IToGui::getToGui().toGuiUpdateBlobDownload( poPkt->getRmtSessionId(), 0, poPkt->getError() );
		xferErr = (EXferError)poPkt->getError();
		return xferErr;
	}

	if( eXferErrorNone == xferErr )
	{
		if( xferInfo.m_hFile )
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::beginBlobReceive: ERROR:(Blob Receive) receive transfer still in progress" );
			xferErr = eXferErrorAlreadyDownloading;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		// get file information
		xferInfo.setFileHashId( poPkt->getBlobHashId() );
		xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
		if( false == xferInfo.getLclSessionId().isVxGUIDValid() )
		{
			xferInfo.getLclSessionId().initializeWithNewVxGUID();
		}

		xferInfo.setRmtFileName( poPkt->getBlobName().c_str() );
		if( 0 == xferInfo.getRmtFileName().length() )
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::beginBlobReceive: ERROR: No file Name\n" );
			xferErr = eXferErrorBadParam;
		}	
	}

	std::string strRmtPath;
	std::string strRmtBlobNameOnly;
    VxFileUtil::seperatePathAndFile(		xferInfo.getRmtFileName(),
                                            strRmtPath,
                                            strRmtBlobNameOnly );


	if( eXferErrorNone == xferErr )
	{
		// make full path
		if( 0 == strRmtBlobNameOnly.length() )
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::beginBlobReceive: ERROR: NULL file Name %s\n",  xferInfo.getRmtFileName().c_str() );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		VxFileUtil::makeFullPath( strRmtBlobNameOnly.c_str(), VxGetIncompleteDirectory().c_str(), xferInfo.getLclFileName() );
		std::string strPath;
		std::string strBlobNameOnly;
		RCODE rc = VxFileUtil::seperatePathAndFile(	xferInfo.getLclFileName(),			
													strPath,			
													strBlobNameOnly );	
		VxFileUtil::makeDirectory( strPath );
		xferInfo.m_u64FileLen = poPkt->getBlobLen();
		xferInfo.m_u64FileOffs = poPkt->getBlobOffset();
		u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );

		if( 0 != xferInfo.m_u64FileOffs )
		{
			if( u64FileLen < xferInfo.m_u64FileOffs )
			{
				xferErr  = eXferErrorFileSeekError;
				LogMsg( LOG_INFO, "BlobXferMgr: ERROR:(Blob Send) %d Blob %s could not be resumed because too short", 
					rc,
					(const char *)xferInfo.getLclFileName().c_str() );
			}
			else
			{
				xferInfo.m_hFile = fopen( xferInfo.getLclFileName().c_str(), "a+" ); // pointer to name of the file
				if( NULL == xferInfo.m_hFile )
				{
					// failed to open file
					xferInfo.m_hFile = NULL;
					rc = VxGetLastError();
					xferSession->setErrorCode( rc );
					xferErr  = eXferErrorFileOpenError;

					LogMsg( LOG_INFO, "BlobXferMgr: ERROR:(Blob Send) %d Blob %s could not be created", 
						rc,
						(const char *)xferInfo.getLclFileName().c_str() );
				}
				else
				{
					// we have valid file so seek to end so we can resume if partial file exists
					if( 0 != (rc = VxFileUtil::fileSeek( xferInfo.m_hFile, xferInfo.m_u64FileOffs )) )
					{
						// seek failed
						xferSession->setErrorCode( rc );
						xferErr  = eXferErrorFileSeekError;
						fclose( xferInfo.m_hFile );
						xferInfo.m_hFile = NULL;
						LogMsg( LOG_INFO, "BlobXferMgr: ERROR: (Blob Send) could not seek to position %d in file %s",
							xferInfo.m_u64FileOffs,
							(const char *)xferInfo.getLclFileName().c_str() );
					}
				}
			}
		}
		else
		{
			// open file and truncate if exists
			xferInfo.m_hFile = fopen( xferInfo.getLclFileName().c_str(), "wb+" ); // pointer to name of the file
			if( NULL == xferInfo.m_hFile )
			{
				// failed to open file
				xferInfo.m_hFile = NULL;
				rc = VxGetLastError();
				xferSession->setErrorCode( rc );
				xferErr = eXferErrorFileCreateError;

				LogMsg( LOG_INFO, "BlobXferMgr: ERROR: %d Blob %s could not be created", 
					rc,
					(const char *)xferInfo.getLclFileName().c_str() );
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_INFO, "BlobXferMgr::(Blob Send) start recieving file %s\n", 
			(const char *)xferInfo.getLclFileName().c_str() );
		poPkt->fillBlobFromPkt( xferSession->getBlobInfo() );
	}

	pktReply.setError( xferErr );
	pktReply.setBlobOffset( xferInfo.m_u64FileOffs );
	if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &pktReply ) )
	{
		xferErr = eXferErrorDisconnected;
	}

	return xferErr;
}

//============================================================================
EXferError BlobXferMgr::txNextBlobChunk( BlobTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked )
{
	if( 0 == xferSession )
	{
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	// fill the packet with data from the file
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( 0 != remoteErr )
	{
		// canceled download by remote user
		LogMsg( LOG_INFO, "FileShareXferMgr:: Cancel Sending file %s", xferInfo.getLclFileName().c_str() );
		onBlobSent( xferSession, xferSession->getBlobInfo(), eXferErrorCanceled, pluginIsLocked );
		return eXferErrorCanceled;
	}

	vx_assert( xferInfo.m_hFile );
	vx_assert( xferInfo.m_u64FileLen );
	if( xferInfo.m_u64FileOffs >= xferInfo.m_u64FileLen )
	{
		//we are done sending file
		if( xferInfo.m_hFile )
		{
			fclose( xferInfo.m_hFile );
			xferInfo.m_hFile  = NULL;
		}

		PktBlobSendCompleteReq oPkt;
		oPkt.setLclSessionId( xferSession->getLclSessionId() );
		oPkt.setRmtSessionId( xferSession->getRmtSessionId() );
		oPkt.setBlobUniqueId( xferSession->getBlobInfo().getAssetUniqueId() );
		m_Plugin.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_ERROR, "BlobXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		onBlobSent( xferSession, xferSession->getBlobInfo(), eXferErrorNone, pluginIsLocked );
		return eXferErrorNone;
	}

	PktBlobChunkReq oPkt;
	// see how much we can read
	uint32_t u32ChunkLen = (uint32_t)(xferInfo.m_u64FileLen - xferInfo.m_u64FileOffs);
	if( PKT_TYPE_BLOB_MAX_DATA_LEN < u32ChunkLen )
	{
		u32ChunkLen = PKT_TYPE_BLOB_MAX_DATA_LEN;
	}

	if( 0 == u32ChunkLen )
	{
		LogMsg( LOG_ERROR, "BlobXferMgr::txNextBlobChunk 0 len u32ChunkLen\n" );
		// what to do?
		return eXferErrorNone;
	}

	// read data into packet
	uint32_t u32BytesRead = (uint32_t)fread(	oPkt.m_au8BlobChunk,
									1,
									u32ChunkLen,
									xferInfo.m_hFile );
	if( u32BytesRead != u32ChunkLen )
	{
		RCODE rc = VxGetLastError();
		xferSession->setErrorCode( rc );
		xferErr = eXferErrorFileReadError;

		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile  = NULL;
		LogMsg( LOG_INFO, "BlobXferMgr: ERROR: %d reading send file at offset %ld when file len %ld file name %s",
					rc,
					xferInfo.m_u64FileOffs,
					xferInfo.m_u64FileLen,
					(const char *)xferInfo.getLclFileName().c_str() );
	}
	else
	{
		xferInfo.m_u64FileOffs += u32ChunkLen;
        oPkt.setChunkLen( (uint16_t)u32ChunkLen );
		oPkt.setLclSessionId( xferInfo.getLclSessionId() );
		oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	}

	if( eXferErrorNone == xferErr )
	{
		if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt ) )
		{
			xferErr = eXferErrorDisconnected;
		}
	}

	if( eXferErrorNone != xferErr )
	{
		IToGui::getToGui().toGuiBlobAction( eAssetActionTxError, xferSession->getBlobInfo().getAssetUniqueId(), xferErr );
	}
	else
	{
		if( xferInfo.calcProgress() )
		{
			IToGui::getToGui().toGuiBlobAction( eAssetActionTxProgress, xferSession->getBlobInfo().getAssetUniqueId(), xferInfo.getProgress() );
		}
	}

	return xferErr;
}

//============================================================================
EXferError BlobXferMgr::rxBlobChunk( bool pluginIsLocked, BlobRxSession * xferSession, PktBlobChunkReq * poPkt )
{
	if( NULL == xferSession )
	{
		return eXferErrorBadParam;
	}

	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	EXferError xferErr = (EXferError)poPkt->getError();
	if( eXferErrorNone != xferErr )
	{
		// canceled by sender
		return xferErr;
	}

	// we are receiving a file
	if( xferInfo.m_hFile )
	{
		//write the chunk of data out to the file
		uint32_t u32BytesWritten = (uint32_t)fwrite(	poPkt->m_au8BlobChunk,
												1,
												poPkt->getChunkLen(),
												xferInfo.m_hFile );
		if( u32BytesWritten != poPkt->getChunkLen() ) 
		{
			RCODE rc = VxGetLastError();
			xferSession->setErrorCode( rc );
			xferErr = eXferErrorFileWriteError;

			LogMsg( LOG_INFO, "VxPktHandler::RxBlobChunk: ERROR %d: writing to file %s",
							rc,
							(const char *)xferInfo.getLclFileName().c_str() );
		}
		else
		{
			//successfully write
			xferInfo.m_u64FileOffs += poPkt->getChunkLen();

			PktBlobChunkReply oPkt;
			oPkt.setDataLen( poPkt->getDataLen() );
			oPkt.setLclSessionId( xferInfo.getLclSessionId() );
			oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );

			if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt ) )
			{
				xferErr = eXferErrorDisconnected;
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		if( xferInfo.calcProgress() )
		{
			IToGui::getToGui().toGuiBlobAction( eAssetActionRxProgress, xferSession->getBlobInfo().getAssetUniqueId(), xferInfo.getProgress() );
		}
	}
	else
	{
		IToGui::getToGui().toGuiBlobAction( eAssetActionRxError, xferSession->getBlobInfo().getAssetUniqueId(), xferErr );
	}

	return xferErr;
}

//============================================================================
void BlobXferMgr::finishBlobReceive( BlobRxSession * xferSession, PktBlobSendCompleteReq * poPkt, bool pluginIsLocked )
{
	// done receiving file
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}
	else
	{
		LogMsg( LOG_ERROR,  "BlobXferMgr::finishBlobReceive: NULL file handle" );
	}

	//// let other act on the received file
	std::string strBlobName = xferInfo.getLclFileName();

	PktBlobSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setBlobUniqueId( xferSession->getBlobInfo().getAssetUniqueId() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strBlobName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onBlobReceived( xferSession, xferSession->getBlobInfo(), (EXferError)poPkt->getError(), pluginIsLocked );
}

//============================================================================
void BlobXferMgr::onBlobReceived( BlobRxSession * xferSession, BlobInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiFileDownloadComplete( xferSession->getLclSessionId(), error );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( eXferErrorNone == error )
	{
		std::string incompleteBlob = xferInfo.getDownloadIncompleteFileName();
		std::string completedBlob = xferInfo.getDownloadCompleteFileName();
		RCODE rc = 0;
		if( 0 == ( rc = VxFileUtil::moveAFile( incompleteBlob.c_str(), completedBlob.c_str() ) ) )
		{
			assetInfo.setAssetName( completedBlob.c_str() );
			assetInfo.setHistoryId( xferSession->getIdent()->getMyOnlineId() );

			if( eXferErrorNone == error )
			{
				assetInfo.setAssetSendState( eAssetSendStateRxSuccess );
			}
			else
			{
				assetInfo.setAssetSendState(  eAssetSendStateRxFail );
			}

			m_BlobMgr.addAsset( assetInfo );
			m_Engine.fromGuiAddFileToLibrary( completedBlob.c_str(), true, xferInfo.getFileHashId().getHashData() );
			if( eXferErrorNone == error )
			{
				IToGui::getToGui().toGuiBlobAction( eAssetActionRxSuccess, xferSession->getBlobInfo().getAssetUniqueId(), 0 );
			}
			else
			{
				IToGui::getToGui().toGuiBlobAction( eAssetActionRxError, xferSession->getBlobInfo().getAssetUniqueId(), error );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "BlobXferMgr::onBlobReceived ERROR %d moving %s to %s\n", rc, incompleteBlob.c_str(), completedBlob.c_str() );
		}
	}

	endBlobXferSession( xferSession, pluginIsLocked );
}

//============================================================================
void BlobXferMgr::onBlobSent( BlobTxSession * xferSession, BlobInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiBlobUploadComplete( xferSession->getRmtSessionId(), error );
	VxSktBase * sktBase		= xferSession->getSkt();
	VxNetIdent * hisIdent	= xferSession->getIdent();
	if( eXferErrorNone != error )
	{
		updateBlobMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, (int)error );
		IToGui::getToGui().toGuiBlobAction( eAssetActionTxError, xferSession->getBlobInfo().getAssetUniqueId(), error );
	}
	else
	{
		updateBlobMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxSuccess, (int)error );
		IToGui::getToGui().toGuiBlobAction( eAssetActionTxSuccess, xferSession->getBlobInfo().getAssetUniqueId(), 0 );
	}

	endBlobXferSession( xferSession, pluginIsLocked, false );
	if( sktBase && sktBase->isConnected() && false == VxIsAppShuttingDown() )
	{
		checkQueForMoreBlobsToSend( pluginIsLocked, hisIdent, sktBase );
	}
}

//============================================================================
void BlobXferMgr::checkQueForMoreBlobsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	// check que and start next xfer
	VxGUID& hisOnlineId = hisIdent->getMyOnlineId();
    std::vector<BlobInfo>::iterator iter;

	m_BlobSendQueMutex.lock();
	for( iter = m_BlobSendQue.begin(); iter != m_BlobSendQue.end(); ++iter )
	{
		if( hisOnlineId == (*iter).getHistoryId() )
		{
			// found asset to send
			BlobInfo& assetInfo = (*iter);
			RCODE rc = createBlobTxSessionAndSend( pluginIsLocked, assetInfo, hisIdent, sktBase );
			if( 0 == rc )
			{
				m_BlobSendQue.erase(iter);
			}

			break;
		}
	}

	m_BlobSendQueMutex.unlock();
}

//============================================================================
void BlobXferMgr::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	BlobTxIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::replaceConnection AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::replaceConnection AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		BlobTxSession * xferSession = (*iter);
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

	BlobRxIter oRxIter;
	for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
	{
		BlobRxSession * xferSession = oRxIter->second;
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "BlobXferMgr::replaceConnection AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void BlobXferMgr::onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	checkQueForMoreBlobsToSend( false, netIdent, sktBase );
}
