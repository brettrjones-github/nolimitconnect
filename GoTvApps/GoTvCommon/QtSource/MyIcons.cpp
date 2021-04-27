//============================================================================
// Copyright (C) 2010 Brett R. Jones
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
#include "MyIcons.h"
#include "AppCommon.h"
#include "VxAppTheme.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxTimer.h>

#include <QFileInfo>
#include <QPainter>

//============================================================================
MyIcons::MyIcons( AppCommon& appCommon )
: m_aoIcons()
, m_MyApp( appCommon)
{
}

//============================================================================
void MyIcons::myIconsStartup( void )
{
	VxTimer timer;
	for( int i = 0; i < eMaxMyIcon; i++ )
	{
		QString strFileName = getIconFile((EMyIcons)i);
		QFileInfo fileInfo( strFileName );
		if( false == fileInfo.exists())
		{
			LogMsg( LOG_ERROR, "MyIcons::myIconsStartup missing icon file %s\n", strFileName.toUtf8().constData() );
			continue;
		}

		QIcon oIcon( strFileName );
		if( oIcon.isNull())
		{
			LogMsg( LOG_ERROR, "MyIcons::myIconsStartup could not get icon %d\n", i);
		}

		m_aoIcons.push_back( oIcon );
	}

	LogMsg( LOG_INFO, "MyIcons took %3.3f ms to load\n", timer.elapsedMs() );
}

//============================================================================
QPixmap	 MyIcons::getIconPixmap( EMyIcons myIcon, QSize iconSize, QColor& iconColor )
{
	QPixmap iconPixmap;
	QString svgName = getIconFile( myIcon );
	if( isSvgFile( svgName ) )
	{
		iconPixmap = getMyApp().getAppTheme().svgRenderer( svgName, iconSize, iconColor );
	}
	else
	{
		QIcon& thisIcon = getIcon( myIcon );
		iconPixmap = thisIcon.pixmap( iconSize );
	}

	return iconPixmap;
}

//============================================================================
QPixmap	 MyIcons::getIconPixmap( EMyIcons myIcon, QSize iconSize )
{
	QPixmap iconPixmap;
	QString svgName = getIconFile( myIcon );
	if( isSvgFile( svgName ) )
	{
		iconPixmap = getMyApp().getAppTheme().svgRenderer( svgName, iconSize );
	}
	else
	{
		QIcon& thisIcon = getIcon( myIcon );
		iconPixmap = thisIcon.pixmap( iconSize );
	}

	return iconPixmap;
}

//============================================================================
bool MyIcons::isSvgFile( QString& iconFileName )
{
	QList<QString> extensionList;
	extensionList << "svg"; //"bmp"<<"jpg"<<"png";
	QFileInfo fileInfo( iconFileName );
	QString ext = fileInfo.suffix();
	return extensionList.contains(ext);
}

