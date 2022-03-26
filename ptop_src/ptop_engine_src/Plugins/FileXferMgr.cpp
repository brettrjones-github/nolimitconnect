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

#include "PluginBase.h"
#include "PluginMgr.h"
#include "FileXferMgr.h"
#include "FileXferPluginSession.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <PktLib/PktsPluginOffer.h>
#include <PktLib/PktsFileShare.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileCrc32.h>
#include <CoreLib/VxGlobals.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//============================================================================
FileXferMgr::FileXferMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr )
: FileXferBaseMgr( engine, plugin )
, m_PluginSessionMgr( sessionMgr )
{
}

//============================================================================
void FileXferMgr::fromGuiStartPluginSession( bool pluginIsLocked, PluginSessionBase * xferSession )
{
	if( xferSession->isRmtInitiated() )
	{
		// wait for remote computer to send PktFileSendReq
	}
	else
	{
		// send file send request
		// begin file transfer
		EXferError xferErr = beginFileSend( pluginIsLocked, ((FileXferPluginSession *)xferSession)->getSkt(), (FileXferPluginSession *)xferSession );
		if( eXferErrorNone != xferErr )
		{
			IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateUploadError, xferErr, xferErr );
		}
	}
}

//============================================================================
EXferError FileXferMgr::beginFileSend( bool pluginIsLocked, VxSktBase * sktBase, FileXferPluginSession * xferSession )
{
	PktFileSendReq oPktReq;
	oPktReq.setLclSessionId( xferSession->getLclSessionId() );
	oPktReq.setRmtSessionId( xferSession->getRmtSessionId() );

	EXferError xferErr = eXferErrorNone;
	xferSession->clearErrorCode();
	VxFileXferInfo& xferInfo = xferSession->getTxXferInfo();
	if( xferInfo.m_hFile )
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::beginFileSend: ERROR: File transfer still in progress" );
		xferErr = eXferErrorAlreadyUploading;
	}

	xferInfo.setXferDirection( eXferDirectionTx );
	if( 0 == xferSession->m_astrFilesToSend.size() )
	{
		xferErr = eXferErrorFileNotFound;
	}

	if( eXferErrorNone == xferErr )
	{
		FileToXfer& fileToXfer = xferSession->m_astrFilesToSend[0];
		xferInfo.setLclFileName( fileToXfer.m_strFileName.c_str() );
		VxFileUtil::getJustFileName( fileToXfer.m_strFileName.c_str(), xferInfo.getRmtFileName() );
		xferInfo.setLclSessionId( fileToXfer.getLclSessionId() );
		xferInfo.setRmtSessionId( fileToXfer.getRmtSessionId() );
		xferInfo.setFileHashId( fileToXfer.getFileHashId() );
		xferSession->m_astrFilesToSend.erase( xferSession->m_astrFilesToSend.begin() );

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
			//if( !m_SharedFilesMgr.getFileHashId( xferInfo.getLclFileName(), xferInfo.getFileHashId() ) )
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
					LogMsg( LOG_INFO, "FileShareXferMgr::beginFileSend: could not seek to position %lld in file %s",
						xferInfo.m_u64FileOffs,
						(const char *)xferInfo.getLclFileName().c_str() );
					xferErr = eXferErrorFileSeekError;
					xferSession->setErrorCode( rc );
				}
			}
		}
	}

	oPktReq.setError( xferErr );
	//get file extension
	uint8_t u8FileType = VxFileUtil::fileExtensionToFileTypeFlag( xferInfo.getLclFileName().c_str() );

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
												xferInfo.getLclFileName(),
												xferInfo.getAssetId(),
												xferInfo.getFileHashId() );

		// file is open and setup so send first chunk of data
		return txNextFileChunk( xferSession, eXferErrorNone, pluginIsLocked );
	}
	else
	{
		endFileXferTxSession( xferSession, pluginIsLocked );
		return xferErr;
	}
}

//============================================================================
void FileXferMgr::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{

}

//============================================================================
void FileXferMgr::onPktFileGetReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{

}

//============================================================================
void FileXferMgr::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktFileSendReq * poPkt = (PktFileSendReq *)pktHdr;
	FileXferPluginSession * xferSession = 0;

	{ // scope for lock
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), true );
		}

		if( 0 == xferSession )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true  );
		}

		if( xferSession )
		{
			beginFileReceive( true, sktBase, xferSession, poPkt);
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginBaseFileXfer::onPktFileSendReq: Could not find session\n" );
		}
	}
}

