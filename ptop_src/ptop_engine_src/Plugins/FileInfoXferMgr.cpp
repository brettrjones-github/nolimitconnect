//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "FileInfoXferMgr.h"

#include "PluginBase.h"
#include "PluginMgr.h"

#include "FileTxSession.h"
#include "FileRxSession.h"

#include "FileInfoBaseMgr.h"
#include "FileInfo.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/VxSearchDefs.h>
#include <PktLib/PktsFileShare.h>
#include <PktLib/PktsFileList.h>
#include <PktLib/PktsPluginOffer.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/AppErr.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//============================================================================
FileInfoXferMgr::FileInfoXferMgr( P2PEngine& engine, PluginBase& plugin, FileInfoBaseMgr& fileInfoMgr )
: m_Engine( engine )
, m_Plugin( plugin )
, m_PluginMgr( engine.getPluginMgr() )
, m_FileInfoMgr( fileInfoMgr )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::FileInfoXferMgr %s %p", DescribePluginType( plugin.getPluginType() ), this );
}

//============================================================================
FileInfoXferMgr::~FileInfoXferMgr()
{
	clearRxSessionsList();
	clearTxSessionsList();
}

//============================================================================
EPluginType FileInfoXferMgr::getPluginType( void ) 
{ 
	return m_Plugin.getPluginType(); 
}

//============================================================================
void FileInfoXferMgr::clearRxSessionsList( void )
{
	std::map<VxGUID, FileRxSession *>::iterator iter;

	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		FileRxSession * xferSession = iter->second;
		delete xferSession;
	}

	m_RxSessions.clear();
}

//============================================================================
void FileInfoXferMgr::clearTxSessionsList( void )
{
	FileTxIter iter;
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		FileTxSession * xferSession = (*iter);
		delete xferSession;
	}

	m_TxSessions.clear();
}

//============================================================================
void FileInfoXferMgr::onAfterUserLogOnThreaded( void )
{
}

//============================================================================
// returns -1 if unknown else percent downloaded
int FileInfoXferMgr::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	int result = -1;
	std::map<VxGUID, FileRxSession *>::iterator iter;

	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		FileRxSession * xferSession = iter->second;
		if( xferSession->getXferInfo().getFileHashId().isEqualTo( fileHashId ) )
		{
			result = xferSession->getXferInfo().getProgress();
			break;
		}
	}

	return result;
}

//============================================================================
void FileInfoXferMgr::fromGuiStartPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	if( false == m_bIsInSession )
	{
		m_bIsInSession = true;
	}
}

//============================================================================
void FileInfoXferMgr::fromGuiStopPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	if( true == m_bIsInSession )
	{
		m_bIsInSession = false;
	}
}

//============================================================================
bool FileInfoXferMgr::fromGuiIsPluginInSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_bIsInSession;
}

//============================================================================
void FileInfoXferMgr::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
	fileShareSettings = m_FileShareSettings;
}

//============================================================================
void FileInfoXferMgr::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
{
	bool wasInSession = fromGuiIsPluginInSession( NULL, 0 );
	if( wasInSession )
	{
		fromGuiStopPluginSession( NULL, 0 );
	}

	m_FileShareSettings = fileShareSettings;
	m_FileShareSettings.saveSettings( m_Engine.getEngineSettings() );
	if( wasInSession )
	{
		fromGuiStartPluginSession( NULL, 0 );
	}
}

//============================================================================
void FileInfoXferMgr::fileAboutToBeDeleted( std::string& fileName )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	FileTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		FileTxSession * xferSession = ( *iter );
		if( xferSession->getXferInfo().getLclFileName() == fileName )
		{
			xferSession->cancelUpload( xferSession->getXferInfo().getLclSessionId() );
			delete xferSession;
			m_TxSessions.erase( iter );
			return;
		}
	}
}

//============================================================================
void FileInfoXferMgr::fromGuiCancelDownload( VxGUID& lclSessionId )
{
	std::map<VxGUID, FileRxSession *>::iterator iter;
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		FileRxSession * xferSession = iter->second;
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			xferSession->cancelDownload( lclSessionId );
			delete xferSession;
			m_RxSessions.erase( iter );
		}
	}
}

//============================================================================
void FileInfoXferMgr::fromGuiCancelUpload( VxGUID& lclSessionId )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	FileTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		FileTxSession * xferSession = ( *iter );
		if( xferSession->getLclSessionId() == lclSessionId )
		{
			xferSession->cancelUpload( lclSessionId );
			delete xferSession;
			m_TxSessions.erase( iter );
			return;
		}
	}
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool FileInfoXferMgr::fromGuiMakePluginOffer(	VxNetIdent*	netIdent,		// identity of friend
												int				pvUserData,
												const char*	pOfferMsg,		// offer message
												const char*	pFileName,
												uint8_t *		fileHashId,
												VxGUID			lclSessionId )	
{
	VxSktBase* sktBase = NULL;
	if( true == m_PluginMgr.pluginApiSktConnectTo( m_Plugin.getPluginType(), netIdent, pvUserData, &sktBase ) )
	{
		PktPluginOfferReq oPkt;
		if( true == m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
			netIdent->getMyOnlineId(), 
			sktBase, 
			&oPkt ) )
		{
			if( pFileName && strlen( pFileName ) )
			{
				if( false == lclSessionId.isVxGUIDValid() )
				{
					lclSessionId.initializeWithNewVxGUID();
				}

				FileRxSession *	xferSession = findOrCreateRxSession( lclSessionId, netIdent, sktBase );
				std::string strFileName = pFileName;
				xferSession->m_astrFilesToXfer.push_back( FileToXfer(strFileName, 0, lclSessionId, lclSessionId, lclSessionId, xferSession->getFileHashId(), pvUserData ) );
			}
			else
			{
				// if no file name then want directory list
				PktFileListReq pktListReq;
				m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
												netIdent->getMyOnlineId(), 
												sktBase, 
												&pktListReq );
			}

			return true;
		}
	}

	return false;
}

