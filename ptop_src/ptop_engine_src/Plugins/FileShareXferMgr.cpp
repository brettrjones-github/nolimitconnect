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
// http://www.nolimitconnect.com
//============================================================================

#include "FileShareXferMgr.h"
#include "FileLibraryMgr.h"

#include "PluginBase.h"
#include "PluginMgr.h"
#include "FileXferMgr.h"
#include "FileTxSession.h"
#include "FileRxSession.h"

#include "SharedFilesMgr.h"
#include "SharedFileInfo.h"

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
FileShareXferMgr::FileShareXferMgr( P2PEngine& engine, 
                                    PluginBase&			plugin, 
									SharedFilesMgr&		sharedFilesMgr,
									FileLibraryMgr&		fileLibraryMgr )
: FileXferBaseMgr( engine, plugin )
, m_SharedFilesMgr( sharedFilesMgr )
, m_FileLibraryMgr( fileLibraryMgr )
, m_bIsInSession(true)
, m_bIsInitialized(false)
//, m_eFileRxOption(eFileXOptionReplaceIfExists)
{
}

//============================================================================
FileShareXferMgr::~FileShareXferMgr()
{
	clearRxSessionsList();
	clearTxSessionsList();
}

//============================================================================
EPluginType FileShareXferMgr::gePluginType( void )
{
	return m_Plugin.getPluginType();
}

//============================================================================
void FileShareXferMgr::clearRxSessionsList( void )
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
void FileShareXferMgr::clearTxSessionsList( void )
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
void FileShareXferMgr::fromGuiUserLoggedOn( void )
{
	m_FileShareSettings.loadSettings( m_Engine.getEngineSettings() );
}

//============================================================================
// returns -1 if unknown else percent downloaded
int FileShareXferMgr::fromGuiGetFileDownloadState( uint8_t * fileHashId )
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
void FileShareXferMgr::fromGuiStartPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	if( false == m_bIsInSession )
	{
		m_bIsInSession = true;
	}
}

//============================================================================
void FileShareXferMgr::fromGuiStopPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	if( true == m_bIsInSession )
	{
		m_bIsInSession = false;
	}
}

//============================================================================
bool FileShareXferMgr::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_bIsInSession;
}

//============================================================================
void FileShareXferMgr::fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings )
{
	fileShareSettings = m_FileShareSettings;
}

//============================================================================
void FileShareXferMgr::fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings )
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
void FileShareXferMgr::fileAboutToBeDeleted( std::string& fileName )
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
void FileShareXferMgr::fromGuiCancelDownload( VxGUID& lclSessionId )
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
void FileShareXferMgr::fromGuiCancelUpload( VxGUID& lclSessionId )
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
bool FileShareXferMgr::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		// identity of friend
												int				pvUserData,
												const char *	pOfferMsg,		// offer message
												const char *	pFileName,
												uint8_t *		fileHashId,
												VxGUID			lclSessionId )	
{
	VxSktBase * sktBase = NULL;
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
				xferSession->m_astrFilesToXfer.push_back( FileToXfer(strFileName, 0, lclSessionId, lclSessionId, lclSessionId, fileHashId, pvUserData ) );
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
int FileShareXferMgr::fromGuiPluginControl(	VxNetIdent *		netIdent,
											const char *		pControl, 
											const char *		pAction,
											uint32_t					u32ActionData,
											VxGUID&				sessionId,
											uint8_t *				fileHashIdIn )
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
			LogMsg( LOG_ERROR, "FileShareXferMgr::fromGuiPluginControl could not find session\n" );
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

		VxSktBase * sktBase = NULL;
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
void FileShareXferMgr::onConnectionLost( VxSktBase * sktBase )
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
void FileShareXferMgr::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	//PktPluginOfferReq * poPkt = (PktPluginOfferReq *)pktHdr;
	//FileTxSession * xferSession = findOrCreateTxSession( netIdent, sktBase );
	//PktFileListReq oPkt;
	//m_Plugin.txPacket( netIdent, xferSession->getSkt(), &oPkt );
}

