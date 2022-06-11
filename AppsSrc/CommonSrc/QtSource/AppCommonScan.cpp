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
	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiClientScanSearchComplete( eScanType );
	}
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
    GuiUser * guiUser = m_UserMgr.getUser( netIdent.getMyOnlineId() );

	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiScanResultSuccess( eScanType, guiUser );
	}
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
    GuiUser * netIdent = m_UserMgr.getUser( netIdentIn.getMyOnlineId() );

	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiSearchResultError( eScanType, netIdent, errCode );
	}
}

//============================================================================
void AppCommon::toGuiSearchResultProfilePic(	VxNetIdent *	netIdentIn, 
												uint8_t *		pu8JpgData, 
												uint32_t		u32JpgDataLen )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    GuiUser * netIdent = m_UserMgr.getUser( netIdentIn->getMyOnlineId() );

	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiSearchResultProfilePic( netIdent, pu8JpgData, u32JpgDataLen );
	}
}

//============================================================================
void AppCommon::toGuiSearchResultFileSearch(	VxNetIdent *	netIdentIn, 		
												VxGUID&			lclSessionId, 
												uint8_t			u8FileType, 
												uint64_t		u64FileLen, 
												const char *	pFileName,
												VxGUID			assetId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    GuiUser * netIdent = m_UserMgr.getUser( netIdentIn->getMyOnlineId() );

	for( auto client : m_ToGuiActivityInterfaceList )
	{
		client->toGuiSearchResultFileSearch( netIdent, lclSessionId, u8FileType, u64FileLen, pFileName, assetId );
	}
}

