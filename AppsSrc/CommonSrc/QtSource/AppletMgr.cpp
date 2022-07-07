//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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

#include "AppletMgr.h"
#include "ActivityBase.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include "ActivityAppSetup.h"
#include "ActivityFileSearch.h"
#include "ActivityScanPeopleSearch.h"
#include "ActivityScanProfiles.h"
#include "ActivityScanStoryBoards.h"
#include "ActivityScanWebCams.h"

#include "AppletApplicationInfo.h"
#include "AppletAboutApp.h"
#include "AppletAboutMeClient.h"

#include "AppletBrowseFiles.h"

#include "AppletCamClipPlayer.h"
#include "AppletCamSettings.h"
#include "AppletCamTest.h"
#include "AppletChatRoomJoinSearch.h"
#include "AppletChooseThumbnail.h"
#include "AppletChatRoomClient.h"
#include "AppletChatRoomJoin.h"
#include "AppletChatRoomJoinSearch.h"
#include "AppletClientRandomConnect.h"
#include "AppletClientStoryboard.h"
#include "AppletDownloads.h"
#include "AppletEditAboutMe.h"
#include "AppletEditAvatarImage.h"
#include "AppletEditStoryboard.h"
#include "AppletFriendListClient.h"
#include "AppletGalleryEmoticon.h"
#include "AppletGalleryImage.h"
#include "AppletGalleryThumb.h"
#include "AppletGetStarted.h"

#include "AppletGroupHostAdmin.h"
#include "AppletGroupJoin.h"
#include "AppletGroupJoinSearch.h"
#include "AppletGroupListClient.h"
#include "AppletGroupListLocalView.h"

#include "AppletHelpNetSignalBars.h"
#include "AppletHostJoinRequestList.h"

#include "AppletHostChatRoomAdmin.h"
#include "AppletHostChatRoomStatus.h"
#include "AppletHostGroupStatus.h"
#include "AppletHostNetworkStatus.h"
#include "AppletHostRandomConnectStatus.h"

#include "AppletHostSelect.h"
#include "AppletIgnoredHosts.h"
#include "AppletInviteAccept.h"
#include "AppletInviteCreate.h"
#include "AppletHostGroupStatus.h"
#include "AppletKodi.h"
#include "AppletLibrary.h"
#include "AppletLog.h"
#include "AppletLogSettings.h"
#include "AppletLogView.h"
#include "AppletNearbyListClient.h"

#include "AppletNetHostingPage.h"
#include "AppletNetworkSettings.h"
#include "AppletPeerSelectFileToSend.h"
#include "AppletPeerSessionFileOffer.h"
#include "AppletPermissionList.h"
#include "AppletPersonalRecorder.h"
#include "AppletPersonOfferList.h"
#include "AppletPlayerVideo.h"
#include "AppletPopupMenu.h"
#include "AppletRandomConnectJoin.h"
#include "AppletRandomConnectJoinSearch.h"

#include "AppletServiceAboutMe.h"
#include "AppletServiceAvatarImage.h"
#include "AppletServiceConnectionTest.h"
#include "AppletServiceHostNetwork.h"
#include "AppletServiceShareFiles.h"
#include "AppletServiceShareWebCam.h"
#include "AppletServiceStoryboard.h"

#include "AppletSettingsAboutMe.h"
#include "AppletSettingsAvatarImage.h"
#include "AppletSettingsConnectionTest.h"
#include "AppletSettingsFileXfer.h"

#include "AppletSettingsHostChatRoom.h"
#include "AppletSettingsHostGroup.h"
#include "AppletSettingsHostNetwork.h"
#include "AppletSettingsHostRandomConnect.h"

#include "AppletSettingsMessenger.h"
#include "AppletSettingsPushToTalk.h"
#include "AppletSettingsRandomConnect.h"
#include "AppletSettingsShareFiles.h"
#include "AppletSettingsShareWebCam.h"
#include "AppletSettingsStoryboard.h"
#include "AppletSettingsTruthOrDare.h"
#include "AppletSettingsVideoPhone.h"
#include "AppletSettingsVoicePhone.h"

#include "AppletSettingsPage.h"
#include "AppletSearchPage.h"
#include "AppletShareOfferList.h"
#include "AppletShareServicesPage.h"
#include "AppletSnapshot.h"
#include "AppletStoryboardClient.h"

