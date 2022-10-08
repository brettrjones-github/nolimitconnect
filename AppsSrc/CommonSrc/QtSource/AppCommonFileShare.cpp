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
void AppCommon::toGuiFileListReply( VxGUID& onlineId, EPluginType pluginType, FileInfo& fileInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	FileListReplySession* replySession = new FileListReplySession( pluginType, m_UserMgr.getUser( onlineId ), fileInfo );

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
void AppCommon::toGuiStartUpload( VxGUID& onlineId, EPluginType pluginType, VxGUID& lclSessionId, FileInfo& fileInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiStartUpload( onlineId, pluginType, lclSessionId, fileInfo );
}

//============================================================================
void AppCommon::slotInternalToGuiStartUpload( VxGUID onlineId, EPluginType pluginType, VxGUID lclSessionId, FileInfo fileInfo )
{
	GuiFileXferSession* fileXferSession = new GuiFileXferSession( pluginType, m_UserMgr.getUser( onlineId ), lclSessionId, fileInfo );

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
void AppCommon::toGuiStartDownload( VxGUID& onlineId, EPluginType pluginType, VxGUID& lclSessionId, FileInfo& fileInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiStartDownload( onlineId, pluginType, lclSessionId, fileInfo );
}

//============================================================================
void AppCommon::slotInternalToGuiStartDownload( VxGUID onlineId, EPluginType pluginType, VxGUID lclSessionId, FileInfo fileInfo )
{
	GuiFileXferSession* fileXferSession = new GuiFileXferSession( pluginType, m_UserMgr.getUser( onlineId ), lclSessionId, fileInfo );
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
void AppCommon::toGuiFileList( FileInfo& fileInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiFileList( fileInfo );
}

//============================================================================
void AppCommon::slotInternalToGuiFileList( FileInfo fileInfo )
{
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->callbackToGuiFileList( fileInfo );
	}

	m_ToGuiFileXferInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiFileListCompleted( void )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiFileListCompleted();
}

//============================================================================
void AppCommon::slotInternalToGuiFileListCompleted( void )
{
	m_ToGuiFileXferInterfaceBusy = true;
	for( auto iter = m_ToGuiFileXferInterfaceList.begin(); iter != m_ToGuiFileXferInterfaceList.end(); ++iter )
	{
		ToGuiFileXferInterface* client = *iter;
		client->callbackToGuiFileListCompleted();
	}

	m_ToGuiFileXferInterfaceBusy = false;
}