//============================================================================
//! packet with remote users reply to offer
void FileShareXferMgr::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktPluginOfferReply * poPkt = (PktPluginOfferReply *)pktHdr;
	FileTxSession * xferSession = findTxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		IToGui::getToGui().toGuiRxedOfferReply(	netIdent,		// identity of friend
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
													xferSession->getLclSessionId(),
													xferSession->getFileHashId(),
													0 ) );
			EXferError xferErr = beginFileSend( xferSession );
			if( eXferErrorNone != xferErr )
			{
				IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateUploadError, xferErr, xferErr );
			}
		}
		else
		{
			endFileXferSession( xferSession );
		}
	}
}

//============================================================================
void FileShareXferMgr::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktFileGetReq * poPkt = (PktFileGetReq *)pktHdr;
	PktFileGetReply oPkt;

	std::string strLclFileName;
	std::string rmtFileName;
	poPkt->getFileName( rmtFileName );
	if( false == m_SharedFilesMgr.getFileFullName( poPkt->getFileHashId(), strLclFileName ) )
	{
		LogMsg( LOG_INFO, "FileXferMgr::onPktFileGetReq file no longer shared %s", rmtFileName.c_str() );
		oPkt.setError( eXferErrorFileNotFound );
	}
	else
	{
		oPkt.setError( canTxFile( netIdent, poPkt->getFileHashId() ) );
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
		oPkt.setFileHashId( xferInfo.getFileHashId() );
		oPkt.setStartOffset( poPkt->getStartOffset() );

		EXferError xferErr  = ( m_Plugin.txPacket( netIdent, sktBase, &oPkt ) ) ? eXferErrorNone : eXferErrorDisconnected;
		if( eXferErrorNone == xferErr )
		{
			xferErr = beginFileSend( xferSession );
		}

		if( eXferErrorNone != xferErr )
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), poPkt->getLclSessionId(), eXferStateUploadError, xferErr, xferErr );
			endFileXferSession( xferSession );
		}
		else
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), poPkt->getLclSessionId(), eXferStateBeginUpload, 0, 0 );
			IToGui::getToGui().toGuiFileXferState( getPluginType(), poPkt->getLclSessionId(), eXferStateInUploadXfer, 0, 0 );
		}
	}
	else
	{
		m_Plugin.txPacket( netIdent, sktBase, &oPkt );
	}
}

//============================================================================
void FileShareXferMgr::onPktFileGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileGetReply\n");
}

//============================================================================
void FileShareXferMgr::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileSendReq\n");
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
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateDownloadError, xferErr, xferErr );
			endFileXferSession( xferSession );
		}
		else
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateBeginDownload, 0, 0 );
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateInDownloadXfer, 0, 0 );
		}

		pktReply.setError( xferErr );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginBaseFileXfer::onPktFileSendReq: Could not find session\n" );
		pktReply.setError( eXferErrorBadParam );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
	}
}

//============================================================================
void FileShareXferMgr::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileSendReply\n" );
	PktPluginOfferReply * poPkt = (PktPluginOfferReply *)pktHdr;
	FileTxSession * xferSession = findTxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		// notify gui of response
		IToGui::getToGui().toGuiRxedOfferReply(	
													xferSession->getIdent(),		// identity of friend
													m_Plugin.getPluginType(),			// plugin
													0,					// plugin defined data
													poPkt->getOfferResponse(),
													xferSession->m_strOfferFile.c_str(),
													0,
													xferSession->getLclSessionId(),
													xferSession->getRmtSessionId() );
		if( eOfferResponseAccept == poPkt->getOfferResponse() )
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateBeginUpload, 0, 0 );
			EXferError xferErr = beginFileSend( xferSession );
			if( eXferErrorNone != xferErr )
			{
				IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateUploadError, xferErr, xferErr );
				endFileXferSession( xferSession );
			}
		}
		else
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateUploadOfferRejected, 0, 0 );
			endFileXferSession( xferSession );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::onPktFileSendReply failed to find session");
	}
}

