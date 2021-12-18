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



#include "AppletPeerViewSharedFiles.h"
#include "FileListReplySession.h"
#include "AppCommon.h"
#include "GuiFileXferSession.h"
#include "ToGuiFileXferInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::toGuiFileListReply(	VxNetIdent *	netIdent, 
									EPluginType		ePluginType, 
									uint8_t			u8FileType, 
									uint64_t		u64FileLen, 
									const char *	pFileName,
									VxGUID			assetId,
									uint8_t *		fileHashData )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	FileListReplySession * replySession = new FileListReplySession( ePluginType, m_UserMgr.getUser( netIdent->getMyOnlineId() ), 
																	u8FileType, u64FileLen, pFileName, assetId, fileHashData );

	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiFileListReply( client.m_UserData, replySession );
	}

	toGuiFileXferClientsUnlock();
	replySession->deleteLater();
}

//============================================================================
void AppCommon::toGuiStartUpload(	VxNetIdent *	netIdent, 
									EPluginType		ePluginType, 
									VxGUID&			lclSessionId, 
									uint8_t			fileType, 
									uint64_t		fileLen, 
									const char *	fileName,
									VxGUID			assetId,
									uint8_t *		fileHashData )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxGUID lclSession( lclSessionId );
	GuiFileXferSession * fileXferSession =  new GuiFileXferSession( ePluginType, m_UserMgr.getUser( netIdent->getMyOnlineId() ), lclSession, 
																	fileType, fileLen, fileName, assetId, fileHashData );
	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiStartUpload( client.m_UserData, fileXferSession );
	}

	toGuiFileXferClientsUnlock();
	fileXferSession->deleteLater();
}

//============================================================================
void AppCommon::toGuiStartDownload(	VxNetIdent *	netIdent, 
									EPluginType		ePluginType, 
									VxGUID&			lclSessionId, 
									uint8_t			u8FileType, 
									uint64_t		u64FileLen, 
									const char *	pFileName,
									VxGUID			assetId,
									uint8_t *		fileHashData )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxGUID lclSession( lclSessionId );
	GuiFileXferSession * fileXferSession = new GuiFileXferSession( ePluginType, m_UserMgr.getUser( netIdent->getMyOnlineId() ), 
																	lclSession, u8FileType, u64FileLen, pFileName, assetId, fileHashData );
	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiStartDownload( client.m_UserData, fileXferSession );
	}

	toGuiFileXferClientsUnlock();
	fileXferSession->deleteLater();
}

//============================================================================
void AppCommon::toGuiFileXferState( VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiFileXferState( client.m_UserData, lclSessionId, eXferState, param1, param2 );
	}

	toGuiFileXferClientsUnlock();
}

//============================================================================
void AppCommon::toGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiFileDownloadComplete( client.m_UserData, lclSessionId, newFileName, xferError );
	}

	toGuiFileXferClientsUnlock();
}

//============================================================================
void AppCommon::toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError  )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiFileUploadComplete( client.m_UserData, lclSessionId, xferError );
	}

	toGuiFileXferClientsUnlock();
}

//============================================================================
void AppCommon::toGuiFileList(	const char *	fileName, 
								uint64_t		fileLen, 
								uint8_t			fileType, 
								bool			isShared,
								bool			isInLibrary,
								VxGUID          assetId,
								uint8_t *		fileHashId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxMyFileInfo fileInfo( fileName, fileType, fileLen, assetId, fileHashId );
	fileInfo.setIsInLibrary( isInLibrary );
	fileInfo.setIsShared( isShared );

	toGuiFileXferClientsLock();
	std::vector<ToGuiFileXferClient>::iterator iter;
	for( iter = m_ToGuiFileXferClientList.begin(); iter != m_ToGuiFileXferClientList.end(); ++iter )
	{
		ToGuiFileXferClient& client = *iter;
		client.m_Callback->toGuiFileList( client.m_UserData, fileInfo );
	}

	toGuiFileXferClientsUnlock();
}