//============================================================================
int FileInfoXferMgr::fromGuiPluginControl(	VxNetIdent*		netIdent,
											const char*		pControl, 
											const char*		pAction,
											uint32_t			u32ActionData,
											VxGUID&				sessionId,
											uint8_t *			fileHashIdIn )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	VxSha1Hash fileHashId( fileHashIdIn );
	if( 0 == strcmp( pControl, "ViewDirectory" ) )
	{
		FileRxSession *	xferSession = 0;
		if( sessionId.isVxGUIDValid() )
		{
			xferSession = findRxSession( sessionId );
		}

		if( 0 == xferSession )
		{
			findRxSession( netIdent );
		}

		if( xferSession )
		{
			PktFileListReq oPkt;
			EXferError xferErr =  m_Plugin.txPacket( netIdent, xferSession->getSkt(), &oPkt ) ? eXferErrorNone : eXferErrorDisconnected;
			return (int)xferErr;
		}
		else
		{
			LogMsg( LOG_ERROR, "FileInfoXferMgr::fromGuiPluginControl could not find session" );
			return (int)eXferErrorBadParam;
		}
	}

	if( 0 == strcmp( pControl, "DownloadFile" ) )
	{
		if( isFileDownloading( fileHashId ) )
		{
			return (int)eXferErrorAlreadyDownloading;
		}

		//if( isFileInDownloadFolder( pAction )
		//	|| m_FileLibraryMgr.isFileInLibrary( fileHashId ) )
		//{
		//	return eXferErrorAlreadyDownloaded;
		//}	

		VxSktBase* sktBase = NULL;
		if( true == m_PluginMgr.pluginApiSktConnectTo( m_Plugin.getPluginType(), netIdent, 0, &sktBase ) )
		{
			FileRxSession *	xferSession = findOrCreateRxSession( sessionId, netIdent, sktBase );
			xferSession->setFileHashId( fileHashId );
			std::string strFileName = pAction;
			xferSession->m_astrFilesToXfer.push_back( FileToXfer(strFileName, 0, sessionId, sessionId, sessionId, xferSession->getFileHashId(), (int)u32ActionData ) );
			return (int)beginFileGet( xferSession );
		}
		else
		{
			return (int)eXferErrorDisconnected;
		}
	}

	return (int)eXferErrorBadParam;
}

//============================================================================
void FileInfoXferMgr::onConnectionLost( VxSktBase* sktBase )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );

	bool erasedSession = true;
	FileTxIter iter;
	while( erasedSession )
	{
		erasedSession = false;
		for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
		{
			FileTxSession * xferSession = ( *iter );
			if( xferSession->getSkt() == sktBase )
			{
				delete xferSession;
				m_TxSessions.erase( iter );
				erasedSession = true;
				break;
			}
		}
	}

	erasedSession = true;
	FileRxIter oRxIter; 
	while( erasedSession )
	{
		erasedSession = false;
		for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
		{
			FileRxSession * xferSession = oRxIter->second;
			if( xferSession->getSkt() == sktBase )
			{
				delete xferSession;
				m_RxSessions.erase( oRxIter );
				erasedSession = true;
				break;
			}
		}
	}
}

//============================================================================
void FileInfoXferMgr::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	//PktPluginOfferReq * poPkt = (PktPluginOfferReq *)pktHdr;
	//FileTxSession * xferSession = findOrCreateTxSession( netIdent, sktBase );
	//PktFileListReq oPkt;
	//m_Plugin.txPacket( netIdent, xferSession->getSkt(), &oPkt );
}

//============================================================================
//! packet with remote users reply to offer
void FileInfoXferMgr::onPktPluginOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktPluginOfferReply * poPkt = (PktPluginOfferReply *)pktHdr;
	FileTxSession * xferSession = findTxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		m_FileInfoMgr.toGuiRxedOfferReply(	netIdent,		// identity of friend
															m_Plugin.getPluginType(),			// plugin type
															0,				// plugin defined data
															poPkt->getOfferResponse(),
															xferSession->m_strOfferFile.c_str(),
															0,
															xferSession->getLclSessionId(),
															xferSession->getRmtSessionId() );
		if( eOfferResponseAccept == poPkt->getOfferResponse() )
		{
			xferSession->m_astrFilesToXfer.push_back( FileToXfer(xferSession->m_strOfferFile, 
													0, 
													xferSession->getLclSessionId(), 
													xferSession->getRmtSessionId(),
													xferSession->getAssetId(),
													xferSession->getFileHashId(),
													0 ) );
			EXferError xferErr = beginFileSend( xferSession );
			if( eXferErrorNone != xferErr )
			{
				m_FileInfoMgr.updateToGuiFileXferState( xferSession->getLclSessionId(), eXferStateUploadError, xferErr );
			}
		}
		else
		{
			endFileXferSession( xferSession );
		}
	}
}

//============================================================================
void FileInfoXferMgr::onPktFileGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktFileGetReq * poPkt = (PktFileGetReq *)pktHdr;
	PktFileGetReply oPkt;
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileGetReq start %s %p rxing %d", DescribePluginType( getPluginType() ), this, m_RxSessions.size() );

	std::string strLclFileName;
	std::string rmtFileName;
	poPkt->getFileName( rmtFileName );
	if( false == m_FileInfoMgr.getFileFullName( poPkt->getAssetId(), poPkt->getFileHashId(), strLclFileName ) )
	{
		LogMsg( LOG_VERBOSE, "FileXferMgr::onPktFileGetReq file no longer shared %s", rmtFileName.c_str() );
		oPkt.setError( eXferErrorFileNotFound );
	}
	else
	{
		oPkt.setError( canTxFile( netIdent, poPkt->getAssetId(), poPkt->getFileHashId() ) );
	}

	if( eXferErrorNone == oPkt.getError() )
	{
		FileTxSession * xferSession = createTxSession( netIdent, sktBase );
		VxFileXferInfo& xferInfo = xferSession->getXferInfo();
		xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
		xferInfo.setAssetId( poPkt->getAssetId() );
		xferInfo.setFileHashId( poPkt->getFileHashId() );
		xferInfo.setFileOffset( poPkt->getStartOffset() );
		xferInfo.setLclFileName( strLclFileName.c_str() );
		xferInfo.setRmtFileName(  rmtFileName.c_str() );
		m_TxSessions.push_back( xferSession );

		oPkt.setLclSessionId( xferInfo.getLclSessionId() );
		oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
		oPkt.setFileName( xferInfo.getRmtFileName() );
		oPkt.setAssetId( xferInfo.getAssetId() );
		oPkt.setFileHashId( xferInfo.getFileHashId() );
		oPkt.setStartOffset( poPkt->getStartOffset() );

		EXferError xferErr  = ( m_Plugin.txPacket( netIdent, sktBase, &oPkt ) ) ? eXferErrorNone : eXferErrorDisconnected;
		if( eXferErrorNone == xferErr )
		{
			xferErr = beginFileSend( xferSession );
		}

		if( eXferErrorNone != xferErr )
		{
			m_FileInfoMgr.updateToGuiFileXferState(  poPkt->getLclSessionId(), eXferStateUploadError, xferErr );
			endFileXferSession( xferSession );
		}
		else
		{
			m_FileInfoMgr.updateToGuiFileXferState(  poPkt->getLclSessionId(), eXferStateBeginUpload, eXferErrorNone );
			m_FileInfoMgr.updateToGuiFileXferState(  poPkt->getLclSessionId(), eXferStateInUploadXfer, eXferErrorNone );
		}
	}
	else
	{
		m_Plugin.txPacket( netIdent, sktBase, &oPkt );
	}

	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileGetReq done %s %p rxing %d", DescribePluginType( getPluginType() ), this, m_RxSessions.size() );
}