//============================================================================
void FileShareXferMgr::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileChunkReq\n");
	PktFileChunkReq * poPkt = (PktFileChunkReq *)pktHdr;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		EXferError xferErr = rxFileChunk( xferSession, poPkt );
		if( eXferErrorNone != xferErr )
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateDownloadError, xferErr, xferErr );
			endFileXferSession( xferSession );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::onPktFileChunkReq failed to find session");
		PktFileChunkReply pktReply;
		pktReply.setDataLen(0);
		pktReply.setError( eXferErrorBadParam );
		m_Plugin.txPacket( netIdent, sktBase, &pktReply );
	}
}

//============================================================================
void FileShareXferMgr::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
static int cnt = 0;
	cnt++;
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileChuckReply start %d", cnt );
    PktFileChunkReply * poPkt = (PktFileChunkReply *)pktHdr;
	FileTxSession * xferSession = findTxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		EXferError xferErr = txNextFileChunk( xferSession );
		if( eXferErrorNone != xferErr )
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateUploadError, xferErr, xferErr );
			endFileXferSession( xferSession );
		}
	}

	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileChuckReply done %d\n", cnt );
}

//============================================================================
void FileShareXferMgr::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileGetCompleteReq\n");
	PktFileGetCompleteReq * poPkt = (PktFileGetCompleteReq *)pktHdr;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	if( xferSession )
	{
		finishFileReceive( xferSession, poPkt );
	}
}

//============================================================================
void FileShareXferMgr::onPktFileGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileGetCompleteReply\n");
}

//============================================================================
void FileShareXferMgr::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileSendCompleteReq\n");
	PktFileSendCompleteReq * poPkt = (PktFileSendCompleteReq *)pktHdr;
	FileRxSession * xferSession = findRxSession( poPkt->getRmtSessionId() );
	//TODO check checksum
	if( xferSession )
	{
		finishFileReceive( xferSession, poPkt );
	}
}

//============================================================================
void FileShareXferMgr::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileSendCompleteReply\n");
}

//============================================================================
void FileShareXferMgr::onPktFindFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFindFileReq\n");
}

//============================================================================
void FileShareXferMgr::onPktFindFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFindFileReply\n");
}

//============================================================================
void FileShareXferMgr::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	PktFileListReq * poPkt = (PktFileListReq *)pktHdr;
	uint32_t reqListIdx = poPkt->getListIndex();
	m_SharedFilesMgr.lockFileListPackets();
	std::vector<PktFileListReply*>& pktList = m_SharedFilesMgr.getFileListPackets();
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

	m_SharedFilesMgr.unlockFileListPackets();
}

//============================================================================
void FileShareXferMgr::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
	FileRxSession * xferSession = findOrCreateRxSession( netIdent, sktBase );
	PktFileListReply * poPkt = (PktFileListReply *)pktHdr;
	RCODE rc = poPkt->getError();
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::onPktFileListReply error %d\n", rc );
		IToGui::getToGui().toGuiFileListReply( netIdent, m_Plugin.getPluginType(), 0, 0, "", VxGUID::nullVxGUID(), 0 );
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
		IToGui::getToGui().toGuiFileListReply( netIdent, 
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
		IToGui::getToGui().toGuiFileListReply( netIdent, m_Plugin.getPluginType(), 0, 0, "", VxGUID::nullVxGUID(), 0 );
		endFileXferSession( xferSession );
	}
}

//============================================================================
void FileShareXferMgr::onPktFileInfoReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileInfoReq\n");
}

//============================================================================
void FileShareXferMgr::onPktFileInfoReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileInfoReply\n");
}

//============================================================================
void FileShareXferMgr::onPktFileInfoErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "FileShareXferMgr::onPktFileInfoErr\n");
}