#include "AppletTestAndDebug.h"
#include "AppletTestHostClient.h"
#include "AppletTestHostService.h"

#include "AppletTheme.h"
#include "AppletUploads.h"
#include "AppletUserIdentity.h"

#include "AppletMultiMessenger.h"
#include "AppletPeerChangeFriendship.h"
#include "AppletPeerReplyFileOffer.h"
#include "AppletPeerTodGame.h"
#include "AppletPeerVideoPhone.h"
#include "AppletPeerViewSharedFiles.h"
#include "AppletPeerVoicePhone.h"

#include "HomeWindow.h"

//============================================================================
AppletMgr::AppletMgr( AppCommon& myMpp, QWidget * parent )
: QWidget( parent )
, m_MyApp( myMpp )
{
}

//============================================================================
RenderGlWidget * AppletMgr::getRenderConsumer( void )
{
    RenderGlWidget * renderConsumer = nullptr;
    for( auto iter = m_ActivityList.begin(); iter != m_ActivityList.end(); ++iter )
    {
        if( eAppletKodi == (*iter)->getAppletType()  )
        {
            if( (*iter)->isVisible() )
            {
                renderConsumer = ( ( AppletKodi * )( *iter ) )->getRenderConsumer();
            }

            break;
        }
    }

    return renderConsumer;
}

//============================================================================
QWidget * AppletMgr::getActiveWindow( void )
{
	QWidget * activeWidget = QApplication::activeWindow();
	if( 0 == activeWidget )
	{
		activeWidget = &m_MyApp.getHomePage();
	}

	return activeWidget;
}

//============================================================================
QFrame * AppletMgr::getAppletFrame( EApplet applet )
{
	return m_MyApp.getAppletFrame( applet );
}