//============================================================================
void FileXferMgr::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktPluginOfferReply * poPkt = (PktPluginOfferReply *)pktHdr;
	FileXferPluginSession * xferSession = 0;

	{ // scope for lock
		VxMutex& pluginMutex = m_Plugin.getPluginMutex();
		pluginMutex.lock();
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), true  );
		}

		if( 0 == xferSession )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true  );
		}

		if( xferSession )
		{
			// notify gui of response
			xferSession->setRmtSessionId( poPkt->getLclSessionId() );
			xferSession->setOfferResponse( poPkt->getOfferResponse() );
			IToGui::getToGui().toGuiRxedOfferReply( 
				xferSession->getIdent(),		// identity of friend
				m_Plugin.getPluginType(),			// plugin
				0,				// plugin defined data
				xferSession->getOfferResponse(),
				xferSession->m_strOfferFile.c_str(),
				0,
				xferSession->getLclSessionId(),
				xferSession->getRmtSessionId() );
			if( eOfferResponseAccept == xferSession->getOfferResponse() )
			{
				fromGuiStartPluginSession( true, xferSession );
			}
			else
			{
				// need to unlock before removing or get a deadlock
				VxGUID lclSessionId = xferSession->getLclSessionId();

				m_PluginSessionMgr.removeSessionBySessionId( true, lclSessionId, xferSession->getOfferResponse()  );
				pluginMutex.unlock();
				return;
			}
		}

		pluginMutex.unlock();
	}
}

//============================================================================
void FileXferMgr::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktFileChunkReq * poPkt = (PktFileChunkReq *)pktHdr;
	FileXferPluginSession * xferSession = 0;
	
	{ // scope for lock
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), true  );
		}

		if( 0 == xferSession )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true  );
		}

		if( xferSession )
		{
			EXferError xferErr = rxFileChunk( xferSession, poPkt );
			if( eXferErrorNone != xferErr )
			{
				PktFileChunkReply pktReply;
				pktReply.setDataLen(0);
				pktReply.setError( xferErr );
				m_Plugin.txPacket( netIdent, sktBase, &pktReply );

				IToGui::getToGui().toGuiFileXferState( getPluginType(), xferSession->getLclSessionId(), eXferStateDownloadError, xferErr, xferErr );
				endFileXferRxSession( xferSession, true );
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "FileShareXferMgr::onPktFileChunkReq failed to find session\n");
			PktFileChunkReply pktReply;
			pktReply.setDataLen(0);
			pktReply.setError( eXferErrorBadParam );
			m_Plugin.txPacket( netIdent, sktBase, &pktReply );
		}
	}
}

//============================================================================
void FileXferMgr::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktFileChunkReply * poPkt = (PktFileChunkReply *)pktHdr;
	FileXferPluginSession * xferSession = 0;
	{ // scope for lock
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
		if( poPkt->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionBySessionId( poPkt->getRmtSessionId(), true  );
		}

		if( 0 == xferSession )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true  );
		}

		txNextFileChunk( xferSession, poPkt->getError(), true );
	}
}

//============================================================================
void FileXferMgr::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFileGetCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{

}

//============================================================================
void FileXferMgr::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktFileSendCompleteReq * pktSendComplete = (PktFileSendCompleteReq *)pktHdr;
	FileXferPluginSession * xferSession = 0;
	{ // scope for lock
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
		if( pktSendComplete->getRmtSessionId().isVxGUIDValid() )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionBySessionId( pktSendComplete->getRmtSessionId(), true  );
		}
		
		if( 0 == xferSession )
		{
			xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true  );
		}

		//TODO check checksum
		if( xferSession )
		{
			finishFileReceive( xferSession, pktSendComplete, true );
		}
	}
}

//============================================================================
void FileXferMgr::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//PktFileSendCompleteReply * poPkt = (PktFileSendCompleteReply *)pktHdr;
	//FileXferPluginSession * xferSession = (FileXferPluginSession *)m_PluginSessionMgr.findSession( netIdent->getMyOnlineId() );
	////TODO check checksum
	//onFileReceived( sktBase, xferSession, xferSession->m_RxFileInfo.getLclFileName().c_str() );
}

//============================================================================
void FileXferMgr::onPktFindFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFindFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFileInfoReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFileInfoReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onPktFileInfoErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void FileXferMgr::onFileSent( FileXferPluginSession * xferSession, std::string pFileName, EXferError xferError, bool pluginIsLocked )
{
	LogMsg( LOG_INFO, "Sent file %s error %d", pFileName.c_str(), xferError );
	IToGui::getToGui().toGuiFileUploadComplete( getPluginType(), xferSession->getLclSessionId(), pFileName, xferError );
	endFileXferTxSession( xferSession, pluginIsLocked );
}