//============================================================================
//! get path to icon file
QString MyIcons::getIconFile( EMyIcons eMyIcon )
{
	switch( eMyIcon )
	{
	case eMyIconUnknown:
		return ":/AppRes/Resources/question-mark.svg";
	case eMyIconApp:
		return ":/AppRes/Resources/gotv_icon.svg";
    case eMyIconAvatarImage:
        return ":/AppRes/Resources/avatar.svg";

    case eMyIconNotifyJoinOverlay:		
        return ":/AppRes/Resources/notify-join.svg";
	case eMyIconNotifyOnlineOverlay:		
		return ":/AppRes/Resources/notify-dot.svg";
    case eMyIconNotifyOfferOverlay:		
        return ":/AppRes/Resources/notify-offer.svg";
    case eMyIconNotifyForbbidenOverlay:		
        return ":/AppRes/Resources/notify-offer.svg";
    case eMyIconNotifyErrorOverlay:		
        return ":/AppRes/Resources/notify-offer.svg";

	case eMyIconPlusOverlay:		// add to library or shared files
		return ":/AppRes/Resources/overlay-plus.svg";
	case eMyIconMinusOverlay:		// remove from library or shared files
		return ":/AppRes/Resources/overlay-minus.svg";
	case eMyIconFriendOverlay:		// is friend
		return ":/AppRes/Resources/overlay-friend.svg";
	case eMyIconGlobeOverlay:		// is global shared or unknown person
		return ":/AppRes/Resources/overlay-globe.svg";

    case eMyIconInformation:
        return ":/AppRes/Resources/info.svg";
	case eMyIconPeople:
		return ":/AppRes/Resources/friend.svg";

	case eMyIconSearch:
		return ":/AppRes/Resources/search-normal.svg";
    case eMyIconSearchJoinGroup:
        return ":/AppRes/Resources/search-group.svg";
    case eMyIconSearchJoinChatRoom:
        return ":/AppRes/Resources/search-chat-room.svg";
    case eMyIconSearchJoinRandomConnect:
        return ":/AppRes/Resources/search-random-connect.svg";
    case eMyIconSearchFolder:
        return ":/AppRes/Resources/file-folder-search.svg";
    case eMyIconSearchPerson:
        return ":/AppRes/Resources/magnifier-on-a-user.svg";
    case eMyIconSearchHeart:
        return ":/AppRes/Resources/search-heart.svg";
    case eMyIconSearchImage:
        return ":/AppRes/Resources/search-image.svg";
    case eMyIconSearchStoryboard:
        return ":/AppRes/Resources/search-storyboard.svg";
    case eMyIconSearchWebCam:
        return ":/AppRes/Resources/search-webcam.svg";

    case eMyIconShareWithPerson:
        return ":/AppRes/Resources/share-with-person.svg";

	case eMyIconServers:
		return ":/AppRes/Resources/radio-tower.svg";
	case eMyIconFileServer:
		return ":/AppRes/Resources/file-folder-share.svg";

    case eMyIconEyeAll:
        return ":/AppRes/Resources/eye-all.svg";
    case eMyIconEyeChatRoom:
        return ":/AppRes/Resources/eye-chat-room.svg";
    case eMyIconEyeFriends:
        return ":/AppRes/Resources/eye-friends.svg";
    case eMyIconEyeGroup:
        return ":/AppRes/Resources/eye-group.svg";
    case eMyIconEyeHide:
        return ":/AppRes/Resources/eye-hide.svg";
    case eMyIconEyeRandomConnect:
        return ":/AppRes/Resources/eye-random-connect.svg";
    case eMyIconEyeShow:
        return ":/AppRes/Resources/eye-show.svg";

	case eMyIconAdministrator:
		return ":/AppRes/Resources/user-tie.svg";
	case eMyIconFriend:
		return ":/AppRes/Resources/friend.svg";
	case eMyIconGuest:
		return ":/AppRes/Resources/man.svg";
	case eMyIconAnonymous:
		return ":/AppRes/Resources/unknown-person.svg";
	case eMyIconIgnored:
		return ":/AppRes/Resources/forbidden.svg";

    case eMyIconClientChatRoom:
        return ":/AppRes/Resources/client-chat-room.svg";
    case eMyIconClientGroup:
        return ":/AppRes/Resources/client-group.svg";
    case eMyIconClientRandomConnect:
        return ":/AppRes/Resources/client-random-connect.svg";
    case eMyIconClientWebCam:
		return ":/AppRes/Resources/client-webcam.svg";

	case eMyIconRelay:
		return ":/AppRes/Resources/question-mark.svg";
	case eMyIconRelayLocked:
	case eMyIconRelayIgnored:
	case eMyIconRelayDisabled:
		return ":/AppRes/Resources/question-mark.svg";

	case eMyIconProfile:
		return ":/AppRes/Resources/magnifier-on-a-user.svg";
	case eMyIconProfileLocked:
	case eMyIconProfileIgnored:
	case eMyIconProfileDisabled:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconFileDownloadDisabled:
		return ":/AppRes/Resources/lock.svg";
	case eMyIconFileDownloadCancel:
		return ":/AppRes/Resources/lock.svg";
	case eMyIconFileDownloadNormal:
		return ":/AppRes/Resources/cloud-download.svg";

	case eMyIconSendFileNormal:
		return ":/AppRes/Resources/cloud-upload.svg";
	case eMyIconSendFileCancel:
		return ":/AppRes/Resources/lock.svg";
	case eMyIconSendFileLocked:
	case eMyIconSendFileDisabled:
	case eMyIconSendFileIgnored:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconWebCamServer:
		return ":/AppRes/Resources/radio-tower.svg";
	case eMyIconWebCamServerLocked:
	case eMyIconWebCamServerIgnored:
	case eMyIconWebCamServerDisabled:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconMultiSession:
		return ":/AppRes/Resources/speech-bubble.svg";
	case eMyIconMultiSessionLocked:
	case eMyIconMultiSessionIgnored:
	case eMyIconMultiSessionDisabled:
		return ":/AppRes/Resources/lock.svg";
	case eMyIconMultiSessionCancel:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconVideoPhoneCancel:
		return ":/AppRes/Resources/videocam-enabled.svg";
	case eMyIconVideoPhoneNormal:
		return ":/AppRes/Resources/videocam-enabled.svg";
	case eMyIconVideoPhoneLocked:
	case eMyIconVideoPhoneIgnored:
	case eMyIconVideoPhoneDisabled:
		return ":/AppRes/Resources/videocam-enabled.svg";

	case eMyIconVoicePhoneCancel:
		return ":/AppRes/Resources/lock.svg";
	case eMyIconVoicePhoneNormal:
		return ":/AppRes/Resources/phone.svg";
	case eMyIconVoicePhoneLocked:
	case eMyIconVoicePhoneIgnored:
	case eMyIconVoicePhoneDisabled:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconMicrophoneOn:
		return ":/AppRes/Resources/microphone.svg";
	case eMyIconMicrophoneOff:
		return ":/AppRes/Resources/microphone-muted.svg";

	case eMyIconMicrophoneCancelNormal:
		return ":/AppRes/Resources/microphone-muted.svg";
	case eMyIconMicrophoneCancelDisabled:
		return ":/AppRes/Resources/microphone-muted.svg";

	case eMyIconTruthOrDareCancel:
		return ":/AppRes/Resources/lock.svg";
	case eMyIconTruthOrDareNormal:
		return ":/AppRes/Resources/dice.svg";
	case eMyIconTruthOrDareLocked:
	case eMyIconTruthOrDareIgnored:
	case eMyIconTruthOrDareDisabled:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconPermissions:
		return ":/AppRes/Resources/key.svg";
    case eMyIconKeyEnabled:
        return ":/AppRes/Resources/key-enabled.svg";
    case eMyIconKeyDisabled:
        return ":/AppRes/Resources/key-disabled.svg";
	case eMyIconNotifyStarWhite:
		return ":/AppRes/Resources/notify-dot.svg";
	case eMyIconNotifyStarGreen:
		return ":/AppRes/Resources/notify-dot.svg";

	case eMyIconRefreshNormal:
		return ":/AppRes/Resources/refresh.svg";
	case eMyIconRefreshDisabled:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconPhoto:
		return ":/AppRes/Resources/mediatype-image.svg";
	case eMyIconMusic:
		return ":/AppRes/Resources/mediatype-audio.svg";
	case eMyIconVideo:
		return ":/AppRes/Resources/mediatype-video.svg";
	case eMyIconDocument:
		return ":/AppRes/Resources/mediatype-text.svg";
	case eMyIconArcOrIso:
		return ":/AppRes/Resources/file-zip.svg";
	case eMyIconOther:
		return ":/AppRes/Resources/mediatype-text.svg";

	case eMyIconUpOneDirectory:
		return ":/AppRes/Resources/file-folder-up-directory.svg";
	case eMyIconFileUpload:
		return ":/AppRes/Resources/cloud-upload.svg";
	case eMyIconFileDownload:
		return ":/AppRes/Resources/cloud-download.svg";
	case eMyIconFileAdd:
		return ":/AppRes/Resources/file-folder-plus.svg";
	case eMyIconFileRemove:
		return ":/AppRes/Resources/file-folder-minus.svg";

	case eMyIconStoryBoardNormal:
		return ":/AppRes/Resources/storyboard.svg";
	case eMyIconStoryBoardDisabled:
	case eMyIconStoryBoardLocked:
	case eMyIconStoryBoardIgnored:
		return ":/AppRes/Resources/lock.svg";

	case eMyIconSpeakerOn:
		return ":/AppRes/Resources/speakers-on.svg";
	case eMyIconSpeakerOff:
		return ":/AppRes/Resources/speaker-muted.svg";

	case eMyIconDebug:
		return ":/AppRes/Resources/bug.svg";
	case eMyIconFileShareOptions:
		return ":/AppRes/Resources/file-folder-normal.svg";
	case eMyIconFileShareAddRemove:
		return ":/AppRes/Resources/file-folder-normal.svg";


	case eMyIconShareFilesDisabled:
		return ":/AppRes/Resources/file-folder-normal.svg";
	case eMyIconShareFilesNormal:
		return ":/AppRes/Resources/file-folder-share.svg";
	case eMyIconShareFilesCancel:
		return ":/AppRes/Resources/file-folder-cancel.svg";

	case eMyIconPauseNormal:
		return ":/AppRes/Resources/mediacontrol-pause.svg";
	case eMyIconPauseDisabled:
		return ":/AppRes/Resources/mediacontrol-pause.svg";

	case eMyIconPlayNormal:
		return ":/AppRes/Resources/play.svg";
	case eMyIconPlayDisabled:
		return ":/AppRes/Resources/play.svg";

	case eMyIconAcceptNormal:
		return ":/AppRes/Resources/check-mark.svg";
	case eMyIconAcceptDisabled:
		return ":/AppRes/Resources/check-mark.svg";
	case eMyIconCancelNormal:
		//return ":/AppRes/Resources/x-mark.svg";
		return ":/AppRes/Resources/x-overlay.svg";
	case eMyIconCancelDisabled:
		//return ":/AppRes/Resources/x-mark.svg";
		return ":/AppRes/Resources/x-overlay.svg";

	case eMyIconCheckMark:
		return ":/AppRes/Resources/check-mark.svg";
	case eMyIconRedX:
		return ":/AppRes/Resources/x-overlay.svg";

    case eMyIconCancel:
        return ":/AppRes/Resources/cancel.svg";

	case eMyIconTrash:
		return ":/AppRes/Resources/trash.svg";

	case eMyIconShredderDisabled:
		return ":/AppRes/Resources/trash.svg";
	case eMyIconShredderNormal:
		return ":/AppRes/Resources/trash.svg";

	case eMyIconFolder:
		return ":/AppRes/Resources/file-folder-normal.svg";
	case eMyIconFolderDisabled:
		return ":/AppRes/Resources/file-folder-normal.svg";

	case eMyIconRecordNormalDisabled:
		return ":/AppRes/Resources/videocam-record.svg";
	case eMyIconRecordNormalCancel:
		return ":/AppRes/Resources/videocam-record.svg";
	case eMyIconRecordNormalYellow:
		return ":/AppRes/Resources/videocam-record.svg";

	case eMyIconRecordMotionDisabled:
		return ":/AppRes/Resources/video-motion-record.svg";
	case eMyIconRecordMotionCancel:
		return ":/AppRes/Resources/video-motion-record.svg";
	case eMyIconRecordMotionNormal:
		return ":/AppRes/Resources/video-motion-record.svg";

	case eMyIconMotionAlarmDisabled:
		return ":/AppRes/Resources/motion-alarm.svg";
	case eMyIconMotionAlarmCancel:
		return ":/AppRes/Resources/motion-alarm.svg";
	case eMyIconMotionAlarmRed:
		return ":/AppRes/Resources/motion-alarm.svg";
	case eMyIconMotionAlarmWhite:
		return ":/AppRes/Resources/motion-alarm.svg";
	case eMyIconMotionAlarmYellow:
		return ":/AppRes/Resources/motion-alarm.svg";

	case eMyIconSendArrowDisabled:
		return ":/AppRes/Resources/send.svg";
	case eMyIconSendArrowCancel:
		return ":/AppRes/Resources/send.svg";;
	case eMyIconSendArrowNormal:
		return ":/AppRes/Resources/send.svg";

	case eMyIconCameraDisabled:
		return ":/AppRes/Resources/camera.svg";
	case eMyIconCameraCancel:
		return ":/AppRes/Resources/camera.svg";
	case eMyIconCameraNormal:
		return ":/AppRes/Resources/camera.svg";

	case eMyIconCamcorderNormal:
		return ":/AppRes/Resources/webcam.svg";
	case eMyIconCamcorderDisabled:
		return ":/AppRes/Resources/webcam.svg";
	case eMyIconCamcorderCancel:
		return ":/AppRes/Resources/webcam-stop.svg";

	case eMyIconSelectCameraDisabled:
		return ":/AppRes/Resources/cam-select.svg";
	case eMyIconSelectCameraNormal:
		return ":/AppRes/Resources/cam-select.svg";

	case eMyIconCameraFlashDisabled:
		return ":/AppRes/Resources/camera-flash.svg";
	case eMyIconCameraFlashNormal:
		return ":/AppRes/Resources/camera-flash.svg";
	case eMyIconCameraFlashCancelNormal:
		return ":/AppRes/Resources/camera-flash.svg";

	case eMyIconSnapshotDisabled:
		return ":/AppRes/Resources/camera-snapshot.svg";
	case eMyIconSnapshotNormal:
		return ":/AppRes/Resources/camera-snapshot.svg";
	case eMyIconSnapshotCancelNormal:
		return ":/AppRes/Resources/camera-snapshot.svg";

	case eMyIconNotepadDisabled:
		return ":/AppRes/Resources/notepad.svg";
	case eMyIconNotepadNormal:
		return ":/AppRes/Resources/notepad.svg";

	case eMyIconGalleryDisabled:
		return ":/AppRes/Resources/image-gallery.svg";
	case eMyIconGalleryNormal:
		return ":/AppRes/Resources/image-gallery.svg";
	case eMyIconGalleryCancel:
		return ":/AppRes/Resources/image-gallery.svg";

	case eMyIconFaceSelectDisabled:
		return ":/AppRes/Resources/emoj01.svg";
	case eMyIconFaceSelectNormal:
		return ":/AppRes/Resources/emoj01.svg";

	case eMyIconFaceCancelNormal:
		return ":/AppRes/Resources/emoj01.svg";

	case eMyIconCamPreviewNormal:
		return ":/AppRes/Resources/cam-preview.svg";
	case eMyIconCamPreviewDisabled:
		return ":/AppRes/Resources/cam-preview.svg";

	case eMyIconCamPreviewCancelNormal:
		return ":/AppRes/Resources/cam-preview.svg";
	case eMyIconCamPreviewCancelDisabled:
		return ":/AppRes/Resources/cam-preview.svg";

	case eMyIconCamRotateNormal:
		return ":/AppRes/Resources/cam-rotate.svg";
	case eMyIconCamRotateDisabled:
		return ":/AppRes/Resources/cam-rotate.svg";

	case eMyIconCamSetupNormal:
		return ":/AppRes/Resources/cam-setup.svg";
	case eMyIconCamSetupDisabled:
		return ":/AppRes/Resources/cam-setup.svg";

	case eMyIconCamSetupCancelNormal:
		return ":/AppRes/Resources/cam-setup.svg";
	case eMyIconCamSetupCancelDisabled:
		return ":/AppRes/Resources/cam-setup.svg";

	case eMyIconCamSelectNormal:
		return ":/AppRes/Resources/cam-select.svg";
	case eMyIconCamSelectDisabled:
		return ":/AppRes/Resources/cam-select.svg";

	case eMyIconImageRotateNormal:
		return ":/AppRes/Resources/image-rotate.svg";
	case eMyIconImageRotateDisabled:
		return ":/AppRes/Resources/image-rotate.svg";

	case eMyIconFileBrowseNormal:
		return ":/AppRes/Resources/file-folder-normal.svg";
	case eMyIconFileBrowseDisabled:
		return ":/AppRes/Resources/file-folder-normal.svg";
	case eMyIconFileBrowseCancel:
		return ":/AppRes/Resources/file-folder-normal.svg";

	case eMyIconMoveUpDirNormal:
		return ":/AppRes/Resources/file-folder-up-directory.svg";
	case eMyIconMoveUpDirDisabled:
		return ":/AppRes/Resources/file-folder-up-directory.svg";
	case eMyIconMoveUpDirCancel:
		return ":/AppRes/Resources/file-folder-up-directory.svg";

	case eMyIconLibraryDisabled:
		return ":/AppRes/Resources/library.svg";
	case eMyIconLibraryNormal:
		return ":/AppRes/Resources/library.svg";
	case eMyIconLibraryCancel:
		return ":/AppRes/Resources/library.svg";

	case eMyIconEchoCancelNormal:
		return ":/AppRes/Resources/speakers-on.svg";
	case eMyIconEchoCancelCancel:
		return ":/AppRes/Resources/speakers-on.svg";

	case eMyIconSendFailed:
		return ":/AppRes/Resources/msg_state_fail_resend.png";

	case eMyIconPersonUnknownNormal:
		return ":/AppRes/Resources/unknown-person.svg";

	case eMyIconMenuNormal:
		return ":/AppRes/Resources/menu_more_horiz.svg";
	case eMyIconMenuDisabled:
		return ":/AppRes/Resources/menu_more_horiz.svg";

	case eMyIconAdministratorDisabled:
		return ":/AppRes/Resources/user-tie.svg";

	case eMyIconNetworkStateDiscover:
		return ":/AppRes/Resources/question-mark.svg";
	case eMyIconNetworkStateDisconnected:
		return ":/AppRes/Resources/question-mark.svg";
	case eMyIconNetworkStateDirectConnected:
		return ":/AppRes/Resources/question-mark.svg";
	case eMyIconNetworkStateHostFail:
		return ":/AppRes/Resources/question-mark.svg";
	case eMyIconNetworkStateRelayConnected:
		return ":/AppRes/Resources/question-mark.svg";

	//=== title bar icons ===//
	case eMyIconPowerOff:
		return ":/AppRes/Resources/power_button.svg";
	case eMyIconHome:
		return ":/AppRes/Resources/home.svg";
    case eMyIconKodi:
        return ":/AppRes/Resources/kodi.svg";
    case eMyIconUserIdentity:
        return ":/AppRes/Resources/user-ident.svg";
    case eMyIconSharedContent:
        return ":/AppRes/Resources/file-folder-share.svg";
	case eMyIconShare:
		return ":/AppRes/Resources/share.svg";
	case eMyIconMenu:
		return ":/AppRes/Resources/menu_more_vert.svg";
	case eMyIconBack:
		return ":/AppRes/Resources/undo.svg";
	
	//=== bottom bar icons ===//
	case eMyIconArrowLeft:
		return ":/AppRes/Resources/arrow-left.svg";
	case eMyIcon30SecBackward:
		return ":/AppRes/Resources/mediacontrol-back-thirty.svg";
	case eMyIcon30SecForward:
		return ":/AppRes/Resources/mediacontrol-forward-thirty.svg";
	case eMyIconArrowRight:
		return ":/AppRes/Resources/arrow-right.svg";
	case eMyIconMediaRepeat:
		return ":/AppRes/Resources/menu_more_vert.svg";
	case eMyIconWindowExpand:
		return ":/AppRes/Resources/window-expand.svg";
	case eMyIconWindowShrink:
		return ":/AppRes/Resources/window-shrink.svg";
	case eMyIconMessenger:
		return ":/AppRes/Resources/speech-bubble.svg";
	case eMyIconGallery:
		return ":/AppRes/Resources/image-gallery.svg";
	case eMyIconVideoStreamViewer:
		return ":/AppRes/Resources/video-stream-viewer.svg";
	case eMyIconCamStreamViewer:
        return ":/AppRes/Resources/client-webcam.svg";
	case eMyIconMusicStreamPlayer:
		return ":/AppRes/Resources/music-stream-player.svg";
	case eMyIconRemoteControl:
		return ":/AppRes/Resources/remote-control.svg";
	case eMyIconPlugins:
		return ":/AppRes/Resources/plugin.svg";
	case eMyIconTheme:
		return ":/AppRes/Resources/theme.svg";
	case eMyIconSettingsGear:
		return ":/AppRes/Resources/settings-gear.svg";


    case eMyIconNetworkKey:
        return ":/AppRes/Resources/wireless-key.svg";
    case eMyIconNetworkSettings:
        return ":/AppRes/Resources/wireless-setup.svg";
    case eMyIconSearchRandomConnect:
        return ":/AppRes/Resources/search-random-person.svg";

	// built in plugin options
	case eMyIconCamStreamProvider:
		return ":/AppRes/Resources/webcam-server.svg";
	case eMyIconGoTvStation:
		return ":/AppRes/Resources/radio-tower.svg";

    case eMyIconShareServices:
        return ":/AppRes/Resources/share-services.svg";
    case eMyIconServiceAvatarImage:
        return ":/AppRes/Resources/share-avatar.svg";
    case eMyIconServiceChatRoom:
        return ":/AppRes/Resources/service-chat-room.svg";
    case eMyIconServiceConnectionTest:
        return ":/AppRes/Resources/service-connection-test.svg";
    case eMyIconServiceHostGroup:
        return ":/AppRes/Resources/service-group-host.svg";
    case eMyIconServiceHostGroupListing:
        return ":/AppRes/Resources/service-group-listing-host.svg";
    case eMyIconServiceHostNetwork:
        return ":/AppRes/Resources/service-network-host.svg";
    case eMyIconServiceShareAboutMe:
        return ":/AppRes/Resources/share-about-me.svg";
    case eMyIconServiceShareFiles:
        return ":/AppRes/Resources/share-files.svg";
    case eMyIconServiceShareStoryboard:
        return ":/AppRes/Resources/share-storyboard.svg";
    case eMyIconServiceShareWebCam:
        return ":/AppRes/Resources/share-webcam.svg";
    case eMyIconServiceRandomConnect:
        return ":/AppRes/Resources/service-connect-random.svg";
    case eMyIconServiceRandomConnectRelay:
        return ":/AppRes/Resources/service-connect-random-relay.svg";
    case eMyIconServiceRelay:
        return ":/AppRes/Resources/service-relay.svg";

    case eMyIconSettingsAboutMe:
        return ":/AppRes/Resources/share-about-me-settings.svg";
    case eMyIconSettingsAvatarImage:
        return ":/AppRes/Resources/share-avatar-settings.svg";
    case eMyIconSettingsChatRoom:
        return ":/AppRes/Resources/service-chat-room-settings.svg";
    case eMyIconSettingsConnectionTest:
        return ":/AppRes/Resources/service-connection-test-settings.svg";
    case eMyIconSettingsHostGroup:
        return ":/AppRes/Resources/service-group-host-settings.svg";
    case eMyIconSettingsHostGroupListing:
        return ":/AppRes/Resources/service-group-listing-host-settings.svg";
    case eMyIconSettingsHostNetwork:
        return ":/AppRes/Resources/service-network-host-settings.svg";
    case eMyIconSettingsShareFiles:
        return ":/AppRes/Resources/share-files-settings.svg";
    case eMyIconSettingsShareStoryboard:
        return ":/AppRes/Resources/share-storyboard-settings.svg";
    case eMyIconSettingsShareWebCam:
        return ":/AppRes/Resources/share-webcam-settings.svg";
    case eMyIconSettingsRandomConnect:
        return ":/AppRes/Resources/service-connect-random-settings.svg";
    case eMyIconSettingsRandomConnectRelay:
        return ":/AppRes/Resources/service-connect-random-relay-settings.svg";
    case eMyIconSettingsRelay:
        return ":/AppRes/Resources/service-relay-settings.svg";
    case eMyIconSettingsFileXfer:
        return ":/AppRes/Resources/file-xfer-settings.svg";
    case eMyIconSettingsMessenger:
        return ":/AppRes/Resources/messenger-settings.svg";
    case eMyIconSettingsTruthOrDare:
        return ":/AppRes/Resources/truth-or-dare-settings.svg";
    case eMyIconSettingsVideoPhone:
        return ":/AppRes/Resources/video-chat-settings.svg";
    case eMyIconSettingsVoicePhone:
        return ":/AppRes/Resources/voice-phone-settings.svg";
    case eMyIconFileXfer:
        return ":/AppRes/Resources/file-xfer.svg";
    case eMyIconTruthOrDare:
        return ":/AppRes/Resources/truth-or-dare.svg";

    case eMyIconEditRedo:
        return ":/AppRes/Resources/edit-action-redo.svg";
    case eMyIconEditUndo:
        return ":/AppRes/Resources/edit-action-undo.svg";

    case eMyIconEditAlignCenter:
        return ":/AppRes/Resources/edit-align-center.svg";
    case eMyIconEditAlignLeft:
        return ":/AppRes/Resources/edit-align-left.svg";
    case eMyIconEditAlignLeftAndRight:
        return ":/AppRes/Resources/edit-align-left-right.svg";
    case eMyIconEditAlignRight:
        return ":/AppRes/Resources/edit-align-right.svg";

    case eMyIconEditBold:
        return ":/AppRes/Resources/edit-bold.svg";
    case eMyIconEditCopy:
        return ":/AppRes/Resources/edit-copy.svg";
    case eMyIconEditCut:
        return ":/AppRes/Resources/edit-cut.svg";
    case eMyIconEditFileNew:
        return ":/AppRes/Resources/edit-file-new.svg";
    case eMyIconEditHeader:
        return ":/AppRes/Resources/edit-print.svg";
    case eMyIconEditItalic:
        return ":/AppRes/Resources/edit-italic.svg";
    case eMyIconEditPaperClip:
        return ":/AppRes/Resources/edit-paperclip.svg";
    case eMyIconEditPaste:
        return ":/AppRes/Resources/edit-paste.svg";
    case eMyIconEditPrint:
        return ":/AppRes/Resources/edit-print.svg";
    case eMyIconEditText:
        return ":/AppRes/Resources/edit-text.svg";
    case eMyIconEditUnderline:
        return ":/AppRes/Resources/edit-underline.svg";

    case eMyIconFileOpen:
        return ":/AppRes/Resources/file-open.svg";
    case eMyIconFileSave:
        return ":/AppRes/Resources/file-save.svg";
    case eMyIconFileSaveAs:
        return ":/AppRes/Resources/file-save-as.svg";

    case eMyIconZoomIn:
        return ":/AppRes/Resources/zoom-in.svg";
    case eMyIconZoomOut:
        return ":/AppRes/Resources/zoom-out.svg";

    case eMyIconOfferFile:
        return ":/AppRes/Resources/offer-file.svg";
    case eMyIconOfferFriendship:
        return ":/AppRes/Resources/offer-friendship.svg";
    case eMyIconOfferList:
        return ":/AppRes/Resources/offer-list.svg";
    case eMyIconOfferTruthOrDare:
        return ":/AppRes/Resources/offer-truth-or-dare.svg";
    case eMyIconOfferVideoChat:
        return ":/AppRes/Resources/offer-video-chat.svg";
    case eMyIconOfferVoicePhone:
        return ":/AppRes/Resources/offer-voice-phone.svg";

    case eMyIconCallList:
        return ":/AppRes/Resources/call-list.svg";

	default:
		return ":/AppRes/Resources/question-mark.svg";// Icon with question mark
	}
}

