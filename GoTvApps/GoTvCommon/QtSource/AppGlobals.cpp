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
#include <app_precompiled_hdr.h>
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiParams.h"


#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>
#include <CoreLib/VxFileUtil.h>

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include <stdio.h>

namespace
{
	VxNetIdent			g_oUserIdent;
}

////============================================================================
GuiParams& GetGuiParams()
{
    static GuiParams g_GuiParams;
    return g_GuiParams;
}

//============================================================================
AppGlobals::AppGlobals( AppCommon& appCommon )
: m_MyApp( appCommon )
{
}

//============================================================================
VxNetIdent * AppGlobals::getUserIdent( void )
{
	return &g_oUserIdent;
}

//============================================================================
bool AppGlobals::saveUserIdentToDatabase( void )
{
	if( false == m_MyApp.getAccountMgr().updateAccount( g_oUserIdent ) )
	{
		LogMsg( LOG_ERROR, "AppGlobals::saveUserIdentToDatabase: ERROR updating database\n" );
		return false;
	}

	return true;
}

//============================================================================
void AppGlobals::launchWebBrowser( const char * pUri )
{
	// NOTE:qt crashes studio sometimes when openUrl so use ShellExecute
#ifdef TARGET_OS_WINDOWS
	ShellExecuteA(0, 0, pUri, 0, 0 , SW_SHOW );
#else
	QString strUrl = pUri;
	QDesktopServices::openUrl( QUrl(strUrl, QUrl::TolerantMode) );
#endif //TARGET_OS_WINDOWS
}

//============================================================================
//! update ident and save to database then send permission change to engine
void  AppGlobals::updatePluginPermission( EPluginType ePluginType, EFriendState ePluginPermission )
{
	g_oUserIdent.setPluginPermission( ePluginType, ePluginPermission );
	if( false == m_MyApp.getAccountMgr().updateAccount( g_oUserIdent ) )
	{
		LogMsg( LOG_ERROR, "UpdatePluginPermissions: ERROR updating database\n");
	}

    m_MyApp.getEngine().setPluginPermission( ePluginType, ePluginPermission );
}

//============================================================================
//! update has picture and save to database then send picture change to engine
void UpdateHasPicture( P2PEngine& engine, int bHasPicture )
{
	g_oUserIdent.setHasProfilePicture(bHasPicture);
	if( false == GetAppInstance().getAccountMgr().updateAccount( g_oUserIdent ) )
	{
		LogMsg( LOG_ERROR, "UpdateHasPicture: ERROR updating database\n");
	}

	engine.setHasPicture( bHasPicture );
}

