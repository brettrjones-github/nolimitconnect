#pragma once
enum EMyIcons
{
	eMyIconNone			= 0,		// no icon to show
	eMyIconUnknown,					// Icon with question mark
	eMyIconApp,
    eMyIconAvatarImage,
    eMyIconNotifyJoinOverlay,		// joined overlay
    eMyIconNotifyOnlineOverlay,		// online dot
    eMyIconNotifyOfferOverlay,		// hand offer overlay
    eMyIconNotifyForbbidenOverlay,	// permission denied
    eMyIconNotifyErrorOverlay,		// error
	eMyIconPlusOverlay,				// add to shared or library icon overlay
	eMyIconMinusOverlay,			// remove from shared or library
	eMyIconFriendOverlay,			// is friend
	eMyIconGlobeOverlay,			// is global shared or unknown person

    eMyIconInformation,
    eMyIconPeople,
	eMyIconServers,
    eMyIconSettingsGear,
	eMyIconFileServer,

    eMyIconEyeAll,
    eMyIconEyeChatRoom,
    eMyIconEyeFriends,
    eMyIconEyeGroup,
    eMyIconEyeHide,
    eMyIconEyeRandomConnect,
    eMyIconEyeShow,

    eMyIconSearch,
    eMyIconSearchJoinGroup,
    eMyIconSearchJoinChatRoom,
    eMyIconSearchJoinRandomConnect,
    eMyIconSearchPerson,
    eMyIconSearchHeart,
    eMyIconSearchWebCam,
    eMyIconSearchImage,
    eMyIconSearchStoryboard,
    eMyIconSearchFolder,

    eMyIconShareWithPerson,

	eMyIconAdministrator,
	eMyIconFriend,
	eMyIconGuest,
	eMyIconAnonymous,
	eMyIconIgnored,

    eMyIconClientChatRoom,
    eMyIconClientGroup,
    eMyIconClientRandomConnect,
    eMyIconClientWebCam,

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

    eMyIconCancel,
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

	eMyIconRecordMovieDisabled,
	eMyIconRecordMovieNormal,

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
	eMyIconNetworkStateHostFail,
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

    eMyIconNetworkKey,
    eMyIconNetworkSettings,
    eMyIconSearchRandomConnect,


    // built in plugin options
    eMyIconCamStreamProvider,
    eMyIconGoTvStation,

    eMyIconShareServices,

    eMyIconServiceAvatarImage,
    eMyIconServiceChatRoom,
    eMyIconServiceConnectionTest,
    eMyIconServiceHostGroup,
    eMyIconServiceHostGroupListing,
    eMyIconServiceHostNetwork,
    eMyIconServiceShareAboutMe,
    eMyIconServiceShareFiles,
    eMyIconServiceShareStoryboard,
    eMyIconServiceShareWebCam,
    eMyIconServiceRandomConnect,
    eMyIconServiceRandomConnectRelay,
    eMyIconServiceRelay,

    eMyIconSettingsAboutMe,
    eMyIconSettingsAvatarImage,
    eMyIconSettingsChatRoom,
    eMyIconSettingsConnectionTest,
    eMyIconSettingsHostGroup,
    eMyIconSettingsHostGroupListing,
    eMyIconSettingsHostNetwork,
    eMyIconSettingsShareFiles,
    eMyIconSettingsShareStoryboard,
    eMyIconSettingsShareWebCam,
    eMyIconSettingsRandomConnect,
    eMyIconSettingsRandomConnectRelay,
    eMyIconSettingsRelay,
    eMyIconFileXfer,
    eMyIconSettingsFileXfer,
    eMyIconSettingsMessenger,
    eMyIconTruthOrDare,
    eMyIconSettingsTruthOrDare,
    eMyIconSettingsVoicePhone,
    eMyIconSettingsVideoPhone,

    eMyIconEditRedo,
    eMyIconEditUndo,
    eMyIconEditAlignCenter,
    eMyIconEditAlignLeft,
    eMyIconEditAlignLeftAndRight,
    eMyIconEditAlignRight,
    eMyIconEditBold,
    eMyIconEditCopy,
    eMyIconEditCut,
    eMyIconEditFileNew,
    eMyIconEditHeader,
    eMyIconEditItalic,
    eMyIconEditPaperClip,
    eMyIconEditPaste,
    eMyIconEditPrint,
    eMyIconEditText,
    eMyIconEditUnderline,

    eMyIconEditHyperLink,

    eMyIconFileOpen,
    eMyIconFileSave,
    eMyIconFileSaveAs,
    eMyIconZoomIn,
    eMyIconZoomOut,

    eMyIconOfferFile,
    eMyIconOfferFriendship,
    eMyIconOfferList,
    eMyIconOfferTruthOrDare,
    eMyIconOfferVideoChat,
    eMyIconOfferVoicePhone,

    eMyIconCallList,

	eMaxMyIcon	// must be last
};