//============================================================================
//! get preloaded icon
QIcon& MyIcons::getIcon( EMyIcons eMyIcon )
{
	return m_aoIcons[ eMyIcon ];
}

//============================================================================
//! get icon for give friendship
EMyIcons MyIcons::getFriendshipIcon( EFriendState eFriendship )
{
	switch( eFriendship )
	{
	case eFriendStateIgnore:	// unknown or disabled or ignore
		return eMyIconIgnored;
	case eFriendStateAnonymous:	// anonymous user
		return eMyIconAnonymous;
	case eFriendStateGuest:	// guest user
		return eMyIconGuest;
	case eFriendStateFriend:		// friend user
		return eMyIconFriend;
	case eFriendStateAdmin:		// friend user
		return eMyIconAdministrator;
	default:
		return eMyIconUnknown;
	}
}

//============================================================================
//! get icon for given plugin
EMyIcons MyIcons::getPluginSettingsIcon( EPluginType ePluginType )
{
    switch( ePluginType )
    {
    case ePluginTypeAdmin:
        return eMyIconAdministrator;

    case ePluginTypeAboutMePage:
        return eMyIconSettingsAboutMe;

    case ePluginTypeClientPeerUser:
    case ePluginTypeHostPeerUser:
        return eMyIconSettingsAvatarImage;

    case ePluginTypeCamServer:
        return eMyIconSettingsShareWebCam;

    case ePluginTypeClientChatRoom:
        return eMyIconSettingsChatRoom;

    case ePluginTypeHostConnectTest:
        return eMyIconSettingsConnectionTest;

    case ePluginTypeClientConnectTest:
        return eMyIconSettingsConnectionTest;

    case ePluginTypeFileXfer:
        return eMyIconSendFileNormal;

    case ePluginTypeFileServer:
        return eMyIconSettingsShareFiles;

    case ePluginTypeHostChatRoom:
        return eMyIconSettingsChatRoom;

    case ePluginTypeHostGroup:
        return eMyIconSettingsHostGroup;

    case ePluginTypeNetworkSearchList:
        return eMyIconSettingsHostGroupListing;

    case ePluginTypeHostNetwork:
        return eMyIconSettingsHostNetwork;

    case ePluginTypeMessenger:
        return eMyIconSettingsMessenger; // eMyIconMultiSession

    case ePluginTypeRelay:
        return eMyIconSettingsRelay;

    case ePluginTypeClientRandomConnect:
        return eMyIconSettingsRandomConnect;

    case ePluginTypeHostRandomConnect:
        return eMyIconSettingsRandomConnectRelay;

    case ePluginTypeStoryboard:
        return eMyIconSettingsShareStoryboard;

    case ePluginTypeTruthOrDare:
        return eMyIconSettingsTruthOrDare;

    case ePluginTypeVideoPhone:
        return eMyIconSettingsVideoPhone;

    case ePluginTypeVoicePhone:
        return eMyIconSettingsVoicePhone;

    default:
        LogMsg( LOG_ERROR, "MyIcons::getPluginIcon: unrecognized plugin type %d\n", ePluginType );
    }

    return eMyIconUnknown;
}