//============================================================================
//! Describe action user can take for given plugin and access
QString DescribePluginAction( VxNetIdent * netIdent, EPluginType ePluginType, EPluginAccess ePluginAccess )
{
	QString strAction;
	switch( ePluginType )
	{
	case ePluginTypeRelay:	// proxy plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			if( netIdent->requiresRelay() )
			{
				strAction = "Cannot be a relay (firewalled)";
			}
			else if( netIdent->isMyPreferedRelay() )
			{
				strAction = "Is my preferred relay";
			}
			else
			{
				strAction = "Can be preferred relay";
			}
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Relay Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeRelay ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Relay is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Relay unavailable due to ignore status";
			break;
        case ePluginAccessInactive:		// access denied because busy
            strAction = "Relay unavailable because is inactive";
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = "Relay unavailable because is busy";
            break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connection to be a relay";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Contact must be online be a relay";
			break;
       }
		break;

	case ePluginTypeWebServer:	// web server plugin ( for profile web page )
        switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "View About Me Page";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "View About Me Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeWebServer ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "About Me Page is disabled";
			break;
		case ePluginAccessInactive:		// plugin inactive
			strAction = "About Me Page is inactive";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "About Me Page unavailable due to ignore status";
			break;
       case ePluginAccessBusy:		// access denied because busy
            strAction = "About Me Page unavailable because is busy";
            break;
	   case ePluginAccessRequiresDirectConnect:		
		   strAction = "Contact must have direct connect to provide About Me Page";
		   break;
	   case ePluginAccessRequiresOnline:		
		   strAction = "About Me Page unavailable because contact is offline";
		   break;
        }
		break;

	case ePluginTypeStoryboard:	// web server plugin ( for storyboard web page )
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "View Story Board";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "View Story Board Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeStoryboard ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Story Board is disabled";
			break;
		case ePluginAccessInactive:		// plugin inactive
			strAction = "Story Board is inactive";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Story Board unavailable due to ignore status";
			break;
		case ePluginAccessBusy:		// access denied because busy
			strAction = "Story Board unavailable because is busy";
			break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect to provide Story Board Page";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Story Board unavailable because contact is offline";
			break;
		}
		break;

	case ePluginTypeFileXfer:	// file offer plugin
		switch( ePluginAccess )
		{

		case ePluginAccessOk:			// plugin access allowed
			strAction = "Send A File";
			break;

		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Send A File Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeFileXfer ) );
			strAction += " permission";
			break;

		case ePluginAccessDisabled:		// plugin disabled 
			strAction = "Shared Files is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Send File unavailable due to ignore status";
            break;
        case ePluginAccessInactive:		// access denied because busy
            strAction = "Send File unavailable because is inactive";
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = "Send File unavailable because is busy";
            break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect to receive Send File";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Send File unavailable because contact is offline";
			break;
       }
		break;

	case ePluginTypeFileServer:	// file share plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "View Shared Files";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Shared Files Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeFileServer ) );
			strAction += " permission";
			break;
		case ePluginAccessInactive:		// no shared files
			strAction = "No Shared Files";
			break;
		case ePluginAccessDisabled:		// plugin disabled 
			strAction = "Shared Files is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Shared Files unavailable due to ignore status";
			break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = "Shared Files unavailable because is busy";
            break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect to Share Files";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Shared Files unavailable because contact is offline";
			break;
        }
		break;

	case ePluginTypeCamServer:	// web cam broadcast plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "View Shared Web Cam";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Shared Web Cam Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeCamServer ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Shared Web Cam is disabled";
			break;
		case ePluginAccessInactive:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Shared Web Cam is not active";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = "Shared Web Cam unavailable due to ignore status";
			break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = "Shared Web Cam unavailable because is busy";
            break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect to Share Web Cam";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Shared Web Cam unavailable because contact is offline";
			break;
        }
		break;

	case ePluginTypeVoicePhone:	// VOIP p2p plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "Voice Call";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Voice Call Requires ";
			strAction += DescribeFriendState(  netIdent->getPluginPermission( ePluginTypeVoicePhone ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Voice Call is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Voice Call unavailable due to ignore status";
			break;
        case ePluginAccessInactive:
            strAction = "Voice Call unavailable because is Inactive";
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = "Voice Call unavailable because is busy";
            break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect for Voice Call";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Voice Call unavailable because contact is offline";
			break;
       }
		break;

	case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "Offer To Play Truth Or Dare";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Truth Or Dare Requires ";
			strAction += DescribeFriendState( netIdent->getPluginPermission( ePluginTypeTruthOrDare ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Truth Or Dare is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Truth Or Dare unavailable due to ignore status";
			break;
        case ePluginAccessInactive:
            strAction = "Truth Or Dare unavailable because is Inactive";
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = "Truth Or Dare unavailable because is busy";
            break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect for Truth Or Dare";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Truth Or Dare unavailable because contact is offline";
			break;
        }
		break;

	case ePluginTypeMessenger:	
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "Offer Chat Session";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Text Chat Requires ";
			strAction += DescribeFriendState( netIdent->getPluginPermission( ePluginTypeMessenger ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Text Chat is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Text Chat unavailable due to ignore status";
			break;
		case ePluginAccessInactive:
			strAction = "Text Chat unavailable because is Inactive";
			break;
		case ePluginAccessBusy:		// access denied because busy
			strAction = "Text Chat unavailable because is busy";
			break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect for Text Chat";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Text Chat unavailable because contact is offline";
			break;
		}
		break;


	case ePluginTypeVideoPhone:	
		switch( ePluginAccess )
		{
		case ePluginAccessOk:			// plugin access allowed
			strAction = "Offer Video Chat";
			break;
		case ePluginAccessLocked:		// insufficient Friend permission level
			strAction = "Video Chat Requires ";
			strAction += DescribeFriendState( netIdent->getPluginPermission( ePluginTypeVideoPhone ) );
			strAction += " permission";
			break;
		case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
			strAction = "Video Chat is disabled";
			break;
		case ePluginAccessIgnored:		// access denied because you are being ignored
			strAction = "Video Chat unavailable due to ignore status";
			break;
		case ePluginAccessInactive:
			strAction = "Video Chat unavailable because is Inactive";
			break;
		case ePluginAccessBusy:		// access denied because busy
			strAction = "Video Chat unavailable because is busy";
			break;
		case ePluginAccessRequiresDirectConnect:		
			strAction = "Contact must have direct connect for Video Chat";
			break;
		case ePluginAccessRequiresOnline:		
			strAction = "Video Chat unavailable because contact is offline";
			break;		
		}
		break;

	default:
		LogMsg( LOG_ERROR, "DescribePluginAction: unrecognized plugin %d\n", ePluginType );
	}

	return strAction;
}

//============================================================================
//! Describe permission level ( and/or friend state )
QString	DescribePermissionLevel( EFriendState friendState )
{
    QString strPermLevel;
    switch( friendState )
    {
    case eFriendStateAdmin:
        strPermLevel = QObject::tr( "Administrator" );
        break;

    case eFriendStateFriend:
        strPermLevel = QObject::tr( "Friend" );
        break;

    case eFriendStateGuest:
        strPermLevel = QObject::tr( "Guest" );
        break;

    case eFriendStateAnonymous:
        strPermLevel = QObject::tr( "Anybody" );
        break;

    case eFriendStateIgnore:
        strPermLevel = QObject::tr( "Disabled or ignored" );
        break;

    default:
        strPermLevel = QObject::tr( "Unknown" );
    }

    return strPermLevel;
}

//============================================================================
//! Describe action user can take for given plugin and access
QString DescribePluginType( EPluginType ePluginType )
{
	QString strPluginType;
	switch( ePluginType )
	{
	case ePluginTypeAdmin:	
		strPluginType = QObject::tr( "Admin" );
		break;

	case ePluginTypeRelay:	// proxy plugin
		strPluginType = QObject::tr("Relay");
		break;

	case ePluginTypeWebServer:	// web server plugin ( for profile web page )
		strPluginType = QObject::tr("Profile Web Page");
		break;

	case ePluginTypeFileXfer:	// file offer plugin
		strPluginType = QObject::tr("Send A File");
		break;

	case ePluginTypeFileServer:	// file share plugin
		strPluginType = QObject::tr("Shared Files");
		break;

	case ePluginTypeCamServer:	// web cam broadcast plugin
		strPluginType = QObject::tr("Shared Web Cam");
		break;

	case ePluginTypeMessenger:	// multi session chat plugin
		strPluginType = QObject::tr("Chat Session");
		break;

	case ePluginTypeVoicePhone:	// VOIP p2p plugin
		strPluginType = QObject::tr("Voice Call");
		break;

	case ePluginTypeVideoPhone:
		strPluginType = QObject::tr("Video Chat");
		break;

	case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		strPluginType = QObject::tr("Play Truth Or Dare");
		break;

	case ePluginTypeStoryboard:	// story board plugin
		strPluginType = QObject::tr("Story Board");
		break;

	default:
		strPluginType = QObject::tr("Unknown Plugin");
		LogMsg( LOG_ERROR, "DescribePluginAction: unrecognized plugin %d\n", ePluginType );
	}

	return strPluginType;
}

//============================================================================
QString DescribePluginAccess( EPluginAccess accessState )
{
	QString strAccess = "";
	switch( accessState )
	{
    case ePluginAccessNotSet:
        strAccess = QObject::tr("Access Unknown");
        break;

	case ePluginAccessOk:
		strAccess = QObject::tr("Access Ok");
		break;

	case ePluginAccessLocked:
		strAccess = QObject::tr("Insufficient Permission");
		break;

	case ePluginAccessDisabled:
		strAccess = QObject::tr("Plugin Disabled");
		break;

	case ePluginAccessIgnored:
		strAccess = QObject::tr("Contact Ignored");
		break;

	case ePluginAccessInactive:
		strAccess = QObject::tr("Plugin Inactive");
		break;

	case ePluginAccessBusy:
		strAccess = QObject::tr("Plugin Busy");
		break;

	case ePluginAccessRequiresDirectConnect:
		strAccess = QObject::tr("Requires Direct Connect");
		break;

	case ePluginAccessRequiresOnline:
		strAccess = QObject::tr("Requires User Be Online");
		break;

	default:
		strAccess = QObject::tr("Unknown Plugin Access");
	}

	return strAccess;
}

//============================================================================
//! show message box to user
void ErrMsgBox( QWidget * parent, int infoLevel, const char * pMsg, ... )
{
	//build message on stack so no out of memory issue
	char szBuffer[4096];
	va_list arg_ptr;
	va_start(arg_ptr, pMsg);
    vsnprintf(szBuffer, 4096, pMsg, arg_ptr);
	szBuffer[4095] = 0;
	va_end(arg_ptr);

	QMessageBox msgBox( parent );
	msgBox.setText( szBuffer );
	msgBox.exec();
}