//============================================================================
void FileInfoXferMgr::onPktFileGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileGetReply %s %p rxing %d", DescribePluginType( getPluginType() ), this, m_RxSessions.size() );
}

//============================================================================
void FileInfoXferMgr::onPktFileSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileSendReq %s %p rxing %d", DescribePluginType( getPluginType() ), this, m_RxSessions.size() );
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktFileSendReq * poPkt = (PktFileSendReq *)pktHdr;
	PktFileSendReply pktReply;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		xferSession->setRmtSessionId( poPkt->getLclSessionId() );
		EXferError xferErr = beginFileReceive( xferSession, poPkt );
		if( eXferErrorNone != xferErr )
		{
			m_FileInfoMgr.updateToGuiFileXferState( xferSession->getLclSessionId(), eXferStateDownloadError, xferErr );
			endFileXferSession( xferSession );
		}
		else
		{
			m_FileInfoMgr.updateToGuiFileXferState(  xferSession->getLclSessionId(), eXferStateBeginDownload, eXferErrorNone );
			m_FileInfoMgr.updateToGuiFileXferState(  xferSession->getLclSessionId(), eXferStateInDownloadXfer, eXferErrorNone );
		}

		pktReply.setError( xferErr );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::onPktFileSendReq: Could not find session" );
		pktReply.setError( eXferErrorBadParam );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
	}
}

//============================================================================
void FileInfoXferMgr::onPktFileSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileSendReply %s %p txing %d", DescribePluginType( getPluginType() ), this, m_TxSessions.size() );
	PktFileSendReply* poPkt = ( PktFileSendReply* )pktHdr;
	FileTxSession * xferSession = findTxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::onPktFileSendReply found tx session" );
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::onPktFileSendReply failed to find tx session");
	}
}

//============================================================================
void FileInfoXferMgr::onPktFileChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktFileChunkReq * poPkt = (PktFileChunkReq *)pktHdr;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileChunkReq %s %p rxing %d", DescribePluginType( getPluginType() ), this, m_RxSessions.size() );
		EXferError xferErr = rxFileChunk( xferSession, poPkt );
		if( eXferErrorNone != xferErr )
		{
			m_FileInfoMgr.updateToGuiFileXferState( xferSession->getLclSessionId(), eXferStateDownloadError, xferErr );
			endFileXferSession( xferSession );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::onPktFileChunkReq failed to find rx session");
		PktFileChunkReply pktReply;
		pktReply.setDataLen(0);
		pktReply.setError( eXferErrorBadParam );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
	}
}

//============================================================================
void FileInfoXferMgr::onPktFileChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
static int cnt = 0;
	cnt++;
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileChuckReply start %d", cnt );
    PktFileChunkReply * poPkt = (PktFileChunkReply *)pktHdr;
	FileTxSession * xferSession = findTxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		EXferError xferErr = txNextFileChunk( xferSession );
		if( eXferErrorNone != xferErr )
		{
			m_FileInfoMgr.updateToGuiFileXferState( xferSession->getLclSessionId(), eXferStateUploadError, xferErr );
			endFileXferSession( xferSession );
		}
	}

	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileChuckReply done %d\n", cnt );
}

//============================================================================
void FileInfoXferMgr::onPktFileGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileGetCompleteReq");
	PktFileGetCompleteReq * poPkt = (PktFileGetCompleteReq *)pktHdr;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		finishFileReceive( xferSession, poPkt );
	}
}

//============================================================================
void FileInfoXferMgr::onPktFileGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileGetCompleteReply");
}

//============================================================================
void FileInfoXferMgr::onPktFileSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileSendCompleteReq");
	PktFileSendCompleteReq * poPkt = (PktFileSendCompleteReq *)pktHdr;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	//TODO check checksum
	if( xferSession )
	{
		finishFileReceive( xferSession, poPkt );
	}
}

//============================================================================
void FileInfoXferMgr::onPktFileSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileSendCompleteReply");
}

//============================================================================
void FileInfoXferMgr::onPktFindFileReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFindFileReq");
}

//============================================================================
void FileInfoXferMgr::onPktFindFileReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFindFileReply");
}

//============================================================================
void FileInfoXferMgr::onPktFileListReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	/*
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktFileListReq * poPkt = (PktFileListReq *)pktHdr;
	uint32_t reqListIdx = poPkt->getListIndex();
	m_FileInfoMgr.lockFileListPackets();
	std::vector<PktFileListReply*>& pktList = m_FileInfoMgr.getFileListPackets();
	if( reqListIdx >= pktList.size() )
	{
		PktFileListReply pktReply;
		if( 0 == pktList.size() )
		{
			pktReply.setError( ERR_NO_SHARED_FILES );
		}
		else
		{
			pktReply.setError( ERR_FILE_LIST_IDX_OUT_OF_RANGE );
		}

		m_Plugin.txPacket( netIdent, sktBase, &pktReply );	
	}
	else
	{
		m_Plugin.txPacket( netIdent, sktBase, pktList[reqListIdx] );	
	}

	m_FileInfoMgr.unlockFileListPackets();
	*/
}

//============================================================================
void FileInfoXferMgr::onPktFileListReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	/*
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	FileRxSession * xferSession = findOrCreateRxSession( netIdent, sktBase );
	PktFileListReply * poPkt = (PktFileListReply *)pktHdr;
	RCODE rc = poPkt->getError();
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::onPktFileListReply error %d\n", rc );
		m_FileInfoMgr.toGuiFileListReply( netIdent, m_Plugin.getPluginType(), 0, 0, "", VxGUID::nullVxGUID(), 0 );
		return;
	}

	if( !poPkt->getIsListCompleted() )
	{
		// request next in list
		PktFileListReq pktReq;
		pktReq.setListIndex( poPkt->getListIndex() + 1 );
		m_Plugin.txPacket( netIdent, sktBase, &pktReq );
	}

	std::vector<VxFileInfo> fileList;
	poPkt->getFileList( fileList );
	std::vector<VxFileInfo>::iterator iter;
	for( iter = fileList.begin(); iter != fileList.end(); ++iter )
	{
		VxFileInfo& fileInfo = (*iter);
		m_FileInfoMgr.toGuiFileListReply( netIdent, 
									m_Plugin.getPluginType(), 
									fileInfo.getFileType(), 
									fileInfo.getFileLength(),
									fileInfo.getFileName().c_str(),
									VxGUID::nullVxGUID(),
									fileInfo.getFileHashId().getHashData() );
	}

	if( poPkt->getIsListCompleted() 
		|| ( 0 == poPkt->getFileCount() ) )
	{
		m_FileInfoMgr.toGuiFileListReply( netIdent, m_Plugin.getPluginType(), 0, 0, "", VxGUID::nullVxGUID(), 0 );
		endFileXferSession( xferSession );
	}
	*/
}

