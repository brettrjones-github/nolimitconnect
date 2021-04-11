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

#include <config_gotvcore.h>
#include "OfferBaseXferMgr.h"
#include "OfferBaseInfo.h"
#include "OfferBaseMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "OfferBaseTxSession.h"
#include "OfferBaseRxSession.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/OfferBase/OfferBaseInfo.h>
#include <GoTvCore/GoTvP2P/OfferBase/OfferBaseRxSession.h>
#include <GoTvCore/GoTvP2P/OfferBase/OfferBaseTxSession.h>

#include <PktLib/PktsOfferXfer.h>
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
	//#define		MAX_OFFER_XFER_OUTSTANDING_PKTS 3
	//#define		MAX_OFFER_XFER_TX_SESSIONS		5;
	const char * OFFER_XFER_DB_NAME = "OfferBaseXferDb.db3";

	//============================================================================
    static void * OfferBaseXferMgrThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		OfferBaseXferMgr * poMgr = (OfferBaseXferMgr *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->assetXferThreadWork( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}


//============================================================================
OfferBaseXferMgr::OfferBaseXferMgr( P2PEngine& engine, OfferBaseMgr& offerMgr, PluginMessenger&	plugin, PluginSessionMgr& pluginSessionMgr, EOfferMgrType offerMgrType )
: m_Engine( engine )
, m_OfferMgr( offerMgr )
, m_PluginMgr( engine.getPluginMgr() )
, m_Plugin( plugin )
, m_PluginSessionMgr( pluginSessionMgr )
{
}

//============================================================================
OfferBaseXferMgr::~OfferBaseXferMgr()
{
	clearRxSessionsList();
	clearTxSessionsList();
}

//============================================================================
void OfferBaseXferMgr::fromGuiUserLoggedOn( void )
{
	if( !m_Initialized )
	{
		m_Initialized = true;
		m_WorkerThread.startThread( (VX_THREAD_FUNCTION_T)OfferBaseXferMgrThreadFunc, this, "OfferBaseXferThrd" );			
	}
}

//============================================================================
void OfferBaseXferMgr::assetXferThreadWork( VxThread * workThread )
{
	if( workThread->isAborted() )
		return;
	// user specific directory should be set
	std::string dbName = VxGetSettingsDirectory();
	dbName += OFFER_XFER_DB_NAME; 
	lockOfferBaseQue();
	m_OfferBaseXferDb.dbShutdown();
	m_OfferBaseXferDb.dbStartup( 1, dbName );
	unlockOfferBaseQue();
	if( workThread->isAborted() )
		return;

	std::vector<VxGUID> assetToSendList;
	m_OfferBaseXferDb.getAllOffers( assetToSendList );
	if( 0 == assetToSendList.size() )
	{
		// nothing to do
		return;
	}

	while( ( false == m_OfferMgr.isOfferListInitialized() )
			&& ( false == workThread->isAborted() ) )
	{
		// waiting for assets to be available
		VxSleep( 500 );
	}

	if( workThread->isAborted() )
		return;

	std::vector<VxGUID>::iterator iter; 
	m_OfferMgr.lockResources();
	lockOfferBaseQue();
	for( iter = assetToSendList.begin(); iter != assetToSendList.end(); ++iter )
	{
		OfferBaseInfo * assetInfo = m_OfferMgr.findOffer( *iter );
		if( assetInfo )
		{
			m_OfferBaseSendQue.push_back( *assetInfo );
		}
		else
		{
			LogMsg( LOG_ERROR, "assetXferThreadWork removing asset not found in list\n" );
			m_OfferBaseXferDb.removeOffer( *iter );
		}
	}

	unlockOfferBaseQue();
	m_OfferMgr.unlockResources();
}

//============================================================================
void OfferBaseXferMgr::fromGuiCancelDownload( VxGUID& lclSessionId )
{
	std::map<VxGUID, OfferBaseRxSession *>::iterator iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelDownload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelDownload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		OfferBaseRxSession * xferSession = iter->second;
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_RxSessions.erase( iter );
			xferSession->cancelDownload( lclSessionId );
			delete xferSession;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelDownload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::fromGuiCancelUpload( VxGUID& lclSessionId )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelUpload AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelUpload AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	OfferBaseTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		OfferBaseTxSession * xferSession = ( *iter );
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( lclSessionId );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fromGuiCancelUpload AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}


//============================================================================
void OfferBaseXferMgr::clearRxSessionsList( void )
{
	std::map<VxGUID, OfferBaseRxSession *>::iterator iter;

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::clearRxSessionsList AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::clearRxSessionsList AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		OfferBaseRxSession * xferSession = iter->second;
		delete xferSession;
	}

	m_RxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::clearRxSessionsList AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::clearTxSessionsList( void )
{
	OfferBaseTxIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		OfferBaseTxSession * xferSession = (*iter);
		delete xferSession;
	}

	m_TxSessions.clear();
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::fileAboutToBeDeleted( std::string& fileName )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fileAboutToBeDeleted AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fileAboutToBeDeleted AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	OfferBaseTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		OfferBaseTxSession * xferSession = ( *iter );
		if( xferSession->getXferInfo().getLclFileName() == fileName )
		{
			m_TxSessions.erase( iter );
			xferSession->cancelUpload( xferSession->getXferInfo().getLclSessionId() );
			delete xferSession;
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "OfferBaseXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			return;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::fileAboutToBeDeleted AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::onConnectionLost( VxSktBase * sktBase )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onConnectionLost AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onConnectionLost AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	bool erasedSession = true;
	OfferBaseTxIter iter;
	while( erasedSession )
	{
		erasedSession = false;
		for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
		{
			OfferBaseTxSession * xferSession = ( *iter );
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
	OfferBaseRxIter oRxIter; 
	while( erasedSession )
	{
		erasedSession = false;
		for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
		{
			OfferBaseRxSession * xferSession = oRxIter->second;
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
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onConnectionLost AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
bool OfferBaseXferMgr::requireFileXfer( EOfferType assetType )
{
	return ( ( eOfferTypePhotoFile == assetType )
		|| ( eOfferTypeAudioFile == assetType )
		|| ( eOfferTypeVideoFile == assetType )
		|| ( eOfferTypeDocumentFile == assetType )
		|| ( eOfferTypeArchiveFile == assetType )
		|| ( eOfferTypeExecutableFile == assetType )
		|| ( eOfferTypeOtherFile == assetType ) );

}

//============================================================================
void OfferBaseXferMgr::onPktOfferSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock start\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock done\n");
	#endif // DEBUG_AUTOPLUGIN_LOCK

	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq\n");
	PktOfferSendReq * poPkt = (PktOfferSendReq *)pktHdr;
	VxGUID& assetOfferId = poPkt->getOfferId();
	EOfferType assetType = (EOfferType)poPkt->getOfferType();
	bool needFileXfer = requireFileXfer( assetType );
	PktOfferSendReply pktReply;
	pktReply.setRequiresFileXfer( needFileXfer );
	pktReply.setError( 0 );
	pktReply.setRmtSessionId( poPkt->getLclSessionId() );
	pktReply.setLclSessionId( poPkt->getRmtSessionId() );
	pktReply.setOfferId( assetOfferId );
	if( false == netIdent->isHisAccessAllowedFromMe( m_Plugin.getPluginType() ) )
	{
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq: permission denied\n" );
		pktReply.setError( eXferErrorPermission );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	if( false == needFileXfer )
	{
		// all we need is in the send request
		OfferBaseInfo assetInfo;
		poPkt->fillOfferFromPkt( assetInfo );
		// make history id his id
		assetInfo.setHistoryId( netIdent->getMyOnlineId() );
		assetInfo.setOfferSendState( eOfferSendStateRxSuccess );
		m_OfferMgr.addOffer( assetInfo );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		m_OfferMgr.announceOfferAction( assetInfo.getOfferId(), eOfferActionRxSuccess, 100 );
		m_OfferMgr.announceOfferAction( assetInfo.getCreatorId(), eOfferActionRxNotifyNewMsg, 100 );
	}
	else
	{
		OfferBaseRxSession * xferSession = findOrCreateRxSession( true, poPkt->getRmtSessionId(), netIdent, sktBase );
		if( xferSession )
		{
			OfferBaseInfo& assetInfo = xferSession->getOfferInfo();
			poPkt->fillOfferFromPkt( assetInfo );
			// make history id his id
			assetInfo.setHistoryId( netIdent->getMyOnlineId() );
			assetInfo.setOfferSendState( eOfferSendStateRxProgress );

			xferSession->setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setLclSessionId( xferSession->getLclSessionId() );
			EXferError xferErr = beginOfferBaseReceive( xferSession, poPkt, pktReply );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateOfferDownload( xferSession->getLclSessionId(), 0, rc );
				endOfferBaseXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg(LOG_ERROR, "PluginOfferBaseOffer::onPktOfferSendReq: Could not create session\n");
			PktOfferSendReply pktReply;
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::assetSendComplete( OfferBaseTxSession * xferSession )
{
	updateOfferMgrSendState( xferSession->getOfferInfo().getOfferId(), eOfferSendStateTxSuccess, 100 );
}

//============================================================================
void OfferBaseXferMgr::onPktOfferSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReply\n");
	PktOfferSendReply * poPkt = (PktOfferSendReply *)pktHdr;
	VxGUID&	assetOfferId =	poPkt->getOfferId();
	OfferBaseInfo * assetInfo = m_OfferMgr.findOffer( assetOfferId );
	if( 0 == assetInfo )
	{
		LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferSendReply failed to find asset id\n");
		updateOfferMgrSendState( assetOfferId, eOfferSendStateTxFail, 0 );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		return;
	}

	bool isFileXfer = (bool)poPkt->getRequiresFileXfer();
	uint32_t rxedErrCode = poPkt->getError();
	OfferBaseTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );

	if( xferSession )
	{
		xferSession->setRmtSessionId( poPkt->getLclSessionId() );
		if( 0 == rxedErrCode )
		{
			if( isFileXfer )
			{
				// we did txNextOfferBaseChunk in begin file send
				//RCODE rc = txNextOfferBaseChunk( xferSession );
				//if( rc )
				//{
				//	//IToGui::getToGui().toGuiUpdateOfferUpload( xferSession->getLclSessionId(), 0, rc );
				//	LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferSendReply beginOfferBaseSend returned error %d\n", rc );
				//	endOfferBaseXferSession( xferSession, true );
				//}
			}
			else
			{
				assetSendComplete( xferSession );
				endOfferBaseXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferSendReply PktOfferSendReply returned error %d\n", poPkt->getError() );
			endOfferBaseXferSession( xferSession, true, false );
			updateOfferMgrSendState( assetOfferId, eOfferSendStateTxFail, rxedErrCode );
		}
	}
	else
	{
		if( isFileXfer )
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferSendReply failed to find session\n");
			updateOfferMgrSendState( assetOfferId, eOfferSendStateTxFail, rxedErrCode );
		}
		else
		{
			updateOfferMgrSendState( assetOfferId, rxedErrCode ? eOfferSendStateTxFail : eOfferSendStateTxSuccess, rxedErrCode );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::onPktOfferChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	OfferBaseRxSession * xferSession = 0;
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferChunkReq\n");
	PktOfferChunkReq * poPkt = (PktOfferChunkReq *)pktHdr;
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferChunkReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferChunkReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findRxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = rxOfferBaseChunk( true, xferSession, poPkt );
			if( eXferErrorNone != xferErr )
			{

				PktOfferChunkReply pktReply;
				pktReply.setLclSessionId( xferSession->getLclSessionId() );
				pktReply.setRmtSessionId( poPkt->getLclSessionId() );
				pktReply.setDataLen(0);
				pktReply.setError( xferErr );
				m_Plugin.txPacket( netIdent, sktBase, &pktReply );

				m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(),eOfferActionRxError, xferErr );
				endOfferBaseXferSession( xferSession, true );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferChunkReq failed to find session\n");
			PktOfferChunkReply pktReply;
			pktReply.setLclSessionId( poPkt->getRmtSessionId() );
			pktReply.setRmtSessionId( poPkt->getLclSessionId() );
			pktReply.setDataLen(0);
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void OfferBaseXferMgr::onPktOfferChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktOfferChunkReply * poPkt = (PktOfferChunkReply *)pktHdr;
	OfferBaseTxSession * xferSession = 0;
static int cnt = 0;
	cnt++;
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferBaseChuckReply start %d\n", cnt );
	{ // scope for lock
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferChunkReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferChunkReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = findTxSession( true, poPkt->getRmtSessionId() );
		}

		if( xferSession )
		{
			EXferError xferErr = txNextOfferBaseChunk( xferSession, poPkt->getError(), true );
			if( eXferErrorNone != xferErr )
			{
				//IToGui::getToGui().toGuiUpdateOfferUpload( xferSession->getLclSessionId(), 0, rc );
				endOfferBaseXferSession( xferSession, true, false );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferBaseChuckReply failed to find session\n" );
		}

		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferBaseChuckReply done %d\n", cnt );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferChunkReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void OfferBaseXferMgr::onPktOfferSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReq AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReq AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReq\n");
	PktOfferSendCompleteReq * poPkt = (PktOfferSendCompleteReq *)pktHdr;
	OfferBaseRxSession * xferSession = findRxSession( true, poPkt->getRmtSessionId() );
	//TODO check checksum
	if( xferSession )
	{
		finishOfferBaseReceive( xferSession, poPkt, true );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReq AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::onPktOfferSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReply AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReply AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReply\n");
	PktOfferSendCompleteReply * poPkt = (PktOfferSendCompleteReply *)pktHdr;
	OfferBaseTxSession * xferSession = findTxSession( true, poPkt->getRmtSessionId() );
	if( xferSession )
	{
		VxFileXferInfo xferInfo = xferSession->getXferInfo();
		LogMsg( LOG_INFO, "OfferBaseXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		//m_PluginMgr.getToGui().toGuiOfferBaseUploadComplete( xferInfo.getLclSessionId(), 0 );
		onOfferBaseSent( xferSession, xferSession->getOfferInfo(), (EXferError)poPkt->getError(), true );
	}
	else
	{
		LogMsg( LOG_ERROR, "OfferBaseXferMgr::onPktOfferSendCompleteReply failed to find session\n");
		updateOfferMgrSendState( poPkt->getOfferId(), eOfferSendStateTxSuccess, 100 );
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferSendCompleteReply AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::onPktOfferBaseXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "OfferBaseXferMgr::onPktOfferBaseXferErr\n");
	// TODO handle error
}

//============================================================================
void OfferBaseXferMgr::endOfferBaseXferSession( OfferBaseRxSession * poSessionIn, bool pluginIsLocked )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::endOfferBaseXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::endOfferBaseXferSession pluginMutex.lock done\n");
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

	OfferBaseRxIter oRxIter = m_RxSessions.begin();
	while( oRxIter != m_RxSessions.end() )
	{
		OfferBaseRxSession * xferSession = oRxIter->second;
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
		LogMsg( LOG_INFO, "OfferBaseXferMgr::endOfferBaseXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
void OfferBaseXferMgr::endOfferBaseXferSession( OfferBaseTxSession * poSessionIn, bool pluginIsLocked, bool requeOffer )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::endOfferBaseXferSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::endOfferBaseXferSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	//if( requeOffer )
	//{
	//	queOffer( poSessionIn->getOfferInfo() );
	//}

	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	OfferBaseTxIter iter = m_TxSessions.begin();
	while( iter != m_TxSessions.end() )
	{
		OfferBaseTxSession * xferSession = (*iter);
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
		LogMsg( LOG_INFO, "OfferBaseXferMgr::endOfferBaseXferSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
OfferBaseRxSession * OfferBaseXferMgr::findRxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseRxIter iter;
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		OfferBaseRxSession * xferSession = iter->second;
		if( xferSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return  xferSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
OfferBaseRxSession * OfferBaseXferMgr::findRxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseRxIter iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		OfferBaseRxSession * rxSession = iter->second;
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return rxSession;
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
OfferBaseRxSession *	OfferBaseXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseRxSession * xferSession = findRxSession( true, netIdent );
	if( NULL == xferSession )
	{
		xferSession = new OfferBaseRxSession( m_Engine, m_OfferMgr, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
OfferBaseRxSession *	OfferBaseXferMgr::findOrCreateRxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateRxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateRxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	if( false == lclSessionId.isVxGUIDValid() )
	{
		lclSessionId.initializeWithNewVxGUID();
	}

	OfferBaseRxSession * xferSession = findRxSession( true, lclSessionId );
	if( NULL == xferSession )
	{
		xferSession = new OfferBaseRxSession( m_Engine, m_OfferMgr, lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateRxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
OfferBaseTxSession * OfferBaseXferMgr::findTxSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		OfferBaseTxSession * txSession = ( *iter );
		if( txSession->getIdent() == netIdent )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
OfferBaseTxSession * OfferBaseXferMgr::findTxSession( bool pluginIsLocked, VxGUID& lclSessionId )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		OfferBaseTxSession * txSession = ( *iter );
		if( txSession->getLclSessionId() == lclSessionId )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return txSession;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
OfferBaseTxSession * OfferBaseXferMgr::createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	OfferBaseTxSession * txSession = new OfferBaseTxSession( m_Engine, m_OfferMgr, sktBase, netIdent );
	return txSession;
}

//============================================================================
OfferBaseTxSession * OfferBaseXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseTxSession * xferSession = findTxSession( true, netIdent );
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
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
OfferBaseTxSession *	 OfferBaseXferMgr::findOrCreateTxSession( bool pluginIsLocked, VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateTxSession pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateTxSession pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	OfferBaseTxSession * xferSession = 0;
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
		xferSession = new OfferBaseTxSession( m_Engine, m_OfferMgr, lclSessionId, sktBase, netIdent );
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
		LogMsg( LOG_INFO, "OfferBaseXferMgr::findOrCreateTxSession pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferSession;
}

//============================================================================
void OfferBaseXferMgr::fromGuiSendOfferBase( OfferBaseInfo& assetInfo )
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
			EXferError xferError = createOfferTxSessionAndSend( false, assetInfo, hisInfo, sktBase );
			if( xferError == eXferErrorNone )
			{
				xferFailed = false;
			}
		}
		else
		{
			queOffer( assetInfo );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "OfferBaseXferMgr::fromGuiSendOfferBase NetIdent not found\n" );
	}

	if( xferFailed )
	{
		onTxFailed( assetInfo.getOfferId(), false );
	}
}

//============================================================================
void OfferBaseXferMgr::onTxFailed( VxGUID& assetOfferId, bool pluginIsLocked )
{
	updateOfferMgrSendState( assetOfferId, eOfferSendStateTxFail, 0 );
}

//============================================================================
void OfferBaseXferMgr::onTxSuccess( VxGUID& assetOfferId, bool pluginIsLocked )
{
	updateOfferMgrSendState( assetOfferId, eOfferSendStateTxSuccess, 0 );
}

//============================================================================
void OfferBaseXferMgr::updateOfferMgrSendState( VxGUID& assetOfferId, EOfferSendState sendState, int param )
{
	m_OfferMgr.updateOfferXferState( assetOfferId, sendState, param );
}

//============================================================================
void OfferBaseXferMgr::queOffer( OfferBaseInfo& assetInfo )
{
	m_OfferBaseSendQueMutex.lock();
	bool foundOfferBase = false;
    std::vector<OfferBaseInfo>::iterator iter;
	for( iter = m_OfferBaseSendQue.begin(); iter != m_OfferBaseSendQue.end(); ++iter )
	{
		if( (*iter).getOfferId() == assetInfo.getOfferId() )
		{
			foundOfferBase = true;
			break;
		}
	}

	if( false == foundOfferBase )
	{
		m_OfferBaseSendQue.push_back( assetInfo );
	}

	m_OfferBaseSendQueMutex.unlock();
}

//============================================================================
EXferError OfferBaseXferMgr::createOfferTxSessionAndSend( bool pluginIsLocked, OfferBaseInfo& assetInfo, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::createOfferTxSessionAndSend pluginMutex.lock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::createOfferTxSessionAndSend pluginMutex.lock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	EXferError xferErr = eXferErrorNone;
	OfferBaseTxSession * txSession = createTxSession( hisIdent, sktBase );
	if( false == txSession->getLclSessionId().isVxGUIDValid() )
	{
		txSession->getLclSessionId().initializeWithNewVxGUID();
	}

	if( false == txSession->getRmtSessionId().isVxGUIDValid() )
	{
		txSession->setRmtSessionId( txSession->getLclSessionId() );
	}

	txSession->setOfferInfo( assetInfo );
	VxFileXferInfo& xferInfo = txSession->getXferInfo();
	xferInfo.setLclSessionId( txSession->getLclSessionId() );
	xferInfo.setRmtSessionId( txSession->getRmtSessionId() );
	xferInfo.setXferDirection( eXferDirectionTx );

	m_TxSessionsMutex.lock();
	m_TxSessions.push_back( txSession );
	m_TxSessionsMutex.unlock();

	updateOfferMgrSendState( assetInfo.getOfferId(), eOfferSendStateTxProgress, 0 );
	if( assetInfo.hasFileName() )
	{
		// need to do first so file handle is set before get asset send reply back
		xferErr = beginOfferBaseSend( txSession );
	}
	else
	{
		// all data was in the request packet .. we just wait for reply
	}

	if( eXferErrorNone != xferErr )
	{
		// failed to open file
		updateOfferMgrSendState( assetInfo.getOfferId(), eOfferSendStateTxFail, xferErr );
		endOfferBaseXferSession( txSession, true, false );
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_INFO, "OfferBaseXferMgr::createOfferTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			pluginMutex.unlock();
		}

		return xferErr;
	}

	PktOfferSendReq sendReq;
	sendReq.fillPktFromOffer( assetInfo );
	sendReq.setLclSessionId( xferInfo.getLclSessionId() );
	sendReq.setRmtSessionId( xferInfo.getRmtSessionId() );
	if( false == m_PluginMgr.pluginApiTxPacket( m_Plugin.getPluginType(), hisIdent->getMyOnlineId(), sktBase, &sendReq ) )
	{
		xferErr = eXferErrorDisconnected;
	}	

	if( eXferErrorNone == xferErr )
	{
		// re que for try some other time
		if( requireFileXfer( assetInfo.getOfferType() ) )
		{
			xferErr = txNextOfferBaseChunk( txSession, eXferErrorNone, true );
		}
	}

	if( eXferErrorNone != xferErr )
	{
		// re que for try some other time
		updateOfferMgrSendState( assetInfo.getOfferId(), eOfferSendStateTxFail, xferErr );
		endOfferBaseXferSession( txSession, true, ( ( eXferErrorFileNotFound == xferErr ) || ( eXferErrorDisconnected == xferErr ) ) ? false : true );
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "OfferBaseXferMgr::createOfferTxSessionAndSend pluginMutex.unlock\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return xferErr;
}

//============================================================================
EXferError OfferBaseXferMgr::beginOfferBaseSend( OfferBaseTxSession * xferSession )
{
	EXferError xferErr = eXferErrorNone;
	xferSession->clearErrorCode();
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "OfferBaseXferMgr::beginOfferBaseSend: ERROR: OfferBase transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	if( eXferErrorNone == xferErr )
	{
		xferInfo.setXferDirection( eXferDirectionTx );
		xferInfo.setLclFileName( xferSession->getOfferInfo().getOfferName().c_str() );
		VxFileUtil::getJustFileName( xferSession->getOfferInfo().getOfferName().c_str(), xferInfo.getRmtFileName() );
		xferInfo.setLclSessionId( xferSession->getLclSessionId() );
		xferInfo.setRmtSessionId( xferSession->getRmtSessionId() );
		xferInfo.setFileHashId( xferSession->getFileHashId() );

		xferInfo.m_u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 == xferInfo.m_u64FileLen )
		{
			// no file found to send
			LogMsg( LOG_INFO, "OfferBaseXferMgr::beginOfferBaseSend: OfferBase %s not found to send", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileNotFound;
		}
		else if( false == xferInfo.getFileHashId().isHashValid() )
		{
			// see if we can get hash from shared files
			//if( !m_SharedOfferBasesMgr.getOfferHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
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
			LogMsg( LOG_INFO, "OfferBaseXferMgr::beginOfferBaseSend: Could not open OfferBase %s", xferInfo.getLclFileName().c_str() );
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
				LogMsg( LOG_INFO, "OfferBaseXferMgr::beginOfferBaseSend: OfferBase %s could not be resumed because too short", 
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
					LogMsg( LOG_INFO, "OfferBaseXferMgr::beginOfferBaseSend: could not seek to position %d in file %s",
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
EXferError OfferBaseXferMgr::beginOfferBaseReceive( OfferBaseRxSession * xferSession, PktOfferSendReq * poPkt, PktOfferSendReply& pktReply )
{
	if( NULL == xferSession )
	{
		LogMsg( LOG_ERROR, "OfferBaseXferMgr::beginOfferBaseReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	uint64_t u64FileLen;
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( poPkt->getError() )
	{
		//IToGui::getToGui().toGuiUpdateOfferDownload( poPkt->getRmtSessionId(), 0, poPkt->getError() );
		xferErr = (EXferError)poPkt->getError();
		return xferErr;
	}

	if( eXferErrorNone == xferErr )
	{
		if( xferInfo.m_hFile )
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::beginOfferBaseReceive: ERROR:(OfferBase Receive) receive transfer still in progress" );
			xferErr = eXferErrorAlreadyDownloading;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		// get file information
		xferInfo.setFileHashId( poPkt->getOfferHashId() );
		xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
		if( false == xferInfo.getLclSessionId().isVxGUIDValid() )
		{
			xferInfo.getLclSessionId().initializeWithNewVxGUID();
		}

		xferInfo.setRmtFileName( poPkt->getOfferName().c_str() );
		if( 0 == xferInfo.getRmtFileName().length() )
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::beginOfferBaseReceive: ERROR: No file Name\n" );
			xferErr = eXferErrorBadParam;
		}	
	}

	std::string strRmtPath;
	std::string strRmtOfferBaseNameOnly;
    VxFileUtil::seperatePathAndFile(		xferInfo.getRmtFileName(),
                                            strRmtPath,
                                            strRmtOfferBaseNameOnly );


	if( eXferErrorNone == xferErr )
	{
		// make full path
		if( 0 == strRmtOfferBaseNameOnly.length() )
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::beginOfferBaseReceive: ERROR: NULL file Name %s\n",  xferInfo.getRmtFileName().c_str() );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		VxFileUtil::makeFullPath( strRmtOfferBaseNameOnly.c_str(), VxGetIncompleteDirectory().c_str(), xferInfo.getLclFileName() );
		std::string strPath;
		std::string strOfferBaseNameOnly;
		RCODE rc = VxFileUtil::seperatePathAndFile(	xferInfo.getLclFileName(),			
													strPath,			
													strOfferBaseNameOnly );	
		VxFileUtil::makeDirectory( strPath );
		xferInfo.m_u64FileLen = poPkt->getOfferLen();
		xferInfo.m_u64FileOffs = poPkt->getOfferOffset();
		u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );

		if( 0 != xferInfo.m_u64FileOffs )
		{
			if( u64FileLen < xferInfo.m_u64FileOffs )
			{
				xferErr  = eXferErrorFileSeekError;
				LogMsg( LOG_INFO, "OfferBaseXferMgr: ERROR:(OfferBase Send) %d OfferBase %s could not be resumed because too short", 
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

					LogMsg( LOG_INFO, "OfferBaseXferMgr: ERROR:(OfferBase Send) %d OfferBase %s could not be created", 
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
						LogMsg( LOG_INFO, "OfferBaseXferMgr: ERROR: (OfferBase Send) could not seek to position %d in file %s",
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

				LogMsg( LOG_INFO, "OfferBaseXferMgr: ERROR: %d OfferBase %s could not be created", 
					rc,
					(const char *)xferInfo.getLclFileName().c_str() );
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_INFO, "OfferBaseXferMgr::(OfferBase Send) start recieving file %s\n", 
			(const char *)xferInfo.getLclFileName().c_str() );
		poPkt->fillOfferFromPkt( xferSession->getOfferInfo() );
	}

	pktReply.setError( xferErr );
	pktReply.setOfferOffset( xferInfo.m_u64FileOffs );
	if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &pktReply ) )
	{
		xferErr = eXferErrorDisconnected;
	}

	return xferErr;
}

//============================================================================
EXferError OfferBaseXferMgr::txNextOfferBaseChunk( OfferBaseTxSession * xferSession, uint32_t remoteErr, bool pluginIsLocked )
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
		onOfferBaseSent( xferSession, xferSession->getOfferInfo(), eXferErrorCanceled, pluginIsLocked );
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

		PktOfferSendCompleteReq oPkt;
		oPkt.setLclSessionId( xferSession->getLclSessionId() );
		oPkt.setRmtSessionId( xferSession->getRmtSessionId() );
		oPkt.setOfferId( xferSession->getOfferInfo().getOfferId() );
		m_Plugin.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_ERROR, "OfferBaseXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		onOfferBaseSent( xferSession, xferSession->getOfferInfo(), eXferErrorNone, pluginIsLocked );
		return eXferErrorNone;
	}

	PktOfferChunkReq oPkt;
	// see how much we can read
	uint32_t u32ChunkLen = (uint32_t)(xferInfo.m_u64FileLen - xferInfo.m_u64FileOffs);
	if( PKT_TYPE_OFFER_MAX_DATA_LEN < u32ChunkLen )
	{
		u32ChunkLen = PKT_TYPE_OFFER_MAX_DATA_LEN;
	}

	if( 0 == u32ChunkLen )
	{
		LogMsg( LOG_ERROR, "OfferBaseXferMgr::txNextOfferBaseChunk 0 len u32ChunkLen\n" );
		// what to do?
		return eXferErrorNone;
	}

	// read data into packet
	uint32_t u32BytesRead = (uint32_t)fread(	oPkt.m_au8OfferChunk,
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
		LogMsg( LOG_INFO, "OfferBaseXferMgr: ERROR: %d reading send file at offset %ld when file len %ld file name %s",
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
		m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionTxError, xferErr );
	}
	else
	{
		if( xferInfo.calcProgress() )
		{
			m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionTxProgress, xferInfo.getProgress() );
		}
	}

	return xferErr;
}

//============================================================================
EXferError OfferBaseXferMgr::rxOfferBaseChunk( bool pluginIsLocked, OfferBaseRxSession * xferSession, PktOfferChunkReq * poPkt )
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
		uint32_t u32BytesWritten = (uint32_t)fwrite(	poPkt->m_au8OfferChunk,
												1,
												poPkt->getChunkLen(),
												xferInfo.m_hFile );
		if( u32BytesWritten != poPkt->getChunkLen() ) 
		{
			RCODE rc = VxGetLastError();
			xferSession->setErrorCode( rc );
			xferErr = eXferErrorFileWriteError;

			LogMsg( LOG_INFO, "VxPktHandler::RxOfferBaseChunk: ERROR %d: writing to file %s",
							rc,
							(const char *)xferInfo.getLclFileName().c_str() );
		}
		else
		{
			//successfully write
			xferInfo.m_u64FileOffs += poPkt->getChunkLen();

			PktOfferChunkReply oPkt;
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
			m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionRxProgress, xferInfo.getProgress() );
		}
	}
	else
	{
		m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionRxError, xferErr );
	}

	return xferErr;
}

//============================================================================
void OfferBaseXferMgr::finishOfferBaseReceive( OfferBaseRxSession * xferSession, PktOfferSendCompleteReq * poPkt, bool pluginIsLocked )
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
		LogMsg( LOG_ERROR,  "OfferBaseXferMgr::finishOfferBaseReceive: NULL file handle" );
	}

	//// let other act on the received file
	std::string strOfferBaseName = xferInfo.getLclFileName();

	PktOfferSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setOfferId( xferSession->getOfferInfo().getOfferId() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strOfferBaseName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onOfferBaseReceived( xferSession, xferSession->getOfferInfo(), (EXferError)poPkt->getError(), pluginIsLocked );
}

//============================================================================
void OfferBaseXferMgr::onOfferBaseReceived( OfferBaseRxSession * xferSession, OfferBaseInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiFileDownloadComplete( xferSession->getLclSessionId(), error );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( eXferErrorNone == error )
	{
		std::string incompleteOffer = xferInfo.getDownloadIncompleteFileName();
		std::string completedOfferBase = xferInfo.getDownloadCompleteFileName();
		RCODE rc = 0;
		if( 0 == ( rc = VxFileUtil::moveAFile( incompleteOffer.c_str(), completedOfferBase.c_str() ) ) )
		{
			assetInfo.setOfferName( completedOfferBase.c_str() );
			assetInfo.setHistoryId( xferSession->getIdent()->getMyOnlineId() );

			if( eXferErrorNone == error )
			{
				assetInfo.setOfferSendState( eOfferSendStateRxSuccess );
			}
			else
			{
				assetInfo.setOfferSendState(  eOfferSendStateRxFail );
			}

			m_OfferMgr.addOffer( assetInfo );
			m_Engine.fromGuiAddFileToLibrary( completedOfferBase.c_str(), true, xferInfo.getFileHashId().getHashData() );
			if( eXferErrorNone == error )
			{
				m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionRxSuccess, 0 );
			}
			else
			{
				m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionRxError, error );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "OfferBaseXferMgr::onOfferBaseReceived ERROR %d moving %s to %s\n", rc, incompleteOffer.c_str(), completedOfferBase.c_str() );
		}
	}

	endOfferBaseXferSession( xferSession, pluginIsLocked );
}

//============================================================================
void OfferBaseXferMgr::onOfferBaseSent( OfferBaseTxSession * xferSession, OfferBaseInfo& assetInfo, EXferError error, bool pluginIsLocked )
{
	//m_PluginMgr.getToGui().toGuiOfferBaseUploadComplete( xferSession->getRmtSessionId(), error );
	VxSktBase * sktBase		= xferSession->getSkt();
	VxNetIdent * hisIdent	= xferSession->getIdent();
	if( eXferErrorNone != error )
	{
		updateOfferMgrSendState( assetInfo.getOfferId(), eOfferSendStateTxFail, (int)error );
		m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionTxError, error );
	}
	else
	{
		updateOfferMgrSendState( assetInfo.getOfferId(), eOfferSendStateTxSuccess, (int)error );
		m_OfferMgr.announceOfferAction( xferSession->getOfferInfo().getOfferId(), eOfferActionTxSuccess, 0 );
	}

	endOfferBaseXferSession( xferSession, pluginIsLocked, false );
	if( sktBase && sktBase->isConnected() && false == VxIsAppShuttingDown() )
	{
		checkQueForMoreOffersToSend( pluginIsLocked, hisIdent, sktBase );
	}
}

//============================================================================
void OfferBaseXferMgr::checkQueForMoreOffersToSend( bool pluginIsLocked, VxNetIdent * hisIdent, VxSktBase * sktBase )
{
	// check que and start next xfer
	VxGUID& hisOnlineId = hisIdent->getMyOnlineId();
    std::vector<OfferBaseInfo>::iterator iter;

	m_OfferBaseSendQueMutex.lock();
	for( iter = m_OfferBaseSendQue.begin(); iter != m_OfferBaseSendQue.end(); ++iter )
	{
		if( hisOnlineId == (*iter).getHistoryId() )
		{
			// found asset to send
			OfferBaseInfo& assetInfo = (*iter);
			RCODE rc = createOfferTxSessionAndSend( pluginIsLocked, assetInfo, hisIdent, sktBase );
			if( 0 == rc )
			{
				m_OfferBaseSendQue.erase(iter);
			}

			break;
		}
	}

	m_OfferBaseSendQueMutex.unlock();
}

//============================================================================
void OfferBaseXferMgr::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	OfferBaseTxIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::replaceConnection AutoPluginLock start\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::replaceConnection AutoPluginLock done\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		OfferBaseTxSession * xferSession = (*iter);
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

	OfferBaseRxIter oRxIter;
	for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
	{
		OfferBaseRxSession * xferSession = oRxIter->second;
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "OfferBaseXferMgr::replaceConnection AutoPluginLock destroy\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void OfferBaseXferMgr::onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	checkQueForMoreOffersToSend( false, netIdent, sktBase );
}