//============================================================================
ActivityBase * AppletMgr::launchApplet( EApplet applet, QWidget * parent, QString launchParam, VxGUID assetId )
{
    // these are permanent applets
    if( eAppletMultiMessenger == applet )
    {
        bringAppletToFront( m_MyApp.getAppletMultiMessenger() );
        return m_MyApp.getAppletMultiMessenger();
    }
    else  if( eAppletDownloads == applet )
    {
        bringAppletToFront( m_MyApp.getAppletDownloads() );
        return m_MyApp.getAppletDownloads();
    }
    else  if( eAppletUploads == applet )
    {
        bringAppletToFront( m_MyApp.getAppletUploads() );
        return m_MyApp.getAppletUploads();
    }

    // other applets
	ActivityBase * appletDialog = findAppletDialog( applet );
	if( appletDialog )
	{
		bringAppletToFront( appletDialog );
        if( applet == eAppletCamClipPlayer )
        {
            AppletCamClipPlayer* player = dynamic_cast<AppletCamClipPlayer*>(appletDialog);
            if( player )
            {
                player->setPlayerAssetId( assetId );
            }          
        }
        // TODO figure out why on button click and attemp to launch the applet is done multiple times
//#ifdef DEBUG
//        m_MyApp.errMessageBox2( QObject::tr( "AppletMgr::launchApplet" ).toUtf8().constData(), QObject::tr( "Applet enum %d already launched\n" ).toUtf8().constData(), applet );
//#endif // DEBUG

		return appletDialog;
	}

	if( 0 == parent )
	{
		parent = getActiveWindow();
	}

    QWidget * launchFrame = m_MyApp.getHomePage().getLaunchPageFrame();
    // QWidget * messengerFrame = m_MyApp.getHomePage().getMessengerParentFrame();

	QString appletMissingTitle = QObject::tr( "Applet Not Yet Implemented" );
	switch( applet )
	{
    case eActivityAppSetup:                 appletDialog = new ActivityAppSetup( m_MyApp, parent ); break;
    case eAppletAboutMeClient:              appletDialog = new AppletAboutMeClient( m_MyApp, parent ); break;
    case eAppletAboutNoLimitConnect:        appletDialog = new AppletAboutApp( m_MyApp, parent ); break;
    case eAppletApplicationInfo:            appletDialog = new AppletApplicationInfo( m_MyApp, parent ); break;
    case eAppletBrowseFiles:                appletDialog = new AppletBrowseFiles (m_MyApp, parent); break;
    case eAppletChatRoomJoin:               appletDialog = new AppletChatRoomJoin( m_MyApp, parent ); break;
    case eAppletChatRoomJoinSearch:         appletDialog = new AppletChatRoomJoinSearch( m_MyApp, parent ); break;
    case eAppletChooseThumbnail:            appletDialog = new AppletChooseThumbnail( m_MyApp, parent ); break;
    case eAppletEditAboutMe:                appletDialog = new AppletEditAboutMe( m_MyApp, parent ); break;
    case eAppletEditAvatarImage:            appletDialog = new AppletEditAvatarImage( m_MyApp, parent ); break;
    case eAppletEditStoryboard:             appletDialog = new AppletEditStoryboard( m_MyApp, parent ); break;
    case eAppletGalleryEmoticon:            appletDialog = new AppletGalleryEmoticon( m_MyApp, parent ); break;
    case eAppletGalleryImage:               appletDialog = new AppletGalleryImage( m_MyApp, parent ); break;
    case eAppletGalleryThumb:               appletDialog = new AppletGalleryThumb( m_MyApp, parent ); break;
    case eAppletGetStarted:                 appletDialog = new AppletGetStarted( m_MyApp, parent ); break;
    case eAppletGroupJoin:                  appletDialog = new AppletGroupJoin( m_MyApp, parent ); break;
    case eAppletGroupJoinSearch:            appletDialog = new AppletGroupJoinSearch( m_MyApp, parent ); break;
    case eAppletGroupListLocalView:         appletDialog = new AppletGroupListLocalView( m_MyApp, parent ); break;
    case eAppletHelpNetSignalBars:          appletDialog = new AppletHelpNetSignalBars( m_MyApp, parent ); break;
    case eAppletRandomConnectJoin:          appletDialog = new AppletRandomConnectJoin( m_MyApp, parent ); break;
    case eAppletRandomConnectJoinSearch:    appletDialog = new AppletRandomConnectJoinSearch( m_MyApp, parent ); break;
    case eAppletStoryboardClient:           appletDialog = new AppletStoryboardClient( m_MyApp, parent ); break;
    case eAppletLog:                        appletDialog = new AppletLog( m_MyApp, parent ); break;        

    case eAppletHomePage:                   m_MyApp.errMessageBox( appletMissingTitle, "Home Page Not Implemented" ); return nullptr;

    case eAppletKodi:                       appletDialog = new AppletKodi( m_MyApp, parent ); break;
    case eAppletLibrary:                    appletDialog = new AppletLibrary( m_MyApp, parent, launchParam ); break;
    case eAppletLogSettings:                appletDialog = new AppletLogSettings( m_MyApp, parent ); break;
    case eAppletLogView:                    appletDialog = new AppletLogView( m_MyApp, parent ); break;

    case eAppletMessengerFrame:                  makeMessengerFullSized(); return appletDialog;
    case eAppletNetworkSettings:            appletDialog = new AppletNetworkSettings( m_MyApp, parent ); break;
    case eAppletPersonalRecorder:           appletDialog = new AppletPersonalRecorder( m_MyApp, parent ); break;
    case eAppletSettingsPage:               appletDialog = new AppletSettingsPage( m_MyApp, parent ); break;

    case eAppletSearchPage:	                appletDialog = new AppletSearchPage( m_MyApp, parent ); break;
    case eAppletSearchPersons:              appletDialog = new ActivityScanPeopleSearch( m_MyApp, eScanTypePeopleSearch, launchFrame ); break;
    case eAppletSearchMood:                 appletDialog = new ActivityScanPeopleSearch( m_MyApp, eScanTypeMoodMsgSearch, launchFrame ); break;
    case eAppletScanAboutMe:                appletDialog = new ActivityScanProfiles( m_MyApp, launchFrame ); break;
    case eAppletScanStoryboard:             appletDialog = new ActivityScanStoryBoards( m_MyApp, launchFrame ); break;
    case eAppletScanSharedFiles:            appletDialog = new ActivityFileSearch( m_MyApp, launchFrame ); break;
    case eAppletScanWebCam:                 appletDialog = new ActivityScanWebCams( m_MyApp, launchFrame ); break;

    case eAppletCamClipPlayer:              appletDialog = new AppletCamClipPlayer( m_MyApp, parent, assetId ); break;
    case eAppletCamSettings:                appletDialog = new AppletCamSettings( m_MyApp, parent ); break;
    case eAppletCamTest:                    appletDialog = new AppletCamTest( m_MyApp, parent ); break;
    case eAppletClientChatRoom:             appletDialog = new AppletChatRoomClient( m_MyApp, parent ); break;
    case eAppletClientRandomConnect:        appletDialog = new AppletClientRandomConnect( m_MyApp, parent ); break;
    case eAppletClientStoryboard:           appletDialog = new AppletClientStoryboard( m_MyApp, parent ); break;

    case eAppletFriendListClient:           appletDialog = new AppletFriendListClient( m_MyApp, parent ); break;
    case eAppletNearbyListClient: 
        if( m_MyApp.getEngine().isNearbyAvailable() )
        {
            appletDialog = new AppletNearbyListClient( m_MyApp, parent );
        }
        else
        {
            QMessageBox::information( parent, QObject::tr( "Wait For Connection Test" ), QObject::tr( "Requires Connection Test Completion Before Launching Nearby " ), QMessageBox::Ok );
        }
        
        break;
        
    case eAppletGroupListClient:            appletDialog = new AppletGroupListClient( m_MyApp, parent ); break;

    case eAppletGroupHostAdmin:             appletDialog = new AppletGroupHostAdmin( m_MyApp, parent ); break;

    case eAppletHostChatRoomAdmin:          appletDialog = new AppletHostChatRoomAdmin( m_MyApp, parent ); break;
    case eAppletHostChatRoomStatus:         appletDialog = new AppletHostChatRoomStatus( m_MyApp, parent ); break;
    case eAppletHostGroupStatus:            appletDialog = new AppletHostGroupStatus( m_MyApp, parent ); break;
    case eAppletHostNetworkStatus:          appletDialog = new AppletHostNetworkStatus( m_MyApp, parent ); break;
    case eAppletHostRandomConnectStatus:    appletDialog = new AppletHostRandomConnectStatus( m_MyApp, parent ); break;

    case eAppletInviteAccept:               appletDialog = new AppletInviteAccept( m_MyApp, parent ); break;
    case eAppletInviteCreate:               appletDialog = new AppletInviteCreate( m_MyApp, parent ); break;

    case eAppletNetHostingPage:             appletDialog = new AppletNetHostingPage( m_MyApp, parent ); break;
    case eAppletHostJoinRequestList:        appletDialog = new AppletHostJoinRequestList( m_MyApp, parent ); break;
    case eAppletHostSelect:                 appletDialog = new AppletHostSelect( m_MyApp, parent ); break;
    case eAppletPersonOfferList:            appletDialog = new AppletPersonOfferList( m_MyApp, parent ); break;
    case eAppletPopupMenu:                  appletDialog = new AppletPopupMenu( m_MyApp, parent ); break;
    case eAppletShareServicesPage:          appletDialog = new AppletShareServicesPage( m_MyApp, parent ); break;

    case eAppletServiceAboutMe:             appletDialog = new AppletServiceAboutMe( m_MyApp, parent ); break;
    case eAppletServiceAvatarImage:         appletDialog = new AppletServiceAvatarImage( m_MyApp, parent ); break;
    case eAppletServiceConnectionTest:      appletDialog = new AppletServiceConnectionTest( m_MyApp, parent ); break;

    case eAppletPermissionList:             appletDialog = new AppletPermissionList( m_MyApp, parent ); break;
    case eAppletServiceHostNetwork:         appletDialog = new AppletServiceHostNetwork( m_MyApp, parent ); break;
    
    case eAppletServiceShareFiles:          appletDialog = new AppletServiceShareFiles( m_MyApp, parent ); break;
    case eAppletServiceShareWebCam:         appletDialog = new AppletServiceShareWebCam( m_MyApp, parent ); break;
    case eAppletServiceStoryboard:          appletDialog = new AppletServiceStoryboard( m_MyApp, parent ); break;

    case eAppletSettingsAboutMe:            appletDialog = new AppletSettingsAboutMe( m_MyApp, parent ); break;
    case eAppletSettingsAvatarImage:        appletDialog = new AppletSettingsAvatarImage( m_MyApp, parent ); break;
    case eAppletSettingsConnectTest:        appletDialog = new AppletSettingsConnectionTest( m_MyApp, parent ); break;
    case eAppletSettingsFileXfer:           appletDialog = new AppletSettingsFileXfer( m_MyApp, parent ); break;

    case eAppletSettingsHostChatRoom:       appletDialog = new AppletSettingsHostChatRoom( m_MyApp, parent ); break;
    case eAppletSettingsHostGroup:          appletDialog = new AppletSettingsHostGroup( m_MyApp, parent ); break;
    case eAppletSettingsHostNetwork:        appletDialog = new AppletSettingsHostNetwork( m_MyApp, parent ); break;
    case eAppletSettingsHostRandomConnect:  appletDialog = new AppletSettingsHostRandomConnect( m_MyApp, parent ); break;

    case eAppletSettingsMessenger:          appletDialog = new AppletSettingsMessenger( m_MyApp, parent ); break;
    case eAppletSettingsPushToTalk:         appletDialog = new AppletSettingsPushToTalk( m_MyApp, parent ); break;
    case eAppletSettingsShareFiles:         appletDialog = new AppletSettingsShareFiles( m_MyApp, parent ); break;
    case eAppletSettingsStoryboard:         appletDialog = new AppletSettingsStoryboard( m_MyApp, parent ); break;
    case eAppletSettingsTruthOrDare:        appletDialog = new AppletSettingsTruthOrDare( m_MyApp, parent ); break;
    case eAppletSettingsVideoPhone:         appletDialog = new AppletSettingsVideoPhone( m_MyApp, parent ); break;
    case eAppletSettingsVoicePhone:         appletDialog = new AppletSettingsTruthOrDare( m_MyApp, parent ); break;
    case eAppletSettingsWebCamServer:       appletDialog = new AppletSettingsShareWebCam( m_MyApp, parent ); break;
    case eAppletShareOfferList:             appletDialog = new AppletShareOfferList( m_MyApp, parent ); break;
    case eAppletSnapshot:                   appletDialog = new AppletSnapshot( m_MyApp, parent ); break;

    case eAppletTestAndDebug:               appletDialog = new AppletTestAndDebug( m_MyApp, parent ); break;
    case eAppletTestHostClient:             appletDialog = new AppletTestHostClient( m_MyApp, parent ); break;
    case eAppletTestHostService:            appletDialog = new AppletTestHostService( m_MyApp, parent ); break;

    case eAppletTheme:                      appletDialog = new AppletTheme( m_MyApp, parent ); break;
    case eAppletUnknown:                    m_MyApp.errMessageBox( appletMissingTitle, QObject::tr("Unknown Or Not Implemented") ); return nullptr;
    case eAppletUserIdentity:               appletDialog = new AppletUserIdentity( m_MyApp, parent ); break;

    case eAppletMultiMessenger:	            appletDialog = new AppletMultiMessenger( m_MyApp, parent ); break;
    case eAppletPeerChangeFriendship:	    appletDialog = new AppletPeerChangeFriendship( m_MyApp, parent ); break;
    case eAppletPeerReplyOfferFile:         appletDialog = new AppletPeerReplyFileOffer( m_MyApp, parent ); break;
    case eAppletPeerTodGame:                appletDialog = new AppletPeerTodGame( m_MyApp, parent ); break;
    case eAppletPeerVideoPhone:             appletDialog = new AppletPeerVideoPhone( m_MyApp, parent ); break;
    case eAppletPeerViewSharedFiles:        appletDialog = new AppletPeerViewSharedFiles( m_MyApp, parent ); break;
    case eAppletPeerVoicePhone:             appletDialog = new AppletPeerVoicePhone( m_MyApp, parent ); break;

    case eAppletPeerSelectFileToSend:       appletDialog = new AppletPeerSelectFileToSend( m_MyApp, parent ); break;
    case eAppletPeerSessionFileOffer:       appletDialog = new AppletPeerSessionFileOffer( m_MyApp, parent ); break;

    case eAppletIgnoredHosts:              appletDialog = new AppletIgnoredHosts( m_MyApp, parent ); break;


//	case eAppletPlayerVideo:
//		appletDialog = new AppletPlayerVideo( m_MyApp, parent );
//		break;
//	case eAppletPlayerMusic:
//		m_MyApp.errMessageBox( appletMissingTitle, "MusicPlayer Not Implemented" );
//		return;
//	case eAppletPlayerPhoto:
//		m_MyApp.errMessageBox( appletMissingTitle, "ImagePlayer Not Implemented" );
//		return;
	//case eAppletStreamViewerVideo:
	//	m_MyApp.errMessageBox( appletMissingTitle, "VideoStreamViewer Not Implemented" );
	//	return;
	//case eAppletStreamViewerCam:
	//	m_MyApp.errMessageBox( appletMissingTitle, "CamStreamViewer Not Implemented" );
	//	return;
	//case eAppletStreamViewerMusic:
	//	m_MyApp.errMessageBox( appletMissingTitle, "MusicStreamViewer Not Implemented" );
	//	return;
	//case eAppletRemoteControl:
	//	appletDialog = new AppletRemoteControl( m_MyApp, parent );
	//	break;
	//case eAppletPlugins:
	//	m_MyApp.errMessageBox( appletMissingTitle, "Applet Plugins Not Implemented" );
	//	return;

//	case ePluginAppletCamProvider:
//		m_MyApp.errMessageBox( appletMissingTitle, "Cam Provider Not Implemented" );
//		return;
//	case ePluginAppletGoTvStation:
//		m_MyApp.errMessageBox( appletMissingTitle, "Video Player Not Implemented" );
//		return;
//	case ePluginAppletGoTvNetworkHost:
//		m_MyApp.errMessageBox( appletMissingTitle, "Network Host Not Implemented" );
//		return;
	default:
		m_MyApp.errMessageBox2( QObject::tr( "AppCommon::launchApplet").toUtf8().constData(), QObject::tr( "Invalid Applet enum %d" ).toUtf8().constData(), applet );
		return nullptr;
	}

	if( appletDialog )
	{
        AppSettings& appSettings = m_MyApp.getAppSettings();
        appSettings.setLastAppletLaunched( applet );

        appletDialog->aboutToLaunchApplet();
        appletDialog->show();
        if( !findAppletDialog( appletDialog ) )
        {
            m_ActivityList.push_back( appletDialog );
        }
    }

    return appletDialog;
}

