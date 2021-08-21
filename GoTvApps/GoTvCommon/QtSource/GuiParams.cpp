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
#include <QScreen>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QDesktopWidget>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <CoreLib/VxDebug.h>

namespace
{
    static const int            TINY_PUSHBUTTON_SIZE = 16;
    static const int            SMALL_PUSHBUTTON_SIZE = 30;
    static const int            MEDIUM_PUSHBUTTON_SIZE = 46;
    static const int            LARGE_PUSHBUTTON_SIZE = 54;
}

//============================================================================
QColor GuiParams::m_OnlineBkgColor( 176, 255, 176 );
QColor GuiParams::m_OfflineBkgColor( 192, 192, 192 );
QColor GuiParams::m_NearbyBkgColor( 176, 176, 255 );
int GuiParams::m_DefaultFontHeight{ 10 };
float GuiParams::m_DisplayScale{1.0f};
int GuiParams::m_TinyPushButtonSize{ TINY_PUSHBUTTON_SIZE };
int GuiParams::m_SmallPushButtonSize{ SMALL_PUSHBUTTON_SIZE };
int GuiParams::m_MediumPushButtonSize{ MEDIUM_PUSHBUTTON_SIZE };
int GuiParams::m_LargePushButtonSize{ LARGE_PUSHBUTTON_SIZE };

//============================================================================
GuiParams::GuiParams()
{
}

//============================================================================
void GuiParams::initGuiParams(int defaultFontHeight)
{
    m_DefaultFontHeight = defaultFontHeight;
    m_SmallPushButtonSize = m_DefaultFontHeight * 3 + 2;
    m_DisplayScale = (float)m_SmallPushButtonSize / (float)SMALL_PUSHBUTTON_SIZE;
    m_TinyPushButtonSize = getScaled(TINY_PUSHBUTTON_SIZE);
    m_MediumPushButtonSize = getScaled(MEDIUM_PUSHBUTTON_SIZE);
    m_LargePushButtonSize = getScaled(LARGE_PUSHBUTTON_SIZE);


    /*
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
    */
}

//============================================================================
int GuiParams::getControlIndicatorWidth( void )
{ 
    return 10; 
}

//============================================================================
QSize GuiParams::getButtonSize( EButtonSize buttonSize )
{
    switch( buttonSize )
    {
    case eButtonSizeTiny:
        return QSize( m_TinyPushButtonSize, m_TinyPushButtonSize );
    case eButtonSizeSmall:
        return QSize( m_SmallPushButtonSize, m_SmallPushButtonSize );
    case eButtonSizeMedium:
        return QSize( m_MediumPushButtonSize, m_MediumPushButtonSize );
    case eButtonSizeLarge:
    default:
        return QSize( m_LargePushButtonSize, m_LargePushButtonSize );
    }
}

//============================================================================
QSize GuiParams::getThumbnailSize( void )
{
    return QSize( 240, 240 ); 
}

//============================================================================
QSize GuiParams::getSnapshotSize( void )
{
    return QSize( getScaled(320), getScaled(240) );
}

//============================================================================
QString GuiParams::describeAge( EAgeType gender )
{
    switch( gender )
    {
    case eAgeTypeUnspecified:
        return QObject::tr( "Any" );
    case eAgeTypeUnder21:
        return QObject::tr( "Under 21" );
    case eAgeType21OrOlder:
        return QObject::tr( "21 Or Older" );
    default:
        return QObject::tr( "Unknown" );
    }
}