//============================================================================
void FileShareXferMgr::endFileXferSession( FileRxSession * poSessionIn )
{
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

	FileRxIter oRxIter = m_RxSessions.begin();
#if defined(TARGET_OS_WINDOWS)
	while( oRxIter != m_RxSessions.end() )
	{
		FileRxSession * xferSession = oRxIter->second;
		if( poSessionIn == xferSession )
		{
			delete xferSession;
			oRxIter = m_RxSessions.erase( oRxIter );
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
void FileShareXferMgr::endFileXferSession( FileTxSession * poSessionIn )
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
FileRxSession * FileShareXferMgr::findRxSession( VxNetIdent * netIdent )
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
FileRxSession * FileShareXferMgr::findRxSession( VxGUID& lclSessionId )
{
	FileRxIter iter = m_RxSessions.find( lclSessionId );
	if( iter != m_RxSessions.end() )
	{
		return iter->second;
	}

	return NULL;
}

//============================================================================
FileRxSession *	FileShareXferMgr::findOrCreateRxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	FileRxSession * xferSession = findRxSession( netIdent );
	if( NULL == xferSession )
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
FileRxSession *	FileShareXferMgr::findOrCreateRxSession( VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
{
	FileRxSession * xferSession = findRxSession( lclSessionId );
	if( ( NULL == xferSession ) && ( lclSessionId.isVxGUIDValid() ) )
	{
		xferSession = new FileRxSession( lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}

	if( NULL == xferSession )
	{
		xferSession = findRxSession( netIdent );
	}

	if( NULL == xferSession )
	{
		xferSession = new FileRxSession( lclSessionId, sktBase, netIdent );
		m_RxSessions.insert( std::make_pair( xferSession->getLclSessionId(), xferSession ) );
	}
	else
	{
		xferSession->setSkt( sktBase );
	}

	return xferSession;
}

//============================================================================
FileTxSession * FileShareXferMgr::findTxSession( VxNetIdent * netIdent )
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
FileTxSession * FileShareXferMgr::findTxSession( VxGUID& lclSessionId )
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
FileTxSession *	FileShareXferMgr::createTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	return new FileTxSession( sktBase, netIdent );
}

//============================================================================
FileTxSession *	FileShareXferMgr::findOrCreateTxSession( VxNetIdent * netIdent, VxSktBase * sktBase )
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
FileTxSession *	 FileShareXferMgr::findOrCreateTxSession(  VxGUID& lclSessionId, VxNetIdent * netIdent, VxSktBase * sktBase )
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
EXferError FileShareXferMgr::beginFileSend( FileTxSession * xferSession )
{
	PktFileSendReq oPktReq;

	EXferError xferErr = eXferErrorNone;
	xferSession->setErrorCode( 0 );
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::beginFileSend: ERROR: File transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	xferInfo.setXferDirection( eXferDirectionTx );
	if( eXferErrorNone == xferErr )
	{
		xferInfo.m_u64FileLen = VxFileUtil::getFileLen( xferInfo.getLclFileName().c_str() );
		if( 0 == xferInfo.m_u64FileLen )
		{
			// no file found to send
			LogMsg( LOG_INFO, "FileShareXferMgr::beginFileSend: File %s not found to send", xferInfo.getLclFileName().c_str() );
			xferErr = eXferErrorFileNotFound;
		}
		else if( false == xferInfo.getFileHashId().isHashValid() )
		{
			// see if we can get hash from shared files
			if( !m_SharedFilesMgr.getFileHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
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
			LogMsg( LOG_INFO, "FileShareXferMgr::beginFileSend: Could not open File %s", xferInfo.getLclFileName().c_str() );
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
				LogMsg( LOG_INFO, "FileShareXferMgr::beginFileSend: File %s could not be resumed because too short", 
					(const char *)xferInfo.getLclFileName().c_str() );
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
					LogMsg( LOG_INFO, "FileShareXferMgr::beginFileSend: could not seek to position %d in file %s",
						xferInfo.m_u64FileOffs,
						(const char *)xferInfo.getLclFileName().c_str() );
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
	oPktReq.setFileName( xferInfo.getRmtFileName().c_str() );
	oPktReq.setFileOffset( xferInfo.getFileOffset() );
	oPktReq.setAssetId( xferInfo.getAssetId() );
	oPktReq.setFileHashId( xferInfo.getFileHashId() );

	if( false == m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPktReq ) )
	{
		if( eXferErrorNone == xferErr )
		{
			xferErr = eXferErrorDisconnected;
		}
	}

	LogMsg( LOG_INFO, "FileShareXferMgr::beginFileSend: start sending file %s to %s", 
						xferInfo.getLclFileName().c_str(),
						xferSession->getIdent()->getOnlineName() );

	if( eXferErrorNone == xferErr )
	{
		IToGui::getToGui().toGuiStartUpload(	xferSession->getIdent(), 
													m_Plugin.getPluginType(), 
													xferInfo.getLclSessionId(), 
													u8FileType, 
													xferInfo.m_u64FileLen, 
													xferInfo.getRmtFileName(),
													xferInfo.getAssetId(),
													xferInfo.getFileHashId() );

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
EXferError FileShareXferMgr::beginFileReceive( FileRxSession * rxSession, PktFileSendReq * poPkt )
{
	if( NULL == rxSession )
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::beginFileReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	VxFileXferInfo& xferInfo = rxSession->getXferInfo();
	xferInfo.setRmtSessionId( poPkt->getLclSessionId() );
	xferInfo.setFileHashId( poPkt->getFileHashId() );
	xferInfo.setRmtFileName( poPkt->getFileName() );
	xferInfo.m_u64FileLen = poPkt->getFileLen();
	xferInfo.m_u64FileOffs = poPkt->getFileOffset();

	if( poPkt->getError() )
	{
		xferErr = (EXferError)poPkt->getError();
		IToGui::getToGui().toGuiFileXferState( getPluginType(), poPkt->getRmtSessionId(), eXferStateDownloadError, xferErr, xferErr );
		endFileXferSession( rxSession );
		return xferErr;
	}

	xferErr = setupFileDownload( xferInfo );

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_INFO, "FileShareXferMgr::(File Send) start recieving file %s\n", 
			(const char *)xferInfo.getLclFileName().c_str() );

		uint8_t u8FileType = VxFileUtil::fileExtensionToFileTypeFlag( xferInfo.getRmtFileName().c_str() );
		IToGui::getToGui().toGuiStartDownload( 
													rxSession->getIdent(), 
													m_Plugin.getPluginType(), 
													xferInfo.getLclSessionId(), 
													u8FileType, 
													xferInfo.m_u64FileLen, 
													xferInfo.getRmtFileName(),
													xferInfo.getAssetId(),
													xferInfo.getFileHashId() );
	}

	// don't send reply.. will get file chunk next anyway
	return xferErr;
}

//============================================================================
EXferError FileShareXferMgr::txNextFileChunk( FileTxSession * xferSession )
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
		oPkt.setFileHashId( xferSession->getFileHashId() );
		m_Plugin.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_ERROR, "FileShareXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		IToGui::getToGui().toGuiFileUploadComplete( getPluginType(), xferInfo.getLclSessionId(), xferInfo.getLclFileName(), eXferErrorNone );
		onFileSent( xferSession, xferInfo.getLclFileName().c_str(), eXferErrorNone );
		return eXferErrorNone;
	}

	xferErr = sendNextFileChunk( xferInfo, xferSession->getIdent(), xferSession->getSkt() );
	return xferErr;
}

//============================================================================
EXferError FileShareXferMgr::rxFileChunk( FileRxSession * xferSession, PktFileChunkReq * poPkt )
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

			LogMsg( LOG_INFO, "VxPktHandler::RxFileChunk: ERROR %d: writing to file %s",
							rc,
							(const char *)xferInfo.getLclFileName().c_str() );
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
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferInfo.getLclSessionId(), eXferStateInDownloadXfer, xferInfo.getProgress(), 0 );
		}
	}
	else
	{
		IToGui::getToGui().toGuiFileXferState( getPluginType(), xferInfo.getLclSessionId(), eXferStateDownloadError, xferErr, xferErr );
	}

	return xferErr;
}

