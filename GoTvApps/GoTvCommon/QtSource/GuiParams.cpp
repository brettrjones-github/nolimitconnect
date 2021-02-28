//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AppTranslate.h"
#include "GuiParams.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include <CoreLib/VxDebug.h>

//============================================================================
QColor GuiParams::m_OnlineBkgColor( 176, 255, 176 );
QColor GuiParams::m_OfflineBkgColor( 192, 192, 192 );
QColor GuiParams::m_NearbyBkgColor( 176, 176, 255 );
int GuiParams::m_DisplayScale{1};

//============================================================================
GuiParams::GuiParams()
{
    //initGuiParams();
}

//============================================================================
void GuiParams::initGuiParams()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if( screen )
    {
        float dpi = screen->physicalDotsPerInch();
        QRect  screenGeometry = screen->availableGeometry();
        float iconSizeInches = ( 48.0f / dpi );
        float maxPixels = screenGeometry.width() > screenGeometry.height() ? screenGeometry.width() : screenGeometry.height();
        float screenSizeInches = maxPixels / dpi;
        float normalPercentOfScreenForIcon = 48.0f/1280.0f;
        float curPercentOfScreenForIcon = 48.0f/maxPixels;

        if( iconSizeInches < 0.3 )
        {
            // 48 pixel icons are less than a quarter inch wide
            // scale up
            m_DisplayScale = 2;
        }
        else if( ( screenSizeInches > 17 ) && ( curPercentOfScreenForIcon < normalPercentOfScreenForIcon ) )
        {
            m_DisplayScale = 2;
        }

        LogMsg( LOG_VERBOSE, "Screen dpi %3.0f pixels %3.0f screen size %3.0f icon size 0x%3f scale %d",
                dpi, maxPixels, screenSizeInches, iconSizeInches, m_DisplayScale );
    }
}

//============================================================================
int GuiParams::getControlIndicatorWidth( void )
{ 
    return 10; 
}

//============================================================================
QSize GuiParams::getThumbnailSize( void )
{
    return QSize( 240, 240 ); 
}

//============================================================================
QSize GuiParams::getSnapshotSize( void )
{
    return QSize( 320 * m_DisplayScale, 240 * m_DisplayScale );
}

//============================================================================
QString GuiParams::describeCommError( ECommErr commErr )
{
    switch( commErr )
    {
    case eCommErrNone:	
        return QObject::tr("No Error ");
    case eCommErrInvalidPkt:		
        return QObject::tr("Comm Error Invalid Packet ");
    case eCommErrUserOffline:	
        return QObject::tr( "Comm Error User Offline ");
    case eCommErrSearchTextToShort:		
        return QObject::tr("Comm Error Search Text To Short ");
    case eCommErrSearchNoMatch:		
        return QObject::tr("Comm Error Search No Match ");
    case eCommErrInvalidHostType:		
        return QObject::tr("Comm Error Invalid Host Type ");
    case eCommErrPluginNotEnabled:		
        return QObject::tr("Comm Error Plugin Not Enabled ");
    case eCommErrPluginPermission:		
        return QObject::tr("Comm Error Plugin Permission Level ");
    default:
        return QObject::tr("Comm Error Invalid ");
    }
}

