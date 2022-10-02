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
// http://www.nolimitconnect.org
//============================================================================



#include "AppletFileShareClientView.h"
#include "FileListReplySession.h"
#include "AppCommon.h"
#include "GuiFileXferSession.h"
#include "ToGuiFileXferInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::toGuiFileListReply(	VxNetIdent*	netIdent, 
									EPluginType		ePluginType, 
									uint8_t			u8FileType, 
									uint64_t		u64FileLen, 
									const char*	pFileName,
									VxGUID			assetId,
									uint8_t *		fileHashData )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	FileListReplySession * replySession = new FileListReplySession( ePluginType, m_UserMgr.getUser( netIdent->getMyOnlineId() ), 
																	u8FileType, u64FileLen, pFileName, assetId, fileHashData );

	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiFileListReply( replySession );
	}

	m_ToGuiFileXferInterfaceBusy = false;
	replySession->deleteLater();
}

//============================================================================
void AppCommon::toGuiStartUpload( VxNetIdent*	netIdent,
								EPluginType		ePluginType,
								VxGUID&			lclSessionId,
								uint8_t			fileType,
								uint64_t		fileLen,
								std::string&	fileName,
								VxGUID&			assetId,
								VxSha1Hash&		fileHashId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiStartUpload( netIdent->getMyOnlineId(), ePluginType, lclSessionId, fileType, fileLen, fileName.c_str(), assetId, fileHashId );
}

//============================================================================
void AppCommon::slotInternalToGuiStartUpload( VxGUID onlineId, EPluginType ePluginType, VxGUID lclSessionId, uint8_t fileType, uint64_t fileLen, QString fileName, VxGUID assetId, VxSha1Hash fileHashId )
{
	GuiFileXferSession* fileXferSession =  new GuiFileXferSession( ePluginType, m_UserMgr.getUser( onlineId ), lclSessionId,
																	fileType, fileLen, fileName.toUtf8().constData(), assetId, fileHashId );
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiStartUpload( fileXferSession );
	}

	m_ToGuiFileXferInterfaceBusy = false;
	fileXferSession->deleteLater();
}

//============================================================================
void AppCommon::toGuiStartDownload( VxNetIdent*		netIdent,
									EPluginType		ePluginType,
									VxGUID&			lclSessionId,
									uint8_t			fileType,
									uint64_t		fileLen,
									std::string&	fileName,
									VxGUID&			assetId,
									VxSha1Hash&		fileHashId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiStartDownload( netIdent->getMyOnlineId(), ePluginType, lclSessionId, fileType, fileLen, fileName.c_str(), assetId, fileHashId );
}

//============================================================================
void AppCommon::slotInternalToGuiStartDownload( VxGUID onlineId, EPluginType ePluginType, VxGUID lclSessionId, uint8_t fileType, uint64_t fileLen, QString fileName, VxGUID assetId, VxSha1Hash fileHashId )
{
	GuiFileXferSession* fileXferSession = new GuiFileXferSession( ePluginType, m_UserMgr.getUser( onlineId ),
		lclSessionId, fileType, fileLen, fileName.toUtf8().constData(), assetId, fileHashId );
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiStartDownload( fileXferSession );
	}

	m_ToGuiFileXferInterfaceBusy = false;
	fileXferSession->deleteLater();
}

//============================================================================
void AppCommon::toGuiFileXferState( EPluginType pluginType, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiFileXferState( pluginType, lclSessionId, eXferState, param1, param2 );
}

//============================================================================
void AppCommon::slotInternalToGuiFileXferState( EPluginType pluginType, VxGUID lclSessionId, EXferState eXferState, int param1, int param2 )
{
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiFileXferState( pluginType, lclSessionId, eXferState, param1, param2 );
	}

	m_ToGuiFileXferInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiFileDownloadComplete( EPluginType pluginType, VxGUID& lclSessionId, std::string& fileName, EXferError xferError )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiFileDownloadComplete( pluginType, lclSessionId, fileName.c_str(), xferError );
}

//============================================================================
void AppCommon::slotInternalToGuiFileDownloadComplete( EPluginType pluginType, VxGUID lclSessionId, QString fileName, EXferError xferError )
{
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiFileDownloadComplete( pluginType, lclSessionId, fileName, xferError );
	}

	m_ToGuiFileXferInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiFileUploadComplete( EPluginType pluginType, VxGUID& lclSessionId, std::string& fileName, EXferError xferError )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiFileUploadComplete( pluginType, lclSessionId, fileName.c_str(), xferError );
}


//============================================================================
void AppCommon::slotInternalToGuiFileUploadComplete( EPluginType pluginType, VxGUID lclSessionId, QString fileName, EXferError xferError )
{
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiFileUploadComplete( pluginType, lclSessionId, xferError );
	}

	m_ToGuiFileXferInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiFileList(	const char*	fileName, 
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

	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->toGuiFileList( fileInfo );
	}

	m_ToGuiFileXferInterfaceBusy = false;
}
