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
// http://www.gotvptop.com
//============================================================================

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
MyIcons::~MyIcons()
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
		return ":/AppRes/Resources/question_mark.svg";
	case eMyIconApp:
		return ":/AppRes/Resources/gotv_icon.svg";
	case eMyIconNotifyOverlay:		// activity flashing dot
		return ":/AppRes/Resources/notify-dot.svg";
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

	case eMyIconServers:
		return ":/AppRes/Resources/radio-tower.svg";
	case eMyIconOptions:
		return ":/AppRes/Resources/settings-gear.svg";
	case eMyIconFileServer:
		return ":/AppRes/Resources/file-folder-share.svg";

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

	case eMyIconWebCamClient:
		return ":/AppRes/Resources/webcam-client.svg";

	case eMyIconRelay:
		return ":/AppRes/Resources/question_mark.svg";
	case eMyIconRelayLocked:
	case eMyIconRelayIgnored:
	case eMyIconRelayDisabled:
		return ":/AppRes/Resources/question_mark.svg";

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
		return ":/AppRes/Resources/webcam.svg";

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
		return ":/AppRes/Resources/question_mark.svg";
	case eMyIconNetworkStateDisconnected:
		return ":/AppRes/Resources/question_mark.svg";
	case eMyIconNetworkStateDirectConnected:
		return ":/AppRes/Resources/question_mark.svg";
	case eMyIconNetworkStateAnchorFail:
		return ":/AppRes/Resources/question_mark.svg";
	case eMyIconNetworkStateRelayConnected:
		return ":/AppRes/Resources/question_mark.svg";

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
		return ":/AppRes/Resources/webcam-client.svg";
	case eMyIconMusicStreamPlayer:
		return ":/AppRes/Resources/music-stream-player.svg";
	case eMyIconRemoteControl:
		return ":/AppRes/Resources/remote-control.svg";
	case eMyIconPlugins:
		return ":/AppRes/Resources/plugin.svg";
	case eMyIconTheme:
		return ":/AppRes/Resources/theme.svg";
	case eMyIconSettings:
		return ":/AppRes/Resources/settings-gear.svg";


    case eMyIconNetworkKey:
        return ":/AppRes/Resources/wireless-key.svg";
    case eMyIconNetworkSettings:
        return ":/AppRes/Resources/wireless-setup.svg";
    case eMyIconSearchRandomPerson:
        return ":/AppRes/Resources/search-random-person.svg";

	// built in plugin options
	case eMyIconCamStreamProvider:
		return ":/AppRes/Resources/webcam-server.svg";
	case eMyIconGoTvStation:
		return ":/AppRes/Resources/radio-tower.svg";
	case eMyIconGoTvNetworkHost:
		return ":/AppRes/Resources/network-host.svg";

	// groups
    case eMyIconNetHosting:
        return ":/AppRes/Resources/network-host.svg";
	case eMyIconGroupUser:
		return ":/AppRes/Resources/group.svg";
	case eMyIconGroupHost:
		return ":/AppRes/Resources/group-host.svg";
	case eMyIconGroupListingHost:
		return ":/AppRes/Resources/group-anchor.svg";
    case eMyIconShareServices:
        return ":/AppRes/Resources/share-services.svg";

	default:
		return ":/AppRes/Resources/question_mark.svg";// Icon with question mark
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
EMyIcons MyIcons::getPluginIcon( EPluginType ePluginType, EPluginAccessState ePluginAccess )
{
	switch( ePluginType )
	{
	case	ePluginTypeAdmin:
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

	case	ePluginTypeRelay:
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

	case ePluginTypeStoryBoard:
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

	case	ePluginTypeWebServer:	// web server plugin ( for profile web page )
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

	case 	ePluginTypeFileOffer:	// file offer plugin
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

	case 	ePluginTypeFileServer:	// file share plugin
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

	case 	ePluginTypeCamServer:	// web cam broadcast plugin
		switch( ePluginAccess )
		{
		case ePluginAccessOk:
			return eMyIconWebCamServer;
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

	case 	ePluginTypeVoicePhone:	// VOIP p2p plugin
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

	case 	ePluginTypeVideoPhone:	// Video phone p2p plugin
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

	case 	ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
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

	case 	ePluginTypeMultiSession:	// multisession p2p plugin
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

    case ePluginTypeRandomConnect:	
        return eMyIconGroupUser;
        break;

    case ePluginTypeGroupHost:
        return eMyIconGroupHost;
        break;

    case ePluginTypeGroupListingHost:
        return eMyIconGroupListingHost;
        break;

    case ePluginTypeNetworkHost:
        return eMyIconNetHosting;
        break;

	default:
		LogMsg( LOG_ERROR, "MyIcons::getPluginIcon: unrecognized plugin type %d\n", ePluginType );
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
	case 	ePluginTypeFileOffer:	// file share plugin
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
	case 	ePluginTypeMultiSession:	// Web Cam Truth Or Dare game p2p plugin
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
	case 	ePluginTypeFileOffer:	// file offer plugin
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
	case 	ePluginTypeMultiSession:	// Web Cam Truth Or Dare game p2p plugin
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
	case 	ePluginTypeFileOffer:	// file share plugin
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
	case 	ePluginTypeMultiSession:	// Web Cam Truth Or Dare game p2p plugin
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