//============================================================================
void FileInfoXferMgr::onPktFileInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileInfoReq");
}

//============================================================================
void FileInfoXferMgr::onPktFileInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileInfoReply");
}

//============================================================================
void FileInfoXferMgr::onPktFileInfoErr( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::onPktFileInfoErr");
}

//============================================================================
void FileInfoXferMgr::endFileXferSession( FileRxSession * poSessionIn )
{
	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	if( getMoveCompletedFilesToDownloadFolder() )
	{
		// file was moved to completed folder.. remove the temporary download file 
		std::string fileName = xferInfo.getDownloadIncompleteFileName();
		if( fileName.length() )
		{
			VxFileUtil::deleteFile( fileName.c_str() );
		}
	}

	FileRxIter oRxIter = m_RxSessions.begin();
#if defined(TARGET_OS_WINDOWS)
	while( oRxIter != m_RxSessions.end() )
	{
		FileRxSession * xferSession = oRxIter->second;
		if( poSessionIn == xferSession )
		{
			oRxIter = m_RxSessions.erase( oRxIter );
			delete xferSession;
		}
		else
		{
			++oRxIter;
		}
	}
#else
    bool erasedSession = true;
    while( erasedSession )
    {
        erasedSession = false;
        for( oRxIter =  m_RxSessions.begin(); oRxIter !=  m_RxSessions.end(); ++oRxIter )
        {
            FileRxSession * xferSession = oRxIter->second;
            if( poSessionIn == xferSession )
            {
                delete xferSession;
                m_RxSessions.erase(oRxIter);
                erasedSession = true;
                break;
            }
        }
    }
#endif // TARGET_OS_WINDOWS
}

//============================================================================
void FileInfoXferMgr::endFileXferSession( FileTxSession * poSessionIn )
{
	VxFileXferInfo& xferInfo = poSessionIn->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	FileTxIter iter = m_TxSessions.begin();
	while( iter != m_TxSessions.end() )
	{
		FileTxSession * xferSession = (*iter);
		if( xferSession == poSessionIn )
		{
			delete xferSession;
			m_TxSessions.erase( iter );
			break;
		}
		else
		{
			++iter;
		}
	}
}

//============================================================================
FileRxSession * FileInfoXferMgr::findRxSession( VxNetIdent* netIdent )
{
	FileRxIter iter;
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		FileRxSession * xferSession = iter->second;
		if( xferSession->getIdent() == netIdent )
		{
			return  xferSession;
		}
	}

	return NULL;
}

//============================================================================
FileRxSession * FileInfoXferMgr::findRxSession( VxGUID& lclSessionId )
{
	FileRxIter iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		return iter->second;
	}

	return NULL;
}

