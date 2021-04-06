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
#include "ThumbXferMgr.h"
#include "ThumbInfo.h"
#include "ThumbMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "ThumbTxSession.h"
#include "ThumbRxSession.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/AssetBase/AssetBaseMgr.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>

#include <PktLib/PktsAssetXfer.h>
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
	const char * ASSET_XFER_DB_NAME = "ThumbXferDb.db3";

	//============================================================================
    static void * ThumbXferMgrThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		ThumbXferMgr * poMgr = (ThumbXferMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetXferThreadWork( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}


//============================================================================
ThumbXferMgr::ThumbXferMgr( PluginMessenger&		plugin, 
							      PluginSessionMgr&		pluginSessionMgr )
: m_Plugin( plugin )
, m_PluginSessionMgr( pluginSessionMgr )
, m_PluginMgr( plugin.getPluginMgr() )
, m_Engine( plugin.getEngine() )
, m_ThumbMgr( m_Engine.getThumbMgr() )
{
}

//============================================================================
ThumbXferMgr::~ThumbXferMgr()
{
	clearRxSessionsList();
	clearTxSessionsList();
}

//============================================================================
void ThumbXferMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_WorkerThread.startThread( (VX_THREAD_FUNCTION_T)ThumbXferMgrThreadFunc, this, "ThumbXferThrd" );			
	}
}