//============================================================================
//! get icon for given plugin
EMyIcons MyIcons::getPluginIcon( EPluginType ePluginType, EPluginAccess ePluginAccess )
{
	switch( ePluginType )
	{
	case ePluginTypeAdmin:
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconAdministrator;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessLocked:            
		case ePluginAccessIgnored:
		case ePluginAccessBusy:
		case ePluginAccessRequiresOnline:
			return eMyIconAdministratorDisabled;
		}

		break;

    case ePluginTypeAboutMePage:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconProfile;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconProfileDisabled;
        case ePluginAccessLocked:
            return eMyIconProfileLocked;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconProfileIgnored;
        }
        break;

    case ePluginTypeClientChatRoom:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconServiceChatRoom;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconServiceChatRoom;
        case ePluginAccessLocked:
            return eMyIconServiceChatRoom;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconServiceChatRoom;
        }
        break;

    case ePluginTypeClientPeerUser:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconAvatarImage;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconAvatarImage;
        case ePluginAccessLocked:
            return eMyIconAvatarImage;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconAvatarImage;
        }
        break;

    case ePluginTypeHostPeerUser:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconAvatarImage;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconAvatarImage;
        case ePluginAccessLocked:
            return eMyIconAvatarImage;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconAvatarImage;
        }
        break;

    case ePluginTypeHostConnectTest:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconServiceConnectionTest;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconServiceConnectionTest;
        case ePluginAccessLocked:
            return eMyIconServiceConnectionTest;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconServiceConnectionTest;
        }
        break;

    case ePluginTypeClientConnectTest:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconServiceConnectionTest;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconServiceConnectionTest;
        case ePluginAccessLocked:
            return eMyIconServiceConnectionTest;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconServiceConnectionTest;
        }
        break;

    case ePluginTypeClientRandomConnect:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconServiceRandomConnect;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconServiceRandomConnect;
        case ePluginAccessLocked:
            return eMyIconServiceRandomConnect;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconServiceRandomConnect;
        }
        break;

    case ePluginTypeHostRandomConnect:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconServiceRandomConnectRelay;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconServiceRandomConnectRelay;
        case ePluginAccessLocked:
            return eMyIconServiceRandomConnectRelay;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconServiceRandomConnectRelay;
        }
        break;

	case ePluginTypeRelay:
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconRelay;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconRelayDisabled;
		case ePluginAccessLocked:            
			return eMyIconRelayLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconRelayIgnored;
		}
		break;

	case ePluginTypeStoryboard:
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconStoryBoardNormal;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconStoryBoardDisabled;
		case ePluginAccessLocked:
			return eMyIconStoryBoardLocked;
		case ePluginAccessIgnored:
		case ePluginAccessBusy:
			return eMyIconStoryBoardIgnored;
		}
		break;

	case ePluginTypeWebServer:	// web server plugin ( for profile web page )
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconProfile;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconProfileDisabled;
		case ePluginAccessLocked:
			return eMyIconProfileLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconProfileIgnored;
		}
		break;

	case ePluginTypeFileXfer:	// file offer plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconSendFileNormal;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconSendFileDisabled;
		case ePluginAccessLocked:
			return eMyIconSendFileLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconSendFileIgnored;
		}
		break;

	case ePluginTypeFileServer:	// file share plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconShareFilesNormal;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconShareFilesDisabled;
		case ePluginAccessLocked:
			return eMyIconShareFilesCancel;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconShareFilesCancel;
		}
		break;

	case ePluginTypeCamServer:	// web cam broadcast plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconServiceShareWebCam;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconWebCamServerDisabled;
		case ePluginAccessLocked:
			return eMyIconWebCamServerLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconWebCamServerIgnored;
		}
		break;

	case ePluginTypeVoicePhone:	// VOIP p2p plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconVoicePhoneNormal;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconVoicePhoneDisabled;
		case ePluginAccessLocked:
			return eMyIconVoicePhoneLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconVoicePhoneIgnored;
		}
		break;

	case ePluginTypeVideoPhone:	// Video phone p2p plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconVideoPhoneNormal;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconVideoPhoneDisabled;
		case ePluginAccessLocked:
			return eMyIconVideoPhoneLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconVideoPhoneIgnored;
		}
		break;

	case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconTruthOrDareNormal;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconTruthOrDareDisabled;
		case ePluginAccessLocked:
			return eMyIconTruthOrDareLocked;
		case ePluginAccessIgnored:
        case ePluginAccessBusy:
			return eMyIconTruthOrDareIgnored;
		}

        break;

	case ePluginTypeMessenger:	// multisession p2p plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconMultiSession;
		case ePluginAccessDisabled:
		case ePluginAccessInactive:
		case ePluginAccessRequiresDirectConnect:
		case ePluginAccessRequiresOnline:
			return eMyIconMultiSessionDisabled;
		case ePluginAccessLocked:
			return eMyIconMultiSessionLocked;
		case ePluginAccessIgnored:
		case ePluginAccessBusy:
			return eMyIconMultiSessionIgnored;
		}
		break;

    case ePluginTypeHostChatRoom:
        switch( ePluginAccess )
        {
        case ePluginAccessOk:
            return eMyIconServiceChatRoom;
        case ePluginAccessDisabled:
        case ePluginAccessInactive:
        case ePluginAccessRequiresDirectConnect:
        case ePluginAccessRequiresOnline:
            return eMyIconServiceChatRoom;
        case ePluginAccessLocked:
            return eMyIconServiceChatRoom;
        case ePluginAccessIgnored:
        case ePluginAccessBusy:
            return eMyIconServiceChatRoom;
        }
        break;

    case ePluginTypeHostGroup:
        return eMyIconServiceHostGroup;
        break;

    case ePluginTypeNetworkSearchList:
        return eMyIconServiceHostGroupListing;
        break;

    case ePluginTypeHostNetwork:
        return eMyIconServiceHostNetwork;
        break;

	default:
		LogMsg( LOG_ERROR, "MyIcons::getPluginIcon: unrecognized plugin type %d\n", ePluginType );
	}
	return eMyIconUnknown;
}