//============================================================================
void FileShareXferMgr::finishFileReceive( FileRxSession * xferSession, PktFileSendCompleteReq * poPkt )
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
		LogMsg( LOG_ERROR,  "FileShareXferMgr::finishFileReceive: NULL file handle" );
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
	oPkt.setAssetId( xferInfo.getAssetId() );
	oPkt.setFileHashId( xferSession->getFileHashId() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strFileName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onFileReceived( xferSession, strFileName.c_str(), (EXferError)poPkt->getError() );
}

//============================================================================
void FileShareXferMgr::onFileReceived( FileRxSession * xferSession, std::string pFileName, EXferError xferError)
{
	VxFileXferInfo& xferInfo = xferSession->getXferInfo();
	if( eXferErrorNone == xferError )
	{
		std::string incompleteFile = xferInfo.getLclFileName();
		std::string completedFile = xferInfo.getDownloadCompleteFileName();
		if( VxFileUtil::fileExists( completedFile.c_str() ) )
		{
			completedFile =  VxFileUtil::makeUniqueFileName( completedFile.c_str() );
		}

		RCODE rc = 0;
		if( 0 == ( rc = VxFileUtil::moveAFile( incompleteFile.c_str(), completedFile.c_str() ) ) )
		{
			m_FileLibraryMgr.fromGuiAddFileToLibrary( completedFile.c_str(), true, xferInfo.getFileHashId().getHashData() );
			IToGui::getToGui().toGuiFileDownloadComplete( gePluginType(), xferSession->getLclSessionId(), completedFile, eXferErrorNone );
		}
		else
		{
			LogMsg( LOG_ERROR, "FileShareXferMgr::onFileReceived ERROR %d moving %s to %s", rc, incompleteFile.c_str(), completedFile.c_str() );
			IToGui::getToGui().toGuiFileDownloadComplete( gePluginType(), xferSession->getLclSessionId(), incompleteFile, eXferErrorFileMoveError );
		}
	}
	else
	{
		IToGui::getToGui().toGuiFileDownloadComplete( gePluginType(), xferSession->getLclSessionId(), xferInfo.getRmtFileName(), xferError );
	}

	endFileXferSession( xferSession );
}

