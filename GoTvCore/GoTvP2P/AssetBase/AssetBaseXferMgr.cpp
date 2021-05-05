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
#include "AssetBaseXferMgr.h"
#include "AssetBaseInfo.h"
#include "AssetBaseMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "AssetBaseTxSession.h"
#include "AssetBaseRxSession.h"

#include <GuiInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
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
	//============================================================================
    static void * AssetBaseXferMgrThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		AssetBaseXferMgr * poMgr = (AssetBaseXferMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetXferThreadWork( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}


//============================================================================
AssetBaseXferMgr::AssetBaseXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName )
: m_Engine( engine )
, m_AssetBaseMgr( assetMgr )
, m_XferInterface( xferInterface )
, m_PluginMgr( engine.getPluginMgr() )
, m_AssetBaseXferDb( stateDbName )
, m_WorkerThreadName( workThreadName )
{
}

//============================================================================
AssetBaseXferMgr::~AssetBaseXferMgr()
{
	clearRxSessionsList();
	clearTxSessionsList();
}

//============================================================================
void AssetBaseXferMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_WorkerThread.startThread( (VX_THREAD_FUNCTION_T)AssetBaseXferMgrThreadFunc, this, m_WorkerThreadName.c_str() );			
	}
}

//============================================================================
void AssetBaseXferMgr::assetXferThreadWork( VxThread * workThread )
{
	if( workThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += m_AssetBaseXferDb.getDatabaseName(); 
	lockAssetBaseQue();
	m_AssetBaseXferDb.dbShutdown();
	m_AssetBaseXferDb.dbStartup( 1, dbName );
	unlockAssetBaseQue();
	if( workThread->isAborted() )
		return;

	std::vector<VxGUID> assetToSendList;
	m_AssetBaseXferDb.getAllAssets( assetToSendList );
	if( 0 == assetToSendList.size() )
	{
		// nothing to do
		return;
	}

	while( ( false == m_AssetBaseMgr.isAssetListInitialized() )
			&& ( false == workThread->isAborted() ) )
	{
		// waiting for assets to be available
		VxSleep( 500 );
	}

	if( workThread->isAborted() )
		return;

	std::vector<VxGUID>::iterator iter; 
	m_AssetBaseMgr.lockResources();
	lockAssetBaseQue();
	for( iter = assetToSendList.begin(); iter != assetToSendList.end(); ++iter )
	{
		AssetBaseInfo * assetInfo = m_AssetBaseMgr.findAsset( *iter );
		if( assetInfo )
		{
			m_AssetBaseSendQue.push_back( *assetInfo );
		}
		else
		{
			LogMsg( LOG_ERROR, "assetXferThreadWork removing asset not found in list\n" );
			m_AssetBaseXferDb.removeAsset( *iter );
		}
	}

	unlockAssetBaseQue();
	m_AssetBaseMgr.unlockResources();
}

//============================================================================
void AssetBaseXferMgr::fromGuiCancelDownload( VxGUID& lclSessionId )
{
	std::map<VxGUID, AssetBaseRxSession *>::iterator iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelDownload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelDownload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		AssetBaseRxSession * xferSession = iter->second;
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_RxSessions.erase( iter );
			xferSession->cancelDownload( lclSessionId );
			delete xferSession;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelDownload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::fromGuiCancelUpload( VxGUID& lclSessionId )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelUpload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelUpload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		AssetBaseTxSession * xferSession = ( *iter );
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( lclSessionId );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}


//============================================================================
void AssetBaseXferMgr::clearRxSessionsList( void )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::clearRxSessionsList AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::clearRxSessionsList AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( auto iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		AssetBaseRxSession * xferSession = iter->second;
		delete xferSession;
	}

	m_RxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::clearRxSessionsList AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::clearTxSessionsList( void )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		AssetBaseTxSession * xferSession = (*iter);
		delete xferSession;
	}

	m_TxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::fileAboutToBeDeleted( std::string& fileName )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fileAboutToBeDeleted AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fileAboutToBeDeleted AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		AssetBaseTxSession * xferSession = ( *iter );
		if( xferSession->getXferInfo().getLclFileName() == fileName )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( xferSession->getXferInfo().getLclSessionId() );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "AssetBaseXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::onConnectionLost( VxSktBase * sktBase )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onConnectionLost AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onConnectionLost AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	bool erasedSession = true;
	while( erasedSession )
	{
		erasedSession = false;
		for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
		{
			AssetBaseTxSession * xferSession = ( *iter );
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
	while( erasedSession )
	{
		erasedSession = false;
		for( auto oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
		{
			AssetBaseRxSession * xferSession = oRxIter->second;
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
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onConnectionLost AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
bool AssetBaseXferMgr::requireFileXfer( EAssetType assetType )
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
void AssetBaseXferMgr::onPktAssetBaseGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetBaseGetReq AutoPluginLock start");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetBaseGetReq AutoPluginLock done");
#endif // DEBUG_AUTOPLUGIN_LOCK

    EXferError xferErr = eXferErrorNone;
    PktBaseGetReq * pktGetReq = (PktBaseGetReq *)pktHdr;

    VxGUID assetUniqueId = pktGetReq->getUniqueId();
    EAssetType assetType = (EAssetType)pktGetReq->getAssetType();
    VxGUID rmtSessionId = pktGetReq->getLclSessionId();
    VxGUID lclSessionId;
    lclSessionId.initializeWithNewVxGUID();
    int64_t startOffs = pktGetReq->getStartOffset();
    int64_t	endOffs = pktGetReq->getEndOffset();	//if 0 then get all


    PktBaseGetReply pktReply;
    pktReply.setAssetType( pktGetReq->getAssetType() );
    pktReply.setUniqueId( pktGetReq->getUniqueId() );
    pktReply.setFileHashId( pktGetReq->getFileHashId() );
    pktReply.setLclSessionId( lclSessionId );
    pktReply.setRmtSessionId( rmtSessionId );
    pktReply.setStartOffset( startOffs );

    if( false == netIdent->isHisAccessAllowedFromMe( m_XferInterface.getPluginType() ) )
    {      
        pktReply.setError( eXferErrorPermission );
        m_XferInterface.txPacket( netIdent, sktBase,  &pktReply );
        return;
    }

    if( !pktGetReq->isValidPkt() || !assetUniqueId.isVxGUIDValid() )
    {
        pktReply.setError( eXferErrorBadParam );
        m_XferInterface.txPacket( netIdent, sktBase,  &pktReply );
        return;
    }

    AssetBaseInfo* assetInfo = m_AssetBaseMgr.findAsset( assetUniqueId );
    if( !assetInfo )
    {
        pktReply.setError( eXferErrorFileNotFound );
        m_XferInterface.txPacket( netIdent, sktBase,  &pktReply );
        return;
    }

    if( !m_AssetBaseMgr.doesAssetExist( *assetInfo ) )
    {
        pktReply.setError( eXferErrorFileNotFound );
        m_XferInterface.txPacket( netIdent, sktBase,  &pktReply );
        return;
    }

    pktReply.setEndOffset( assetInfo->getAssetLength() );
    AssetBaseTxSession* xferSession = findOrCreateTxSession( false, lclSessionId, netIdent, sktBase );
    if( !xferSession )
    {
        pktReply.setError( eXferErrorBusy );
        m_XferInterface.txPacket( netIdent, sktBase,  &pktReply );
        return;
    }

    std::string lclFileName = assetInfo->getAssetName();
    std::string rmtFileName;
    std::string rmtPath;
    RCODE rc = VxFileUtil::seperatePathAndFile( lclFileName, rmtPath, rmtFileName );
    if( rmtFileName.empty() )
    {
        LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetBaseGetReq Asset File Name Error" );
        pktReply.setError( eXferErrorFileNotFound );
        m_XferInterface.txPacket( netIdent, sktBase,  &pktReply );
        return;
    }

    VxFileXferInfo& xferInfo = xferSession->getXferInfo();
    xferInfo.setRmtSessionId( lclSessionId );
    xferInfo.setFileHashId( pktGetReq->getFileHashId() );
    xferInfo.setFileOffset( pktGetReq->getStartOffset() );
    xferInfo.setLclFileName( lclFileName.c_str() );
    xferInfo.setRmtFileName(  rmtFileName.c_str() );
    m_TxSessions.push_back( xferSession );

    xferErr  = ( m_XferInterface.txPacket( netIdent, sktBase, &pktReply ) ) ? eXferErrorNone : eXferErrorDisconnected;
    if( eXferErrorNone == xferErr )
    {
        xferErr = beginAssetBaseSend( xferSession );
    }

    if( eXferErrorNone != xferErr )
    {
        onAssetBaseUploadError( netIdent, *assetInfo, xferErr );
        endAssetBaseXferSession( xferSession, true, false );
    }
    else
    {
        onAssetBaseBeginUpload( netIdent, *assetInfo );
    }
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock start\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock done\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK

	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq\n");
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
	if( false == netIdent->isHisAccessAllowedFromMe( m_XferInterface.getPluginType() ) )
	{
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq: permission denied\n" );
		pktReply.setError( eXferErrorPermission );
        m_XferInterface.txPacket( netIdent, sktBase, &pktReply );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	if( false == needFileXfer )
	{
		// all we need is in the send request
		AssetBaseInfo assetInfo;
		poPkt->fillAssetFromPkt( assetInfo );
		// make history id his id
		assetInfo.setHistoryId( netIdent->getMyOnlineId() );
		assetInfo.setAssetSendState( eAssetSendStateRxSuccess );
		m_AssetBaseMgr.addAsset( assetInfo );
		m_XferInterface.txPacket( netIdent, sktBase, &pktReply );
		IToGui::getToGui().toGuiAssetAction( eAssetActionRxSuccess, assetInfo.getAssetUniqueId(), 100 );
		IToGui::getToGui().toGuiAssetAction( eAssetActionRxNotifyNewMsg, assetInfo.getCreatorId(), 100 );
	}
	else
	{
		AssetBaseRxSession * xferSession = findOrCreateRxSession( true, poPkt->getRmtSessionId(), netIdent, sktBase );
		if( xferSession )
		{
			AssetBaseInfo& assetInfo = xferSession->getAssetBaseInfo();
			poPkt->fillAssetFromPkt( assetInfo );
			// make history id his id
			assetInfo.setHistoryId( netIdent->getMyOnlineId() );
			assetInfo.setAssetSendState( eAssetSendStateRxProgress );

			xferSession->setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setLclSessionId( xferSession->getLclSessionId() );
			EXferError xferErr = beginAssetBaseReceive( xferSession, poPkt, pktReply );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateAssetDownload( xferSession->getLclSessionId(), 0, rc );
				endAssetBaseXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg(LOG_ERROR, "PluginAssetBaseOffer::onPktAssetSendReq: Could not create session\n");
			PktAssetSendReply pktReply;
			pktReply.setError( eXferErrorBadParam );
			m_XferInterface.txPacket( netIdent, sktBase, &pktReply );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::assetSendComplete( AssetBaseTxSession * xferSession )
{
	updateAssetMgrSendState( xferSession->getAssetBaseInfo().getAssetUniqueId(), eAssetSendStateTxSuccess, 100 );
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReply\n");
	PktAssetSendReply * poPkt = (PktAssetSendReply *)pktHdr;
	VxGUID&	assetUniqueId =	poPkt->getUniqueId();
	AssetBaseInfo * assetInfo = m_AssetBaseMgr.findAsset( assetUniqueId );
	if( 0 == assetInfo )
	{
		LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetSendReply failed to find asset id\n");
		updateAssetMgrSendState( assetUniqueId, eAssetSendStateTxFail, 0 );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	bool isFileXfer = (bool)poPkt->getRequiresFileXfer();
	uint32_t rxedErrCode = poPkt->getError();
	AssetBaseTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );

	if( xferSession )
	{
		xferSession->setRmtSessionId( poPkt->getLclSessionId() );
		if( 0 == rxedErrCode )
		{
			if( isFileXfer )
			{
				// we did txNextAssetBaseChunk in begin file send
				//RCODE rc = txNextAssetBaseChunk( xferSession );
				//if( rc )
				//{
				//	//IToGui::getToGui().toGuiUpdateAssetUpload( xferSession->getLclSessionId(), 0, rc );
				//	LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetSendReply beginAssetBaseSend returned error %d\n", rc );
				//	endAssetBaseXferSession( xferSession, true );
				//}
			}
			else
			{
				assetSendComplete( xferSession );
				endAssetBaseXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetSendReply PktAssetSendReply returned error %d\n", poPkt->getError() );
			endAssetBaseXferSession( xferSession, true, false );
			updateAssetMgrSendState( assetUniqueId, eAssetSendStateTxFail, rxedErrCode );
		}
	}
	else
	{
		if( isFileXfer )
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetSendReply failed to find session\n");
			updateAssetMgrSendState( assetUniqueId, eAssetSendStateTxFail, rxedErrCode );
		}
		else
		{
			updateAssetMgrSendState( assetUniqueId, rxedErrCode ? eAssetSendStateTxFail : eAssetSendStateTxSuccess, rxedErrCode );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	AssetBaseRxSession * xferSession = 0;
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetChunkReq\n");
	PktAssetChunkReq * poPkt = (PktAssetChunkReq *)pktHdr;
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetChunkReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
        AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetChunkReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findRxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = rxAssetBaseChunk( true, xferSession, poPkt );
			if( eXferErrorNone != xferErr )
			{

				PktAssetChunkReply pktReply;
				pktReply.setLclSessionId( xferSession->getLclSessionId() );
				pktReply.setRmtSessionId( poPkt->getLclSessionId() );
				pktReply.setDataLen(0);
				pktReply.setError( xferErr );
				m_XferInterface.txPacket( netIdent, sktBase, &pktReply );

				IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, xferSession->getAssetBaseInfo().getAssetUniqueId(), xferErr );
				endAssetBaseXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetChunkReq failed to find session\n");
			PktAssetChunkReply pktReply;
			pktReply.setLclSessionId( poPkt->getRmtSessionId() );
			pktReply.setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setDataLen(0);
			pktReply.setError( eXferErrorBadParam );
			m_XferInterface.txPacket( netIdent, sktBase, &pktReply );
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktAssetChunkReply * poPkt = (PktAssetChunkReply *)pktHdr;
	AssetBaseTxSession * xferSession = 0;
static int cnt = 0;
	cnt++;
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetBaseChuckReply start %d\n", cnt );
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetChunkReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
        AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetChunkReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findTxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = txNextAssetBaseChunk( xferSession, poPkt->getError(), true );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateAssetUpload( xferSession->getLclSessionId(), 0, rc );
				endAssetBaseXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetBaseChuckReply failed to find session\n" );
		}

		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetBaseChuckReply done %d\n", cnt );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetChunkReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReq\n");
	PktAssetSendCompleteReq * poPkt = (PktAssetSendCompleteReq *)pktHdr;
	AssetBaseRxSession * xferSession = findRxSession( true, poPkt->getRmtSessionId() );
	//TODO check checksum
	if( xferSession )
	{
		finishAssetBaseReceive( xferSession, poPkt, true );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReply\n");
	PktAssetSendCompleteReply * poPkt = (PktAssetSendCompleteReply *)pktHdr;
	AssetBaseTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );
	if( xferSession )
	{
		VxFileXferInfo xferInfo = xferSession->getXferInfo();
		LogMsg( LOG_INFO, "AssetBaseXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		//m_PluginMgr.getToGui().toGuiAssetBaseUploadComplete( xferInfo.getLclSessionId(), 0 );
		onAssetBaseSent( xferSession, xferSession->getAssetBaseInfo(), (EXferError)poPkt->getError(), true );
	}
	else
	{
		LogMsg( LOG_ERROR, "AssetBaseXferMgr::onPktAssetSendCompleteReply failed to find session\n");
		updateAssetMgrSendState( poPkt->getAssetUniqueId(), eAssetSendStateTxSuccess, 100 );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetSendCompleteReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::onPktAssetBaseXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AssetBaseXferMgr::onPktAssetBaseXferErr\n");
	// TODO handle error
}

//============================================================================
void AssetBaseXferMgr::endAssetBaseXferSession( AssetBaseRxSession * poSessionIn, bool pluginIsLocked )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::endAssetBaseXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::endAssetBaseXferSession pluginMutex.lock done\n");
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

    auto rxIter = m_RxSessions.begin();
	while( rxIter != m_RxSessions.end() )
	{
		AssetBaseRxSession * xferSession = rxIter->second;
		if( poSessionIn == xferSession )
		{
			m_RxSessions.erase( rxIter );
			delete xferSession;
			break;
		}
		else
		{
			++rxIter;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::endAssetBaseXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
void AssetBaseXferMgr::endAssetBaseXferSession( AssetBaseTxSession * poSessionIn, bool pluginIsLocked, bool requeAsset )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::endAssetBaseXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::endAssetBaseXferSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	//if( requeAsset )
	//{
	//	queAsset( poSessionIn->getAssetBaseInfo() );
	//}

	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	auto iter = m_TxSessions.begin();
	while( iter != m_TxSessions.end() )
	{
		AssetBaseTxSession * xferSession = (*iter);
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
		LogMsg( LOG_INFO, "AssetBaseXferMgr::endAssetBaseXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
AssetBaseRxSession * AssetBaseXferMgr::findRxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		AssetBaseRxSession * xferSession = iter->second;
		if( xferSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return  xferSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
AssetBaseRxSession * AssetBaseXferMgr::findRxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	auto iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		AssetBaseRxSession * rxSession = iter->second;
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return rxSession;
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
AssetBaseRxSession *	AssetBaseXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	AssetBaseRxSession * xferSession = findRxSession( true, netIdent );
	if( NULL == xferSession )
	{
		xferSession = new AssetBaseRxSession( m_Engine, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
AssetBaseRxSession *	AssetBaseXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	if( false == lclSessionId.isVxGUIDValid() )
	{
		lclSessionId.initializeWithNewVxGUID();
	}

	AssetBaseRxSession * xferSession = findRxSession( true, lclSessionId );
	if( NULL == xferSession )
	{
		xferSession = new AssetBaseRxSession( m_Engine, lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
AssetBaseTxSession * AssetBaseXferMgr::findTxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		AssetBaseTxSession * txSession = ( *iter );
		if( txSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
AssetBaseTxSession * AssetBaseXferMgr::findTxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		AssetBaseTxSession * txSession = ( *iter );
		if( txSession->getLclSessionId() == lclSessionId )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
AssetBaseTxSession *	AssetBaseXferMgr::createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	AssetBaseTxSession * txSession = new AssetBaseTxSession( m_Engine, sktBase, netIdent );
	return txSession;
}

//============================================================================
AssetBaseTxSession *	AssetBaseXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	AssetBaseTxSession * xferSession = findTxSession( true, netIdent );
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
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
AssetBaseTxSession* AssetBaseXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	AssetBaseTxSession * xferSession = 0;
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
		xferSession = new AssetBaseTxSession( m_Engine, lclSessionId, sktBase, netIdent );
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
		LogMsg( LOG_INFO, "AssetBaseXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
bool AssetBaseXferMgr::fromGuiSendAssetBase( AssetBaseInfo& assetInfo )
{
	bool xferFailed = true;
	BigListInfo * hisInfo = m_Engine.getBigListMgr().findBigListInfo( assetInfo.getHistoryId() );
	if( hisInfo )
	{
		// first try to connect and send.. if that fails then que and will send when next connected
		VxSktBase * sktBase = 0;
		m_PluginMgr.pluginApiSktConnectTo( m_XferInterface.getPluginType(), hisInfo, 0, &sktBase );
		if( 0 != sktBase )
		{
			EXferError xferError = createAssetTxSessionAndSend( false, assetInfo, hisInfo, sktBase );
			if( xferError == eXferErrorNone )
			{
				xferFailed = false;
			}
		}
		else
		{
			queAsset( assetInfo );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "AssetBaseXferMgr::fromGuiSendAssetBase NetIdent not found\n" );
	}

	if( xferFailed )
	{
		onTxFailed( assetInfo.getAssetUniqueId(), false );
	}

    return !xferFailed;
}

//============================================================================
bool AssetBaseXferMgr::fromGuiRequestAssetBase( AssetBaseInfo& assetInfo )
{
    VxNetIdent* netIdent = m_Engine.getBigListMgr().findBigListInfo( assetInfo.getOnlineId() );
    if( netIdent )
    {
        return fromGuiRequestAssetBase( netIdent, assetInfo );
    }

    return false;
}

//============================================================================
bool AssetBaseXferMgr::fromGuiRequestAssetBase( VxNetIdent * netIdent, AssetBaseInfo& assetInfo )
{
    if( !netIdent || !assetInfo.getAssetUniqueId().isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "fromGuiRequestAssetBase invalid param" );
        vx_assert( false );
        return false;
    }

    bool xferFailed = true;
    if( netIdent )
    {
        // first try to connect and send.. if that fails then que and will send when next connected
        VxSktBase * sktBase = 0;
        m_PluginMgr.pluginApiSktConnectTo( m_XferInterface.getPluginType(), netIdent, 0, &sktBase );
        if( sktBase )
        {
            EXferError xferError = createAssetRxSessionAndReceive( false, assetInfo, netIdent, sktBase );
            if( xferError == eXferErrorNone )
            {
                xferFailed = false;
            }
        }
        else
        {
            queAsset( assetInfo );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "AssetBaseXferMgr::fromGuiSendAssetBase NetIdent not found\n" );
    }

    if( xferFailed )
    {
        onTxFailed( assetInfo.getAssetUniqueId(), false );
    }

    return !xferFailed;

}

//============================================================================
void AssetBaseXferMgr::onTxFailed( VxGUID& assetUniqueId, bool pluginIsLocked )
{
	updateAssetMgrSendState( assetUniqueId, eAssetSendStateTxFail, 0 );
}

//============================================================================
void AssetBaseXferMgr::onTxSuccess( VxGUID& assetUniqueId, bool pluginIsLocked )
{
	updateAssetMgrSendState( assetUniqueId, eAssetSendStateTxSuccess, 0 );
}

//============================================================================
void AssetBaseXferMgr::updateAssetMgrSendState( VxGUID& assetUniqueId, EAssetSendState sendState, int param )
{
	m_AssetBaseMgr.updateAssetXferState( assetUniqueId, sendState, param );
}

//============================================================================
void AssetBaseXferMgr::queAsset( AssetBaseInfo& assetInfo )
{
	m_AssetBaseSendQueMutex.lock();
	bool foundAssetBase = false;
    std::vector<AssetBaseInfo>::iterator iter;
	for( iter = m_AssetBaseSendQue.begin(); iter != m_AssetBaseSendQue.end(); ++iter )
	{
		if( (*iter).getAssetUniqueId() == assetInfo.getAssetUniqueId() )
		{
			foundAssetBase = true;
			break;
		}
	}

	if( false == foundAssetBase )
	{
		m_AssetBaseSendQue.push_back( assetInfo );
	}

	m_AssetBaseSendQueMutex.unlock();
}

//============================================================================
EXferError AssetBaseXferMgr::createAssetTxSessionAndSend( bool pluginIsLocked, AssetBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_XferInterface.getAssetXferMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::createAssetTxSessionAndSend pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::createAssetTxSessionAndSend pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	EXferError xferErr = eXferErrorNone;
	AssetBaseTxSession * txSession = createTxSession( hisIdent, sktBase );
	if( false == txSession->getLclSessionId().isVxGUIDValid() )
	{
		txSession->getLclSessionId().initializeWithNewVxGUID();
	}

	if( false == txSession->getRmtSessionId().isVxGUIDValid() )
	{
		txSession->setRmtSessionId( txSession->getLclSessionId() );
	}

	txSession->setAssetBaseInfo( assetInfo );
	VxFileXferInfo& xferInfo = txSession->getXferInfo();
	xferInfo.setLclSessionId( txSession->getLclSessionId() );
	xferInfo.setRmtSessionId( txSession->getRmtSessionId() );
	xferInfo.setXferDirection( eXferDirectionTx );

	m_TxSessionsMutex.lock();
	m_TxSessions.push_back( txSession );
	m_TxSessionsMutex.unlock();

	updateAssetMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxProgress, 0 );
	if( assetInfo.hasFileName() )
	{
		// need to do first so file handle is set before get asset send reply back
		xferErr = beginAssetBaseSend( txSession );
	}
	else
	{
		// all data was in the request packet .. we just wait for reply
	}

	if( eXferErrorNone != xferErr )
	{
		// failed to open file
		updateAssetMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, xferErr );
		endAssetBaseXferSession( txSession, true, false );
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "AssetBaseXferMgr::createAssetTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return xferErr;
	}

	PktAssetSendReq sendReq;
	sendReq.fillPktFromAsset( assetInfo );
	sendReq.setLclSessionId( xferInfo.getLclSessionId() );
	sendReq.setRmtSessionId( xferInfo.getRmtSessionId() );
	if( false == m_PluginMgr.pluginApiTxPacket( m_XferInterface.getPluginType(), hisIdent->getMyOnlineId(), sktBase, &sendReq ) )
	{
		xferErr = eXferErrorDisconnected;
	}	

	if( eXferErrorNone == xferErr )
	{
		// re que for try some other time
		if( requireFileXfer( assetInfo.getAssetType() ) )
		{
			xferErr = txNextAssetBaseChunk( txSession, eXferErrorNone, true );
		}
	}

	if( eXferErrorNone != xferErr )
	{
		// re que for try some other time
		updateAssetMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, xferErr );
		endAssetBaseXferSession( txSession, true, ( ( eXferErrorFileNotFound == xferErr ) || ( eXferErrorDisconnected == xferErr ) ) ? false : true );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "AssetBaseXferMgr::createAssetTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferErr;
}

//============================================================================
EXferError AssetBaseXferMgr::createAssetRxSessionAndReceive( bool pluginIsLocked, AssetBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
    EXferError xferErr = eXferErrorNone;
    PktAssetGetReq pktReq;
    pktReq.setAssetType( assetInfo.getAssetType() );
    pktReq.setUniqueId( assetInfo.getAssetUniqueId() );
    pktReq.getLclSessionId().initializeWithNewVxGUID();
    if( !m_XferInterface.txPacket(  hisIdent, sktBase, &pktReq ) )
    {
        xferErr = eXferErrorDisconnected;
    }

    return xferErr;
}

//============================================================================
EXferError AssetBaseXferMgr::beginAssetBaseSend( AssetBaseTxSession * xferSession )
{
	EXferError xferErr = eXferErrorNone;
	xferSession->clearErrorCode();
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "AssetBaseXferMgr::beginAssetBaseSend: ERROR: AssetBase transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	if( eXferErrorNone == xferErr )
	{
		xferInfo.setXferDirection( eXferDirectionTx );
		xferInfo.setLclFileName( xferSession->getAssetBaseInfo().getAssetName().c_str() );
		VxFileUtil::getJustFileName( xferSession->getAssetBaseInfo().getAssetName().c_str(), xferInfo.getRmtFileName() );
		xferInfo.setLclSessionId( xferSession->getLclSessionId() );
		xferInfo.setRmtSessionId( xferSession->getRmtSessionId() );
		xferInfo.setFileHashId( xferSession->getFileHashId() );

		xferInfo.m_u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 == xferInfo.m_u64FileLen )
		{
			// no file found to send
			LogMsg( LOG_INFO, "AssetBaseXferMgr::beginAssetBaseSend: AssetBase %s not found to send", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileNotFound;
		}
		else if( false == xferInfo.getFileHashId().isHashValid() )
		{
			// see if we can get hash from shared files
			//if( !m_SharedAssetBasesMgr.getAssetHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
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
			LogMsg( LOG_INFO, "AssetBaseXferMgr::beginAssetBaseSend: Could not open AssetBase %s", xferInfo.getLclFileName().c_str() );
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
				LogMsg( LOG_INFO, "AssetBaseXferMgr::beginAssetBaseSend: AssetBase %s could not be resumed because too short", 
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
					LogMsg( LOG_INFO, "AssetBaseXferMgr::beginAssetBaseSend: could not seek to position %d in file %s",
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
EXferError AssetBaseXferMgr::beginAssetBaseReceive( AssetBaseRxSession * xferSession, PktAssetSendReq * poPkt, PktAssetSendReply& pktReply )
{
	if( NULL == xferSession )
	{
		LogMsg( LOG_ERROR, "AssetBaseXferMgr::beginAssetBaseReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	uint64_t u64FileLen;
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( poPkt->getError() )
	{
		//IToGui::getToGui().toGuiUpdateAssetDownload( poPkt->getRmtSessionId(), 0, poPkt->getError() );
		xferErr = (EXferError)poPkt->getError();
		return xferErr;
	}

	if( eXferErrorNone == xferErr )
	{
		if( xferInfo.m_hFile )
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::beginAssetBaseReceive: ERROR:(AssetBase Receive) receive transfer still in progress" );
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
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::beginAssetBaseReceive: ERROR: No file Name\n" );
			xferErr = eXferErrorBadParam;
		}	
	}

	std::string strRmtPath;
	std::string strRmtAssetBaseNameOnly;
    VxFileUtil::seperatePathAndFile(		xferInfo.getRmtFileName(),
                                            strRmtPath,
                                            strRmtAssetBaseNameOnly );
	if( eXferErrorNone == xferErr )
	{
		// make full path
		if( 0 == strRmtAssetBaseNameOnly.length() )
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::beginAssetBaseReceive: ERROR: NULL file Name %s\n",  xferInfo.getRmtFileName().c_str() );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		VxFileUtil::makeFullPath( strRmtAssetBaseNameOnly.c_str(), VxGetIncompleteDirectory().c_str(), xferInfo.getLclFileName() );
		std::string strPath;
		std::string strAssetBaseNameOnly;
		RCODE rc = VxFileUtil::seperatePathAndFile(	xferInfo.getLclFileName(),			
													strPath,			
													strAssetBaseNameOnly );	
		VxFileUtil::makeDirectory( strPath );
		xferInfo.m_u64FileLen = poPkt->getAssetLen();
		xferInfo.m_u64FileOffs = poPkt->getAssetOffset();
		u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );

		if( 0 != xferInfo.m_u64FileOffs )
		{
			if( u64FileLen < xferInfo.m_u64FileOffs )
			{
				xferErr  = eXferErrorFileSeekError;
				LogMsg( LOG_INFO, "AssetBaseXferMgr: ERROR:(AssetBase Send) %d AssetBase %s could not be resumed because too short", 
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

					LogMsg( LOG_INFO, "AssetBaseXferMgr: ERROR:(AssetBase Send) %d AssetBase %s could not be created", 
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
						LogMsg( LOG_INFO, "AssetBaseXferMgr: ERROR: (AssetBase Send) could not seek to position %d in file %s",
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

				LogMsg( LOG_INFO, "AssetBaseXferMgr: ERROR: %d AssetBase %s could not be created", 
					rc,
					(const char *)xferInfo.getLclFileName().c_str() );
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_INFO, "AssetBaseXferMgr::(AssetBase Send) start recieving file %s\n", 
			(const char *)xferInfo.getLclFileName().c_str() );
		poPkt->fillAssetFromPkt( xferSession->getAssetBaseInfo() );
	}

	pktReply.setError( xferErr );
	pktReply.setAssetOffset( xferInfo.m_u64FileOffs );
	if( false == m_XferInterface.txPacket( xferSession->getIdent(), xferSession->getSkt(), &pktReply ) )
	{
		xferErr = eXferErrorDisconnected;
	}

	return xferErr;
}

//============================================================================
EXferError AssetBaseXferMgr::txNextAssetBaseChunk( AssetBaseTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked )
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
		onAssetBaseSent( xferSession, xferSession->getAssetBaseInfo(), eXferErrorCanceled, pluginIsLocked );
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
		oPkt.setAssetUniqueId( xferSession->getAssetBaseInfo().getAssetUniqueId() );
		m_XferInterface.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_ERROR, "AssetBaseXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		onAssetBaseSent( xferSession, xferSession->getAssetBaseInfo(), eXferErrorNone, pluginIsLocked );
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
		LogMsg( LOG_ERROR, "AssetBaseXferMgr::txNextAssetBaseChunk 0 len u32ChunkLen\n" );
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
		LogMsg( LOG_INFO, "AssetBaseXferMgr: ERROR: %d reading send file at offset %ld when file len %ld file name %s",
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
		if( false == m_XferInterface.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt ) )
		{
			xferErr = eXferErrorDisconnected;
		}
	}

	if( eXferErrorNone != xferErr )
	{
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxError, xferSession->getAssetBaseInfo().getAssetUniqueId(), xferErr );
	}
	else
	{
		if( xferInfo.calcProgress() )
		{
			IToGui::getToGui().toGuiAssetAction( eAssetActionTxProgress, xferSession->getAssetBaseInfo().getAssetUniqueId(), xferInfo.getProgress() );
		}
	}

	return xferErr;
}

//============================================================================
EXferError AssetBaseXferMgr::rxAssetBaseChunk( bool pluginIsLocked, AssetBaseRxSession * xferSession, PktAssetChunkReq * poPkt )
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

			LogMsg( LOG_INFO, "VxPktHandler::RxAssetBaseChunk: ERROR %d: writing to file %s",
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

			if( false == m_XferInterface.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt ) )
			{
				xferErr = eXferErrorDisconnected;
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		if( xferInfo.calcProgress() )
		{
			IToGui::getToGui().toGuiAssetAction( eAssetActionRxProgress, xferSession->getAssetBaseInfo().getAssetUniqueId(), xferInfo.getProgress() );
		}
	}
	else
	{
		IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, xferSession->getAssetBaseInfo().getAssetUniqueId(), xferErr );
	}

	return xferErr;
}

//============================================================================
void AssetBaseXferMgr::finishAssetBaseReceive( AssetBaseRxSession * xferSession, PktAssetSendCompleteReq * poPkt, bool pluginIsLocked )
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
		LogMsg( LOG_ERROR,  "AssetBaseXferMgr::finishAssetBaseReceive: NULL file handle" );
	}

	//// let other act on the received file
	std::string strAssetBaseName = xferInfo.getLclFileName();

	PktAssetSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setAssetUniqueId( xferSession->getAssetBaseInfo().getAssetUniqueId() );
	m_XferInterface.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strAssetBaseName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onAssetBaseReceived( xferSession, xferSession->getAssetBaseInfo(), (EXferError)poPkt->getError(), pluginIsLocked );
}

//============================================================================
void AssetBaseXferMgr::onAssetBaseReceived( AssetBaseRxSession * xferSession, AssetBaseInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiFileDownloadComplete( xferSession->getLclSessionId(), error );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( eXferErrorNone == error )
	{
		std::string incompleteAsset = xferInfo.getDownloadIncompleteFileName();
		std::string completedAssetBase = xferInfo.getDownloadCompleteFileName();
		RCODE rc = 0;
		if( 0 == ( rc = VxFileUtil::moveAFile( incompleteAsset.c_str(), completedAssetBase.c_str() ) ) )
		{
			assetInfo.setAssetName( completedAssetBase.c_str() );
			assetInfo.setHistoryId( xferSession->getIdent()->getMyOnlineId() );

			if( eXferErrorNone == error )
			{
				assetInfo.setAssetSendState( eAssetSendStateRxSuccess );
			}
			else
			{
				assetInfo.setAssetSendState(  eAssetSendStateRxFail );
			}

			m_AssetBaseMgr.addAsset( assetInfo );
			m_Engine.fromGuiAddFileToLibrary( completedAssetBase.c_str(), true, xferInfo.getFileHashId().getHashData() );
			if( eXferErrorNone == error )
			{
				IToGui::getToGui().toGuiAssetAction( eAssetActionRxSuccess, xferSession->getAssetBaseInfo().getAssetUniqueId(), 0 );
			}
			else
			{
				IToGui::getToGui().toGuiAssetAction( eAssetActionRxError, xferSession->getAssetBaseInfo().getAssetUniqueId(), error );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "AssetBaseXferMgr::onAssetBaseReceived ERROR %d moving %s to %s\n", rc, incompleteAsset.c_str(), completedAssetBase.c_str() );
		}
	}

	endAssetBaseXferSession( xferSession, pluginIsLocked );
}

//============================================================================
void AssetBaseXferMgr::onAssetBaseSent( AssetBaseTxSession * xferSession, AssetBaseInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiAssetBaseUploadComplete( xferSession->getRmtSessionId(), error );
	VxSktBase * sktBase		= xferSession->getSkt();
	VxNetIdent * hisIdent	= xferSession->getIdent();
	if( eXferErrorNone != error )
	{
		updateAssetMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxFail, (int)error );
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxError, xferSession->getAssetBaseInfo().getAssetUniqueId(), error );
	}
	else
	{
		updateAssetMgrSendState( assetInfo.getAssetUniqueId(), eAssetSendStateTxSuccess, (int)error );
		IToGui::getToGui().toGuiAssetAction( eAssetActionTxSuccess, xferSession->getAssetBaseInfo().getAssetUniqueId(), 0 );
	}

	endAssetBaseXferSession( xferSession, pluginIsLocked, false );
	if( sktBase && sktBase->isConnected() && false == VxIsAppShuttingDown() )
	{
		checkQueForMoreAssetsToSend( pluginIsLocked, hisIdent, sktBase );
	}
}

//============================================================================
void AssetBaseXferMgr::checkQueForMoreAssetsToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	// check que and start next xfer
	VxGUID& hisOnlineId = hisIdent->getMyOnlineId();
    std::vector<AssetBaseInfo>::iterator iter;

	m_AssetBaseSendQueMutex.lock();
	for( iter = m_AssetBaseSendQue.begin(); iter != m_AssetBaseSendQue.end(); ++iter )
	{
		if( hisOnlineId == (*iter).getHistoryId() )
		{
			// found asset to send
			AssetBaseInfo& assetInfo = (*iter);
			RCODE rc = createAssetTxSessionAndSend( pluginIsLocked, assetInfo, hisIdent, sktBase );
			if( 0 == rc )
			{
				m_AssetBaseSendQue.erase(iter);
			}

			break;
		}
	}

	m_AssetBaseSendQueMutex.unlock();
}

//============================================================================
void AssetBaseXferMgr::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::replaceConnection AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
    AutoXferLock pluginMutexLock( m_XferInterface.getAssetXferMutex() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::replaceConnection AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( auto iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		AssetBaseTxSession * xferSession = (*iter);
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

	for( auto oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
	{
		AssetBaseRxSession * xferSession = oRxIter->second;
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "AssetBaseXferMgr::replaceConnection AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void AssetBaseXferMgr::onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	checkQueForMoreAssetsToSend( false, netIdent, sktBase );
}