//============================================================================
//! get setup icon for given plugin
EMyIcons MyIcons::getPluginSetupIcon( EPluginType ePluginType)
{
    switch( ePluginType )
    {
    case ePluginTypeAdmin:
        return eMyIconUnknown; // not implemented

    case ePluginTypeAboutMePage:
        return eMyIconSettingsAboutMe;

    case ePluginTypeClientPeerUser:
    case ePluginTypeHostPeerUser:
         return eMyIconSettingsAvatarImage;

    case ePluginTypeClientChatRoom:
        return eMyIconSettingsChatRoom;

    case ePluginTypeHostConnectTest:
        return eMyIconSettingsConnectionTest;

    case ePluginTypeClientConnectTest:
        return eMyIconSettingsConnectionTest;

    case ePluginTypeClientRandomConnect:
        return eMyIconSettingsRandomConnect;

    case ePluginTypeHostRandomConnect:
        return eMyIconSettingsRandomConnectRelay;

    case ePluginTypeRelay:
        return eMyIconSettingsRelay;

    case ePluginTypeStoryboard:
        return eMyIconSettingsShareStoryboard;

    case ePluginTypeWebServer:	// web server plugin ( for profile web page )
        return eMyIconSettingsShareWebCam;

    case ePluginTypeFileXfer:	// file offer plugin
        return eMyIconSettingsFileXfer;

    case ePluginTypeFileServer:	// file share plugin
        return eMyIconSettingsShareFiles;

    case ePluginTypeCamServer:	// web cam broadcast plugin
        return eMyIconSettingsShareWebCam;

    case ePluginTypeVoicePhone:	// VOIP p2p plugin
        return eMyIconSettingsVoicePhone;

    case ePluginTypeVideoPhone:	// Video phone p2p plugin
        return eMyIconSettingsVideoPhone;

    case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
        return eMyIconSettingsTruthOrDare;

    case ePluginTypeMessenger:	// multisession p2p plugin
        return eMyIconSettingsMessenger;

    case ePluginTypeHostChatRoom:
        return eMyIconServiceChatRoom;

    case ePluginTypeHostGroup:
        return eMyIconSettingsHostGroup;

    case ePluginTypeNetworkSearchList:
        return eMyIconSettingsHostGroupListing;

    case ePluginTypeHostNetwork:
        return eMyIconSettingsHostNetwork;

    default:
        LogMsg( LOG_ERROR, "MyIcons::getPluginIcon: unrecognized plugin type %d\n", ePluginType );
        return eMyIconUnknown;
    }

    return eMyIconUnknown;
}