//============================================================================
void FileXferMgr::onFileReceived( FileXferPluginSession * xferSession, std::string pFileName, EXferError xferError, bool pluginIsLocked )
{
	LogMsg( LOG_INFO, "File received %s error %d", pFileName.c_str(), xferError );
	VxFileXferInfo& xferInfo = xferSession->getRxXferInfo();
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
			m_PluginMgr.getEngine().fromGuiAddFileToLibrary( completedFile.c_str(), true, xferInfo.getFileHashId().getHashData() );
			IToGui::getToGui().toGuiFileDownloadComplete( getPluginType(), xferSession->getLclSessionId(), completedFile, eXferErrorNone );
		}
		else
		{
            LogMsg( LOG_ERROR, "FileShareXferMgr::onFileReceived ERROR %d moving %s to %s", rc, incompleteFile.c_str(), completedFile.c_str() );
			IToGui::getToGui().toGuiFileDownloadComplete( getPluginType(), xferSession->getLclSessionId(), incompleteFile, eXferErrorFileMoveError );
		}
	}
	else
	{
		std::string emptyStr{ "" };
		IToGui::getToGui().toGuiFileDownloadComplete( getPluginType(), xferSession->getLclSessionId(), emptyStr, xferError );
	}

	endFileXferRxSession( xferSession, pluginIsLocked );
}

//============================================================================
EXferError FileXferMgr::beginFileReceive( bool pluginIsLocked, VxSktBase * sktBase, FileXferPluginSession * xferSession, PktFileSendReq * poPkt )
{
	if( NULL == xferSession )
	{
		LogMsg( LOG_ERROR, "FileXferMgr::beginFileReceive: NULL skt info" );
		return eXferErrorBadParam;
	}

	VxFileXferInfo& xferInfo = xferSession->m_RxFileInfo;
	xferInfo.setLclSessionId( xferSession->getLclSessionId() );
	xferInfo.setRmtSessionId( poPkt->getRmtSessionId() );
	xferInfo.setFileHashId( poPkt->getFileHashId() );
	xferInfo.setRmtFileName( poPkt->getFileName() );
	xferInfo.m_u64FileLen = poPkt->getFileLen();
	xferInfo.m_u64FileOffs = poPkt->getFileOffset();
	xferInfo.m_AssetId = poPkt->getAssetId();

	EXferError xferErr = setupFileDownload( xferInfo );

	if( eXferErrorNone == xferErr )
	{
		LogMsg( LOG_INFO, "FileXferMgr::(File Send) start recieving file %s\n", 
			(const char *)xferInfo.getLclFileName().c_str() );

		IToGui::getToGui().toGuiStartDownload( xferSession->getIdent(), 
									m_Plugin.getPluginType(), 
									xferInfo.getLclSessionId(), 
									poPkt->getFileType(), 
									xferInfo.m_u64FileLen, 
									xferInfo.getRmtFileName(),
									xferInfo.getAssetId(),
									xferInfo.getFileHashId() );
	}

	// don't send reply.. will get file chunk next anyway
	return xferErr;
}

//============================================================================
EXferError FileXferMgr::txNextFileChunk( FileXferPluginSession * xferSession, uint32_t remoteErr, bool pluginIsLocked )
{
	if( 0 == xferSession )
	{
		return eXferErrorBadParam;
	}

	EXferError xferErr = eXferErrorNone;
	// fill the packet with data from the file
	VxFileXferInfo& xferInfo = xferSession->getTxXferInfo();
	if( 0 != remoteErr )
	{
		// canceled download by remote user
		LogMsg( LOG_INFO, "FileShareXferMgr:: Cancel Sending file %s", xferInfo.getLclFileName().c_str() );
		onFileSent( xferSession, xferInfo.getLclFileName().c_str(), eXferErrorCanceled, pluginIsLocked );
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

		PktFileSendCompleteReq oPkt;
		oPkt.setLclSessionId( xferInfo.getLclSessionId() );
		oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
		oPkt.setFileHashId( xferInfo.getFileHashId() );
		m_Plugin.txPacket(  xferSession->getIdent(), xferSession->getSkt(), &oPkt );

		LogMsg( LOG_INFO, "FileShareXferMgr:: Done Sending file %s", xferInfo.getLclFileName().c_str() );
		IToGui::getToGui().toGuiFileUploadComplete( getPluginType(), xferInfo.getLclSessionId(), xferInfo.getLclFileName(), eXferErrorNone );
		onFileSent( xferSession, xferInfo.getLclFileName().c_str(), eXferErrorNone, pluginIsLocked );
		return eXferErrorNone;
	}

	xferErr = sendNextFileChunk( xferInfo, xferSession->getIdent(), xferSession->getSkt() );
	return xferErr;
}

