#pragma once
enum EMyIcons
{
	eMyIconNone			= 0,		// no icon to show
	eMyIconUnknown,					// Icon with question mark
	eMyIconApp,
	eMyIconNotifyOverlay,			// activity flashing dot
	eMyIconPlusOverlay,				// add to shared or library icon overlay
	eMyIconMinusOverlay,			// remove from shared or library
	eMyIconFriendOverlay,			// is friend
	eMyIconGlobeOverlay,			// is global shared or unknown person

    eMyIconInformation,
    eMyIconPeople,
	eMyIconServers,
	eMyIconOptions,
	eMyIconFileServer,

    eMyIconSearch,
    eMyIconSearchPerson,
    eMyIconSearchHeart,
    eMyIconSearchWebCam,
    eMyIconSearchImage,
    eMyIconSearchStoryboard,
    eMyIconSearchFolder,

	eMyIconAdministrator,
	eMyIconFriend,
	eMyIconGuest,
	eMyIconAnonymous,
	eMyIconIgnored,

	eMyIconWebCamClient,

	eMyIconRelay,
	eMyIconRelayLocked,
	eMyIconRelayDisabled,
	eMyIconRelayIgnored,

	eMyIconProfile,
	eMyIconProfileLocked,
	eMyIconProfileDisabled,
	eMyIconProfileIgnored,

	eMyIconWebCamServer,
	eMyIconWebCamServerLocked,
	eMyIconWebCamServerDisabled,
	eMyIconWebCamServerIgnored,

	eMyIconFolder,
	eMyIconFolderDisabled,

	eMyIconMultiSession,
	eMyIconMultiSessionLocked,
	eMyIconMultiSessionDisabled,
	eMyIconMultiSessionIgnored,
	eMyIconMultiSessionCancel,

	eMyIconVoicePhoneCancel,
	eMyIconVoicePhoneNormal,
	eMyIconVoicePhoneDisabled,
	eMyIconVoicePhoneLocked,
	eMyIconVoicePhoneIgnored,

	eMyIconVideoPhoneCancel,
	eMyIconVideoPhoneNormal,
	eMyIconVideoPhoneDisabled,
	eMyIconVideoPhoneLocked,
	eMyIconVideoPhoneIgnored,

	eMyIconTruthOrDareCancel,
	eMyIconTruthOrDareNormal,
	eMyIconTruthOrDareDisabled,
	eMyIconTruthOrDareLocked,
	eMyIconTruthOrDareIgnored,

	eMyIconPermissions,
    eMyIconKeyEnabled,
    eMyIconKeyDisabled,

	eMyIconNotifyStarWhite,
	eMyIconNotifyStarGreen,

	eMyIconRefreshNormal,
	eMyIconRefreshDisabled,

	eMyIconPhoto,
	eMyIconMusic,
	eMyIconVideo,
	eMyIconDocument,
	eMyIconArcOrIso,
	eMyIconOther,

	eMyIconUpOneDirectory,
	eMyIconFileUpload,
	eMyIconFileDownload,
	eMyIconFileAdd,
	eMyIconFileRemove,

	eMyIconStoryBoardNormal,
	eMyIconStoryBoardDisabled,
	eMyIconStoryBoardLocked,
	eMyIconStoryBoardIgnored,

	eMyIconMicrophoneOff,
	eMyIconMicrophoneOn,

	eMyIconMicrophoneCancelDisabled,
	eMyIconMicrophoneCancelNormal,

	eMyIconDebug,
	eMyIconFileShareOptions,
	eMyIconFileShareAddRemove,

	eMyIconShareFilesDisabled,
	eMyIconShareFilesNormal,
	eMyIconShareFilesCancel,

	eMyIconPauseDisabled,
	eMyIconPauseNormal,

	eMyIconPlayDisabled,
	eMyIconPlayNormal,

	eMyIconRedX,
	eMyIconCheckMark,

	eMyIconTrash,

	eMyIconShredderDisabled,
	eMyIconShredderNormal,

	eMyIconFileDownloadDisabled,
	eMyIconFileDownloadCancel,
	eMyIconFileDownloadNormal,

	eMyIconSendFileDisabled,
	eMyIconSendFileCancel,
	eMyIconSendFileNormal,
	eMyIconSendFileLocked,
	eMyIconSendFileIgnored,

	eMyIconRecordNormalDisabled,
	eMyIconRecordNormalCancel,
	eMyIconRecordNormalYellow,