//============================================================================
FileRxSession *	FileInfoXferMgr::findOrCreateRxSession( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	FileRxSession * xferSession = findRxSession( netIdent );
	if( !xferSession )
	{
		xferSession = new FileRxSession( sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	return xferSession;
}

//============================================================================
FileRxSession *	FileInfoXferMgr::findOrCreateRxSession( VxGUID& lclSessionId, VxNetIdent* netIdent, VxSktBase* sktBase )
{
	FileRxSession * xferSession = findRxSession( lclSessionId );
	if( ( xferSession ) && ( lclSessionId.isVxGUIDValid() ) )
	{
		xferSession = new FileRxSession( lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}

	if( !xferSession && !lclSessionId.isVxGUIDValid() )
	{
		xferSession = findRxSession( netIdent );
	}

	if( !xferSession )
	{
		xferSession = new FileRxSession( lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
		LogMsg( LOG_VERBOSE, "FileInfoXferMgr::findOrCreateRxSession %s %p rxing %d", DescribePluginType( getPluginType() ), this, m_RxSessions.size() );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	return xferSession;
}

//============================================================================
FileTxSession * FileInfoXferMgr::findTxSession( VxNetIdent* netIdent )
{
	FileTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		FileTxSession * txSession = ( *iter );
		if( txSession->getIdent() == netIdent )
		{
			return txSession;
		}
	}

	return NULL;
}

//============================================================================
FileTxSession * FileInfoXferMgr::findTxSession( VxGUID& lclSessionId )
{
	FileTxIter iter;
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		FileTxSession * txSession = ( *iter );
		if( txSession->getLclSessionId() == lclSessionId )
		{
			return txSession;
		}
	}

	return NULL;
}

//============================================================================
FileTxSession *	FileInfoXferMgr::createTxSession( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	return new FileTxSession( sktBase, netIdent );
}

//============================================================================
FileTxSession *	FileInfoXferMgr::findOrCreateTxSession( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	FileTxSession * xferSession = findTxSession( netIdent );
	if( NULL == xferSession )
	{
		xferSession = createTxSession( netIdent, sktBase );
		m_TxSessions.push_back( xferSession );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	return xferSession;
}

//============================================================================
FileTxSession *	 FileInfoXferMgr::findOrCreateTxSession(  VxGUID& lclSessionId, VxNetIdent* netIdent, VxSktBase* sktBase )
{
	FileTxSession * xferSession = 0;
	if( lclSessionId.isVxGUIDValid() )
	{
		xferSession = findTxSession( lclSessionId );
	}

	if( 0 == xferSession )
	{
		xferSession = findTxSession( netIdent );
	}

	if( NULL == xferSession )
	{
		xferSession = new FileTxSession( lclSessionId, sktBase, netIdent );
		m_TxSessions.push_back( xferSession );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	return xferSession;
}

//============================================================================
EXferError FileInfoXferMgr::beginFileSend( FileTxSession * xferSession )
{
	PktFileSendReq oPktReq;

	EXferError xferErr = eXferErrorNone;
	xferSession->setErrorCode( 0 );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::beginFileSend: ERROR: File transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	xferInfo.setXferDirection( eXferDirectionTx );
	if( eXferErrorNone == xferErr )
	{
		xferInfo.m_u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 == xferInfo.m_u64FileLen )
		{
			// no file found to send
			LogMsg( LOG_VERBOSE, "FileInfoXferMgr::beginFileSend: File %s not found to send", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileNotFound;
		}
		else if( false == xferInfo.getFileHashId().isHashValid() )
		{
			// see if we can get hash from shared files
			if( !m_FileInfoMgr.getFileHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
			{
				// TODO.. que for hash
			}
		}
	}

	if( eXferErrorNone == xferErr )
	{
		xferInfo.m_hFile = fopen( xferInfo.getLclFileName().c_str(), "rb" ); 
		if( NULL == xferInfo.m_hFile )
		{
			// open file failed
			xferInfo.m_hFile = NULL;
			LogMsg( LOG_VERBOSE, "FileInfoXferMgr::beginFileSend: Could not open File %s", xferInfo.getLclFileName().c_str() );
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
				LogMsg( LOG_VERBOSE, "FileInfoXferMgr::beginFileSend: File %s could not be resumed because too short", 
					(const char*)xferInfo.getLclFileName().c_str() );
				xferErr = eXferErrorFileSeekError;
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
					LogMsg( LOG_VERBOSE, "FileInfoXferMgr::beginFileSend: could not seek to position %d in file %s",
						xferInfo.m_u64FileOffs,
						(const char*)xferInfo.getLclFileName().c_str() );
					xferErr = eXferErrorFileSeekError;
				}
			}
		}
	}

	oPktReq.setError( xferErr );
	//get file extension
	std::string	strExt;
	VxFileUtil::getFileExtension( xferInfo.getLclFileName(), strExt );

	uint8_t u8FileType = VxFileUtil::fileExtensionToFileTypeFlag( strExt.c_str() );

	oPktReq.setFileType( u8FileType );
	oPktReq.setFileLen( xferInfo.m_u64FileLen );
	oPktReq.setLclSessionId( xferInfo.getLclSessionId() );
	oPktReq.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPktReq.setAssetId( xferInfo.getAssetId() );
	oPktReq.setFileName( xferInfo.getRmtFileName().c_str() );
	oPktReq.setFileOffset( xferInfo.getFileOffset() );
	oPktReq.setFileHashId( xferInfo.getFileHashId() );

	if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPktReq ) )
	{
		if( eXferErrorNone == xferErr )
		{
			xferErr = eXferErrorDisconnected;
		}
	}

	LogMsg( LOG_VERBOSE, "FileInfoXferMgr::beginFileSend: start sending file %s to %s", 
						xferInfo.getLclFileName().c_str(),
						xferSession->getIdent()->getOnlineName() );

	if( eXferErrorNone == xferErr )
	{
		m_FileInfoMgr.toGuiStartUpload(		xferSession->getIdent(), 
													m_Plugin.getPluginType(), 
													xferInfo.getLclSessionId(), 
													u8FileType, 
													xferInfo.m_u64FileLen, 
													xferInfo.getRmtFileName().c_str(),
													xferInfo.getAssetId(),
													xferInfo.getFileHashId().getHashData() );

		// file is open and setup so send first chunk of data
		return txNextFileChunk( xferSession );
	}
	else
	{
		endFileXferSession( xferSession );
		return xferErr;
	}
}

//============================================================================
EXferError FileInfoXferMgr::beginFileReceive( FileRxSession * rxSession, PktFileSendReq * poPkt )
{
	if( NULL == rxSession )
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::beginFileReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	VxFileXferInfo& xferInfo = rxSession->getXferInfo();
	xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
	xferInfo.setAssetId( poPkt->getAssetId() );
	xferInfo.setFileHashId( poPkt->getFileHashId() );
	xferInfo.setRmtFileName( poPkt->getFileName() );
	xferInfo.setFileLength( poPkt->getFileLen() );
	xferInfo.setFileOffset( poPkt->getFileOffset() );

	if( poPkt->getError() )
	{
		xferErr = (EXferError)poPkt->getError();
		m_FileInfoMgr.updateToGuiFileXferState(  poPkt->getRmtSessionId(), eXferStateDownloadError, xferErr, xferErr );
		endFileXferSession( rxSession );
		return xferErr;
	}

	xferErr = setupFileDownload( xferInfo, rxSession->getIdent() );

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_VERBOSE, "FileInfoXferMgr::(File Send) start recieving file %s", 
			(const char*)xferInfo.getLclFileName().c_str() );

		uint8_t u8FileType = VxFileUtil::fileExtensionToFileTypeFlag( xferInfo.getRmtFileName().c_str() );
		m_FileInfoMgr.toGuiStartDownload( 
													rxSession->getIdent(), 
													m_Plugin.getPluginType(), 
													xferInfo.getLclSessionId(), 
													u8FileType, 
													xferInfo.m_u64FileLen, 
													xferInfo.getRmtFileName().c_str(),
													xferInfo.getAssetId(),
													xferInfo.getFileHashId().getHashData() );
	}

	// don't send reply.. will get file chunk next anyway
	return xferErr;
}

//============================================================================
EXferError FileInfoXferMgr::txNextFileChunk( FileTxSession * xferSession )
{
	if( NULL == xferSession )
	{
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	// fill the packet with data from the file
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
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
		// move file from to be sent to sent
		//xferSession->m_astrFilesSent.push_back( xferSession->m_astrFilesToXfer[0] );
		//xferSession->m_astrFilesToXfer.erase( xferSession->m_astrFilesToXfer.begin() );
		//xferSession->m_astrFilesToXfer.erase( xferSession->m_astrFilesToXfer.begin() );

		PktFileSendCompleteReq oPkt;
		oPkt.setLclSessionId( xferSession->getLclSessionId() );
		oPkt.setRmtSessionId( xferSession->getRmtSessionId() );
		oPkt.setAssetId( xferSession->getAssetId() );
		oPkt.setFileHashId( xferSession->getFileHashId() );
		m_Plugin.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_ERROR, "FileInfoXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		m_FileInfoMgr.toGuiFileUploadComplete( xferInfo.getLclSessionId(), eXferErrorNone );
		onFileSent( xferSession, xferInfo.getLclFileName().c_str(), eXferErrorNone );
		return eXferErrorNone;
	}

	xferErr = sendNextFileChunk( xferInfo, xferSession->getIdent(), xferSession->getSkt() );
	return xferErr;
}