//============================================================================
void FileShareXferMgr::onFileSent( FileTxSession * xferSession, std::string pFileName, EXferError error )
{
	IToGui::getToGui().toGuiFileUploadComplete( gePluginType(), xferSession->getLclSessionId(), pFileName, error );
	endFileXferSession( xferSession );

	checkQueForMoreFilesToSend();
}

//============================================================================
void FileShareXferMgr::checkQueForMoreFilesToSend( void )
{
	//TODO check que and start next xfer
}

//============================================================================
void FileShareXferMgr::finishFileReceive( FileRxSession * xferSession, PktFileGetCompleteReq * poPkt )
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
		LogMsg( LOG_ERROR,  "FileShareXferMgr::finishFileReceive: NULL file handle" );
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
	oPkt.setFileHashId( xferInfo.getFileHashId() );
	oPkt.setError( xferSession->getErrorCode() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strFileName.c_str() );
	onFileReceived( xferSession, strFileName.c_str(), eXferErrorNone );
}

//============================================================================
RCODE FileShareXferMgr::sendFileShareError(		VxSktBase *		sktBase,		// socket
												int				iPktType,	// type of packet
												unsigned short	u16Cmd,		// packet command
												long			rc,			// error code
												const char *	pMsg, ...)	// error message
{
	// send an error message
	RCODE rcSendErr = 0;
	//build message on stack so no out of memory issue
	char szBuffer[2048];
	va_list argList;
	va_start(argList, pMsg);
	vsnprintf( szBuffer, 2048, pMsg, argList );
	va_end(argList);
	LogMsg( LOG_INFO, szBuffer );
	// now send the message with correct reply depending on type of packet received

	//switch( iPktType )
	//{	
	//case PKT_TYPE_FILE_CHUNK:
	//	{
	//		PktFileChunkAck gPkt;
	//		gPkt.u32Error = rc;
	//		strcpy( gPkt.as8Arg1, szBuffer );
	//		m_Plugin->m_Plugin.txPacket( sktBase, &gPkt );
	//		break;
	//	}
	//case PKT_TYPE_CMD_1ARG_REQ:
	//	{
	//		// make a reply packet
	//		PktCmd1ArgReply gPkt;
	//		gPkt.u16Cmd = u16Cmd;
	//		gPkt.u64Error = rc;
	//		strcpy( gPkt.as8Arg1, szBuffer );
	//		m_Plugin->m_Plugin.txPacket( sktBase, &gPkt );
	//		break;
	//	}
	//case PKT_TYPE_CMD_2ARG_REQ:
	//	{
	//		// make a reply packet
	//		PktCmd2ArgReply gPkt;
	//		gPkt.u16Cmd = u16Cmd;
	//		gPkt.u32Error = rc;
	//		strcpy( gPkt.as8Arg1, szBuffer );
	//		m_Plugin->m_Plugin.txPacket( sktBase, &gPkt );
	//		break;
	//	}
	//default:
	//	{
	//		// must never happen
	//		vx_assert( false );
	//		LogMsg( LOG_ERROR, "ERROR: unrecognized packet type in Send Error" );
	//		break;
	//	}
	//}// end of switch
	return rcSendErr;
}