//============================================================================
EXferError FileXferMgr::rxFileChunk( FileXferPluginSession * xferSession, PktFileChunkReq * poPkt )
{
	if( 0 == xferSession )
	{
		return eXferErrorBadParam;
	}

	VxFileXferInfo& xferInfo = xferSession->getRxXferInfo();
	EXferError xferErr = (EXferError)poPkt->getError();
	if( eXferErrorNone != xferErr )
	{
		// canceled by sender
		IToGui::getToGui().toGuiFileXferState( getPluginType(), xferInfo.getLclSessionId(), eXferStateDownloadError, xferErr, xferErr );
		return xferErr;
	}

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
void FileXferMgr::finishFileReceive( FileXferPluginSession * xferSession, PktFileSendCompleteReq * poPkt, bool pluginIsLocked  )
{
	// done receiving file
	VxFileXferInfo& xferInfo = xferSession->getRxXferInfo();
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
	xferSession->m_astrFilesReceived.push_back( 
		FileToXfer(strFileName, 0, xferInfo.getLclSessionId(), 
		xferInfo.getRmtSessionId(), xferInfo.getAssetId(), xferInfo.getFileHashId(), 0 ) );

	//=== acknowlege ===//
	PktFileSendCompleteReply oPkt;
	oPkt.setLclSessionId( xferInfo.getLclSessionId() );
	oPkt.setRmtSessionId( xferInfo.getRmtSessionId() );
	oPkt.setFileHashId( xferSession->getFileHashId() );
	m_Plugin.txPacket( xferSession->getIdent(), xferSession->getSkt(), &oPkt );
	LogMsg( LOG_INFO,  "VxPktHandler: Done Receiving file %s", strFileName.c_str() );

	xferSession->setErrorCode( poPkt->getError() );
	onFileReceived( xferSession, strFileName.c_str(), (EXferError)poPkt->getError(), pluginIsLocked );
}

//============================================================================
long FileXferMgr::sendFileXferError(	VxSktBase *		sktBase,		// socket
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
	//		m_Plugin.txPacket( sktBase, &gPkt );
	//		break;
	//	}
	//case PKT_TYPE_CMD_1ARG_REQ:
	//	{
	//		// make a reply packet
	//		PktCmd1ArgReply gPkt;
	//		gPkt.u16Cmd = u16Cmd;
	//		gPkt.u64Error = rc;
	//		strcpy( gPkt.as8Arg1, szBuffer );
	//		m_Plugin.txPacket( sktBase, &gPkt );
	//		break;
	//	}
	//case PKT_TYPE_CMD_2ARG_REQ:
	//	{
	//		// make a reply packet
	//		PktCmd2ArgReply gPkt;
	//		gPkt.u16Cmd = u16Cmd;
	//		gPkt.u32Error = rc;
	//		strcpy( gPkt.as8Arg1, szBuffer );
	//		m_Plugin.txPacket( sktBase, &gPkt );
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
bool FileXferMgr::endFileXferRxSession( FileXferPluginSession * xferSession, bool pluginIsLocked )
{
	VxFileXferInfo& xferInfo = xferSession->getRxXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	std::string fileName = xferInfo.getLclFileName(); // this should be the incomplete directory file.. completed file should already be copied
	if( fileName.length() )
	{
		VxFileUtil::deleteFile( fileName.c_str() );
	}

	if( xferInfo.getLclSessionId().isVxGUIDValid() )
	{
		m_PluginSessionMgr.endPluginSession( xferInfo.getLclSessionId(), pluginIsLocked );
	}
	else
	{
		m_PluginSessionMgr.endPluginSession( xferSession, pluginIsLocked );
	}

	return 0;
}

//============================================================================
bool FileXferMgr::endFileXferTxSession( FileXferPluginSession * xferSession, bool pluginIsLocked )
{
	VxFileXferInfo& xferInfo = xferSession->getTxXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile = NULL;
	}

	if( xferInfo.getLclSessionId().isVxGUIDValid() )
	{
		m_PluginSessionMgr.endPluginSession( xferInfo.getLclSessionId(), pluginIsLocked );
	}
	else
	{
		m_PluginSessionMgr.endPluginSession( xferSession, pluginIsLocked );
	}

	return 0;
}
/*
//============================================================================
bool FileXferMgr::makeDownloadCompleteFileName( std::string& strRemoteFileName, std::string& strRetDownloadCompleteFileName )
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
*/