//============================================================================
EXferError FileInfoXferMgr::rxFileChunk( FileRxSession * xferSession, PktFileChunkReq * poPkt )
{
	if( NULL == xferSession )
	{
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	vx_assert( xferSession );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	// we are receiving a file
	if( xferInfo.m_hFile )
	{
		//write the chunk of data out to the file
		uint32_t u32BytesWritten = (uint32_t)fwrite(	poPkt->m_au8FileChunk,
			1,
			poPkt->getChunkLen(),
			xferInfo.m_hFile );
		if( u32BytesWritten != poPkt->getChunkLen() ) 
		{
			RCODE rc = VxGetLastError();
			xferSession->setErrorCode( rc );
			xferErr = eXferErrorFileWriteError;

			LogMsg( LOG_VERBOSE, "VxPktHandler::RxFileChunk: ERROR %d: writing to file %s",
							rc,
							(const char*)xferInfo.getLclFileName().c_str() );
		}
		else
		{
			//successfully write
			xferInfo.m_u64FileOffs += poPkt->getChunkLen();

			PktFileChunkReply oPkt;
			oPkt.setDataLen( poPkt->getDataLen() );
			oPkt.setLclSessionId( xferInfo.getLclSessionId() );
			xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
			oPkt.setRmtSessionId( poPkt->getLclSessionId() );
			oPkt.setAssetId( poPkt->getAssetId() );

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
			m_FileInfoMgr.updateToGuiFileXferState( xferInfo.getLclSessionId(), eXferStateInDownloadXfer, eXferErrorNone, xferInfo.getProgress() );
		}
	}
	else
	{
		m_FileInfoMgr.updateToGuiFileXferState( xferInfo.getLclSessionId(), eXferStateDownloadError, xferErr );
	}

	return xferErr;
}

//============================================================================
void FileInfoXferMgr::finishFileReceive( FileRxSession * xferSession, PktFileSendCompleteReq * poPkt )
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
		LogMsg( LOG_ERROR,  "FileInfoXferMgr::finishFileReceive: NULL file handle" );
	}

	// let other act on the received file
	std::string strFileName = xferInfo.getLclFileName();
	xferSession->m_astrFilesXfered.push_back( 
			FileToXfer(strFileName, 0, xferInfo.getLclSessionId(), 
			xferInfo.getRmtSessionId(), xferInfo.getAssetId(), xferInfo.getFileHashId(), 0 ) );

	//=== acknowlege ===//
	PktFileSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setAssetId( xferSession->getAssetId() );
	oPkt.setFileHashId( xferSession->getFileHashId() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_VERBOSE,  "VxPktHandler: Done Receiving file %s", strFileName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onFileReceived( xferSession, strFileName.c_str(), (EXferError)poPkt->getError() );
}

//============================================================================
void FileInfoXferMgr::onFileReceived( FileRxSession * xferSession, const char* pFileName, EXferError xferError )
{
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( eXferErrorNone == xferError )
	{
		std::string incompleteFile = xferInfo.getLclFileName();
		if( getMoveCompletedFilesToDownloadFolder() )
		{
			std::string completedFile = xferInfo.getDownloadCompleteFileName();
			if( VxFileUtil::fileExists( completedFile.c_str() ) )
			{
				completedFile =  VxFileUtil::makeUniqueFileName( completedFile.c_str() );
			}

			RCODE rc = 0;
			if( 0 == ( rc = VxFileUtil::moveAFile( incompleteFile.c_str(), completedFile.c_str() ) ) )
			{
				m_FileInfoMgr.onFileDownloadComplete( xferSession->getIdent(), xferSession->getSkt(), xferInfo.getLclSessionId(), completedFile, xferInfo.getAssetId(), xferInfo.getFileHashId() );
			}
			else
			{
				LogMsg( LOG_ERROR, "FileInfoXferMgr::onFileReceived ERROR %d moving %s to %s", rc, incompleteFile.c_str(), completedFile.c_str() );
				m_FileInfoMgr.toGuiFileDownloadComplete( xferSession->getLclSessionId(), "", eXferErrorFileMoveError );
			}
		}
		else if( VxFileUtil::fileExists( incompleteFile.c_str() ) )
		{
			m_FileInfoMgr.onFileDownloadComplete( xferSession->getIdent(), xferSession->getSkt(), xferInfo.getLclSessionId(), incompleteFile, xferInfo.getAssetId(), xferInfo.getFileHashId() );
		}
		else
		{
			m_FileInfoMgr.toGuiFileDownloadComplete( xferSession->getLclSessionId(), "", eXferErrorFileMoveError );
		}
	}
	else
	{
		m_FileInfoMgr.toGuiFileDownloadComplete( xferSession->getLclSessionId(), "", xferError );
	}

	endFileXferSession( xferSession );
}

//============================================================================
void FileInfoXferMgr::onFileSent( FileTxSession * xferSession, const char* pFileName, EXferError error )
{
	m_FileInfoMgr.toGuiFileUploadComplete( xferSession->getLclSessionId(), error );
	endFileXferSession( xferSession );

	checkQueForMoreFilesToSend();
}

//============================================================================
void FileInfoXferMgr::checkQueForMoreFilesToSend( void )
{
	//TODO check que and start next xfer
}

//============================================================================
void FileInfoXferMgr::finishFileReceive( FileRxSession * xferSession, PktFileGetCompleteReq * poPkt )
{
	// done receiving file
	xferSession->setErrorCode( poPkt->getError() );
	VxFileXferInfo xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}
	else
	{
		LogMsg( LOG_ERROR,  "FileInfoXferMgr::finishFileReceive: NULL file handle" );
	}

	// let other act on the received file
	std::string strFileName = xferInfo.getLclFileName();
	if( 0 == xferSession->getErrorCode() )
	{
		xferSession->m_astrFilesXfered.push_back( FileToXfer( strFileName, 0, xferInfo.getLclSessionId(), xferInfo.getRmtSessionId(), xferInfo.getAssetId(), xferInfo.getFileHashId(), 0 ) );
	}

	//=== acknowlege ===//
	PktFileSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setAssetId( xferInfo.getAssetId() );
	oPkt.setFileHashId( xferInfo.getFileHashId() );
	oPkt.setError( xferSession->getErrorCode() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_VERBOSE,  "FileInfoXferMgr::finishFileReceive: Done Receiving file %s", strFileName.c_str() );
	onFileReceived( xferSession, strFileName.c_str(), eXferErrorNone );
}

//============================================================================
RCODE FileInfoXferMgr::sendFileShareError(		VxSktBase *		sktBase,		// socket
												int				iPktType,	// type of packet
												unsigned short	u16Cmd,		// packet command
												long			rc,			// error code
												const char*	pMsg, ...)	// error message
{
	// send an error message
	RCODE rcSendErr = 0;
	//build message on stack so no out of memory issue
	char szBuffer[2048];
	va_list argList;
	va_start(argList, pMsg);
	vsnprintf( szBuffer, 2048, pMsg, argList );
	va_end(argList);
	LogMsg( LOG_VERBOSE, szBuffer );
	
	return rcSendErr;
}