	eMyIconRecordMotionDisabled,
	eMyIconRecordMotionCancel,
	eMyIconRecordMotionNormal,

	eMyIconMotionAlarmDisabled,
	eMyIconMotionAlarmCancel,
	eMyIconMotionAlarmRed,
	eMyIconMotionAlarmWhite,
	eMyIconMotionAlarmYellow,

	eMyIconAcceptNormal,
	eMyIconAcceptDisabled,
	eMyIconCancelNormal,
	eMyIconCancelDisabled,

	eMyIconSendArrowNormal,
	eMyIconSendArrowDisabled,
	eMyIconSendArrowCancel,

	eMyIconCameraDisabled,
	eMyIconCameraNormal,
	eMyIconCameraCancel,

	eMyIconCamcorderNormal,
	eMyIconCamcorderDisabled,
	eMyIconCamcorderCancel,

	eMyIconSelectCameraDisabled,
	eMyIconSelectCameraNormal,

	eMyIconSpeakerOn,
	eMyIconSpeakerOff,

	eMyIconCameraFlashDisabled,
	eMyIconCameraFlashNormal,
	eMyIconCameraFlashCancelNormal,

	eMyIconSnapshotDisabled,
	eMyIconSnapshotNormal,
	eMyIconSnapshotCancelNormal,

	eMyIconNotepadDisabled,
	eMyIconNotepadNormal,

	eMyIconGalleryDisabled,
	eMyIconGalleryNormal,
	eMyIconGalleryCancel,

	eMyIconFaceSelectDisabled,
	eMyIconFaceSelectNormal,

	eMyIconFaceCancelNormal,

	eMyIconCamPreviewNormal,
	eMyIconCamPreviewDisabled,
	eMyIconCamPreviewCancelNormal,
	eMyIconCamPreviewCancelDisabled,

	eMyIconCamRotateNormal,
	eMyIconCamRotateDisabled,
	eMyIconCamSetupNormal,
	eMyIconCamSetupDisabled,
	eMyIconCamSetupCancelNormal,
	eMyIconCamSetupCancelDisabled,
	eMyIconCamSelectNormal,
	eMyIconCamSelectDisabled,

	eMyIconImageRotateNormal,
	eMyIconImageRotateDisabled,

	eMyIconFileBrowseNormal,
	eMyIconFileBrowseDisabled,
	eMyIconFileBrowseCancel,

	eMyIconMoveUpDirNormal,
	eMyIconMoveUpDirDisabled,
	eMyIconMoveUpDirCancel,

	eMyIconLibraryDisabled,
	eMyIconLibraryNormal,
	eMyIconLibraryCancel,

	eMyIconEchoCancelNormal,
	eMyIconEchoCancelCancel,
	eMyIconSendFailed,
	eMyIconPersonUnknownNormal,

	eMyIconMenuNormal,
	eMyIconMenuDisabled,

	eMyIconAdministratorDisabled,

	eMyIconNetworkStateDiscover,
	eMyIconNetworkStateDisconnected,
	eMyIconNetworkStateDirectConnected,
	eMyIconNetworkStateAnchorFail,
	eMyIconNetworkStateRelayConnected,


	//=== title bar icons ===//
	eMyIconPowerOff,
	eMyIconHome,
	eMyIconShare,
	eMyIconMenu,
	eMyIconBack,

	//=== bottom bar icons ===//
	eMyIconArrowLeft,
	eMyIcon30SecBackward,
	eMyIcon30SecForward,
	eMyIconArrowRight,
	eMyIconMediaRepeat,
	eMyIconWindowExpand,
	eMyIconWindowShrink,

	//=== applet icons ===//
    eMyIconKodi,
    eMyIconUserIdentity,
    eMyIconSharedContent,

	eMyIconMessenger,
	eMyIconGallery,

	eMyIconVideoStreamViewer,
	eMyIconCamStreamViewer,
	eMyIconMusicStreamPlayer,

	eMyIconRemoteControl,

	eMyIconPlugins,
	eMyIconTheme,
	eMyIconSettings,

    eMyIconNetworkKey,
    eMyIconNetworkSettings,
    eMyIconSearchRandomPerson,

	// built in plugin options
	eMyIconCamStreamProvider,
	eMyIconGoTvStation,
	eMyIconGoTvNetworkHost,

	eMyIconGroupUser,
    eMyIconNetHosting,
	eMyIconGroupHost,
	eMyIconGroupListingHost,

    eMyIconShareServices,

	eMaxMyIcon	// must be last
};
