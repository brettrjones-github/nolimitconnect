//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
#include "FileXferBaseMgr.h"
#include "FileXferPluginSession.h"

#include <GuiInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <PktLib/PktsPluginOffer.h>
#include <PktLib/PktsFileShare.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileCrc32.h>
#include <CoreLib/VxGlobals.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//============================================================================
FileXferBaseMgr::FileXferBaseMgr( P2PEngine& engine, PluginBase& plugin )
: m_Engine( engine )
, m_Plugin( plugin )
, m_PluginMgr( engine.getPluginMgr() )
{
}

//============================================================================
EXferError FileXferBaseMgr::setupFileDownload( VxFileXferInfo& xferInfo )
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
			LogMsg( LOG_ERROR, "FileShareXferMgr::beginFileReceive: ERROR: No file Name\n" );
			xferErr = eXferErrorBadParam;
		}	
	}

	if( eXferErrorNone == xferErr )
	{
		if( VxFileUtil::isFullPath( rmtFileName.c_str() ))
		{
			LogMsg( LOG_ERROR, "FileShareXferMgr::beginFileReceive: ERROR: FULL file Name %s\n",  rmtFileName.c_str() );
			xferErr = eXferErrorBadParam;
		}
	}

	if( eXferErrorNone == xferErr )
	{
		makeIncompleteFileName( rmtFileName, xferInfo.getLclFileName() );
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
					(const char *)xferInfo.getLclFileName().c_str() );
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
						(const char *)xferInfo.getLclFileName().c_str() );
				}
				else
				{
					// we have valid file so seek to end so we can resume if partial file exists
					RCODE rc = 0;
					if( 0 != (rc = VxFileUtil::fileSeek(xferInfo.m_hFile, xferInfo.m_u64FileOffs )) )
					{
						xferErr = eXferErrorFileOpenAppendError;
						// seek failed
						fclose( xferInfo.m_hFile );
						xferInfo.m_hFile = NULL;
						LogMsg( LOG_INFO, "FileXferBaseMgr: ERROR: (File Rx) could not seek to position %d in file %s",
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
				xferErr = eXferErrorFileOpenError;
				// failed to open file
				xferInfo.m_hFile = NULL;
				RCODE rc = VxGetLastError();
				LogMsg( LOG_INFO, "FileShareXferMgr: ERROR: %d File %s could not be created", 
					rc,
					(const char *)xferInfo.getLclFileName().c_str() );
			}
		}
	}

	return xferErr;
}

//============================================================================
bool FileXferBaseMgr::makeIncompleteFileName( std::string& strRemoteFileName, std::string& strRetIncompleteFileName )
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
}

//============================================================================
EXferError FileXferBaseMgr::sendNextFileChunk( VxFileXferInfo& xferInfo, VxNetIdent * netIdent, VxSktBase * skt )
{
	EXferError xferErr = eXferErrorNone;
	PktFileChunkReq oPkt;
	// see how much we can read
	uint32_t u32ChunkLen = (uint32_t)(xferInfo.m_u64FileLen - xferInfo.m_u64FileOffs);
	if( PKT_TYPE_FILE_MAX_DATA_LEN < u32ChunkLen )
	{
		u32ChunkLen = PKT_TYPE_FILE_MAX_DATA_LEN;
	}

	if( 0 == u32ChunkLen )
	{
		LogMsg( LOG_ERROR, "FileShareXferMgr::txNextFileChunk 0 len u32ChunkLen\n" );
		// what to do?
		return eXferErrorNone;
	}

	// read data into packet
	uint32_t u32BytesRead = (uint32_t)fread(	oPkt.m_au8FileChunk,
											1,
											u32ChunkLen,
											xferInfo.m_hFile );
	if( u32BytesRead != u32ChunkLen )
	{
		RCODE rc = VxGetLastError();
		//xferSession->setErrorCode( rc );
		xferErr = eXferErrorFileReadError;

		fclose( xferInfo.m_hFile );
		xferInfo.m_hFile  = NULL;
		LogMsg( LOG_INFO, "FileXferBaseMgr: ERROR: %d reading send file at offset %ld when file len %ld file name %s",
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
		if( false == m_Plugin.txPacket( netIdent, skt, &oPkt ) )
		{
			xferErr = eXferErrorDisconnected;
		}
	}

	if( eXferErrorNone != xferErr )
	{
		IToGui::getToGui().toGuiFileXferState( xferInfo.getLclSessionId(), eXferStateUploadError, xferErr, xferErr );
	}
	else
	{
		if( xferInfo.calcProgress() )
		{
			IToGui::getToGui().toGuiFileXferState( xferInfo.getLclSessionId(), eXferStateInUploadXfer, xferInfo.getProgress(), 0 );
		}
	}

	return xferErr;
}