//============================================================================
bool FileInfoXferMgr::isFileDownloading( VxSha1Hash& fileHashId )
{
	FileRxIter iter;
	for( iter = m_RxSessions.begin(); iter != m_RxSessions.end(); ++iter )
	{
		FileRxSession * xferSession = iter->second;
		if( xferSession->getFileHashId() == fileHashId )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
bool FileInfoXferMgr::isFileInDownloadFolder( const char* pPartialFileName )
{
	std::string	strFullFileName = VxGetDownloadsDirectory() + pPartialFileName;
	return VxFileUtil::fileExists(strFullFileName.c_str()) ? 1 : 0;
}

//============================================================================
void FileInfoXferMgr::replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	FileTxIter iter;
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	for( iter = m_TxSessions.begin(); iter != m_TxSessions.end(); ++iter )
	{
		FileTxSession * xferSession = (*iter);
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}

	FileRxIter oRxIter;
	for( oRxIter = m_RxSessions.begin(); oRxIter != m_RxSessions.end(); ++oRxIter )
	{
		FileRxSession * xferSession = oRxIter->second;
		if( xferSession->getSkt() == poOldSkt )
		{
			xferSession->setSkt( poNewSkt );
		}
	}
}

//============================================================================
EXferError FileInfoXferMgr::beginFileGet( FileRxSession * xferSession )
{
	if( ( false == xferSession->isXferingFile() ) &&
		xferSession->m_astrFilesToXfer.size() )
	{
		LogMsg( LOG_VERBOSE, "FileInfoXferMgr::beginFileGet %p %s", this, xferSession->m_astrFilesToXfer[0].m_strFileName.c_str() );
		PktFileGetReq oPkt;
		oPkt.setFileName( xferSession->m_astrFilesToXfer[0].m_strFileName );
		oPkt.setLclSessionId( xferSession->m_astrFilesToXfer[0].getLclSessionId() );
		oPkt.setRmtSessionId( xferSession->m_astrFilesToXfer[0].getRmtSessionId() );
		oPkt.setAssetId( xferSession->m_astrFilesToXfer[0].getAssetId() );
		oPkt.setFileHashId( xferSession->m_astrFilesToXfer[0].getFileHashId() );
		return ( m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
												xferSession->getIdent()->getMyOnlineId(), 
												xferSession->getSkt(), 
												&oPkt ) ) ? eXferErrorNone : eXferErrorDisconnected;
	}
	else if( xferSession->isXferingFile() )
	{
		return eXferErrorBusy;
	}
	else if( xferSession->m_astrFilesToXfer.empty() )
	{
		return eXferErrorBadParam;
	}

	return eXferErrorDisconnected;
}

//============================================================================
EXferError FileInfoXferMgr::canTxFile( VxNetIdent* netIdent, VxGUID& assetId, VxSha1Hash& fileHashId  )
{
	EXferError xferErr = eXferErrorFileNotFound;
	//#define FILE_XFER_ERR_BUSY						0x0010
	//#define FILE_XFER_ERR_FILE_NOT_FOUND			0x0020
	//#define FILE_XFER_ERR_PERMISSION				0x0040
	if( m_FileInfoMgr.isFileShared( assetId, fileHashId ) )
	{
		//TODO check for busy and permission
		xferErr = eXferErrorNone;
	}

	return xferErr;
}

//============================================================================
bool FileInfoXferMgr::isViewFileListMatch( FileTxSession * xferSession, FileInfo& fileInfo )
{
	size_t viewDirLen = xferSession->m_strViewDirectory.length();
	bool bRootDir = viewDirLen ? false : true;

	if( fileInfo.getLocalFileName().length() >= viewDirLen )
	{
		if( VXFILE_TYPE_DIRECTORY == fileInfo.m_u8FileType )
		{
			if( bRootDir )
			{
				return true;
			}
			else if( 0 == strncmp( xferSession->m_strViewDirectory.c_str(), fileInfo.getLocalFileName().c_str(), viewDirLen ) )
			{
				if( fileInfo.getLocalFileName().length() == viewDirLen )
				{
					// is this directory
					return false;
				}

				return true;
			}

			return false;
		}
		else
		{
			if( 0 == strncmp( xferSession->m_strViewDirectory.c_str(), fileInfo.getLocalFileName().c_str(), viewDirLen ) )
			{
				const char* pLeftOver = &(fileInfo.getLocalFileName().c_str()[viewDirLen]);
				if( strchr(pLeftOver, '/') )
				{
					// is in one of the sub dirs
					return false;
				}

				return true;
			}
			// not the same dir
			return false;
		}
	}

	return false;
}

/*
//============================================================================
bool FileInfoXferMgr::makeDownloadCompleteFileName( std::string& strRemoteFileName, std::string& strRetDownloadCompleteFileName )
{
	std::string justFileName;
	VxFileUtil::getJustFileName( strRemoteFileName.c_str(), justFileName );
	strRetDownloadCompleteFileName = VxGetDownloadsDirectory() + justFileName;
	while( VxFileUtil::fileExists( strRetDownloadCompleteFileName.c_str() ) )
	{
		if( false == VxFileUtil::incrementFileName( strRetDownloadCompleteFileName ) )
		{
			break;
		}
	}

	return strRetDownloadCompleteFileName.size() ? true : false;
}

//============================================================================
bool FileInfoXferMgr::makeIncompleteFileName( std::string& strRemoteFileName, std::string& strRetIncompleteFileName )
{
	std::string justFileName;
	VxFileUtil::getJustFileName( strRemoteFileName.c_str(), justFileName );
	strRetIncompleteFileName = VxGetIncompleteDirectory() + justFileName;
	while( VxFileUtil::fileExists( strRetIncompleteFileName.c_str() ) )
	{
		if( false == VxFileUtil::incrementFileName( strRetIncompleteFileName ) )
		{
			break;
		}
	}

	return strRetIncompleteFileName.size() ? true : false;
}*/


//============================================================================
bool FileInfoXferMgr::startDownload( FileInfo& fileInfo, VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	bool result{ false };
	if( false == lclSessionId.isVxGUIDValid() )
	{
		lclSessionId.initializeWithNewVxGUID();
	}

	FileRxSession* fileRxSession = findOrCreateRxSession( lclSessionId, netIdent, sktBase );
	if( fileRxSession )
	{
		fileRxSession->setAssetId( fileInfo.getAssetId() );
		fileRxSession->setFileHashId( fileInfo.getFileHashId() );
		FileToXfer fileToXfer( fileInfo.getShortFileName(), 0, lclSessionId, lclSessionId, fileInfo.getAssetId(), fileInfo.getFileHashId(), 0 );

		fileRxSession->m_astrFilesToXfer.push_back( fileToXfer );
		result = beginFileGet( fileRxSession ) == eXferErrorNone;
	}

	return result;
}