//============================================================================
//! get disabled plugin icon
EMyIcons MyIcons::getDisabledPluginIcon( EPluginType ePluginType )
{
	switch( ePluginType )
	{
	case	ePluginTypeWebServer:	// web server plugin ( for profile web page )
		return eMyIconProfileDisabled;
	case 	ePluginTypeFileXfer:	// file share plugin
		return eMyIconSendFileDisabled;
	case 	ePluginTypeFileServer:	// file share plugin
		return eMyIconShareFilesDisabled;
	case 	ePluginTypeCamServer:	// web cam broadcast plugin
		return eMyIconWebCamServerDisabled;
	case 	ePluginTypeVoicePhone:	// VOIP p2p plugin
		return eMyIconVoicePhoneDisabled;
	case 	ePluginTypeVideoPhone:	// Video phone p2p plugin
		return eMyIconVideoPhoneDisabled;
	case 	ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		return eMyIconTruthOrDareDisabled;
	case 	ePluginTypeMessenger:	// Web Cam Truth Or Dare game p2p plugin
		return eMyIconMultiSessionDisabled;
	default:		
		return eMyIconUnknown;
	}
}

//============================================================================
//! get icon for given plugin.. return icon for friendship level
EMyIcons MyIcons::getLockedPluginIcon( EPluginType ePluginType )
{
	switch( ePluginType )
	{
	case	ePluginTypeWebServer:	// web server plugin ( for profile web page )
		return eMyIconProfileLocked;
	case 	ePluginTypeFileXfer:	// file offer plugin
		return eMyIconSendFileNormal;
	case 	ePluginTypeFileServer:	// file share plugin
		return eMyIconShareFilesCancel;
	case 	ePluginTypeCamServer:	// web cam broadcast plugin
		return eMyIconWebCamServerLocked;
	case 	ePluginTypeVoicePhone:	// VOIP p2p plugin
		return eMyIconVoicePhoneLocked;
	case 	ePluginTypeVideoPhone:	// Video phone p2p plugin
		return eMyIconVideoPhoneLocked;
	case 	ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		return eMyIconTruthOrDareLocked;
	case 	ePluginTypeMessenger:	// Web Cam Truth Or Dare game p2p plugin
		return eMyIconMultiSessionLocked;
	default:		
		return eMyIconUnknown;
	}
}
//============================================================================
EMyIcons MyIcons::getIgnoredPluginIcon( EPluginType ePluginType )
{
	switch( ePluginType )
	{
	case	ePluginTypeWebServer:	// web server plugin ( for profile web page )
		return eMyIconProfileIgnored;
	case 	ePluginTypeFileXfer:	// file share plugin
		return eMyIconSendFileIgnored;
	case 	ePluginTypeFileServer:	// file share plugin
		return eMyIconShareFilesCancel;
	case 	ePluginTypeCamServer:	// web cam broadcast plugin
		return eMyIconWebCamServerIgnored;
	case 	ePluginTypeVoicePhone:	// VOIP p2p plugin
		return eMyIconVoicePhoneIgnored;
	case 	ePluginTypeVideoPhone:	// Video phone p2p plugin
		return eMyIconVideoPhoneIgnored;
	case 	ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
		return eMyIconTruthOrDareIgnored;
	case 	ePluginTypeMessenger:	// Web Cam Truth Or Dare game p2p plugin
		return eMyIconMultiSessionIgnored;
	default:		
		return eMyIconUnknown;
	}
}

