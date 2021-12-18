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
// http://www.nolimitconnect.com
//============================================================================


#include "AppCommon.h"
#include "ActivityScanProfiles.h"
#include "ActivityScanWebCams.h"
#include "ActivityScanPeopleSearch.h"
//#include "ActivityRandomConnect.h"
#include "ToGuiActivityInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::toGuiScanSearchComplete( EScanType eScanType )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	std::vector<ToGuiActivityClient>::iterator iter;
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiScanSearchComplete: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& toGuiClient = *iter;
		toGuiClient.m_Callback->toGuiClientScanSearchComplete( toGuiClient.m_UserData, eScanType );
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiScanSearchComplete: toGuiActivityClientsUnlock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiScanResultSuccess( EScanType eScanType, VxNetIdent * netIdentIn )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    GuiUser * netIdent = m_UserMgr.getUser( netIdentIn->getMyOnlineId() );
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiScanResultSuccess: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
    toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiScanResultSuccess( client.m_UserData, eScanType, netIdent );
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiScanResultSuccess: toGuiActivityClientsUnlock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdentIn, int errCode )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    GuiUser * netIdent = m_UserMgr.getUser( netIdentIn->getMyOnlineId() );
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSearchResultError: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiSearchResultError( client.m_UserData, eScanType, netIdent, errCode );
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSearchResultError: toGuiActivityClientsUnlock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsUnlock();
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
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSearchResultProfilePic: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiSearchResultProfilePic( client.m_UserData, netIdent, pu8JpgData, u32JpgDataLen );
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSearchResultProfilePic: toGuiActivityClientsUnlock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsUnlock();
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
	std::vector<ToGuiActivityClient>::iterator iter;
#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSearchResultFileSearch: toGuiActivityClientsLock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsLock();
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& toGuiClient = *iter;
		toGuiClient.m_Callback->toGuiSearchResultFileSearch( toGuiClient.m_UserData, netIdent, lclSessionId, u8FileType, u64FileLen, pFileName, assetId );
	}

#ifdef DEBUG_TOGUI_CLIENT_MUTEX
	LogMsg( LOG_INFO, "toGuiSearchResultFileSearch: toGuiActivityClientsUnlock\n" );
#endif // DEBUG_TOGUI_CLIENT_MUTEX
	toGuiActivityClientsUnlock();
}

