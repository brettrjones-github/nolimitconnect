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

#include "AppletPlayerCamClip.h"
#include "AppletCamClient.h"
#include "AppletCamSettings.h"
#include "AppletChatRoomJoinSearch.h"
#include "AppletChooseThumbnail.h"
#include "AppletChatRoomClient.h"
#include "AppletChatRoomJoin.h"
#include "AppletChatRoomJoinSearch.h"
#include "AppletClientRandomConnect.h"
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
#include "AppletPlayerPhoto.h"
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
#include "AppletSoundSettings.h"
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
#include "AppletFileShareClientView.h"
#include "AppletPeerVoicePhone.h"

#include "AppletAboutMeServerViewMine.h"
#include "AppletCamServerViewMine.h"
#include "AppletFileShareServerViewMine.h"
#include "AppletStoryboardServerViewMine.h"

#include "HomeWindow.h"

//============================================================================
AppletMgr::AppletMgr( AppCommon& myMpp, QWidget* parent )
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
QWidget* AppletMgr::getActiveWindow( void )
{
	QWidget* activeWidget = QApplication::activeWindow();
	if( 0 == activeWidget )
	{
		activeWidget = &m_MyApp.getHomePage();
	}

	return activeWidget;
}

//============================================================================
QFrame* AppletMgr::getAppletFrame( EApplet applet )
{
	return m_MyApp.getAppletFrame( applet );
}