//============================================================================
bool FileShareXferMgr::isFileDownloading( VxSha1Hash& fileHashId )
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
bool FileShareXferMgr::isFileInDownloadFolder( const char * pPartialFileName )
{
	std::string	strFullFileName = VxGetDownloadsDirectory() + pPartialFileName;
	return VxFileUtil::fileExists(strFullFileName.c_str()) ? 1 : 0;
}

//============================================================================
void FileShareXferMgr::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
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
EXferError FileShareXferMgr::beginFileGet( FileRxSession * xferSession )
{
	if( ( false == xferSession->isXferingFile() ) &&
		xferSession->m_astrFilesToXfer.size() )
	{
		PktFileGetReq oPkt;
		oPkt.setFileName( xferSession->m_astrFilesToXfer[0].m_strFileName );
		oPkt.setLclSessionId( xferSession->m_astrFilesToXfer[0].getLclSessionId() );
		oPkt.setRmtSessionId( xferSession->m_astrFilesToXfer[0].getRmtSessionId() );
		oPkt.setFileHashId( xferSession->m_astrFilesToXfer[0].getFileHashId() );
		return ( m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
												xferSession->getIdent()->getMyOnlineId(), 
												xferSession->getSkt(), 
												&oPkt ) ) ? eXferErrorNone : eXferErrorDisconnected;
	}

	return eXferErrorNone;
}

//============================================================================
EXferError FileShareXferMgr::canTxFile( VxNetIdent * netIdent, VxSha1Hash& fileHashId  )
{
	EXferError xferErr = eXferErrorFileNotFound;
	//#define FILE_XFER_ERR_BUSY						0x0010
	//#define FILE_XFER_ERR_FILE_NOT_FOUND			0x0020
	//#define FILE_XFER_ERR_PERMISSION				0x0040
	if( m_SharedFilesMgr.isFileShared( fileHashId ) )
	{
		//TODO check for busy and permission
		xferErr = eXferErrorNone;
	}

	return xferErr;
}

//============================================================================
bool FileShareXferMgr::isViewFileListMatch( FileTxSession * xferSession, SharedFileInfo& fileInfo )
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
				const char * pLeftOver = &(fileInfo.getLocalFileName().c_str()[viewDirLen]);
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
bool FileShareXferMgr::makeDownloadCompleteFileName( std::string& strRemoteFileName, std::string& strRetDownloadCompleteFileName )
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
bool FileShareXferMgr::makeIncompleteFileName( std::string& strRemoteFileName, std::string& strRetIncompleteFileName )
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