//============================================================================
QString GuiParams::describeContentCatagory( EContentCatagory content )
{
    switch( content )
    {
    case eContentCatagoryUnspecified:
        return QObject::tr( "Any" );
    case eContentCatagoryVideo:
        return QObject::tr( "Video" );
    case eContentCatagoryAudio:
        return QObject::tr( "Auidio" );
    case eContentCatagoryImage:
        return QObject::tr( "Image" );
    case eContentCatagoryText:
        return QObject::tr( "Text" );
    case eContentCatagoryPersonal:
        return QObject::tr( "Personal" );
    case eContentCatagoryOther:
        return QObject::tr( "Other" );

    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
QString GuiParams::describeContentRating( EContentRating content )
{
    switch( content )
    {
    case eContentRatingUnspecified:
        return QObject::tr( "Any" );
    case eContentRatingFamily:
        return QObject::tr( "Family" );
    case eContentRatingAdult:
        return QObject::tr( "Adult" );
    case eContentRatingXXX:
        return QObject::tr( "XXX" );
    case eContentRatingDarkWeb:
        return QObject::tr( "DarkWeb" );
    case eContentRatingPersonal:
        return QObject::tr( "Personal" );

    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
//! describe friend state
QString GuiParams::describeFriendState( EFriendState eFriendState )
{
    switch( eFriendState )
    {
    case eFriendStateAnonymous:	// anonymous user
        return QObject::tr("Anonymous ");
    case eFriendStateGuest:		// guest user
        return QObject::tr("Guest ");
    case eFriendStateFriend:	// friend user
        return QObject::tr("Friend ");
    case eFriendStateAdmin:		// administrator
        return QObject::tr("Administrator ");
    default:
        return QObject::tr("Ignore ");
    }
}

//============================================================================
QString GuiParams::describeGender( EGenderType gender )
{
    switch( gender )
    {
    case eGenderTypeUnspecified:
        return QObject::tr( "Any" );
    case eGenderTypeMale:
        return QObject::tr( "Male" );
    case eGenderTypeFemale:
        return QObject::tr( "Female" );
    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
QString GuiParams::describeHostSearchStatus( EHostSearchStatus searchStatus )
{
    switch( searchStatus )
    {
    case eHostSearchUnknown:
        return QObject::tr("Host Search Status Unknown ");
    case eHostSearchInvalidUrl:
        return QObject::tr("Host Search Invalid Url ");
    case eHostSearchQueryIdInProgress:
        return QObject::tr("Host Search Querying Id In Progress ");
    case eHostSearchQueryIdSuccess:
        return QObject::tr("Host Search Querying Id Success ");
    case eHostSearchQueryIdFailed:
        return QObject::tr("Host Search Query Id Failed ");
    case eHostSearchConnecting:
        return QObject::tr("Host Search Connecting ");
    case eHostSearchConnectSuccess:
        return QObject::tr("Host Search Connect Success ");
    case eHostSearchConnectFailed:
        return QObject::tr("Host Search Connect Failed ");

    case eHostSearchSendingSearchRequest:
        return QObject::tr("Host Search Sending Request ");
    case eHostSearchSendSearchRequestFailed:
        return QObject::tr( "Host Search Send Request Failed ");
    case eHostSearchSuccess:
        return QObject::tr("Host Search Success ");
    case eHostSearchFail:
        return QObject::tr("Host Search Failed ");

    case eHostSearchFailPermission:
        return QObject::tr("Host Search Fail Permission ");
    case eHostSearchFailConnectDropped:
        return QObject::tr("Host Search Fail Connect Dropped ");
    case eHostSearchInvalidParam:
        return QObject::tr("Host Search Invalid Param ");
    case eHostSearchPluginDisabled:
        return QObject::tr("Host Search Plugin Disabled ");
    case eHostSearchNoMatches:
        return QObject::tr("Host Search No Matches ");
    case eHostSearchCompleted:
        return QObject::tr("Host Search Completed ");
    default:
        return QObject::tr("Invalid Host Search Param ");
    }
}

//============================================================================
QString GuiParams::describeLanguage( ELanguageType language )
{
    return AppTranslate::describeLanguage( language );
}

//============================================================================
//! Describe action user can take for given plugin and access
QString GuiParams::describePluginAction( VxNetIdent * netIdent, EPluginType ePluginType, EPluginAccess ePluginAccess )
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeRelay ) );
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeWebServer ) );
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeStoryboard ) );
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeFileXfer ) );
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeFileServer ) );
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeCamServer ) );
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
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeVoicePhone ) );
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
            strAction += describeFriendState( netIdent->getPluginPermission( ePluginTypeTruthOrDare ) );
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
            strAction += describeFriendState( netIdent->getPluginPermission( ePluginTypeMessenger ) );
            strAction += " permission";
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
            strAction = "Text Chat is disabled";
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = "Text Chat unavailable due to ignore status";
            break;
        case ePluginAccessInactive:
            strAction = QObject::tr("Text Chat unavailable because is Inactive");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Text Chat unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect for Text Chat");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Text Chat unavailable because contact is offline");
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
            strAction = QObject::tr("Video Chat Requires ");
            strAction += describeFriendState( netIdent->getPluginPermission( ePluginTypeVideoPhone ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
            strAction = QObject::tr("Video Chat is disabled");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Video Chat unavailable due to ignore status");
            break;
        case ePluginAccessInactive:
            strAction = QObject::tr("Video Chat unavailable because is Inactive");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Video Chat unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect for Video Chat");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Video Chat unavailable because contact is offline");
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
QString GuiParams::describePermissionLevel( EFriendState friendState )
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
QString GuiParams::describePluginAccess( EPluginAccess accessState )
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
QString GuiParams::describePluginPermission( EFriendState ePluginPermission )
{
    switch(ePluginPermission)
    {
    case eFriendStateIgnore:
        return QObject::tr("Disabled");
    case eFriendStateAnonymous:
        return QObject::tr("Anonymous");
    case eFriendStateGuest:
        return QObject::tr("Guest");
    case eFriendStateFriend:
        return QObject::tr("Friends");
    default:
        return QObject::tr("Unknown");
    }
}

//============================================================================
//! Describe plugin
QString GuiParams::describePluginType( EPluginType ePluginType )
{
    QString strPluginType;
    switch( ePluginType )
    {
    case ePluginTypeAdmin:	
        strPluginType = QObject::tr( "Admin" );
        break;

    case ePluginTypeWebServer:	// web server plugin ( for profile web page )
        strPluginType = QObject::tr("About Me Page");
        break;

    case ePluginTypeAvatarImage:
        strPluginType = QObject::tr("Avatar Image");
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
        strPluginType = QObject::tr("Messenger");
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

    case ePluginTypeChatRoomClient:	//!< chat room user client plugin
        strPluginType = QObject::tr("Chat Room Client");
        break;
    case ePluginTypeChatRoomHost:	//!< chat room hosting plugin
        strPluginType = QObject::tr("Chat Room Host");
        break;
    case ePluginTypeConnectTestClient:	//!< Connection Test Client
        strPluginType = QObject::tr("Connect Test Client");
        break;
    case ePluginTypeConnectTestHost:	//!< Connection Test Service
        strPluginType = QObject::tr("Connect Test Host");
        break;
    case ePluginTypeGroupClient:	//!< group client
        strPluginType = QObject::tr("Group Client");
        break;
    case ePluginTypeGroupHost:   //!< group hosting
        strPluginType = QObject::tr("Group Host");
        break;
    case ePluginTypeRandomConnectClient:	//!< Random connect to another person client
        strPluginType = QObject::tr("Random Connect Client");
        break;
    case ePluginTypeRandomConnectHost:	//!< Random connect to another person hosting
        strPluginType = QObject::tr("Random Connect Host");
        break;
    case ePluginTypeNetworkClient:	//!< network client
        strPluginType = QObject::tr("Network Client");
        break;
    case ePluginTypeNetworkHost:	//!< master network hosting
        strPluginType = QObject::tr("Network Host");
        break;
    case  ePluginTypeNetworkSearchList:	//!< group and chat room list for network search
        strPluginType = QObject::tr("Network Search");
        break;
    case ePluginTypeRelay:	// proxy plugin
        strPluginType = QObject::tr("Relay");
        break;
    default:
        strPluginType = QObject::tr("Unknown Plugin");
        LogMsg( LOG_ERROR, "DescribePluginAction: unrecognized plugin %d\n", ePluginType );
    }

    return strPluginType;
}

//============================================================================
QString GuiParams::describeResponse( EOfferResponse eOfferResponse )
{
    switch( eOfferResponse )
    {
    case eOfferResponseAccept:
        return QObject::tr("Offer Accepted");

    case eOfferResponseReject:
        return QObject::tr("Offer Rejected");

    case eOfferResponseBusy:
        return QObject::tr("Busy");

    case eOfferResponseCancelSession:
        return QObject::tr("Session Canceled");

    case eOfferResponseEndSession:
        return QObject::tr("User Ended Session");

    case eOfferResponseUserOffline:
        return QObject::tr("User Is Offline");

    default:
        return QObject::tr("Session Ended");
    }
}

//============================================================================
QString GuiParams::describeEXferError( EXferError xferError )
{
    switch( xferError )
    {
    case eXferErrorNone:
        return QObject::tr("No Error");
    case eXferErrorDisconnected:
        return QObject::tr("Disconnected");
    case eXferErrorPermission:
        return QObject::tr("Permission Error");
    case eXferErrorFileNotFound:
        return QObject::tr("File Not Found");
    case eXferErrorCanceled:
        return QObject::tr("Canceled");
    case eXferErrorBadParam:
        return QObject::tr("Bad Param");
    case eXferErrorAtSrc:
        return QObject::tr("Error At Source");
    case eXferErrorBusy:
        return QObject::tr("Busy");
    case eXferErrorAlreadyDownloading:
        return QObject::tr("Already Downloading");
    case eXferErrorAlreadyDownloaded:
        return QObject::tr("Already Downloaded");
    case eXferErrorAlreadyUploading:
        return QObject::tr("Already Uploading");
    case eXferErrorFileCreateError:
        return QObject::tr("File Create Error");
    case eXferErrorFileOpenAppendError:
        return QObject::tr("File Append Error");
    case eXferErrorFileOpenError:
        return QObject::tr("File Open Error");
    case eXferErrorFileSeekError:
        return QObject::tr("Seek Error");
    case eXferErrorFileReadError:
        return QObject::tr("Read Error");
    case eXferErrorFileWriteError:
        return QObject::tr("Write Error");
    case eXferErrorFileMoveError:
        return QObject::tr("Move File Error");
    default:
        return QObject::tr("Unknown Error");
    }
}

//============================================================================
QString GuiParams::describeEXferState( EXferState xferState )
{
    switch( xferState )
    {
    case eXferStateUploadNotStarted:
        return QObject::tr("Upload Not Started");
    case eXferStateWaitingOfferResponse:
        return QObject::tr("Waiting Response");
    case eXferStateInUploadQue:
        return QObject::tr("In Upload Que");
    case eXferStateBeginUpload:
        return QObject::tr("Start Upload");
    case eXferStateInUploadXfer:
        return QObject::tr("Uploading");
    case eXferStateCompletedUpload:
        return QObject::tr("Completed Upload");
    case eXferStateUserCanceledUpload:
        return QObject::tr("Upload Canceled");
    case eXferStateUploadOfferRejected:
        return QObject::tr("Offer Rejected");
    case eXferStateUploadError:
        return QObject::tr("ERROR");
    case eXferStateDownloadNotStarted:
        return QObject::tr("Download Not Started");
    case eXferStateInDownloadQue:
        return QObject::tr("In Download Que");
    case eXferStateBeginDownload:
        return QObject::tr("Start Download");
    case eXferStateInDownloadXfer:
        return QObject::tr("Downloading");
    case eXferStateCompletedDownload:
        return QObject::tr("Completed Download");
    case eXferStateUserCanceledDownload:
        return QObject::tr("Download Canceled");
    case eXferStateDownloadError:
        return QObject::tr("ERROR");
    case eXferStateUnknown:
    default:
        return QObject::tr("Unknown State");
    }
}

//============================================================================
QString GuiParams::describeStatus( EHostAnnounceStatus hostStatus )
{
    switch( hostStatus )
    {
    case eHostAnnounceUnknown:
        return QObject::tr( "Host Announce Status Unknown" );
    case eHostAnnounceInvalidUrl:
        return QObject::tr( "Host Announce Invalid Url" );
    case eHostAnnounceQueryIdInProgress:
        return QObject::tr( "Host Announce Querying Id In Progress" );
    case eHostAnnounceQueryIdSuccess:
        return QObject::tr( "Host Announce Querying Id Success" );
    case eHostAnnounceQueryIdFailed:
        return QObject::tr( "Host Announce Query Id Failed" );
    case eHostAnnounceConnecting:
        return QObject::tr( "Host Announce Connecting" );
    case eHostAnnounceHandshaking:
        return QObject::tr( "Host Announce Handshaking" );
    case eHostAnnounceHandshakeTimeout:
        return QObject::tr( "Host Announce Handshake Timeout" );
    case eHostAnnounceConnectSuccess:
        return QObject::tr( "Host Announce Connect Success" );
    case eHostAnnounceConnectFailed:
        return QObject::tr( "Host Announce Connect Failed" );
    case eHostAnnounceSendingJoinRequest:
        return QObject::tr( "Host Announce Sending Request" );
    case eHostAnnounceSendJoinRequestFailed:
        return QObject::tr( "Host Announce Send Request Failed" );
    case eHostAnnounceSuccess:
        return QObject::tr( "Host Announce Success" );
    case eHostAnnounceFail:
        return QObject::tr( "Host Announce Failed" );
    case eHostAnnounceFailPermission:
        return QObject::tr( "Host Announce Fail Permission" );
    case eHostAnnounceFailConnectDropped:
        return QObject::tr( "Host Announce Fail Connect Dropped" );
    case eHostAnnounceInvalidParam:
        return QObject::tr( "Host Announce Invalid Param" );
    case eHostAnnounceDone:
        return QObject::tr( "Host Announce Done" );
    default:
        return QObject::tr( "Invalid Host Param" );
    };
}

//============================================================================
QString GuiParams::describeStatus( EHostJoinStatus hostStatus )
{
    switch( hostStatus )
    {
    case eHostJoinUnknown:
        return QObject::tr( "Host Join Status Unknown " );
    case eHostJoinInvalidUrl:
        return QObject::tr( "Host Join Invalid Url" );
    case eHostJoinQueryIdInProgress:
        return QObject::tr( "Host Join Querying Id In Progress" );
    case eHostJoinQueryIdSuccess:
        return QObject::tr( "Host Join Querying Id Success" );
    case eHostJoinQueryIdFailed:
        return QObject::tr( "Host Join Query Id Failed" );
    case eHostJoinConnecting:
        return QObject::tr( "Host Join Connecting" );
    case eHostAnnounceHandshaking:
        return QObject::tr( "Host Join Handshaking" );
    case eHostAnnounceHandshakeTimeout:
        return QObject::tr( "Host Join Handshake Timeout" );
    case eHostJoinConnectSuccess:
        return QObject::tr( "Host Join Connect Success" );
    case eHostJoinConnectFailed:
        return QObject::tr( "Host Join Connect Failed" );
    case eHostJoinSendingJoinRequest:
        return QObject::tr( "Host Join Sending Request" );
    case eHostJoinSendJoinRequestFailed:
        return QObject::tr( "Host Join Send Request Failed" );
    case eHostJoinSuccess:
        return QObject::tr( "Host Join Success" );
    case eHostJoinFail:
        return QObject::tr( "Host Join Failed" );
    case eHostJoinFailPermission:
        return QObject::tr( "Host Join Fail Permission" );
    case eHostJoinFailConnectDropped:
        return QObject::tr( "Host Join Fail Connect Dropped" );
    case eHostJoinInvalidParam:
        return QObject::tr( "Host Join Invalid Param" );
    case eHostJoinDone:
        return QObject::tr( "Host Join Done" );
    default:
        return QObject::tr( "Invalid Host Param" );
    }
}

//============================================================================
QString GuiParams::describeStatus( EHostSearchStatus hostStatus )
{
    switch( hostStatus )
    {
    case eHostSearchUnknown:
        return QObject::tr( "Host Search Status Unknown " );
    case eHostSearchInvalidUrl:
        return QObject::tr( "Host Search Invalid Url" );
    case eHostSearchQueryIdInProgress:
        return QObject::tr( "Host Search Querying Id In Progress" );
    case eHostSearchQueryIdSuccess:
        return QObject::tr( "Host Search Querying Id Success" );
    case eHostSearchQueryIdFailed:
        return QObject::tr( "Host Search Query Id Failed" );
    case eHostSearchConnecting:
        return QObject::tr( "Host Search Connecting" );
    case eHostSearchHandshaking:
        return QObject::tr( "Host Announce Handshaking" );
    case eHostSearchHandshakeTimeout:
        return QObject::tr( "Host Announce Handshake Timeout" );
    case eHostSearchConnectSuccess:
        return QObject::tr( "Host Search Connect Success" );
    case eHostSearchConnectFailed:
        return QObject::tr( "Host Search Connect Failed" );
    case eHostSearchSendingSearchRequest:
        return QObject::tr( "Host Search Sending Request" );
    case eHostSearchSendSearchRequestFailed:
        return QObject::tr( "Host Search Send Request Failed" );
    case eHostSearchSuccess:
        return QObject::tr( "Host Search Success" );
    case eHostSearchFail:
        return QObject::tr( "Host Search Failed" );
    case eHostSearchFailPermission:
        return QObject::tr( "Host Search Fail Permission" );
    case eHostSearchFailConnectDropped:
        return QObject::tr( "Host Search Fail Connect Dropped" );
    case eHostSearchInvalidParam:
        return QObject::tr( "Host Search Invalid Param" );
    case eHostSearchDone:
        return QObject::tr( "Host Search Done" );
    default:
        return QObject::tr( "Invalid Host Param" );
    }
}

//============================================================================
QString GuiParams::describeFileLength( uint64_t fileLen )
{
    QString scaleText;
    char buf[ 128 ];
    if( fileLen >= 1000000000000ULL )
    {
        scaleText = QObject::tr( "TB" );
        sprintf( buf, "%3.1f ", (double)(fileLen) / 1000000000000.0);
    }
    else if( fileLen >= 1000000000ULL )
    {
        scaleText = QObject::tr( "GB" );
        sprintf( buf, "%3.1f ", (double)(fileLen) / 1000000000.0);
    }
    else if( fileLen >= 1000000 )
    {
        scaleText = QObject::tr( "MB" );
        sprintf( buf, "%3.1fMB ", (double)(fileLen) / 1000000.0);
    }
    else if( fileLen >= 1000 )
    {
        scaleText = QObject::tr( "KB" );
        sprintf( buf, "%3.1fKB ", (double)(fileLen) / 1000.0);
    }
    else
    {
        scaleText = QObject::tr( "Bytes" );
        sprintf( buf, "%3.1fBytes ", (double)fileLen );
    }

    QString strFormatedLen = buf;
    return strFormatedLen + scaleText;
}

//============================================================================
QString GuiParams::describeOrientation( Qt::Orientation qtOrientation )
{
    switch( qtOrientation )
    {
    case Qt::Horizontal:
        return QObject::tr("Horizontal");
    case Qt::Vertical:
    default:
        return QObject::tr("Vertical");
    }
}
