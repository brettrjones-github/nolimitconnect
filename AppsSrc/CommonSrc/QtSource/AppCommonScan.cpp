//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "AppCommon.h"
#include "ActivityScanProfiles.h"
#include "ActivityScanWebCams.h"
#include "ActivityScanPeopleSearch.h"
#include "ToGuiActivityInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::toGuiScanSearchComplete( EScanType eScanType )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiScanSearchComplete( eScanType );
}

//============================================================================
void AppCommon::slotInternalToGuiScanSearchComplete( EScanType eScanType )
{
	m_ToGuiActivityInterfaceBusy = true;
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiClientScanSearchComplete( eScanType );
	}

	m_ToGuiActivityInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiScanResultSuccess( EScanType eScanType, VxNetIdent* netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiScanResultSuccess( eScanType, *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiScanResultSuccess( EScanType eScanType, VxNetIdent netIdent )
{
    GuiUser* guiUser = m_UserMgr.getUser( netIdent.getMyOnlineId() );

	m_ToGuiActivityInterfaceBusy = true;
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiScanResultSuccess( eScanType, guiUser );
	}

	m_ToGuiActivityInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiSearchResultError( EScanType eScanType, VxNetIdent* netIdentIn, int errCode )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	emit signalInternalToGuiSearchResultError( eScanType, *netIdentIn, errCode );
}

//============================================================================
void AppCommon::slotInternalToGuiSearchResultError( EScanType eScanType, VxNetIdent netIdentIn, int errCode )
{
    GuiUser* guiUser = m_UserMgr.getUser( netIdentIn.getMyOnlineId() );

	m_ToGuiActivityInterfaceBusy = true;
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiSearchResultError( eScanType, guiUser, errCode );
	}

	m_ToGuiActivityInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiSearchResultProfilePic(	VxNetIdent*	netIdentIn, 
												uint8_t *		pu8JpgData, 
												uint32_t		u32JpgDataLen )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    GuiUser* guiUser = m_UserMgr.getUser( netIdentIn->getMyOnlineId() );

	m_ToGuiActivityInterfaceBusy = true;
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiSearchResultProfilePic( guiUser, pu8JpgData, u32JpgDataLen );
	}

	m_ToGuiActivityInterfaceBusy = false;
}

//============================================================================
void AppCommon::toGuiSearchResultFileSearch( VxGUID& onlineId, EPluginType pluginType, VxGUID& lclSessionId, FileInfo& fileInfo )
{
	emit signalInternalToGuiSearchResultFileSearch( onlineId, pluginType, lclSessionId, fileInfo );
}

//============================================================================
void AppCommon::slotInternalToGuiSearchResultFileSearch( VxGUID onlineId, EPluginType pluginType, VxGUID lclSessionId, FileInfo fileInfo )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	GuiUser* guiUser = m_UserMgr.getUser( onlineId );

	m_ToGuiActivityInterfaceBusy = true;
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiSearchResultFileSearch( guiUser, pluginType, lclSessionId, fileInfo );
	}

	m_ToGuiActivityInterfaceBusy = false;
}