//============================================================================
ActivityBase * AppletMgr::launchApplet( EApplet applet, QWidget* parent, QString launchParam, VxGUID assetId )
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
        if( launchAppletAllowed( eAppletDownloads ) )
        {
            bringAppletToFront( m_MyApp.getAppletUploads() );
                return m_MyApp.getAppletUploads();
        }
        else
        {
            return nullptr;
        }
    }

    // other applets
	ActivityBase * appletDialog = findAppletDialog( applet );
	if( appletDialog )
	{
		bringAppletToFront( appletDialog );
        if( applet == eAppletPlayerCamClip )
        {
            AppletPlayerCamClip* player = dynamic_cast<AppletPlayerCamClip*>(appletDialog);
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

    QWidget* launchFrame = m_MyApp.getHomePage().getLaunchPageFrame();
    // QWidget* messengerFrame = m_MyApp.getHomePage().getMessengerParentFrame();

	QString appletMissingTitle = QObject::tr( "Applet Not Yet Implemented" );
	switch( applet )
	{
    case eAppletAboutMeServerViewMine:
        if( viewMyServerAllowed( eAppletAboutMeServerViewMine ) )
        {
            AppletAboutMeServerViewMine* myViewDlg = new AppletAboutMeServerViewMine( m_MyApp, parent );
            if( myViewDlg )
            {
                myViewDlg->setIdentity( m_MyApp.getUserMgr().getMyIdent() );
                appletDialog = myViewDlg;
            }
        }
        break;

    case eAppletCamServerViewMine:
        if( viewMyServerAllowed( eAppletCamServerViewMine ) )
        {
            AppletCamServerViewMine* myViewDlg = new AppletCamServerViewMine( m_MyApp, parent );
            if( myViewDlg )
            {
                myViewDlg->setupCamFeed( m_MyApp.getUserMgr().getMyIdent() );
                appletDialog = myViewDlg;
            }
        }
        break;

    case eAppletStoryboardServerViewMine:
        if( viewMyServerAllowed( eAppletStoryboardServerViewMine ) )
        {
            AppletStoryboardServerViewMine* myViewDlg = new AppletStoryboardServerViewMine( m_MyApp, parent );
            if( myViewDlg )
            {
                myViewDlg->setIdentity( m_MyApp.getUserMgr().getMyIdent() );
                appletDialog = myViewDlg;
            }
        }
        break;

    case eAppletFileShareServerViewMine:
        if( viewMyServerAllowed( eAppletFileShareServerViewMine ) )
        {
            AppletFileShareServerViewMine* myViewDlg = new AppletFileShareServerViewMine( m_MyApp, parent );
            if( myViewDlg )
            {
                myViewDlg->setIdentity( m_MyApp.getUserMgr().getMyIdent() );
                appletDialog = myViewDlg;
            }
        }
        break;

    case eAppletFileShareClientView:        if( launchAppletAllowed( eAppletFileShareClientView ) ) appletDialog = new AppletFileShareClientView( m_MyApp, parent ); break;

    case eActivityAppSetup:                 if( launchAppletAllowed( eActivityAppSetup ) ) appletDialog = new ActivityAppSetup( m_MyApp, parent ); break;
    case eAppletAboutMeClient:              if( launchAppletAllowed( eAppletAboutMeClient ) ) appletDialog = new AppletAboutMeClient( m_MyApp, parent ); break;
    case eAppletAboutNoLimitConnect:        if( launchAppletAllowed( eAppletAboutNoLimitConnect ) ) appletDialog = new AppletAboutApp( m_MyApp, parent ); break;
    case eAppletApplicationInfo:            if( launchAppletAllowed( eAppletApplicationInfo ) ) appletDialog = new AppletApplicationInfo( m_MyApp, parent ); break;
    case eAppletBrowseFiles:                if( launchAppletAllowed( eAppletBrowseFiles ) ) appletDialog = new AppletBrowseFiles (m_MyApp, parent); break;
    case eAppletCamClient:                  if( launchAppletAllowed( eAppletCamClient ) ) appletDialog = new AppletCamClient( m_MyApp, parent ); break;
    case eAppletChatRoomJoin:               if( launchAppletAllowed( eAppletChatRoomJoin ) ) appletDialog = new AppletChatRoomJoin( m_MyApp, parent ); break;
    case eAppletChatRoomJoinSearch:         if( launchAppletAllowed( eAppletChatRoomJoinSearch ) ) appletDialog = new AppletChatRoomJoinSearch( m_MyApp, parent ); break;
    case eAppletChooseThumbnail:            if( launchAppletAllowed( eAppletChooseThumbnail ) ) appletDialog = new AppletChooseThumbnail( m_MyApp, parent ); break;
    case eAppletEditAboutMe:                if( launchAppletAllowed( eAppletEditAboutMe ) ) appletDialog = new AppletEditAboutMe( m_MyApp, parent ); break;
    case eAppletEditAvatarImage:            if( launchAppletAllowed( eAppletEditAvatarImage ) ) appletDialog = new AppletEditAvatarImage( m_MyApp, parent ); break;
    case eAppletEditStoryboard:             if( launchAppletAllowed( eAppletEditStoryboard ) ) appletDialog = new AppletEditStoryboard( m_MyApp, parent ); break;
    case eAppletGalleryEmoticon:            if( launchAppletAllowed( eAppletGalleryEmoticon ) ) appletDialog = new AppletGalleryEmoticon( m_MyApp, parent ); break;
    case eAppletGalleryImage:               if( launchAppletAllowed( eAppletGalleryImage ) ) appletDialog = new AppletGalleryImage( m_MyApp, parent ); break;
    case eAppletGalleryThumb:               if( launchAppletAllowed( eAppletGalleryThumb ) ) appletDialog = new AppletGalleryThumb( m_MyApp, parent ); break;
    case eAppletGetStarted:                 if( launchAppletAllowed( eAppletGetStarted ) ) appletDialog = new AppletGetStarted( m_MyApp, parent ); break;
    case eAppletGroupJoin:                  if( launchAppletAllowed( eAppletGroupJoin ) ) appletDialog = new AppletGroupJoin( m_MyApp, parent ); break;
    case eAppletGroupJoinSearch:            if( launchAppletAllowed( eAppletGroupJoinSearch ) ) appletDialog = new AppletGroupJoinSearch( m_MyApp, parent ); break;
    case eAppletGroupListLocalView:         if( launchAppletAllowed( eAppletGroupListLocalView ) ) appletDialog = new AppletGroupListLocalView( m_MyApp, parent ); break;
    case eAppletHelpNetSignalBars:          if( launchAppletAllowed( eAppletHelpNetSignalBars ) ) appletDialog = new AppletHelpNetSignalBars( m_MyApp, parent ); break;
    case eAppletRandomConnectJoin:          if( launchAppletAllowed( eAppletRandomConnectJoin ) ) appletDialog = new AppletRandomConnectJoin( m_MyApp, parent ); break;
    case eAppletRandomConnectJoinSearch:    if( launchAppletAllowed( eAppletRandomConnectJoinSearch ) ) appletDialog = new AppletRandomConnectJoinSearch( m_MyApp, parent ); break;
    case eAppletSoundSettings:              if( launchAppletAllowed( eAppletSoundSettings ) ) appletDialog = new AppletSoundSettings( m_MyApp, parent ); break;
    case eAppletStoryboardClient:           if( launchAppletAllowed( eAppletStoryboardClient ) ) appletDialog = new AppletStoryboardClient( m_MyApp, parent ); break;
    case eAppletLog:                        if( launchAppletAllowed( eAppletLog ) ) appletDialog = new AppletLog( m_MyApp, parent ); break;

    case eAppletHomePage:                   m_MyApp.errMessageBox( appletMissingTitle, "Home Page Not Implemented" ); return nullptr;

    case eAppletKodi:                       if( launchAppletAllowed( eAppletKodi ) ) appletDialog = new AppletKodi( m_MyApp, parent ); break;
    case eAppletLibrary:                    if( launchAppletAllowed( eAppletLibrary ) ) appletDialog = new AppletLibrary( m_MyApp, parent, launchParam ); break;
    case eAppletLogSettings:                if( launchAppletAllowed( eAppletLogSettings ) ) appletDialog = new AppletLogSettings( m_MyApp, parent ); break;
    case eAppletLogView:                    if( launchAppletAllowed( eAppletLogView ) ) appletDialog = new AppletLogView( m_MyApp, parent ); break;

    case eAppletMessengerFrame:             makeMessengerFullSized(); return appletDialog;

    case eAppletNetworkSettings:            if( launchAppletAllowed( eAppletNetworkSettings ) ) appletDialog = new AppletNetworkSettings( m_MyApp, parent ); break;
    case eAppletPersonalRecorder:           if( launchAppletAllowed( eAppletPersonalRecorder ) ) appletDialog = new AppletPersonalRecorder( m_MyApp, parent ); break;
    case eAppletSettingsPage:               if( launchAppletAllowed( eAppletSettingsPage ) ) appletDialog = new AppletSettingsPage( m_MyApp, parent ); break;

    case eAppletSearchPage:	                if( launchAppletAllowed( eAppletSearchPage ) ) appletDialog = new AppletSearchPage( m_MyApp, parent ); break;
    case eAppletSearchPersons:              if( launchAppletAllowed( eAppletSearchPersons ) ) appletDialog = new ActivityScanPeopleSearch( m_MyApp, eScanTypePeopleSearch, launchFrame ); break;
    case eAppletSearchMood:                 if( launchAppletAllowed( eAppletSearchMood ) ) appletDialog = new ActivityScanPeopleSearch( m_MyApp, eScanTypeMoodMsgSearch, launchFrame ); break;
    case eAppletScanAboutMe:                if( launchAppletAllowed( eAppletScanAboutMe ) ) appletDialog = new ActivityScanProfiles( m_MyApp, launchFrame ); break;
    case eAppletScanStoryboard:             if( launchAppletAllowed( eAppletScanStoryboard ) ) appletDialog = new ActivityScanStoryBoards( m_MyApp, launchFrame ); break;
    case eAppletScanSharedFiles:            if( launchAppletAllowed( eAppletScanSharedFiles ) ) appletDialog = new ActivityFileSearch( m_MyApp, launchFrame ); break;
    case eAppletScanWebCam:                 if( launchAppletAllowed( eAppletScanWebCam ) ) appletDialog = new ActivityScanWebCams( m_MyApp, launchFrame ); break;

    case eAppletPlayerCamClip:              if( launchAppletAllowed( eAppletPlayerCamClip ) ) appletDialog = new AppletPlayerCamClip( m_MyApp, parent, assetId ); break;
    case eAppletCamSettings:                if( launchAppletAllowed( eAppletCamSettings ) ) appletDialog = new AppletCamSettings( m_MyApp, parent ); break;
    case eAppletClientChatRoom:             if( launchAppletAllowed( eAppletClientChatRoom ) ) appletDialog = new AppletChatRoomClient( m_MyApp, parent ); break;
    case eAppletClientRandomConnect:        if( launchAppletAllowed( eAppletClientRandomConnect ) ) appletDialog = new AppletClientRandomConnect( m_MyApp, parent ); break;

    case eAppletFriendListClient:           if( launchAppletAllowed( eAppletFriendListClient ) ) appletDialog = new AppletFriendListClient( m_MyApp, parent ); break;
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
        
    case eAppletGroupListClient:            if( launchAppletAllowed( eAppletFriendListClient ) ) appletDialog = new AppletGroupListClient( m_MyApp, parent ); break;

    case eAppletGroupHostAdmin:             if( launchAppletAllowed( eAppletGroupHostAdmin ) ) appletDialog = new AppletGroupHostAdmin( m_MyApp, parent ); break;

    case eAppletHostChatRoomAdmin:          if( launchAppletAllowed( eAppletHostChatRoomAdmin ) ) appletDialog = new AppletHostChatRoomAdmin( m_MyApp, parent ); break;
    case eAppletHostChatRoomStatus:         if( launchAppletAllowed( eAppletHostChatRoomStatus ) ) appletDialog = new AppletHostChatRoomStatus( m_MyApp, parent ); break;
    case eAppletHostGroupStatus:            if( launchAppletAllowed( eAppletHostGroupStatus ) ) appletDialog = new AppletHostGroupStatus( m_MyApp, parent ); break;
    case eAppletHostNetworkStatus:          if( launchAppletAllowed( eAppletHostNetworkStatus ) ) appletDialog = new AppletHostNetworkStatus( m_MyApp, parent ); break;
    case eAppletHostRandomConnectStatus:    if( launchAppletAllowed( eAppletHostRandomConnectStatus ) ) appletDialog = new AppletHostRandomConnectStatus( m_MyApp, parent ); break;

    case eAppletInviteAccept:               if( launchAppletAllowed( eAppletInviteAccept ) ) appletDialog = new AppletInviteAccept( m_MyApp, parent ); break;
    case eAppletInviteCreate:               if( launchAppletAllowed( eAppletInviteCreate ) ) appletDialog = new AppletInviteCreate( m_MyApp, parent ); break;

    case eAppletNetHostingPage:             if( launchAppletAllowed( eAppletNetHostingPage ) ) appletDialog = new AppletNetHostingPage( m_MyApp, parent ); break;
    case eAppletHostJoinRequestList:        if( launchAppletAllowed( eAppletHostJoinRequestList ) ) appletDialog = new AppletHostJoinRequestList( m_MyApp, parent ); break;
    case eAppletHostSelect:                 if( launchAppletAllowed( eAppletHostSelect ) ) appletDialog = new AppletHostSelect( m_MyApp, parent ); break;
    case eAppletPersonOfferList:            if( launchAppletAllowed( eAppletPersonOfferList ) ) appletDialog = new AppletPersonOfferList( m_MyApp, parent ); break;
    case eAppletPopupMenu:                  if( launchAppletAllowed( eAppletPopupMenu ) ) appletDialog = new AppletPopupMenu( m_MyApp, parent ); break;
    case eAppletShareServicesPage:          if( launchAppletAllowed( eAppletShareServicesPage ) ) appletDialog = new AppletShareServicesPage( m_MyApp, parent ); break;

    case eAppletServiceAboutMe:             if( launchAppletAllowed( eAppletServiceAboutMe ) ) appletDialog = new AppletServiceAboutMe( m_MyApp, parent ); break;
    case eAppletServiceAvatarImage:         if( launchAppletAllowed( eAppletServiceAvatarImage ) ) appletDialog = new AppletServiceAvatarImage( m_MyApp, parent ); break;
    case eAppletServiceConnectionTest:      if( launchAppletAllowed( eAppletServiceConnectionTest ) ) appletDialog = new AppletServiceConnectionTest( m_MyApp, parent ); break;

    case eAppletPermissionList:             if( launchAppletAllowed( eAppletPermissionList ) ) appletDialog = new AppletPermissionList( m_MyApp, parent ); break;
    case eAppletServiceHostNetwork:         if( launchAppletAllowed( eAppletServiceHostNetwork ) ) appletDialog = new AppletServiceHostNetwork( m_MyApp, parent ); break;
    
    case eAppletServiceShareFiles:          if( launchAppletAllowed( eAppletServiceShareFiles ) ) appletDialog = new AppletServiceShareFiles( m_MyApp, parent ); break;
    case eAppletServiceShareWebCam:         if( launchAppletAllowed( eAppletServiceShareWebCam ) ) appletDialog = new AppletServiceShareWebCam( m_MyApp, parent ); break;
    case eAppletServiceStoryboard:          if( launchAppletAllowed( eAppletServiceStoryboard ) ) appletDialog = new AppletServiceStoryboard( m_MyApp, parent ); break;

    case eAppletSettingsAboutMe:            if( launchAppletAllowed( eAppletSettingsAboutMe ) ) appletDialog = new AppletSettingsAboutMe( m_MyApp, parent ); break;
    case eAppletSettingsAvatarImage:        if( launchAppletAllowed( eAppletSettingsAvatarImage ) ) appletDialog = new AppletSettingsAvatarImage( m_MyApp, parent ); break;
    case eAppletSettingsConnectTest:        if( launchAppletAllowed( eAppletSettingsConnectTest ) ) appletDialog = new AppletSettingsConnectionTest( m_MyApp, parent ); break;
    case eAppletSettingsFileXfer:           if( launchAppletAllowed( eAppletSettingsFileXfer ) ) appletDialog = new AppletSettingsFileXfer( m_MyApp, parent ); break;

    case eAppletSettingsHostChatRoom:       if( launchAppletAllowed( eAppletSettingsHostChatRoom ) ) appletDialog = new AppletSettingsHostChatRoom( m_MyApp, parent ); break;
    case eAppletSettingsHostGroup:          if( launchAppletAllowed( eAppletSettingsHostGroup ) ) appletDialog = new AppletSettingsHostGroup( m_MyApp, parent ); break;
    case eAppletSettingsHostNetwork:        if( launchAppletAllowed( eAppletSettingsHostNetwork ) ) appletDialog = new AppletSettingsHostNetwork( m_MyApp, parent ); break;
    case eAppletSettingsHostRandomConnect:  if( launchAppletAllowed( eAppletSettingsHostRandomConnect ) ) appletDialog = new AppletSettingsHostRandomConnect( m_MyApp, parent ); break;

    case eAppletSettingsMessenger:          if( launchAppletAllowed( eAppletSettingsMessenger ) ) appletDialog = new AppletSettingsMessenger( m_MyApp, parent ); break;
    case eAppletSettingsPushToTalk:         if( launchAppletAllowed( eAppletSettingsPushToTalk ) ) appletDialog = new AppletSettingsPushToTalk( m_MyApp, parent ); break;
    case eAppletSettingsShareFiles:         if( launchAppletAllowed( eAppletSettingsShareFiles ) ) appletDialog = new AppletSettingsShareFiles( m_MyApp, parent ); break;
    case eAppletSettingsStoryboard:         if( launchAppletAllowed( eAppletSettingsStoryboard ) ) appletDialog = new AppletSettingsStoryboard( m_MyApp, parent ); break;
    case eAppletSettingsTruthOrDare:        if( launchAppletAllowed( eAppletSettingsTruthOrDare ) ) appletDialog = new AppletSettingsTruthOrDare( m_MyApp, parent ); break;
    case eAppletSettingsVideoPhone:         if( launchAppletAllowed( eAppletSettingsVideoPhone ) ) appletDialog = new AppletSettingsVideoPhone( m_MyApp, parent ); break;
    case eAppletSettingsVoicePhone:         if( launchAppletAllowed( eAppletSettingsVoicePhone ) ) appletDialog = new AppletSettingsTruthOrDare( m_MyApp, parent ); break;
    case eAppletSettingsWebCamServer:       if( launchAppletAllowed( eAppletSettingsWebCamServer ) ) appletDialog = new AppletSettingsShareWebCam( m_MyApp, parent ); break;
    case eAppletShareOfferList:             if( launchAppletAllowed( eAppletShareOfferList ) ) appletDialog = new AppletShareOfferList( m_MyApp, parent ); break;
    case eAppletSnapshot:                   if( launchAppletAllowed( eAppletSnapshot ) ) appletDialog = new AppletSnapshot( m_MyApp, parent ); break;

    case eAppletTestAndDebug:               if( launchAppletAllowed( eAppletTestAndDebug ) ) appletDialog = new AppletTestAndDebug( m_MyApp, parent ); break;
    case eAppletTestHostClient:             if( launchAppletAllowed( eAppletTestHostClient ) ) appletDialog = new AppletTestHostClient( m_MyApp, parent ); break;
    case eAppletTestHostService:            if( launchAppletAllowed( eAppletTestHostService ) ) appletDialog = new AppletTestHostService( m_MyApp, parent ); break;

    case eAppletTheme:                      if( launchAppletAllowed( eAppletTheme ) ) appletDialog = new AppletTheme( m_MyApp, parent ); break;

    case eAppletUnknown:                    m_MyApp.errMessageBox( appletMissingTitle, QObject::tr("Unknown Or Not Implemented") ); return nullptr;

    case eAppletUserIdentity:               if( launchAppletAllowed( eAppletUserIdentity ) ) appletDialog = new AppletUserIdentity( m_MyApp, parent ); break;

    case eAppletMultiMessenger:	            if( launchAppletAllowed( eAppletMultiMessenger ) ) appletDialog = new AppletMultiMessenger( m_MyApp, parent ); break;
    case eAppletPeerChangeFriendship:	    if( launchAppletAllowed( eAppletPeerChangeFriendship ) ) appletDialog = new AppletPeerChangeFriendship( m_MyApp, parent ); break;
    case eAppletPeerReplyOfferFile:         if( launchAppletAllowed( eAppletPeerReplyOfferFile ) ) appletDialog = new AppletPeerReplyFileOffer( m_MyApp, parent ); break;
    case eAppletPeerTodGame:                if( launchAppletAllowed( eAppletPeerTodGame ) ) appletDialog = new AppletPeerTodGame( m_MyApp, parent ); break;
    case eAppletPeerVideoPhone:             if( launchAppletAllowed( eAppletPeerVideoPhone ) ) appletDialog = new AppletPeerVideoPhone( m_MyApp, parent ); break;
    case eAppletPeerVoicePhone:             if( launchAppletAllowed( eAppletPeerVoicePhone ) ) appletDialog = new AppletPeerVoicePhone( m_MyApp, parent ); break;

    case eAppletPeerSelectFileToSend:       if( launchAppletAllowed( eAppletPeerSelectFileToSend ) ) appletDialog = new AppletPeerSelectFileToSend( m_MyApp, parent ); break;
    case eAppletPeerSessionFileOffer:       if( launchAppletAllowed( eAppletPeerSessionFileOffer ) ) appletDialog = new AppletPeerSessionFileOffer( m_MyApp, parent ); break;

    case eAppletIgnoredHosts:               if( launchAppletAllowed( eAppletIgnoredHosts ) ) appletDialog = new AppletIgnoredHosts( m_MyApp, parent ); break;

    case eAppletPlayerPhoto:                if( launchAppletAllowed( eAppletPlayerPhoto ) ) appletDialog = new AppletPlayerPhoto( m_MyApp, parent ); break;
    case eAppletPlayerVideo:                if( launchAppletAllowed( eAppletPlayerVideo ) ) appletDialog = new AppletPlayerVideo( m_MyApp, parent ); break;
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
//	case ePluginAppletNlcStation:
//		m_MyApp.errMessageBox( appletMissingTitle, "Video Player Not Implemented" );
//		return;
//	case ePluginAppletNlcNetworkHost:
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

//============================================================================
bool AppletMgr::viewMyServerAllowed( EApplet applet )
{
    EPluginType pluginType{ ePluginTypeInvalid };
    switch( applet )
    {
    case eAppletAboutMeServerViewMine:
        pluginType = ePluginTypeAboutMePageServer;
        break;
    case eAppletCamServerViewMine:
        pluginType = ePluginTypeCamServer;
        break;
    case eAppletFileShareServerViewMine:
        pluginType = ePluginTypeFileShareServer;
        break;
    case eAppletStoryboardServerViewMine:
        pluginType = ePluginTypeStoryboardServer;
        break;
    default:
        break;
    }

    if( ePluginTypeInvalid == pluginType )
    {
        QMessageBox::information( this, QObject::tr( "Cannot View Disabled Service" ), 
            QObject::tr( "Unknown Plugin Service" ), QMessageBox::Ok );
        return false;
    }

    bool isEnabled = isServiceEnabled( pluginType ); 
    if( !isEnabled )
    {
        QMessageBox::information( this, QObject::tr( "Cannot View Disabled Service" ),
            GuiParams::describePluginType( pluginType ) + QObject::tr( " Cannot be viewed when permission is disable" ), QMessageBox::Ok );
        return false;
    }

    return true;
}

//============================================================================
bool AppletMgr::isServiceEnabled( EPluginType pluginType )
{
    return m_MyApp.getUserMgr().getMyIdent()->getPluginPermission( pluginType ) != eFriendStateIgnore;
}

//============================================================================
bool AppletMgr::launchAppletAllowed( EApplet applet )
{
    bool launchAllowed{ true };



    return launchAllowed;
}