//============================================================================
QString GuiParams::describeAge( int age )
{
    QString ageStr = QString::number( age );
    if( ( age >= 80 ) || ( age < 0 ) )
    {
        ageStr = QObject::tr( "Old" );
    }
    else if( age == 0 )
    {
        ageStr = QObject::tr( "Any" );
    }
    else if( age < 21 )
    {
        ageStr = QObject::tr( "Young" );
    }
    else if( age >= 21 && age < 80 )
    {
        ageStr = QObject::tr( "Middle Age" );
    }

    return ageStr;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
//============================================================================
QString GuiParams::describeCamStatus( QCamera::Status camStatus )
{
    QString camDesc = QObject::tr( "Unknown Cam Status" );
    switch(camStatus)
    {

    case QCamera::Status::UnavailableStatus:
        camDesc =  QObject::tr( "Cam Status Unavailable" );
        break;
    case QCamera::Status::UnloadedStatus:
        camDesc =  QObject::tr( "Cam Status Unloaded" );
        break;
    case QCamera::Status::LoadingStatus:
        camDesc =  QObject::tr( "Cam Status Loading" );
        break;
    case QCamera::Status::UnloadingStatus:
        camDesc =  QObject::tr( "Cam Status Unloading" );
        break;
    case QCamera::Status::LoadedStatus:
        camDesc =  QObject::tr( "Cam Status Loaded" );
        break;
    case QCamera::Status::StandbyStatus:
        camDesc =  QObject::tr( "Cam Status Standby" );
        break;
    case QCamera::Status::StartingStatus:
        camDesc =  QObject::tr( "Cam Status Starting" );
        break;
    case QCamera::Status::StoppingStatus:
        camDesc =  QObject::tr( "Cam Status Stopping" );
        break;
    case QCamera::Status::ActiveStatus:
        camDesc =  QObject::tr( "Cam Status Active" );
        break;
    }

    return camDesc;
}

//============================================================================
QString GuiParams::describeCamState( QCamera::State camState )
{
    QString camDesc = QObject::tr( "Unknown Cam State" );
    switch(camState)
    {
    case QCamera::State::UnloadedState:
        camDesc =  QObject::tr( "Cam State Unloaded" );
        break;
    case QCamera::State::LoadedState:
        camDesc =  QObject::tr( "Cam State Loaded" );
        break;
    case QCamera::State::ActiveState:
        camDesc =  QObject::tr( "Cam State Active" );
        break;
    }

    return camDesc;
}

#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

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
QString GuiParams::describePluginAction( GuiUser * netIdent, EPluginType ePluginType, EPluginAccess ePluginAccess )
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
                strAction = QObject::tr("Cannot be a relay (firewalled)");
            }
            else if( netIdent->isMyPreferedRelay() )
            {
                strAction = QObject::tr("Is my preferred relay");
            }
            else
            {
                strAction = QObject::tr("Can be preferred relay");
            }
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Relay Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeRelay ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Relay is disabled");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Relay unavailable due to ignore status");
            break;
        case ePluginAccessInactive:		// access denied because busy
            strAction = QObject::tr("Relay unavailable because is inactive");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Relay unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connection to be a relay");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Contact must be online be a relay");
            break;
        }
        break;

    case ePluginTypeWebServer:	// web server plugin ( for profile web page )
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("View About Me Page");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("View About Me Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeWebServer ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("About Me Page is disabled");
            break;
        case ePluginAccessInactive:		// plugin inactive
            strAction = QObject::tr("About Me Page is inactive");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("About Me Page unavailable due to ignore status");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("About Me Page unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect to provide About Me Page");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("About Me Page unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeStoryboard:	// web server plugin ( for storyboard web page )
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("View Story Board");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("View Story Board Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeStoryboard ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Story Board is disabled");
            break;
        case ePluginAccessInactive:		// plugin inactive
            strAction = QObject::tr("Story Board is inactive");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Story Board unavailable due to ignore status");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Story Board unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect to provide Story Board Page");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Story Board unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeFileXfer:	// file offer plugin
        switch( ePluginAccess )
        {

        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("Send A File");
            break;

        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Send A File Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeFileXfer ) );
            strAction += QObject::tr(" permission");
            break;

        case ePluginAccessDisabled:		// plugin disabled 
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Shared Files is disabled");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Send File unavailable due to ignore status");
            break;
        case ePluginAccessInactive:		// access denied because busy
            strAction = QObject::tr("Send File unavailable because is inactive");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Send File unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect to receive Send File");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Send File unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeFileServer:	// file share plugin
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("View Shared Files");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Shared Files Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeFileServer ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessInactive:		// no shared files
            strAction = QObject::tr("No Shared Files");
            break;
        case ePluginAccessDisabled:		// plugin disabled 
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Shared Files is disabled");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Shared Files unavailable due to ignore status");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Shared Files unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect to Share Files");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Shared Files unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeCamServer:	// web cam broadcast plugin
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("View Shared Web Cam");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Shared Web Cam Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeCamServer ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Shared Web Cam is disabled");
            break;
        case ePluginAccessInactive:		// plugin disabled or no files shared or no web cam broadcast
            strAction = QObject::tr("Shared Web Cam is not active");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Shared Web Cam unavailable due to ignore status");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Shared Web Cam unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect to Share Web Cam");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Shared Web Cam unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeVoicePhone:	// VOIP p2p plugin
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("Voice Call");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Voice Call Requires ");
            strAction += describeFriendState(  netIdent->getPluginPermission( ePluginTypeVoicePhone ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Voice Call is disabled");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Voice Call unavailable due to ignore status");
            break;
        case ePluginAccessInactive:
            strAction = QObject::tr("Voice Call unavailable because is Inactive");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Voice Call unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect for Voice Call");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Voice Call unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("Offer To Play Truth Or Dare");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Truth Or Dare Requires ");
            strAction += describeFriendState( netIdent->getPluginPermission( ePluginTypeTruthOrDare ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Truth Or Dare is disabled");
            break;
        case ePluginAccessIgnored:		// access denied because you are being ignored
            strAction = QObject::tr("Truth Or Dare unavailable due to ignore status");
            break;
        case ePluginAccessInactive:
            strAction = QObject::tr("Truth Or Dare unavailable because is Inactive");
            break;
        case ePluginAccessBusy:		// access denied because busy
            strAction = QObject::tr("Truth Or Dare unavailable because is busy");
            break;
        case ePluginAccessRequiresDirectConnect:		
            strAction = QObject::tr("Contact must have direct connect for Truth Or Dare");
            break;
        case ePluginAccessRequiresOnline:		
            strAction = QObject::tr("Truth Or Dare unavailable because contact is offline");
            break;
        }
        break;

    case ePluginTypeMessenger:	
        switch( ePluginAccess )
        {
        case ePluginAccessOk:			// plugin access allowed
            strAction = QObject::tr("Offer Chat Session");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Text Chat Requires ");
            strAction += describeFriendState( netIdent->getPluginPermission( ePluginTypeMessenger ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
            strAction = QObject::tr("Text Chat is disabled");
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
            strAction = QObject::tr("Offer Video Chat");
            break;
        case ePluginAccessLocked:		// insufficient Friend permission level
            strAction = QObject::tr("Video Chat Requires ");
            strAction += describeFriendState( netIdent->getPluginPermission( ePluginTypeVideoPhone ) );
            strAction += QObject::tr(" permission");
            break;
        case ePluginAccessDisabled:		// plugin disabled or no files shared or no web cam broadcast
        case ePluginAccessNotSet:
        case eMaxPluginAccessState:
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

    case ePluginTypeClientPeerUser:
        strPluginType = QObject::tr("Client Peer user");
        break;

    case ePluginTypeHostPeerUser:
        strPluginType = QObject::tr("Host Peer user");
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


    case ePluginTypeClientGroup:	//!< group client
        strPluginType = QObject::tr("Client Group");
        break;
    case ePluginTypeClientChatRoom:	//!< chat room client
        strPluginType = QObject::tr("Client Chat Room");
        break;
    case ePluginTypeClientRandomConnect:	//!< Random connect to another person client
        strPluginType = QObject::tr("Client Random Connect");
        break;
    case ePluginTypeClientNetwork:	//!< Random connect to another person client
        strPluginType = QObject::tr("Client No Limit Network");
        break;
    case ePluginTypeClientConnectTest:	//!< Connection Test Client
        strPluginType = QObject::tr("Client Connect Test");
        break;

    case ePluginTypeHostGroup:   //!< group hosting
        strPluginType = QObject::tr("Host Group");
        break;
    case ePluginTypeHostChatRoom:	//!< chat room hosting plugin
        strPluginType = QObject::tr("Host Chat Room");
        break;
    case ePluginTypeHostRandomConnect:	//!< Random connect to another person hosting
        strPluginType = QObject::tr("Host Random Connect");
        break;
    case ePluginTypeHostNetwork:	//!< master network hosting
        strPluginType = QObject::tr("Host No Limit Network");
        break;
    case ePluginTypeHostConnectTest:	//!< Connection Test Service
        strPluginType = QObject::tr("Host Connect Test");
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
std::string GuiParams::describePlugin( EPluginType ePluginType, bool rmtInitiated )
{
    std::string strPluginDesc = "";

    switch(ePluginType)
    {
    case ePluginTypeAdmin:
        strPluginDesc = QObject::tr( "Administration Service" ).toUtf8().constData();
        break;

    case ePluginTypeAboutMePage:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared About Me Page" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "About Me Page Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeClientPeerUser:
        strPluginDesc = QObject::tr( "Peer User Client" ).toUtf8().constData();
        break;
    case ePluginTypeHostPeerUser:
        strPluginDesc = QObject::tr( "Peer User Host (me)" ).toUtf8().constData();
        break;

    case ePluginTypeCamServer:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared Web Cam" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Web Cam Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeFileServer:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared Files" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Shared Files Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeFileXfer:
        strPluginDesc = QObject::tr( "Person To Person File Transfer" ).toUtf8().constData();
        break;

    case ePluginTypeHostConnectTest:
        strPluginDesc = QObject::tr( "Connection Test Service" ).toUtf8().constData();
        break;

    case ePluginTypeClientConnectTest:
        strPluginDesc = QObject::tr( "Connection Test Client" ).toUtf8().constData();
        break;

    case ePluginTypeClientGroup:
    case ePluginTypeHostGroup:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Group User" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Host Group Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeClientChatRoom:
        strPluginDesc = QObject::tr( "Chat Room User" ).toUtf8().constData();
        break;
    case ePluginTypeHostChatRoom:
        strPluginDesc = QObject::tr( "Host Chat Room Service" ).toUtf8().constData();
        break;

    case ePluginTypeClientRandomConnect:
        strPluginDesc = QObject::tr( "Connect To Random Person" ).toUtf8().constData();
        break;

    case ePluginTypeHostRandomConnect:
        strPluginDesc = QObject::tr( "Connect To Random Person Service" ).toUtf8().constData();
        break;

    case ePluginTypeNetworkSearchList:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Host List Search Service" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Host List Search Service" ).toUtf8().constData();
        }
        break;

    case ePluginTypeHostNetwork:
        strPluginDesc = QObject::tr( "Host No Limit Network" ).toUtf8().constData();
        break;

    case ePluginTypeMessenger:
        strPluginDesc = QObject::tr( "Messanger Service" ).toUtf8().constData();
        break;


    case ePluginTypeRelay:
        strPluginDesc = QObject::tr( "Relay Service" ).toUtf8().constData();
        break;

    case ePluginTypeStoryboard:
        if( rmtInitiated )
        {
            strPluginDesc = QObject::tr( "Shared Story Page (Blog)" ).toUtf8().constData();
        }
        else
        {
            strPluginDesc = QObject::tr( "Shared Story Page Service (Blog)" ).toUtf8().constData();
        }
        break;

    case ePluginTypeTruthOrDare:
        strPluginDesc = QObject::tr( "Truth Or Dare Video Chat Game" ).toUtf8().constData();
        break;

    case ePluginTypeVideoPhone:
        strPluginDesc = QObject::tr( "Phone Call With Video Chat" ).toUtf8().constData();
        break;

    case ePluginTypeVoicePhone:
        strPluginDesc = QObject::tr( "Phone Call With Voice Only" ).toUtf8().constData();
        break;

    case ePluginTypeCameraService:
        strPluginDesc = QObject::tr( "Camera Feed Service" ).toUtf8().constData();
        break;

    case ePluginTypeMJPEGReader:
        strPluginDesc = QObject::tr( "MJPEG Movie Reader" ).toUtf8().constData();
        break;

    case ePluginTypeMJPEGWriter:
        strPluginDesc = QObject::tr( "MJPEG Movie Recorder" ).toUtf8().constData();
        break;

    case ePluginTypePersonalRecorder:
        strPluginDesc = QObject::tr( "Personal Notes And Media Recorder" ).toUtf8().constData();
        break;

    case ePluginTypeNetServices:
        strPluginDesc = QObject::tr( "Network Services" ).toUtf8().constData();
        break;

    case ePluginTypeSearch:
        strPluginDesc = QObject::tr( "Search Services" ).toUtf8().constData();
        break;

    case ePluginTypeSndReader:
        strPluginDesc = QObject::tr( "Recorded Audio Reader" ).toUtf8().constData();
        break;

    case ePluginTypeSndWriter:
        strPluginDesc = QObject::tr( "Audio Recorder" ).toUtf8().constData();
        break;

    case ePluginTypeWebServer:
        strPluginDesc = QObject::tr( "Server for About Me And Story Pages" ).toUtf8().constData();
        break;

    default:
        strPluginDesc = QObject::tr( "UNKNOWN PLUGIN" ).toUtf8().constData();
    }

    return strPluginDesc;
}

//============================================================================
QString GuiParams::describePluginOffer( EPluginType ePluginType )
{
    QString strPluginOffer;
    switch( ePluginType )
    {
    case ePluginTypeRelay:	// proxy plugin
        strPluginOffer = QObject::tr(" Relay ");
        break;

    case ePluginTypeWebServer:	// web server plugin ( for profile web page )
        strPluginOffer = QObject::tr(" View Profile Page " );
        break;

    case ePluginTypeFileXfer:	// file offer plugin
        strPluginOffer = QObject::tr(" Receive A File " );
        break;

    case ePluginTypeFileServer:	// file share plugin
        strPluginOffer = QObject::tr(" View Shared Files " );
        break;

    case ePluginTypeCamServer:	// web cam broadcast plugin
        strPluginOffer = QObject::tr(" View Shared Web Cam ");
        break;

    case ePluginTypeMessenger:	// multi session chat plugin
        strPluginOffer = QObject::tr(" Join Chat Session " );
        break;

    case ePluginTypeVoicePhone:	// VOIP p2p plugin
        strPluginOffer = QObject::tr(" Voice Phone Call ");
        break;

    case ePluginTypeVideoPhone:
        strPluginOffer = QObject::tr(" Video Chat Offer ");
        break;

    case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
        strPluginOffer = QObject::tr(" Play Truth Or Dare ");
        break;

    case ePluginTypeStoryboard:	// story board plugin
        strPluginOffer = QObject::tr(" View Story Board ");
        break;

    default:
        strPluginOffer = QObject::tr("Unknown Plugin Offer");
        LogMsg( LOG_ERROR, "AppletPeerBase::describePluginOffer: unrecognized plugin %d\n", ePluginType );
    }

    return strPluginOffer;
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
    case eHostJoinHandshaking:
        return QObject::tr( "Host Join Handshaking" );
    case eHostJoinHandshakeTimeout:
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
QString GuiParams::describeOfferState( EOfferState offerState )
{
    switch( offerState )
    {
    case eOfferStateNone:
        return QObject::tr( "No Offer" );
    case eOfferStateSending:
        return QObject::tr( "Seding Offer" );
    case eOfferStateSent:
        return QObject::tr( "Offer Sent" );
    case eOfferStateSendFailed:
        return QObject::tr( "Offer Send Failed" );
    case eOfferStateRxedByUser:
        return QObject::tr( "Offer Recieved By User" );
    case eOfferStateBusy:
        return QObject::tr( "User Is Busy" );
    case eOfferStateAccepted:
        return QObject::tr( "Offer Accepted" );
    case eOfferStateRejected:
        return QObject::tr( "Offer Rejected" );
    case eOfferStateCanceled:
        return QObject::tr( "Offer Canceled" );
    case eOfferStateUserOffline:
        return QObject::tr( "User Is Offline" );
    case eOfferStateInSession:
        return QObject::tr( "Offer Is In Session" );
    case eOfferStateSessionComplete:
        return QObject::tr( "Offer Session Complete" );
    case eOfferStateSessionFailed:
        return QObject::tr( "Offer Session Failed" );
    default:
        return QObject::tr( "Unknown Offer Type" );
    }
}

//============================================================================
QString GuiParams::describeOfferType( EOfferType offerType )
{
    switch( offerType )
    {
    case eOfferTypeUnknown:
        return QObject::tr( "Unknonn Offer Type" );
    case eOfferTypeJoinGroup:
        return QObject::tr( "Offer: Join Group" );
    case eOfferTypeJoinChatRoom:
        return QObject::tr( "Offer: Join Chat Roome" );
    case eOfferTypeJoinRandomConnect:
        return QObject::tr( "Offer: Join Random Connect" );
    case eOfferTypePhotoFile:
        return QObject::tr( "Offer: Photo File" );
    case eOfferTypeAudioFile:
        return QObject::tr( "Offer: Audio File" );
    case eOfferTypeVideoFile:
        return QObject::tr( "Offer: Video File" );
    case eOfferTypeDocumentFile:
        return QObject::tr( "Offer: Document File" );
    case eOfferTypeArchiveFile:
        return QObject::tr( "Offer: Archive File" );
    case eOfferTypeExecutableFile:
        return QObject::tr( "Offer: Executable File" );
    case eOfferTypeOtherFile:
        return QObject::tr( "Offer: Unknown File Type" );
    case eOfferTypeDirectory:
        return QObject::tr( "Offer: Folder Of Files" );
    case eOfferTypeFriendship:
        return QObject::tr( "Offer: Friendship" );
    case eOfferTypeMessenger:
        return QObject::tr( "Offer: Instant Message Session" );
    case eOfferTypeTruthOrDare:
        return QObject::tr( "Offer: Truth Or Dare Game" );
    case eOfferTypeVideoPhone:
        return QObject::tr( "Offer: Video Chat" );
    case eOfferTypeVoicePhone:
        return QObject::tr( "Offer: Voice Phone Call" );

    case eOfferTypeChatText:
        return QObject::tr( "Offer: Chat Text" );
    case eOfferTypeChatFace:
        return QObject::tr( "Offer: Chat Fase" );
    case eOfferTypeChatStockAvatar:
        return QObject::tr( "Offer: Stock Avatar" );
    case eOfferTypeChatCustomAvatar:
        return QObject::tr( "Offer: Custom Avatar" );
    case eOfferTypeThumbnail:
        return QObject::tr( "Offer: Thumbnail" );
    case eOfferTypeCamRecord:
        return QObject::tr( "Offer: Cam Recording" );
    default:
        return QObject::tr( "Unknown Offer Type" );
    }
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

//============================================================================
QString GuiParams::describeFriendship( EFriendState friendState )
{
    switch( friendState )
    {
    case eFriendStateAnonymous:	// anonymous user
        return QObject::tr( "Anonymous " );
    case eFriendStateGuest:		// guest user
        return QObject::tr( "Guest " );
    case eFriendStateFriend:	// friend user
        return  QObject::tr( "Friend " );
    case eFriendStateAdmin:		// administrator
        return  QObject::tr( "Administrator " );
    default:
        return  QObject::tr( "Ignore " );
    }
}