//============================================================================
EXferError FileInfoXferMgr::setupFileDownload( VxFileXferInfo& xferInfo, VxNetIdent* netIdent )
{
	EXferError xferErr = eXferErrorNone;
	uint64_t u64FileLen = 0;
	if( false == xferInfo.getLclSessionId().isVxGUIDValid() )
	{
		xferInfo.getLclSessionId().initializeWithNewVxGUID();
	}

	xferInfo.setXferDirection( eXferDirectionRx );
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "FileXferBaseMgr::setupFileDownload: ERROR:(File Receive) receive transfer still in progress" );
		xferErr = eXferErrorAlreadyDownloading;
	}

	std::string rmtFileName = xferInfo.getRmtFileName();
	if( eXferErrorNone == xferErr )
	{
		if( 0 == rmtFileName.length() )
		{
			LogMsg( LOG_ERROR, "FileInfoXferMgr::beginFileReceive: ERROR: No file Name" );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		if( VxFileUtil::isFullPath( rmtFileName.c_str() ) )
		{
			LogMsg( LOG_ERROR, "FileInfoXferMgr::beginFileReceive: ERROR: FULL file Name %s", rmtFileName.c_str() );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		makeIncompleteFileName( rmtFileName, xferInfo.getLclFileName(), netIdent );
		std::string filePath;
		std::string justFileName;
		VxFileUtil::seperatePathAndFile( xferInfo.getLclFileName(), filePath, justFileName );
		VxFileUtil::makeDirectory( filePath );
	}

	if( eXferErrorNone == xferErr )
	{
		u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 != xferInfo.m_u64FileOffs )
		{
			if( u64FileLen < xferInfo.m_u64FileOffs )
			{
				xferErr = eXferErrorBadParam;
				LogMsg( LOG_INFO, "FileXferBaseMgr: ERROR:(File Rx) %d File %s could not be resumed because too short",
					xferErr,
					( const char* )xferInfo.getLclFileName().c_str() );
			}
			else
			{
				xferInfo.m_hFile = fopen( xferInfo.getLclFileName().c_str(), "a+" ); // pointer to name of the file
				if( NULL == xferInfo.m_hFile )
				{
					// failed to open file
					xferInfo.m_hFile = NULL;
					RCODE rc = VxGetLastError();
					xferErr = eXferErrorFileOpenAppendError;

					LogMsg( LOG_INFO, "FileXferBaseMgr: ERROR:(File Rx) %d File %s could not be created",
						rc,
						( const char* )xferInfo.getLclFileName().c_str() );
				}
				else
				{
					// we have valid file so seek to end so we can resume if partial file exists
					RCODE rc = 0;
					if( 0 != ( rc = VxFileUtil::fileSeek( xferInfo.m_hFile, xferInfo.m_u64FileOffs ) ) )
					{
						xferErr = eXferErrorFileOpenAppendError;
						// seek failed
						fclose( xferInfo.m_hFile );
						xferInfo.m_hFile = NULL;
						LogMsg( LOG_INFO, "FileXferBaseMgr: ERROR: (File Rx) could not seek to position %d in file %s",
							xferInfo.m_u64FileOffs,
							( const char* )xferInfo.getLclFileName().c_str() );
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
				xferErr = eXferErrorFileOpenError;
				// failed to open file
				xferInfo.m_hFile = NULL;
				RCODE rc = VxGetLastError();
				LogMsg( LOG_INFO, "FileInfoXferMgr: ERROR: %d File %s could not be created",
					rc,
					( const char* )xferInfo.getLclFileName().c_str() );
			}
		}
	}

	return xferErr;
}

//============================================================================
bool FileInfoXferMgr::makeIncompleteFileName( std::string& strRemoteFileName, std::string& strRetIncompleteFileName, VxNetIdent* netIdent )
{
	VxGUID onlineId;
	if( netIdent )
	{
		onlineId = netIdent->getMyOnlineId();
	}

	std::string justFileName;
	VxFileUtil::getJustFileName( strRemoteFileName.c_str(), justFileName );
	//strRetIncompleteFileName = VxGetIncompleteDirectory() + justFileName;
	strRetIncompleteFileName = m_FileInfoMgr.getIncompleteFileXferDirectory( onlineId ) + justFileName;
	while( VxFileUtil::fileExists( strRetIncompleteFileName.c_str() ) )
	{
		if( false == VxFileUtil::incrementFileName( strRetIncompleteFileName ) )
		{
			break;
		}
	}

	return strRetIncompleteFileName.size() ? true : false;
}

//============================================================================
EXferError FileInfoXferMgr::sendNextFileChunk( VxFileXferInfo& xferInfo, VxNetIdent* netIdent, VxSktBase* skt )
{
	EXferError xferErr = eXferErrorNone;
	PktFileChunkReq oPkt;
	// see how much we can read
	uint32_t u32ChunkLen = ( uint32_t )( xferInfo.m_u64FileLen - xferInfo.m_u64FileOffs );
	if( PKT_TYPE_FILE_MAX_DATA_LEN < u32ChunkLen )
	{
		u32ChunkLen = PKT_TYPE_FILE_MAX_DATA_LEN;
	}

	if( 0 == u32ChunkLen )
	{
		LogMsg( LOG_ERROR, "FileInfoXferMgr::txNextFileChunk 0 len u32ChunkLen" );
		// what to do?
		return eXferErrorNone;
	}

	// read data into packet
	uint32_t u32BytesRead = ( uint32_t )fread( oPkt.m_au8FileChunk,
		1,
		u32ChunkLen,
		xferInfo.m_hFile );
	if( u32BytesRead != u32ChunkLen )
	{
		RCODE rc = VxGetLastError();
		//xferSession->setErrorCode( rc );
		xferErr = eXferErrorFileReadError;

		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
		LogMsg( LOG_INFO, "FileXferBaseMgr: ERROR: %d reading send file at offset %ld when file len %ld file name %s",
			rc,
			xferInfo.m_u64FileOffs,
			xferInfo.m_u64FileLen,
			( const char* )xferInfo.getLclFileName().c_str() );
	}
	else
	{
		xferInfo.m_u64FileOffs += u32ChunkLen;
		oPkt.setChunkLen( ( uint16_t )u32ChunkLen );
		oPkt.setLclSessionId( xferInfo.getLclSessionId() );
		oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
		oPkt.setAssetId( xferInfo.getAssetId() );
	}

	if( eXferErrorNone == xferErr )
	{
		if( false == m_Plugin.txPacket( netIdent, skt, &oPkt ) )
		{
			xferErr = eXferErrorDisconnected;
		}
	}

	if( eXferErrorNone != xferErr )
	{
		m_FileInfoMgr.updateToGuiFileXferState( xferInfo.getLclSessionId(), eXferStateUploadError, xferErr );
	}
	else
	{
		if( xferInfo.calcProgress() )
		{
			m_FileInfoMgr.updateToGuiFileXferState( xferInfo.getLclSessionId(), eXferStateInUploadXfer, eXferErrorNone, xferInfo.getProgress() );
		}
	}

	return xferErr;
}