//============================================================================
EMyIcons MyIcons::getFileIcon( uint8_t u8FileType )
{
	switch( u8FileType )
	{
	case	VXFILE_TYPE_PHOTO:	
		return eMyIconPhoto;
	case 	VXFILE_TYPE_AUDIO:	
		return eMyIconMusic;
	case 	VXFILE_TYPE_VIDEO:	
		return eMyIconVideo;
	case 	VXFILE_TYPE_DOC:	
		return eMyIconDocument;
	case 	VXFILE_TYPE_ARCHIVE_OR_CDIMAGE:	
		return eMyIconArcOrIso;
	case 	VXFILE_TYPE_DIRECTORY:	
		return eMyIconFolder;
	default:
		return eMyIconOther;
	}
}

//============================================================================
//! draw a icon with given color
void MyIcons::drawIcon( EMyIcons eIcon, QPainter* painter, const QRect& rect, QColor& iconColor )
{
    if( ( eIcon < eMaxMyIcon ) && painter )
    {
        QPixmap	pixmap = getIconPixmap( eIcon, rect.size(), iconColor );
        if( !pixmap.isNull() )
        {
            painter->save();

            painter->setRenderHint( QPainter::Antialiasing, true );
            painter->setRenderHint( QPainter::TextAntialiasing, true );
            painter->setRenderHint( QPainter::SmoothPixmapTransform, true );

            painter->drawPixmap( rect, pixmap );

            painter->restore();
        }
    }
}