//============================================================================
void AppletMgr::activityStateChange( ActivityBase * activity, bool isCreated )
{
	if( !isCreated )
	{
        removeApplet( activity );
	}
	else
	{
		addApplet( activity );
	}
}

//============================================================================
void AppletMgr::bringAppletToFront( ActivityBase * appletDialog )
{
    if( appletDialog )
    {
        if( !appletDialog->isVisible() )
        {
            appletDialog->show();
        }
    }


    // TODO force window to front
}

//============================================================================
ActivityBase * AppletMgr::findAppletDialog( EApplet applet )
{
	QVector<ActivityBase *>::iterator iter;
	for( iter = m_ActivityList.begin(); iter != m_ActivityList.end(); ++iter )
	{
		if( applet == (*iter)->getAppletType() )
		{
			return *iter;
		}
	}
	
	return nullptr;
}

//============================================================================
ActivityBase * AppletMgr::findAppletDialog( ActivityBase * activity )
{
    if( activity )
    {
        QVector<ActivityBase *>::iterator iter;
        for( iter = m_ActivityList.begin(); iter != m_ActivityList.end(); ++iter )
        {
            if( activity == ( *iter ) )
            {
                return *iter;
            }
        }
    }

    return nullptr;
}

//============================================================================
void AppletMgr::addApplet( ActivityBase * activity )
{
	if( 0 == findAppletDialog( activity ) )
	{
        m_ActivityList.push_back( activity );
	}
	else 
	{
		LogMsg( LOG_ERROR, " AppletMgr::addApplet: adding already existing applet\n" );
	}
}

//============================================================================
void AppletMgr::removeApplet( EApplet applet )
{
	QVector<ActivityBase *>::iterator iter;
	for( iter = m_ActivityList.begin(); iter != m_ActivityList.end(); ++iter )
	{
		if( applet == ( *iter )->getAppletType() )
		{
            m_ActivityList.erase( iter );
			break;
		}
	}
}

//============================================================================
void AppletMgr::removeApplet( ActivityBase * activity )
{
    QVector<ActivityBase *>::iterator iter;
    for( iter = m_ActivityList.begin(); iter != m_ActivityList.end(); ++iter )
    {
        if( activity == ( *iter ) )
        {
            m_ActivityList.erase( iter );
            break;
        }
    }
}

//============================================================================
void AppletMgr::makeMessengerFullSized( void )
{
    m_MyApp.getHomePage().setIsMaxScreenSize( true, true );
}