//============================================================================
void ThumbXferMgr::assetXferThreadWork( VxThread * workThread )
{
	if( workThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += ASSET_XFER_DB_NAME; 
	lockThumbQue();
	m_ThumbXferDb.dbShutdown();
	m_ThumbXferDb.dbStartup( 1, dbName );
	unlockThumbQue();
	if( workThread->isAborted() )
		return;

	std::vector<VxGUID> assetToSendList;
	m_ThumbXferDb.getAllThumbs( assetToSendList );
	if( 0 == assetToSendList.size() )
	{
		// nothing to do
		return;
	}

	while( ( false == m_ThumbMgr.isAssetListInitialized() )
			&& ( false == workThread->isAborted() ) )
	{
		// waiting for assets to be available
		VxSleep( 500 );
	}

	if( workThread->isAborted() )
		return;

	std::vector<VxGUID>::iterator iter; 
	m_ThumbMgr.lockResources();
	lockThumbQue();
	for( iter = assetToSendList.begin(); iter != assetToSendList.end(); ++iter )
	{
		ThumbInfo * assetInfo = dynamic_cast<ThumbInfo *>(m_ThumbMgr.findAsset( *iter ));
		if( assetInfo )
		{
			m_ThumbSendQue.push_back( *assetInfo );
		}
		else
		{
			LogMsg( LOG_ERROR, "assetXferThreadWork removing asset not found in list\n" );
			m_ThumbXferDb.removeThumb( *iter );
		}
	}

	unlockThumbQue();
	m_ThumbMgr.unlockResources();
}

//============================================================================
void ThumbXferMgr::fromGuiCancelDownload( VxGUID& lclSessionId )
{
	std::map<VxGUID, ThumbRxSession *>::iterator iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelDownload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelDownload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		ThumbRxSession * xferSession = iter->second;
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_RxSessions.erase( iter );
			xferSession->cancelDownload( lclSessionId );
			delete xferSession;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelDownload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::fromGuiCancelUpload( VxGUID& lclSessionId )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelUpload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelUpload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	ThumbTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		ThumbTxSession * xferSession = ( *iter );
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( lclSessionId );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}


//============================================================================
void ThumbXferMgr::clearRxSessionsList( void )
{
	std::map<VxGUID, ThumbRxSession *>::iterator iter;

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::clearRxSessionsList AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::clearRxSessionsList AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		ThumbRxSession * xferSession = iter->second;
		delete xferSession;
	}

	m_RxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::clearRxSessionsList AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::clearTxSessionsList( void )
{
	ThumbTxIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		ThumbTxSession * xferSession = (*iter);
		delete xferSession;
	}

	m_TxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::fileAboutToBeDeleted( std::string& fileName )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fileAboutToBeDeleted AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fileAboutToBeDeleted AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	ThumbTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		ThumbTxSession * xferSession = ( *iter );
		if( xferSession->getXferInfo().getLclFileName() == fileName )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( xferSession->getXferInfo().getLclSessionId() );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "ThumbXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::onConnectionLost( VxSktBase * sktBase )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onConnectionLost AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onConnectionLost AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	bool erasedSession = true;
	ThumbTxIter iter;
	while( erasedSession )
	{
		erasedSession = false;
		for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
		{
			ThumbTxSession * xferSession = ( *iter );
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
	ThumbRxIter oRxIter; 
	while( erasedSession )
	{
		erasedSession = false;
		for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
		{
			ThumbRxSession * xferSession = oRxIter->second;
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
	LogMsg( LOG_INFO, "ThumbXferMgr::onConnectionLost AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
bool ThumbXferMgr::requireFileXfer( EAssetType assetType )
{
	return ( ( eAssetTypePhoto == assetType )
		|| ( eAssetTypePhoto == assetType )
		|| ( eAssetTypeVideo == assetType )
		|| ( eAssetTypeDocument == assetType )
		|| ( eAssetTypeArchives == assetType )
		|| ( eAssetTypeExe == assetType )
		|| ( eAssetTypeOtherFiles == assetType ) );

}

//============================================================================
void ThumbXferMgr::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock start\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock done\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK

	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq\n");
	PktAssetSendReq * poPkt = (PktAssetSendReq *)pktHdr;
	VxGUID& assetUniqueId = poPkt->getUniqueId();
	EAssetType assetType = (EAssetType)poPkt->getAssetType();
	bool needFileXfer = requireFileXfer( assetType );
	PktAssetSendReply pktReply;
	pktReply.setRequiresFileXfer( needFileXfer );
	pktReply.setError( 0 );
	pktReply.setRmtSessionId( poPkt->getLclSessionId() );
	pktReply.setLclSessionId( poPkt->getRmtSessionId() );
	pktReply.setUniqueId( assetUniqueId );
	if( false == netIdent->isHisAccessAllowedFromMe( m_Plugin.getPluginType() ) )
	{
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq: permission denied\n" );
		pktReply.setError( eXferErrorPermission );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	if( false == needFileXfer )
	{
		// all we need is in the send request
		ThumbInfo assetInfo;
		poPkt->fillAssetFromPkt( assetInfo );
		// make history id his id
		assetInfo.setHistoryId( netIdent->getMyOnlineId() );
		assetInfo.setAssetSendState( eAssetSendStateRxSuccess );
		m_ThumbMgr.addAsset( assetInfo );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		IToGui::getToGui().toGuiAssetAction( eAssetActionRxSuccess, assetInfo.getAssetUniqueId(), 100 );
		IToGui::getToGui().toGuiAssetAction( eAssetActionRxNotifyNewMsg, assetInfo.getCreatorId(), 100 );
	}
	else
	{
		ThumbRxSession * xferSession = findOrCreateRxSession( true, poPkt->getRmtSessionId(), netIdent, sktBase );
		if( xferSession )
		{
			ThumbInfo& assetInfo = xferSession->getAssetInfo();
			poPkt->fillAssetFromPkt( assetInfo );
			// make history id his id
			assetInfo.setHistoryId( netIdent->getMyOnlineId() );
			assetInfo.setAssetSendState( eAssetSendStateRxProgress );

			xferSession->setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setLclSessionId( xferSession->getLclSessionId() );
			EXferError xferErr = beginThumbReceive( xferSession, poPkt, pktReply );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateThumbDownload( xferSession->getLclSessionId(), 0, rc );
				endThumbXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg(LOG_ERROR, "PluginThumbOffer::onPktAssetSendReq: Could not create session\n");
			PktAssetSendReply pktReply;
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::assetSendComplete( ThumbTxSession * xferSession )
{
	updateThumbMgrSendState( xferSession->getAssetInfo().getAssetUniqueId(), eAssetSendStateTxSuccess, 100 );
}

//============================================================================
void ThumbXferMgr::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReply\n");
	PktAssetSendReply * poPkt = (PktAssetSendReply *)pktHdr;
	VxGUID&	assetUniqueId =	poPkt->getUniqueId();
	ThumbInfo * assetInfo = dynamic_cast<ThumbInfo *>(m_ThumbMgr.findAsset( assetUniqueId ));
	if( 0 == assetInfo )
	{
		LogMsg( LOG_ERROR, "ThumbXferMgr::onPktAssetSendReply failed to find asset id\n");
		updateThumbMgrSendState( assetUniqueId, eAssetSendStateTxFail, 0 );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	bool isFileXfer = (bool)poPkt->getRequiresFileXfer();
	uint32_t rxedErrCode = poPkt->getError();
	ThumbTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );

	if( xferSession )
	{
		xferSession->setRmtSessionId( poPkt->getLclSessionId() );
		if( 0 == rxedErrCode )
		{
			if( isFileXfer )
			{
				// we did txNextThumbChunk in begin file send
				//RCODE rc = txNextThumbChunk( xferSession );
				//if( rc )
				//{
				//	//IToGui::getToGui().toGuiUpdateThumbUpload( xferSession->getLclSessionId(), 0, rc );
				//	LogMsg( LOG_ERROR, "ThumbXferMgr::onPktAssetSendReply beginThumbSend returned error %d\n", rc );
				//	endThumbXferSession( xferSession, true );
				//}
			}
			else
			{
				assetSendComplete( xferSession );
				endThumbXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::onPktAssetSendReply PktAssetSendReply returned error %d\n", poPkt->getError() );
			endThumbXferSession( xferSession, true, false );
			updateThumbMgrSendState( assetUniqueId, eAssetSendStateTxFail, rxedErrCode );
		}
	}
	else
	{
		if( isFileXfer )
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::onPktAssetSendReply failed to find session\n");
			updateThumbMgrSendState( assetUniqueId, eAssetSendStateTxFail, rxedErrCode );
		}
		else
		{
			updateThumbMgrSendState( assetUniqueId, rxedErrCode ? eAssetSendStateTxFail : eAssetSendStateTxSuccess, rxedErrCode );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	ThumbRxSession * xferSession = 0;
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetChunkReq\n");
	PktAssetChunkReq * poPkt = (PktAssetChunkReq *)pktHdr;
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetChunkReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetChunkReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findRxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = rxThumbChunk( true, xferSession, poPkt );
			if( eXferErrorNone != xferErr )
			{

				PktAssetChunkReply pktReply;
				pktReply.setLclSessionId( xferSession->getLclSessionId() );
				pktReply.setRmtSessionId( poPkt->getLclSessionId() );
				pktReply.setDataLen(0);
				pktReply.setError( xferErr );
				m_Plugin.txPacket( netIdent, sktBase, &pktReply );

				IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, xferSession->getAssetInfo().getAssetUniqueId(), xferErr );
				endThumbXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::onPktAssetChunkReq failed to find session\n");
			PktAssetChunkReply pktReply;
			pktReply.setLclSessionId( poPkt->getRmtSessionId() );
			pktReply.setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setDataLen(0);
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void ThumbXferMgr::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktAssetChunkReply * poPkt = (PktAssetChunkReply *)pktHdr;
	ThumbTxSession * xferSession = 0;
static int cnt = 0;
	cnt++;
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktThumbChuckReply start %d\n", cnt );
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetChunkReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetChunkReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findTxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = txNextThumbChunk( xferSession, poPkt->getError(), true );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateThumbUpload( xferSession->getLclSessionId(), 0, rc );
				endThumbXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::onPktThumbChuckReply failed to find session\n" );
		}

		LogMsg( LOG_INFO, "ThumbXferMgr::onPktThumbChuckReply done %d\n", cnt );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetChunkReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void ThumbXferMgr::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReq\n");
	PktAssetSendCompleteReq * poPkt = (PktAssetSendCompleteReq *)pktHdr;
	ThumbRxSession * xferSession = findRxSession( true, poPkt->getRmtSessionId() );
	//TODO check checksum
	if( xferSession )
	{
		finishThumbReceive( xferSession, poPkt, true );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReply\n");
	PktAssetSendCompleteReply * poPkt = (PktAssetSendCompleteReply *)pktHdr;
	ThumbTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );
	if( xferSession )
	{
		VxFileXferInfo xferInfo = xferSession->getXferInfo();
		LogMsg( LOG_INFO, "ThumbXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		//m_PluginMgr.getToGui().toGuiThumbUploadComplete( xferInfo.getLclSessionId(), 0 );
		onThumbSent( xferSession, xferSession->getAssetInfo(), (EXferError)poPkt->getError(), true );
	}
	else
	{
		LogMsg( LOG_ERROR, "ThumbXferMgr::onPktAssetSendCompleteReply failed to find session\n");
		updateThumbMgrSendState( poPkt->getAssetUniqueId(), eAssetSendStateTxSuccess, 100 );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktAssetSendCompleteReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::onPktThumbXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktThumbXferErr\n");
	// TODO handle error
}

//============================================================================
void ThumbXferMgr::endThumbXferSession( ThumbRxSession * poSessionIn, bool pluginIsLocked )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::endThumbXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::endThumbXferSession pluginMutex.lock done\n");
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

	ThumbRxIter oRxIter = m_RxSessions.begin();
	while( oRxIter != m_RxSessions.end() )
	{
		ThumbRxSession * xferSession = oRxIter->second;
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
		LogMsg( LOG_INFO, "ThumbXferMgr::endThumbXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
void ThumbXferMgr::endThumbXferSession( ThumbTxSession * poSessionIn, bool pluginIsLocked, bool requeThumb )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::endThumbXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::endThumbXferSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	//if( requeThumb )
	//{
	//	queThumb( poSessionIn->getAssetInfo() );
	//}

	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	ThumbTxIter iter = m_TxSessions.begin();
	while( iter != m_TxSessions.end() )
	{
		ThumbTxSession * xferSession = (*iter);
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
		LogMsg( LOG_INFO, "ThumbXferMgr::endThumbXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
ThumbRxSession * ThumbXferMgr::findRxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbRxIter iter;
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		ThumbRxSession * xferSession = iter->second;
		if( xferSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return  xferSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
ThumbRxSession * ThumbXferMgr::findRxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbRxIter iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		ThumbRxSession * rxSession = iter->second;
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return rxSession;
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
ThumbRxSession *	ThumbXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbRxSession * xferSession = findRxSession( true, netIdent );
	if( NULL == xferSession )
	{
		xferSession = new ThumbRxSession( m_Engine, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
ThumbRxSession *	ThumbXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	if( false == lclSessionId.isVxGUIDValid() )
	{
		lclSessionId.initializeWithNewVxGUID();
	}

	ThumbRxSession * xferSession = findRxSession( true, lclSessionId );
	if( NULL == xferSession )
	{
		xferSession = new ThumbRxSession( m_Engine, lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
ThumbTxSession * ThumbXferMgr::findTxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		ThumbTxSession * txSession = ( *iter );
		if( txSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
ThumbTxSession * ThumbXferMgr::findTxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		ThumbTxSession * txSession = ( *iter );
		if( txSession->getLclSessionId() == lclSessionId )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
ThumbTxSession *	ThumbXferMgr::createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	ThumbTxSession * txSession = new ThumbTxSession( m_Engine, sktBase, netIdent );
	return txSession;
}

//============================================================================
ThumbTxSession *	ThumbXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbTxSession * xferSession = findTxSession( true, netIdent );
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
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
ThumbTxSession *	 ThumbXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	ThumbTxSession * xferSession = 0;
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
		xferSession = new ThumbTxSession( m_Engine, lclSessionId, sktBase, netIdent );
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
		LogMsg( LOG_INFO, "ThumbXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
void ThumbXferMgr::fromGuiSendThumb( ThumbInfo& assetInfo )
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
			EXferError xferError = createThumbTxSessionAndSend( false, assetInfo, hisInfo, sktBase );
			if( xferError == eXferErrorNone )
			{
				xferFailed = false;
			}
		}
		else
		{
			queThumb( assetInfo );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "ThumbXferMgr::fromGuiSendThumb NetIdent not found\n" );
	}

	if( xferFailed )
	{
		onTxFailed( assetInfo.getAssetUniqueId(), false );
	}
}

//============================================================================
void ThumbXferMgr::onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked )
{
	updateThumbMgrSendState( assetUniqueId, eAssetSendStateTxFail, 0 );
}

//============================================================================
void ThumbXferMgr::onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked )
{
	updateThumbMgrSendState( assetUniqueId, eAssetSendStateTxSuccess, 0 );
}

//============================================================================
void ThumbXferMgr::updateThumbMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param )
{
	m_ThumbMgr.updateAssetXferState( assetUniqueId, sendState, param );
}

//============================================================================
void ThumbXferMgr::queThumb( ThumbInfo& assetInfo )
{
	m_ThumbSendQueMutex.lock();
	bool foundThumb = false;
    std::vector<ThumbInfo>::iterator iter;
	for( iter = m_ThumbSendQue.begin(); iter != m_ThumbSendQue.end(); ++iter )
	{
		if( (*iter).getAssetUniqueId() == assetInfo.getAssetUniqueId() )
		{
			foundThumb = true;
			break;
		}
	}

	if( false == foundThumb )
	{
		m_ThumbSendQue.push_back( assetInfo );
	}

	m_ThumbSendQueMutex.unlock();
}

//============================================================================
EXferError ThumbXferMgr::createThumbTxSessionAndSend( bool pluginIsLocked, ThumbInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::createThumbTxSessionAndSend pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::createThumbTxSessionAndSend pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	EXferError xferErr = eXferErrorNone;
	ThumbTxSession * txSession = createTxSession( hisIdent, sktBase );
	if( false == txSession->getLclSessionId().isVxGUIDValid() )
	{
		txSession->getLclSessionId().initializeWithNewVxGUID();
	}

	if( false == txSession->getRmtSessionId().isVxGUIDValid() )
	{
		txSession->setRmtSessionId( txSession->getLclSessionId() );
	}

	txSession->setThumbInfo( assetInfo );
	VxFileXferInfo& xferInfo = txSession->getXferInfo();
	xferInfo.setLclSessionId( txSession->getLclSessionId() );
	xferInfo.setRmtSessionId( txSession->getRmtSessionId() );
	xferInfo.setXferDirection( eXferDirectionTx );

	m_TxSessionsMutex.lock();
	m_TxSessions.push_back( txSession );
	m_TxSessionsMutex.unlock();

	updateThumbMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxProgress, 0 );
	if( assetInfo.hasFileName() )
	{
		// need to do first so file handle is set before get asset send reply back
		xferErr = beginThumbSend( txSession );
	}
	else
	{
		// all data was in the request packet .. we just wait for reply
	}

	if( eXferErrorNone != xferErr )
	{
		// failed to open file
		updateThumbMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, xferErr );
		endThumbXferSession( txSession, true, false );
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "ThumbXferMgr::createThumbTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return xferErr;
	}

	PktAssetSendReq sendReq;
	sendReq.fillPktFromAsset( assetInfo );
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
			xferErr = txNextThumbChunk( txSession, eXferErrorNone, true );
		}
	}

	if( eXferErrorNone != xferErr )
	{
		// re que for try some other time
		updateThumbMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, xferErr );
		endThumbXferSession( txSession, true, ( ( eXferErrorFileNotFound == xferErr ) || ( eXferErrorDisconnected == xferErr ) ) ? false : true );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "ThumbXferMgr::createThumbTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferErr;
}

//============================================================================
EXferError ThumbXferMgr::beginThumbSend( ThumbTxSession * xferSession )
{
	EXferError xferErr = eXferErrorNone;
	xferSession->clearErrorCode();
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "ThumbXferMgr::beginThumbSend: ERROR: Thumb transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	if( eXferErrorNone == xferErr )
	{
		xferInfo.setXferDirection( eXferDirectionTx );
		xferInfo.setLclFileName( xferSession->getAssetInfo().getAssetName().c_str() );
		VxFileUtil::getJustFileName( xferSession->getAssetInfo().getAssetName().c_str(), xferInfo.getRmtFileName() );
		xferInfo.setLclSessionId( xferSession->getLclSessionId() );
		xferInfo.setRmtSessionId( xferSession->getRmtSessionId() );
		xferInfo.setFileHashId( xferSession->getFileHashId() );

		xferInfo.m_u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 == xferInfo.m_u64FileLen )
		{
			// no file found to send
			LogMsg( LOG_INFO, "ThumbXferMgr::beginThumbSend: Thumb %s not found to send", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileNotFound;
		}
		else if( false == xferInfo.getFileHashId().isHashValid() )
		{
			// see if we can get hash from shared files
			//if( !m_SharedThumbsMgr.getAssetHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
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
			LogMsg( LOG_INFO, "ThumbXferMgr::beginThumbSend: Could not open Thumb %s", xferInfo.getLclFileName().c_str() );
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
				LogMsg( LOG_INFO, "ThumbXferMgr::beginThumbSend: Thumb %s could not be resumed because too short", 
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
					LogMsg( LOG_INFO, "ThumbXferMgr::beginThumbSend: could not seek to position %d in file %s",
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
EXferError ThumbXferMgr::beginThumbReceive( ThumbRxSession * xferSession, PktAssetSendReq * poPkt, PktAssetSendReply& pktReply )
{
	if( NULL == xferSession )
	{
		LogMsg( LOG_ERROR, "ThumbXferMgr::beginThumbReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	uint64_t u64FileLen;
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( poPkt->getError() )
	{
		//IToGui::getToGui().toGuiUpdateThumbDownload( poPkt->getRmtSessionId(), 0, poPkt->getError() );
		xferErr = (EXferError)poPkt->getError();
		return xferErr;
	}

	if( eXferErrorNone == xferErr )
	{
		if( xferInfo.m_hFile )
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::beginThumbReceive: ERROR:(Thumb Receive) receive transfer still in progress" );
			xferErr = eXferErrorAlreadyDownloading;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		// get file information
		xferInfo.setFileHashId( poPkt->getAssetHashId() );
		xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
		if( false == xferInfo.getLclSessionId().isVxGUIDValid() )
		{
			xferInfo.getLclSessionId().initializeWithNewVxGUID();
		}

		xferInfo.setRmtFileName( poPkt->getAssetName().c_str() );
		if( 0 == xferInfo.getRmtFileName().length() )
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::beginThumbReceive: ERROR: No file Name\n" );
			xferErr = eXferErrorBadParam;
		}	
	}

	std::string strRmtPath;
	std::string strRmtThumbNameOnly;
    VxFileUtil::seperatePathAndFile(		xferInfo.getRmtFileName(),
                                            strRmtPath,
                                            strRmtThumbNameOnly );


	if( eXferErrorNone == xferErr )
	{
		// make full path
		if( 0 == strRmtThumbNameOnly.length() )
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::beginThumbReceive: ERROR: NULL file Name %s\n",  xferInfo.getRmtFileName().c_str() );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		VxFileUtil::makeFullPath( strRmtThumbNameOnly.c_str(), VxGetIncompleteDirectory().c_str(), xferInfo.getLclFileName() );
		std::string strPath;
		std::string strThumbNameOnly;
		RCODE rc = VxFileUtil::seperatePathAndFile(	xferInfo.getLclFileName(),			
													strPath,			
													strThumbNameOnly );	
		VxFileUtil::makeDirectory( strPath );
		xferInfo.m_u64FileLen = poPkt->getAssetLen();
		xferInfo.m_u64FileOffs = poPkt->getAssetOffset();
		u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );

		if( 0 != xferInfo.m_u64FileOffs )
		{
			if( u64FileLen < xferInfo.m_u64FileOffs )
			{
				xferErr  = eXferErrorFileSeekError;
				LogMsg( LOG_INFO, "ThumbXferMgr: ERROR:(Thumb Send) %d Thumb %s could not be resumed because too short", 
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

					LogMsg( LOG_INFO, "ThumbXferMgr: ERROR:(Thumb Send) %d Thumb %s could not be created", 
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
						LogMsg( LOG_INFO, "ThumbXferMgr: ERROR: (Thumb Send) could not seek to position %d in file %s",
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

				LogMsg( LOG_INFO, "ThumbXferMgr: ERROR: %d Thumb %s could not be created", 
					rc,
					(const char *)xferInfo.getLclFileName().c_str() );
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_INFO, "ThumbXferMgr::(Thumb Send) start recieving file %s\n", 
			(const char *)xferInfo.getLclFileName().c_str() );
		poPkt->fillAssetFromPkt( xferSession->getAssetInfo() );
	}

	pktReply.setError( xferErr );
	pktReply.setAssetOffset( xferInfo.m_u64FileOffs );
	if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &pktReply ) )
	{
		xferErr = eXferErrorDisconnected;
	}

	return xferErr;
}

//============================================================================
EXferError ThumbXferMgr::txNextThumbChunk( ThumbTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked )
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
		onThumbSent( xferSession, xferSession->getAssetInfo(), eXferErrorCanceled, pluginIsLocked );
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

		PktAssetSendCompleteReq oPkt;
		oPkt.setLclSessionId( xferSession->getLclSessionId() );
		oPkt.setRmtSessionId( xferSession->getRmtSessionId() );
		oPkt.setAssetUniqueId( xferSession->getAssetInfo().getAssetUniqueId() );
		m_Plugin.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_ERROR, "ThumbXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		onThumbSent( xferSession, xferSession->getAssetInfo(), eXferErrorNone, pluginIsLocked );
		return eXferErrorNone;
	}

	PktAssetChunkReq oPkt;
	// see how much we can read
	uint32_t u32ChunkLen = (uint32_t)(xferInfo.m_u64FileLen - xferInfo.m_u64FileOffs);
	if( PKT_TYPE_ASSET_MAX_DATA_LEN < u32ChunkLen )
	{
		u32ChunkLen = PKT_TYPE_ASSET_MAX_DATA_LEN;
	}

	if( 0 == u32ChunkLen )
	{
		LogMsg( LOG_ERROR, "ThumbXferMgr::txNextThumbChunk 0 len u32ChunkLen\n" );
		// what to do?
		return eXferErrorNone;
	}

	// read data into packet
	uint32_t u32BytesRead = (uint32_t)fread(	oPkt.m_au8AssetChunk,
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
		LogMsg( LOG_INFO, "ThumbXferMgr: ERROR: %d reading send file at offset %ld when file len %ld file name %s",
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
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxError, xferSession->getAssetInfo().getAssetUniqueId(), xferErr );
	}
	else
	{
		if( xferInfo.calcProgress() )
		{
			IToGui::getToGui().toGuiAssetAction( eAssetActionTxProgress, xferSession->getAssetInfo().getAssetUniqueId(), xferInfo.getProgress() );
		}
	}

	return xferErr;
}

//============================================================================
EXferError ThumbXferMgr::rxThumbChunk( bool pluginIsLocked, ThumbRxSession * xferSession, PktAssetChunkReq * poPkt )
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
		uint32_t u32BytesWritten = (uint32_t)fwrite(	poPkt->m_au8AssetChunk,
												1,
												poPkt->getChunkLen(),
												xferInfo.m_hFile );
		if( u32BytesWritten != poPkt->getChunkLen() ) 
		{
			RCODE rc = VxGetLastError();
			xferSession->setErrorCode( rc );
			xferErr = eXferErrorFileWriteError;

			LogMsg( LOG_INFO, "VxPktHandler::RxThumbChunk: ERROR %d: writing to file %s",
							rc,
							(const char *)xferInfo.getLclFileName().c_str() );
		}
		else
		{
			//successfully write
			xferInfo.m_u64FileOffs += poPkt->getChunkLen();

			PktAssetChunkReply oPkt;
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
			IToGui::getToGui().toGuiAssetAction( eAssetActionRxProgress, xferSession->getAssetInfo().getAssetUniqueId(), xferInfo.getProgress() );
		}
	}
	else
	{
		IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, xferSession->getAssetInfo().getAssetUniqueId(), xferErr );
	}

	return xferErr;
}

//============================================================================
void ThumbXferMgr::finishThumbReceive( ThumbRxSession * xferSession, PktAssetSendCompleteReq * poPkt, bool pluginIsLocked )
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
		LogMsg( LOG_ERROR,  "ThumbXferMgr::finishThumbReceive: NULL file handle" );
	}

	//// let other act on the received file
	std::string strThumbName = xferInfo.getLclFileName();

	PktAssetSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setAssetUniqueId( xferSession->getAssetInfo().getAssetUniqueId() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strThumbName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onThumbReceived( xferSession, xferSession->getAssetInfo(), (EXferError)poPkt->getError(), pluginIsLocked );
}

//============================================================================
void ThumbXferMgr::onThumbReceived( ThumbRxSession * xferSession, ThumbInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiFileDownloadComplete( xferSession->getLclSessionId(), error );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( eXferErrorNone == error )
	{
		std::string incompleteThumb = xferInfo.getDownloadIncompleteFileName();
		std::string completedThumb = xferInfo.getDownloadCompleteFileName();
		RCODE rc = 0;
		if( 0 == ( rc = VxFileUtil::moveAFile( incompleteThumb.c_str(), completedThumb.c_str() ) ) )
		{
			assetInfo.setAssetName( completedThumb.c_str() );
			assetInfo.setHistoryId( xferSession->getIdent()->getMyOnlineId() );

			if( eXferErrorNone == error )
			{
				assetInfo.setAssetSendState( eAssetSendStateRxSuccess );
			}
			else
			{
				assetInfo.setAssetSendState(  eAssetSendStateRxFail );
			}

			m_ThumbMgr.addAsset( assetInfo );
			m_Engine.fromGuiAddFileToLibrary( completedThumb.c_str(), true, xferInfo.getFileHashId().getHashData() );
			if( eXferErrorNone == error )
			{
				IToGui::getToGui().toGuiAssetAction( eAssetActionRxSuccess, xferSession->getAssetInfo().getAssetUniqueId(), 0 );
			}
			else
			{
				IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, xferSession->getAssetInfo().getAssetUniqueId(), error );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "ThumbXferMgr::onThumbReceived ERROR %d moving %s to %s\n", rc, incompleteThumb.c_str(), completedThumb.c_str() );
		}
	}

	endThumbXferSession( xferSession, pluginIsLocked );
}

//============================================================================
void ThumbXferMgr::onThumbSent( ThumbTxSession * xferSession, ThumbInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiThumbUploadComplete( xferSession->getRmtSessionId(), error );
	VxSktBase * sktBase		= xferSession->getSkt();
	VxNetIdent * hisIdent	= xferSession->getIdent();
	if( eXferErrorNone != error )
	{
		updateThumbMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, (int)error );
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxError, xferSession->getAssetInfo().getAssetUniqueId(), error );
	}
	else
	{
		updateThumbMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxSuccess, (int)error );
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxSuccess, xferSession->getAssetInfo().getAssetUniqueId(), 0 );
	}

	endThumbXferSession( xferSession, pluginIsLocked, false );
	if( sktBase && sktBase->isConnected() && false == VxIsAppShuttingDown() )
	{
		checkQueForMoreThumbsToSend( pluginIsLocked, hisIdent, sktBase );
	}
}

//============================================================================
void ThumbXferMgr::checkQueForMoreThumbsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	// check que and start next xfer
	VxGUID& hisOnlineId = hisIdent->getMyOnlineId();
    std::vector<ThumbInfo>::iterator iter;

	m_ThumbSendQueMutex.lock();
	for( iter = m_ThumbSendQue.begin(); iter != m_ThumbSendQue.end(); ++iter )
	{
		if( hisOnlineId == (*iter).getHistoryId() )
		{
			// found asset to send
			ThumbInfo& assetInfo = (*iter);
			RCODE rc = createThumbTxSessionAndSend( pluginIsLocked, assetInfo, hisIdent, sktBase );
			if( 0 == rc )
			{
				m_ThumbSendQue.erase(iter);
			}

			break;
		}
	}

	m_ThumbSendQueMutex.unlock();
}

//============================================================================
void ThumbXferMgr::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	ThumbTxIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::replaceConnection AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::replaceConnection AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		ThumbTxSession * xferSession = (*iter);
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

	ThumbRxIter oRxIter;
	for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
	{
		ThumbRxSession * xferSession = oRxIter->second;
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "ThumbXferMgr::replaceConnection AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void ThumbXferMgr::onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	checkQueForMoreThumbsToSend( false, netIdent, sktBase );
}
