cmake_minimum_required(VERSION 3.12.2)

project(_NoLimitConnectApp)

foreach (EnvVar IN ITEMS ConfigurationName QTDIR)
  if ("$ENV{${EnvVar}}" STREQUAL "")
    message(WARNING, "Environmental variable '${EnvVar}' is not defined.")
  endif ()
endforeach ()
set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_executable(_NoLimitConnectApp
  WIN32
  ../../../../app_precompiled_hdr.cpp
  ../../../../app_precompiled_hdr.h
  ../../../../GoTvAppConfig.h
  ../../../../GoTvCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../GoTvCpuArchDefines.h
  ../../../../GoTvDebugConfig.h
  ../../../../GoTvDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../GoTvTargetOsConfig.h
  ../../../../GuiInterface/config_libgotvptop.h
  ../../../../GuiInterface/GoTvRenderFrame.cpp
  ../../../../GuiInterface/GoTvRenderFrame.h
  ../../../../GuiInterface/IAudioInterface.h
  ../../../../GuiInterface/IDefs.cpp
  ../../../../GuiInterface/IDefs.h
  ../../../../GuiInterface/IFromGui.h
  ../../../../GuiInterface/IGoTv.cpp
  ../../../../GuiInterface/IGoTv.h
  ../../../../GuiInterface/IGoTvEvents.h
  ../../../../GuiInterface/IGoTvFromGui.cpp
  ../../../../GuiInterface/IGoTvGeometry.h
  ../../../../GuiInterface/IGoTvRender.cpp
  ../../../../GuiInterface/IGoTvRender.h
  ../../../../GuiInterface/IGoTvToGui.cpp
  ../../../../GuiInterface/ILog.cpp
  ../../../../GuiInterface/ILog.h
  ../../../../GuiInterface/IScan.h
  ../../../../GuiInterface/IToGui.cpp
  ../../../../GuiInterface/IToGui.h
  ../../../../GuiInterface/OsInterface/AssetDirectoryList.hh
  ../../../../GuiInterface/OsInterface/OsInterface.cpp
  ../../../../GuiInterface/OsInterface/OsInterface.h
  ../../../../GuiInterface/OsWin32/IWin32.cpp
  ../../../../GuiInterface/OsWin32/IWin32.h
  ../../../config_gotvapps.h
  ../../../CommonSrc/QtSource/AcceptCancelWidget.cpp
  ../../../CommonSrc/QtSource/AcceptCancelWidget.h
  ../../../CommonSrc/QtSource/AccountDb.cpp
  ../../../CommonSrc/QtSource/AccountDb.h
  ../../../CommonSrc/QtSource/AccountMgr.cpp
  ../../../CommonSrc/QtSource/AccountMgr.h
  ../../../CommonSrc/QtSource/ActivityAddRemoveLibraryFiles.cpp
  ../../../CommonSrc/QtSource/ActivityAddRemoveLibraryFiles.h
  ../../../CommonSrc/QtSource/ActivityAppSetup.cpp
  ../../../CommonSrc/QtSource/ActivityAppSetup.h
  ../../../CommonSrc/QtSource/ActivityBase.cpp
  ../../../CommonSrc/QtSource/ActivityBase.h
  ../../../CommonSrc/QtSource/ActivityBrowseFiles.cpp
  ../../../CommonSrc/QtSource/ActivityBrowseFiles.h
  ../../../CommonSrc/QtSource/ActivityChooseTestWebsiteUrl.cpp
  ../../../CommonSrc/QtSource/ActivityChooseTestWebsiteUrl.h
  ../../../CommonSrc/QtSource/ActivityCreateAccount.cpp
  ../../../CommonSrc/QtSource/ActivityCreateAccount.h
  ../../../CommonSrc/QtSource/ActivityDownloadItemMenu.cpp
  ../../../CommonSrc/QtSource/ActivityDownloadItemMenu.h
  ../../../CommonSrc/QtSource/ActivityFileSearch.cpp
  ../../../CommonSrc/QtSource/ActivityFileSearch.h
  ../../../CommonSrc/QtSource/ActivityHelpSignal.cpp
  ../../../CommonSrc/QtSource/ActivityHelpSignal.h
  ../../../CommonSrc/QtSource/ActivityInformation.cpp
  ../../../CommonSrc/QtSource/ActivityInformation.h
  ../../../CommonSrc/QtSource/ActivityIsPortOpenTest.cpp
  ../../../CommonSrc/QtSource/ActivityIsPortOpenTest.h
  ../../../CommonSrc/QtSource/ActivityMessageBox.cpp
  ../../../CommonSrc/QtSource/ActivityMessageBox.h
  ../../../CommonSrc/QtSource/ActivityNetworkState.cpp
  ../../../CommonSrc/QtSource/ActivityNetworkState.h
  ../../../CommonSrc/QtSource/ActivityPermissions.cpp
  ../../../CommonSrc/QtSource/ActivityPermissions.h
  ../../../CommonSrc/QtSource/ActivityScanPeopleSearch.cpp
  ../../../CommonSrc/QtSource/ActivityScanPeopleSearch.h
  ../../../CommonSrc/QtSource/ActivityScanProfiles.cpp
  ../../../CommonSrc/QtSource/ActivityScanProfiles.h
  ../../../CommonSrc/QtSource/ActivityScanStoryBoards.cpp
  ../../../CommonSrc/QtSource/ActivityScanStoryBoards.h
  ../../../CommonSrc/QtSource/ActivityScanWebCams.cpp
  ../../../CommonSrc/QtSource/ActivityScanWebCams.h
  ../../../CommonSrc/QtSource/ActivitySessionOptions.cpp
  ../../../CommonSrc/QtSource/ActivitySessionOptions.h
  ../../../CommonSrc/QtSource/ActivityShowHelp.cpp
  ../../../CommonSrc/QtSource/ActivityShowHelp.h
  ../../../CommonSrc/QtSource/ActivitySnapShot.cpp
  ../../../CommonSrc/QtSource/ActivitySnapShot.h
  ../../../CommonSrc/QtSource/ActivitySoundOptions.cpp
  ../../../CommonSrc/QtSource/ActivitySoundOptions.h
  ../../../CommonSrc/QtSource/ActivityTimedMessage.cpp
  ../../../CommonSrc/QtSource/ActivityTimedMessage.h
  ../../../CommonSrc/QtSource/ActivityViewAudioFile.cpp
  ../../../CommonSrc/QtSource/ActivityViewAudioFile.h
  ../../../CommonSrc/QtSource/ActivityViewLibraryFiles.cpp
  ../../../CommonSrc/QtSource/ActivityViewLibraryFiles.h
  ../../../CommonSrc/QtSource/ActivityViewMySharedFiles.cpp
  ../../../CommonSrc/QtSource/ActivityViewMySharedFiles.h
  ../../../CommonSrc/QtSource/ActivityViewVideoFile.cpp
  ../../../CommonSrc/QtSource/ActivityViewVideoFile.h
  ../../../CommonSrc/QtSource/ActivityYesNoMsgBox.cpp
  ../../../CommonSrc/QtSource/ActivityYesNoMsgBox.h
  ../../../CommonSrc/QtSource/AppCommon.cpp
  ../../../CommonSrc/QtSource/AppCommon.h
  ../../../CommonSrc/QtSource/AppCommonAudio.cpp
  ../../../CommonSrc/QtSource/AppCommonEvents.cpp
  ../../../CommonSrc/QtSource/AppCommonLogin.cpp
  ../../../CommonSrc/QtSource/AppCommonRender.cpp
  ../../../CommonSrc/QtSource/AppDefs.cpp
  ../../../CommonSrc/QtSource/AppDefs.h
  ../../../CommonSrc/QtSource/AppGlobals.cpp
  ../../../CommonSrc/QtSource/AppGlobals.h
  ../../../CommonSrc/QtSource/AppletAboutApp.cpp
  ../../../CommonSrc/QtSource/AppletAboutApp.h
  ../../../CommonSrc/QtSource/AppletApplicationInfo.cpp
  ../../../CommonSrc/QtSource/AppletApplicationInfo.h
  ../../../CommonSrc/QtSource/AppletBase.cpp
  ../../../CommonSrc/QtSource/AppletBase.h
  ../../../CommonSrc/QtSource/AppletBrowseFiles.cpp
  ../../../CommonSrc/QtSource/AppletBrowseFiles.h
  ../../../CommonSrc/QtSource/AppletCallList.cpp
  ../../../CommonSrc/QtSource/AppletCallList.h
  ../../../CommonSrc/QtSource/AppletCamSettings.cpp
  ../../../CommonSrc/QtSource/AppletCamSettings.h
  ../../../CommonSrc/QtSource/AppletChatRoomClient.cpp
  ../../../CommonSrc/QtSource/AppletChatRoomClient.h
  ../../../CommonSrc/QtSource/AppletChatRoomJoin.cpp
  ../../../CommonSrc/QtSource/AppletChatRoomJoin.h
  ../../../CommonSrc/QtSource/AppletChatRoomJoinSearch.cpp
  ../../../CommonSrc/QtSource/AppletChatRoomJoinSearch.h
  ../../../CommonSrc/QtSource/AppletChooseThumbnail.cpp
  ../../../CommonSrc/QtSource/AppletChooseThumbnail.h
  ../../../CommonSrc/QtSource/AppletClientAvatarImage.cpp
  ../../../CommonSrc/QtSource/AppletClientAvatarImage.h
  ../../../CommonSrc/QtSource/AppletClientBase.cpp
  ../../../CommonSrc/QtSource/AppletClientBase.h
  ../../../CommonSrc/QtSource/AppletClientRandomConnect.cpp
  ../../../CommonSrc/QtSource/AppletClientRandomConnect.h
  ../../../CommonSrc/QtSource/AppletClientShareFiles.cpp
  ../../../CommonSrc/QtSource/AppletClientShareFiles.h
  ../../../CommonSrc/QtSource/AppletClientShareWebCam.cpp
  ../../../CommonSrc/QtSource/AppletClientShareWebCam.h
  ../../../CommonSrc/QtSource/AppletClientStoryboard.cpp
  ../../../CommonSrc/QtSource/AppletClientStoryboard.h
  ../../../CommonSrc/QtSource/AppletDownloads.cpp
  ../../../CommonSrc/QtSource/AppletDownloads.h
  ../../../CommonSrc/QtSource/AppletEditAboutMe.cpp
  ../../../CommonSrc/QtSource/AppletEditAboutMe.h
  ../../../CommonSrc/QtSource/AppletEditAvatarImage.cpp
  ../../../CommonSrc/QtSource/AppletEditAvatarImage.h
  ../../../CommonSrc/QtSource/AppletEditStoryboard.cpp
  ../../../CommonSrc/QtSource/AppletEditStoryboard.h
  ../../../CommonSrc/QtSource/AppletGalleryImage.cpp
  ../../../CommonSrc/QtSource/AppletGalleryImage.h
  ../../../CommonSrc/QtSource/AppletGalleryThumb.cpp
  ../../../CommonSrc/QtSource/AppletGalleryThumb.h
  ../../../CommonSrc/QtSource/AppletGroupJoin.cpp
  ../../../CommonSrc/QtSource/AppletGroupJoin.h
  ../../../CommonSrc/QtSource/AppletGroupJoinSearch.cpp
  ../../../CommonSrc/QtSource/AppletGroupJoinSearch.h
  ../../../CommonSrc/QtSource/AppletJoinBase.cpp
  ../../../CommonSrc/QtSource/AppletJoinBase.h
  ../../../CommonSrc/QtSource/AppletKodi.cpp
  ../../../CommonSrc/QtSource/AppletKodi.h
  ../../../CommonSrc/QtSource/AppletLaunchPage.cpp
  ../../../CommonSrc/QtSource/AppletLaunchPage.h
  ../../../CommonSrc/QtSource/AppletLaunchWidget.cpp
  ../../../CommonSrc/QtSource/AppletLaunchWidget.h
  ../../../CommonSrc/QtSource/AppletLibrary.cpp
  ../../../CommonSrc/QtSource/AppletLibrary.h
  ../../../CommonSrc/QtSource/AppletLog.cpp
  ../../../CommonSrc/QtSource/AppletLog.h
  ../../../CommonSrc/QtSource/AppletLogSettings.cpp
  ../../../CommonSrc/QtSource/AppletLogSettings.h
  ../../../CommonSrc/QtSource/AppletLogView.cpp
  ../../../CommonSrc/QtSource/AppletLogView.h
  ../../../CommonSrc/QtSource/AppletMgr.cpp
  ../../../CommonSrc/QtSource/AppletMgr.h
  ../../../CommonSrc/QtSource/AppletMultiMessenger.cpp
  ../../../CommonSrc/QtSource/AppletMultiMessenger.h
  ../../../CommonSrc/QtSource/AppletNetHostingPage.cpp
  ../../../CommonSrc/QtSource/AppletNetHostingPage.h
  ../../../CommonSrc/QtSource/AppletNetworkSettings.cpp
  ../../../CommonSrc/QtSource/AppletNetworkSettings.h
  ../../../CommonSrc/QtSource/AppletOfferList.cpp
  ../../../CommonSrc/QtSource/AppletOfferList.h
  ../../../CommonSrc/QtSource/AppletPeerBase.cpp
  ../../../CommonSrc/QtSource/AppletPeerBase.h
  ../../../CommonSrc/QtSource/AppletPeerChangeFriendship.cpp
  ../../../CommonSrc/QtSource/AppletPeerChangeFriendship.h
  ../../../CommonSrc/QtSource/AppletPeerReplyFileOffer.cpp
  ../../../CommonSrc/QtSource/AppletPeerReplyFileOffer.h
  ../../../CommonSrc/QtSource/AppletPeerSelectFileToSend.cpp
  ../../../CommonSrc/QtSource/AppletPeerSelectFileToSend.h
  ../../../CommonSrc/QtSource/AppletPeerSessionFileOffer.cpp
  ../../../CommonSrc/QtSource/AppletPeerSessionFileOffer.h
  ../../../CommonSrc/QtSource/AppletPeerTodGame.cpp
  ../../../CommonSrc/QtSource/AppletPeerTodGame.h
  ../../../CommonSrc/QtSource/AppletPeerVideoPhone.cpp
  ../../../CommonSrc/QtSource/AppletPeerVideoPhone.h
  ../../../CommonSrc/QtSource/AppletPeerViewSharedFiles.cpp
  ../../../CommonSrc/QtSource/AppletPeerViewSharedFiles.h
  ../../../CommonSrc/QtSource/AppletPeerVoicePhone.cpp
  ../../../CommonSrc/QtSource/AppletPeerVoicePhone.h
  ../../../CommonSrc/QtSource/AppletPermissionList.cpp
  ../../../CommonSrc/QtSource/AppletPermissionList.h
  ../../../CommonSrc/QtSource/AppletPersonalRecorder.cpp
  ../../../CommonSrc/QtSource/AppletPersonalRecorder.h
  ../../../CommonSrc/QtSource/AppletPlayerBase.cpp
  ../../../CommonSrc/QtSource/AppletPlayerBase.h
  ../../../CommonSrc/QtSource/AppletPlayerVideo.cpp
  ../../../CommonSrc/QtSource/AppletPlayerVideo.h
  ../../../CommonSrc/QtSource/AppletRandomConnectJoin.cpp
  ../../../CommonSrc/QtSource/AppletRandomConnectJoin.h
  ../../../CommonSrc/QtSource/AppletRandomConnectJoinSearch.cpp
  ../../../CommonSrc/QtSource/AppletRandomConnectJoinSearch.h
  ../../../CommonSrc/QtSource/AppletSearchPage.cpp
  ../../../CommonSrc/QtSource/AppletSearchPage.h
  ../../../CommonSrc/QtSource/AppletServiceAboutMe.cpp
  ../../../CommonSrc/QtSource/AppletServiceAboutMe.h
  ../../../CommonSrc/QtSource/AppletServiceAvatarImage.cpp
  ../../../CommonSrc/QtSource/AppletServiceAvatarImage.h
  ../../../CommonSrc/QtSource/AppletServiceBase.cpp
  ../../../CommonSrc/QtSource/AppletServiceBase.h
  ../../../CommonSrc/QtSource/AppletServiceBaseSettings.cpp
  ../../../CommonSrc/QtSource/AppletServiceBaseSettings.h
  ../../../CommonSrc/QtSource/AppletServiceChatRoom.cpp
  ../../../CommonSrc/QtSource/AppletServiceChatRoom.h
  ../../../CommonSrc/QtSource/AppletServiceConnectionTest.cpp
  ../../../CommonSrc/QtSource/AppletServiceConnectionTest.h
  ../../../CommonSrc/QtSource/AppletServiceHostGroup.cpp
  ../../../CommonSrc/QtSource/AppletServiceHostGroup.h
  ../../../CommonSrc/QtSource/AppletServiceHostNetwork.cpp
  ../../../CommonSrc/QtSource/AppletServiceHostNetwork.h
  ../../../CommonSrc/QtSource/AppletServiceRandomConnect.cpp
  ../../../CommonSrc/QtSource/AppletServiceRandomConnect.h
  ../../../CommonSrc/QtSource/AppletServiceRandomConnectRelay.cpp
  ../../../CommonSrc/QtSource/AppletServiceRandomConnectRelay.h
  ../../../CommonSrc/QtSource/AppletServiceRelay.cpp
  ../../../CommonSrc/QtSource/AppletServiceRelay.h
  ../../../CommonSrc/QtSource/AppletServiceShareFiles.cpp
  ../../../CommonSrc/QtSource/AppletServiceShareFiles.h
  ../../../CommonSrc/QtSource/AppletServiceShareWebCam.cpp
  ../../../CommonSrc/QtSource/AppletServiceShareWebCam.h
  ../../../CommonSrc/QtSource/AppletServiceStoryboard.cpp
  ../../../CommonSrc/QtSource/AppletServiceStoryboard.h
  ../../../CommonSrc/QtSource/AppletSettingsAboutMe.cpp
  ../../../CommonSrc/QtSource/AppletSettingsAboutMe.h
  ../../../CommonSrc/QtSource/AppletSettingsAvatarImage.cpp
  ../../../CommonSrc/QtSource/AppletSettingsAvatarImage.h
  ../../../CommonSrc/QtSource/AppletSettingsBase.cpp
  ../../../CommonSrc/QtSource/AppletSettingsBase.h
  ../../../CommonSrc/QtSource/AppletSettingsChatRoom.cpp
  ../../../CommonSrc/QtSource/AppletSettingsChatRoom.h
  ../../../CommonSrc/QtSource/AppletSettingsConnectionTest.cpp
  ../../../CommonSrc/QtSource/AppletSettingsConnectionTest.h
  ../../../CommonSrc/QtSource/AppletSettingsFileXfer.cpp
  ../../../CommonSrc/QtSource/AppletSettingsFileXfer.h
  ../../../CommonSrc/QtSource/AppletSettingsHostBase.cpp
  ../../../CommonSrc/QtSource/AppletSettingsHostBase.h
  ../../../CommonSrc/QtSource/AppletSettingsHostChatRoom.cpp
  ../../../CommonSrc/QtSource/AppletSettingsHostChatRoom.h
  ../../../CommonSrc/QtSource/AppletSettingsHostGroup.cpp
  ../../../CommonSrc/QtSource/AppletSettingsHostGroup.h
  ../../../CommonSrc/QtSource/AppletSettingsHostNetwork.cpp
  ../../../CommonSrc/QtSource/AppletSettingsHostNetwork.h
  ../../../CommonSrc/QtSource/AppletSettingsHostRandomConnect.cpp
  ../../../CommonSrc/QtSource/AppletSettingsHostRandomConnect.h
  ../../../CommonSrc/QtSource/AppletSettingsMessenger.cpp
  ../../../CommonSrc/QtSource/AppletSettingsMessenger.h
  ../../../CommonSrc/QtSource/AppletSettingsPage.cpp
  ../../../CommonSrc/QtSource/AppletSettingsPage.h
  ../../../CommonSrc/QtSource/AppletSettingsRandomConnect.cpp
  ../../../CommonSrc/QtSource/AppletSettingsRandomConnect.h
  ../../../CommonSrc/QtSource/AppletSettingsRandomConnectRelay.cpp
  ../../../CommonSrc/QtSource/AppletSettingsRandomConnectRelay.h
  ../../../CommonSrc/QtSource/AppletSettingsShareFiles.cpp
  ../../../CommonSrc/QtSource/AppletSettingsShareFiles.h
  ../../../CommonSrc/QtSource/AppletSettingsShareWebCam.cpp
  ../../../CommonSrc/QtSource/AppletSettingsShareWebCam.h
  ../../../CommonSrc/QtSource/AppletSettingsStoryboard.cpp
  ../../../CommonSrc/QtSource/AppletSettingsStoryboard.h
  ../../../CommonSrc/QtSource/AppletSettingsTruthOrDare.cpp
  ../../../CommonSrc/QtSource/AppletSettingsTruthOrDare.h
  ../../../CommonSrc/QtSource/AppletSettingsVideoPhone.cpp
  ../../../CommonSrc/QtSource/AppletSettingsVideoPhone.h
  ../../../CommonSrc/QtSource/AppletSettingsVoicePhone.cpp
  ../../../CommonSrc/QtSource/AppletSettingsVoicePhone.h
  ../../../CommonSrc/QtSource/AppletShareServicesPage.cpp
  ../../../CommonSrc/QtSource/AppletShareServicesPage.h
  ../../../CommonSrc/QtSource/AppletSnapshot.cpp
  ../../../CommonSrc/QtSource/AppletSnapshot.h
  ../../../CommonSrc/QtSource/AppletTestAndDebug.cpp
  ../../../CommonSrc/QtSource/AppletTestAndDebug.h
  ../../../CommonSrc/QtSource/AppletTheme.cpp
  ../../../CommonSrc/QtSource/AppletTheme.h
  ../../../CommonSrc/QtSource/AppletUploads.cpp
  ../../../CommonSrc/QtSource/AppletUploads.h
  ../../../CommonSrc/QtSource/AppletUserIdentity.cpp
  ../../../CommonSrc/QtSource/AppletUserIdentity.h
  ../../../CommonSrc/QtSource/AppProfile.cpp
  ../../../CommonSrc/QtSource/AppProfile.h
  ../../../CommonSrc/QtSource/AppSettings.cpp
  ../../../CommonSrc/QtSource/AppSettings.h
  ../../../CommonSrc/QtSource/AppSetup.cpp
  ../../../CommonSrc/QtSource/AppSetup.h
  ../../../CommonSrc/QtSource/AppTranslate.cpp
  ../../../CommonSrc/QtSource/AppTranslate.h
  ../../../CommonSrc/QtSource/AssetBaseWidget.cpp
  ../../../CommonSrc/QtSource/AssetBaseWidget.h
  ../../../CommonSrc/QtSource/AssetFaceWidget.cpp
  ../../../CommonSrc/QtSource/AssetFaceWidget.h
  ../../../CommonSrc/QtSource/AssetPhotoWidget.cpp
  ../../../CommonSrc/QtSource/AssetPhotoWidget.h
  ../../../CommonSrc/QtSource/AssetTextWidget.cpp
  ../../../CommonSrc/QtSource/AssetTextWidget.h
  ../../../CommonSrc/QtSource/AssetVideoWidget.cpp
  ../../../CommonSrc/QtSource/AssetVideoWidget.h
  ../../../CommonSrc/QtSource/AssetVoiceWidget.cpp
  ../../../CommonSrc/QtSource/AssetVoiceWidget.h
  ../../../CommonSrc/QtSource/AvatarBar.cpp
  ../../../CommonSrc/QtSource/AvatarBar.h
  ../../../CommonSrc/QtSource/BottomBarWidget.cpp
  ../../../CommonSrc/QtSource/BottomBarWidget.h
  ../../../CommonSrc/QtSource/CallListItem.cpp
  ../../../CommonSrc/QtSource/CallListItem.h
  ../../../CommonSrc/QtSource/CallListWidget.cpp
  ../../../CommonSrc/QtSource/CallListWidget.h
  ../../../CommonSrc/QtSource/CamLogic.cpp
  ../../../CommonSrc/QtSource/CamLogic.h
  ../../../CommonSrc/QtSource/CamTitleBarPreview.cpp
  ../../../CommonSrc/QtSource/CamTitleBarPreview.h
  ../../../CommonSrc/QtSource/ChatEntryWidget.cpp
  ../../../CommonSrc/QtSource/ChatEntryWidget.h
  ../../../CommonSrc/QtSource/ChatRoomWidget.cpp
  ../../../CommonSrc/QtSource/ChatRoomWidget.h
  ../../../CommonSrc/QtSource/DialogConfirmRemoveMessage.cpp
  ../../../CommonSrc/QtSource/DialogConfirmRemoveMessage.h
  ../../../CommonSrc/QtSource/EventsQtToGoTv.cpp
  ../../../CommonSrc/QtSource/EventsQtToGoTv.h
  ../../../CommonSrc/QtSource/FileActionBarWidget.cpp
  ../../../CommonSrc/QtSource/FileActionBarWidget.h
  ../../../CommonSrc/QtSource/FileActionMenu.cpp
  ../../../CommonSrc/QtSource/FileActionMenu.h
  ../../../CommonSrc/QtSource/FileItemInfo.cpp
  ../../../CommonSrc/QtSource/FileItemInfo.h
  ../../../CommonSrc/QtSource/FileItemWidget.cpp
  ../../../CommonSrc/QtSource/FileItemWidget.h
  ../../../CommonSrc/QtSource/FileListReplySession.cpp
  ../../../CommonSrc/QtSource/FileListReplySession.h
  ../../../CommonSrc/QtSource/FileShareItemWidget.cpp
  ../../../CommonSrc/QtSource/FileShareItemWidget.h
  ../../../CommonSrc/QtSource/FileXferWidget.cpp
  ../../../CommonSrc/QtSource/FileXferWidget.h
  ../../../CommonSrc/QtSource/FriendList.cpp
  ../../../CommonSrc/QtSource/FriendList.h
  ../../../CommonSrc/QtSource/FriendListDelegate.cpp
  ../../../CommonSrc/QtSource/FriendListDelegate.h
  ../../../CommonSrc/QtSource/FriendListEntryWidget.cpp
  ../../../CommonSrc/QtSource/FriendListEntryWidget.h
  ../../../CommonSrc/QtSource/FriendListWidget.cpp
  ../../../CommonSrc/QtSource/FriendListWidget.h
  ../../../CommonSrc/QtSource/NoLimitConnect.ico
  ../../../CommonSrc/QtSource/NoLimitConnect.rc
  ../../../CommonSrc/QtSource/GuiConnectMgr.cpp
  ../../../CommonSrc/QtSource/GuiConnectMgr.h
  ../../../CommonSrc/QtSource/GuiFileActionSession.cpp
  ../../../CommonSrc/QtSource/GuiFileActionSession.h
  ../../../CommonSrc/QtSource/GuiFileXferSession.cpp
  ../../../CommonSrc/QtSource/GuiFileXferSession.h
  ../../../CommonSrc/QtSource/GuiHelpers.cpp
  ../../../CommonSrc/QtSource/GuiHelpers.h
  ../../../CommonSrc/QtSource/GuiHostSession.cpp
  ../../../CommonSrc/QtSource/GuiHostSession.h
  ../../../CommonSrc/QtSource/GuiOfferMgr.cpp
  ../../../CommonSrc/QtSource/GuiOfferMgr.h
  ../../../CommonSrc/QtSource/GuiOfferSession.cpp
  ../../../CommonSrc/QtSource/GuiOfferSession.h
  ../../../CommonSrc/QtSource/GuiParams.cpp
  ../../../CommonSrc/QtSource/GuiThumb.cpp
  ../../../CommonSrc/QtSource/GuiThumb.h
  ../../../CommonSrc/QtSource/GuiThumbList.cpp
  ../../../CommonSrc/QtSource/GuiThumbList.h
  ../../../CommonSrc/QtSource/GuiThumbMgr.cpp
  ../../../CommonSrc/QtSource/GuiThumbMgr.h
  ../../../CommonSrc/QtSource/GuiUser.cpp
  ../../../CommonSrc/QtSource/GuiUser.h
  ../../../CommonSrc/QtSource/GuiUserJoin.cpp
  ../../../CommonSrc/QtSource/GuiUserJoin.h
  ../../../CommonSrc/QtSource/GuiUserJoinList.cpp
  ../../../CommonSrc/QtSource/GuiUserJoinList.h
  ../../../CommonSrc/QtSource/GuiUserJoinMgr.cpp
  ../../../CommonSrc/QtSource/GuiUserJoinMgr.h
  ../../../CommonSrc/QtSource/GuiUserJoinSession.cpp
  ../../../CommonSrc/QtSource/GuiUserJoinSession.h
  ../../../CommonSrc/QtSource/GuiUserMgr.cpp
  ../../../CommonSrc/QtSource/GuiUserMgr.h
  ../../../CommonSrc/QtSource/GuiUserSessionBase.cpp
  ../../../CommonSrc/QtSource/GuiUserSessionBase.h
  ../../../CommonSrc/QtSource/HelpBrowser.cpp
  ../../../CommonSrc/QtSource/HelpBrowser.h
  ../../../CommonSrc/QtSource/HistoryEntryWidget.cpp
  ../../../CommonSrc/QtSource/HistoryEntryWidget.h
  ../../../CommonSrc/QtSource/HistoryListWidget.cpp
  ../../../CommonSrc/QtSource/HistoryListWidget.h
  ../../../CommonSrc/QtSource/HomeWindow.cpp
  ../../../CommonSrc/QtSource/HomeWindow.h
  ../../../CommonSrc/QtSource/HostListItem.cpp
  ../../../CommonSrc/QtSource/HostListItem.h
  ../../../CommonSrc/QtSource/HostListWidget.cpp
  ../../../CommonSrc/QtSource/HostListWidget.h
  ../../../CommonSrc/QtSource/IdentWidget.cpp
  ../../../CommonSrc/QtSource/IdentWidget.h
  ../../../CommonSrc/QtSource/ImageListEntry.cpp
  ../../../CommonSrc/QtSource/ImageListEntry.h
  ../../../CommonSrc/QtSource/ImageListRow.cpp
  ../../../CommonSrc/QtSource/ImageListRow.h
  ../../../CommonSrc/QtSource/ImageListWidget.cpp
  ../../../CommonSrc/QtSource/ImageListWidget.h
  ../../../CommonSrc/QtSource/ImagePickBar.cpp
  ../../../CommonSrc/QtSource/ImagePickBar.h
  ../../../CommonSrc/QtSource/InformationWidget.cpp
  ../../../CommonSrc/QtSource/InformationWidget.h
  ../../../CommonSrc/QtSource/InputAllWidget.cpp
  ../../../CommonSrc/QtSource/InputAllWidget.h
  ../../../CommonSrc/QtSource/InputBaseWidget.cpp
  ../../../CommonSrc/QtSource/InputBaseWidget.h
  ../../../CommonSrc/QtSource/InputFaceWidget.cpp
  ../../../CommonSrc/QtSource/InputFaceWidget.h
  ../../../CommonSrc/QtSource/InputPhotoWidget.cpp
  ../../../CommonSrc/QtSource/InputPhotoWidget.h
  ../../../CommonSrc/QtSource/InputTextWidget.cpp
  ../../../CommonSrc/QtSource/InputTextWidget.h
  ../../../CommonSrc/QtSource/InputVideoWidget.cpp
  ../../../CommonSrc/QtSource/InputVideoWidget.h
  ../../../CommonSrc/QtSource/InputVoiceWidget.cpp
  ../../../CommonSrc/QtSource/InputVoiceWidget.h
  ../../../CommonSrc/QtSource/InstMsgWidget.cpp
  ../../../CommonSrc/QtSource/InstMsgWidget.h
  ../../../CommonSrc/QtSource/LogMgr.cpp
  ../../../CommonSrc/QtSource/LogMgr.h
  ../../../CommonSrc/QtSource/LogWidget.cpp
  ../../../CommonSrc/QtSource/LogWidget.h
  ../../../CommonSrc/QtSource/MenuBase.cpp
  ../../../CommonSrc/QtSource/MenuBase.h
  ../../../CommonSrc/QtSource/MenuCam.cpp
  ../../../CommonSrc/QtSource/MenuCam.h
  ../../../CommonSrc/QtSource/MenuDefs.h
  ../../../CommonSrc/QtSource/MessengerPage.cpp
  ../../../CommonSrc/QtSource/MessengerPage.h
  ../../../CommonSrc/QtSource/MultiSessionState.cpp
  ../../../CommonSrc/QtSource/MultiSessionState.h
  ../../../CommonSrc/QtSource/MyIcons.cpp
  ../../../CommonSrc/QtSource/MyIcons.h
  ../../../CommonSrc/QtSource/MyIconsDefs.h
  ../../../CommonSrc/QtSource/MySndMgr.cpp
  ../../../CommonSrc/QtSource/MySndMgr.h
  ../../../CommonSrc/QtSource/NetAvailStatusWidget.cpp
  ../../../CommonSrc/QtSource/NetAvailStatusWidget.h
  ../../../CommonSrc/QtSource/NetClientGuiLogic.cpp
  ../../../CommonSrc/QtSource/NetClientGuiLogic.h
  ../../../CommonSrc/QtSource/NetHostGuiLogic.cpp
  ../../../CommonSrc/QtSource/NetHostGuiLogic.h
  ../../../CommonSrc/QtSource/NetworkKeyWidget.cpp
  ../../../CommonSrc/QtSource/NetworkKeyWidget.h
  ../../../CommonSrc/QtSource/NetworkTestWidget.cpp
  ../../../CommonSrc/QtSource/NetworkTestWidget.h
  ../../../CommonSrc/QtSource/OfferBarWidget.cpp
  ../../../CommonSrc/QtSource/OfferBarWidget.h
  ../../../CommonSrc/QtSource/OfferListItem.cpp
  ../../../CommonSrc/QtSource/OfferListItem.h
  ../../../CommonSrc/QtSource/OfferListWidget.cpp
  ../../../CommonSrc/QtSource/OfferListWidget.h
  ../../../CommonSrc/QtSource/OfferMgr.cpp
  ../../../CommonSrc/QtSource/OfferMgr.h
  ../../../CommonSrc/QtSource/OfferSessionCallbackInterface.h
  ../../../CommonSrc/QtSource/OfferSessionLogic.cpp
  ../../../CommonSrc/QtSource/OfferSessionLogic.h
  ../../../CommonSrc/QtSource/OfferSessionState.cpp
  ../../../CommonSrc/QtSource/OfferSessionState.h
  ../../../CommonSrc/QtSource/OfferWidget.cpp
  ../../../CommonSrc/QtSource/OfferWidget.h
  ../../../CommonSrc/QtSource/P2PThingFileMenu.cpp
  ../../../CommonSrc/QtSource/P2PThingFileShare.cpp
  ../../../CommonSrc/QtSource/P2PThingFriend.cpp
  ../../../CommonSrc/QtSource/P2PThingMedia.cpp
  ../../../CommonSrc/QtSource/P2PThingNotify.cpp
  ../../../CommonSrc/QtSource/P2PThingOfferToFriend.cpp
  ../../../CommonSrc/QtSource/P2PThingOptions.cpp
  ../../../CommonSrc/QtSource/P2PThingScan.cpp
  ../../../CommonSrc/QtSource/P2PThingSearch.cpp
  ../../../CommonSrc/QtSource/P2PThingServers.cpp
  ../../../CommonSrc/QtSource/PermissionListItem.cpp
  ../../../CommonSrc/QtSource/PermissionListItem.h
  ../../../CommonSrc/QtSource/PermissionWidget.cpp
  ../../../CommonSrc/QtSource/PermissionWidget.h
  ../../../CommonSrc/QtSource/PlayerGlWidget.cpp
  ../../../CommonSrc/QtSource/PlayerGlWidget.h
  ../../../CommonSrc/QtSource/PluginSettingsWidget.cpp
  ../../../CommonSrc/QtSource/PluginSettingsWidget.h
  ../../../CommonSrc/QtSource/PopupMenu.cpp
  ../../../CommonSrc/QtSource/PopupMenu.h
  ../../../CommonSrc/QtSource/PopupMenuDelegate.cpp
  ../../../CommonSrc/QtSource/PopupMenuDelegate.h
  ../../../CommonSrc/QtSource/RasterWindow.cpp
  ../../../CommonSrc/QtSource/RasterWindow.h
  ../../../CommonSrc/QtSource/RelayServiceWidget.cpp
  ../../../CommonSrc/QtSource/RelayServiceWidget.h
  ../../../CommonSrc/QtSource/RenderGlLogic.cpp
  ../../../CommonSrc/QtSource/RenderGlLogic.h
  ../../../CommonSrc/QtSource/RenderGlMatrix.cpp
  ../../../CommonSrc/QtSource/RenderGlOffScreenSurface.cpp
  ../../../CommonSrc/QtSource/RenderGlOffScreenSurface.h
  ../../../CommonSrc/QtSource/RenderGlShaders.cpp
  ../../../CommonSrc/QtSource/RenderGlShaders.h
  ../../../CommonSrc/QtSource/RenderGlWidget.cpp
  ../../../CommonSrc/QtSource/RenderGlWidget.h
  ../../../CommonSrc/QtSource/RenderGlWidgetFonts.cpp
  ../../../CommonSrc/QtSource/RenderGlWidgetRender.cpp
  ../../../CommonSrc/QtSource/RenderGlWidgetShaderInterface.cpp
  ../../../CommonSrc/QtSource/RenderGlWidgetTextures.cpp
  ../../../CommonSrc/QtSource/RenderKodiThread.cpp
  ../../../CommonSrc/QtSource/RenderKodiThread.h
  ../../../CommonSrc/QtSource/RenderLogoShaders.cpp
  ../../../CommonSrc/QtSource/RenderLogoShaders.h
  ../../../CommonSrc/QtSource/RenderShaderDefsGl.cpp
  ../../../CommonSrc/QtSource/RenderShaderDefsGles.cpp
  ../../../CommonSrc/QtSource/RenderShaderQt.cpp
  ../../../CommonSrc/QtSource/RenderShaderQt.h
  ../../../CommonSrc/QtSource/SearchParamsWidget.cpp
  ../../../CommonSrc/QtSource/SearchParamsWidget.h
  ../../../CommonSrc/QtSource/ServiceSettingsWidget.cpp
  ../../../CommonSrc/QtSource/ServiceSettingsWidget.h
  ../../../CommonSrc/QtSource/SessionWidget.cpp
  ../../../CommonSrc/QtSource/SessionWidget.h
  ../../../CommonSrc/QtSource/SoundCtrlWidget.cpp
  ../../../CommonSrc/QtSource/SoundCtrlWidget.h
  ../../../CommonSrc/QtSource/SoundDefs.h
  ../../../CommonSrc/QtSource/soundio/AudioDefs.h
  ../../../CommonSrc/QtSource/soundio/AudioInIo.cpp
  ../../../CommonSrc/QtSource/soundio/AudioInIo.h
  ../../../CommonSrc/QtSource/soundio/AudioInThread.cpp
  ../../../CommonSrc/QtSource/soundio/AudioInThread.h
  ../../../CommonSrc/QtSource/soundio/AudioIoMgr.cpp
  ../../../CommonSrc/QtSource/soundio/AudioIoMgr.h
  ../../../CommonSrc/QtSource/soundio/AudioMixer.cpp
  ../../../CommonSrc/QtSource/soundio/AudioMixer.h
  ../../../CommonSrc/QtSource/soundio/AudioOutIo.cpp
  ../../../CommonSrc/QtSource/soundio/AudioOutIo.h
  ../../../CommonSrc/QtSource/soundio/AudioOutThread.cpp
  ../../../CommonSrc/QtSource/soundio/AudioOutThread.h
  ../../../CommonSrc/QtSource/soundio/AudioTestGenerator.cpp
  ../../../CommonSrc/QtSource/soundio/AudioTestGenerator.h
  ../../../CommonSrc/QtSource/soundio/AudioUtils.cpp
  ../../../CommonSrc/QtSource/soundio/AudioUtils.h
  ../../../CommonSrc/QtSource/soundio/IAudioInterface.h
  ../../../CommonSrc/QtSource/StoryWidget.cpp
  ../../../CommonSrc/QtSource/StoryWidget.h
  ../../../CommonSrc/QtSource/TextChatLogic.cpp
  ../../../CommonSrc/QtSource/TextChatLogic.h
  ../../../CommonSrc/QtSource/ThumbnailChooseWidget.cpp
  ../../../CommonSrc/QtSource/ThumbnailChooseWidget.h
  ../../../CommonSrc/QtSource/ThumbnailEditWidget.cpp
  ../../../CommonSrc/QtSource/ThumbnailEditWidget.h
  ../../../CommonSrc/QtSource/ThumbnailViewWidget.cpp
  ../../../CommonSrc/QtSource/ThumbnailViewWidget.h
  ../../../CommonSrc/QtSource/TitleBarWidget.cpp
  ../../../CommonSrc/QtSource/TitleBarWidget.h
  ../../../CommonSrc/QtSource/TodGameDefs.cpp
  ../../../CommonSrc/QtSource/TodGameDefs.h
  ../../../CommonSrc/QtSource/TodGameLogic.cpp
  ../../../CommonSrc/QtSource/TodGameLogic.h
  ../../../CommonSrc/QtSource/TodGameWidget.cpp
  ../../../CommonSrc/QtSource/TodGameWidget.h
  ../../../CommonSrc/QtSource/ToGuiActivityClient.cpp
  ../../../CommonSrc/QtSource/ToGuiActivityClient.h
  ../../../CommonSrc/QtSource/ToGuiActivityInterface.h
  ../../../CommonSrc/QtSource/ToGuiFileXferClient.cpp
  ../../../CommonSrc/QtSource/ToGuiFileXferClient.h
  ../../../CommonSrc/QtSource/ToGuiFileXferInterface.h
  ../../../CommonSrc/QtSource/ToGuiHardwareControlInterface.h
  ../../../CommonSrc/QtSource/ToGuiHardwareCtrlClient.cpp
  ../../../CommonSrc/QtSource/ToGuiHardwareCtrlClient.h
  ../../../CommonSrc/QtSource/ToGuiOfferInterface.h
  ../../../CommonSrc/QtSource/ToGuiThumbUpdateClient.cpp
  ../../../CommonSrc/QtSource/ToGuiThumbUpdateClient.h
  ../../../CommonSrc/QtSource/ToGuiThumbUpdateInterface.h
  ../../../CommonSrc/QtSource/ToGuiUserUpdateClient.cpp
  ../../../CommonSrc/QtSource/ToGuiUserUpdateClient.h
  ../../../CommonSrc/QtSource/ToGuiUserUpdateInterface.h
  ../../../CommonSrc/QtSource/UserJoinListItem.cpp
  ../../../CommonSrc/QtSource/UserJoinListItem.h
  ../../../CommonSrc/QtSource/UserJoinListWidget.cpp
  ../../../CommonSrc/QtSource/UserJoinListWidget.h
  ../../../CommonSrc/QtSource/UserListItem.cpp
  ../../../CommonSrc/QtSource/UserListItem.h
  ../../../CommonSrc/QtSource/UserListWidget.cpp
  ../../../CommonSrc/QtSource/UserListWidget.h
  ../../../CommonSrc/QtSource/UserProfile.h
  ../../../CommonSrc/QtSource/VidWidget.cpp
  ../../../CommonSrc/QtSource/VidWidget.h
  ../../../CommonSrc/QtSource/VxAppDisplay.cpp
  ../../../CommonSrc/QtSource/VxAppDisplay.h
  ../../../CommonSrc/QtSource/VxAppStyle.cpp
  ../../../CommonSrc/QtSource/VxAppStyle.h
  ../../../CommonSrc/QtSource/VxAppStyle2.cpp
  ../../../CommonSrc/QtSource/VxAppStyle2.h
  ../../../CommonSrc/QtSource/VxAppTheme.cpp
  ../../../CommonSrc/QtSource/VxAppTheme.h
  ../../../CommonSrc/QtSource/VxAspectFrame.cpp
  ../../../CommonSrc/QtSource/VxAspectFrame.h
  ../../../CommonSrc/QtSource/VxAvatarImage.cpp
  ../../../CommonSrc/QtSource/VxAvatarImage.h
  ../../../CommonSrc/QtSource/VxComboBox.cpp
  ../../../CommonSrc/QtSource/VxComboBox.h
  ../../../CommonSrc/QtSource/VxComboBoxFileFilter.cpp
  ../../../CommonSrc/QtSource/VxComboBoxFileFilter.h
  ../../../CommonSrc/QtSource/VxContextMenu.cpp
  ../../../CommonSrc/QtSource/VxContextMenu.h
  ../../../CommonSrc/QtSource/VxFlatButton.cpp
  ../../../CommonSrc/QtSource/VxFlatButton.h
  ../../../CommonSrc/QtSource/VxFrame.cpp
  ../../../CommonSrc/QtSource/VxFrame.h
  ../../../CommonSrc/QtSource/VxLabel.cpp
  ../../../CommonSrc/QtSource/VxLabel.h
  ../../../CommonSrc/QtSource/VxMenu.cpp
  ../../../CommonSrc/QtSource/VxMenu.h
  ../../../CommonSrc/QtSource/VxMenuButton.cpp
  ../../../CommonSrc/QtSource/VxMenuButton.h
  ../../../CommonSrc/QtSource/VxMenuButtonBottom.cpp
  ../../../CommonSrc/QtSource/VxMenuButtonBottom.h
  ../../../CommonSrc/QtSource/VxMenuButtonTop.cpp
  ../../../CommonSrc/QtSource/VxMenuButtonTop.h
  ../../../CommonSrc/QtSource/VxMyFileInfo.cpp
  ../../../CommonSrc/QtSource/VxMyFileInfo.h
  ../../../CommonSrc/QtSource/VxPlainTextEdit.cpp
  ../../../CommonSrc/QtSource/VxPlainTextEdit.h
  ../../../CommonSrc/QtSource/VxProgressBar.cpp
  ../../../CommonSrc/QtSource/VxProgressBar.h
  ../../../CommonSrc/QtSource/VxPushButton.cpp
  ../../../CommonSrc/QtSource/VxPushButton.h
  ../../../CommonSrc/QtSource/VxResourceToRealFile.cpp
  ../../../CommonSrc/QtSource/VxResourceToRealFile.h
  ../../../CommonSrc/QtSource/VxScrollArea.cpp
  ../../../CommonSrc/QtSource/VxScrollArea.h
  ../../../CommonSrc/QtSource/VxShredderButton.cpp
  ../../../CommonSrc/QtSource/VxShredderButton.h
  ../../../CommonSrc/QtSource/VxSndInstance.cpp
  ../../../CommonSrc/QtSource/VxSndInstance.h
  ../../../CommonSrc/QtSource/VxSpinProgress.cpp
  ../../../CommonSrc/QtSource/VxSpinProgress.h
  ../../../CommonSrc/QtSource/VxTagLabel.cpp
  ../../../CommonSrc/QtSource/VxTagLabel.h
  ../../../CommonSrc/QtSource/VxTilePositioner.cpp
  ../../../CommonSrc/QtSource/VxTilePositioner.h
  ../../../CommonSrc/QtSource/VxWidgetBase.cpp
  ../../../CommonSrc/QtSource/VxWidgetBase.h
  ../../../CommonSrc/Resources/bars_1_internet.png
  ../../../CommonSrc/Resources/bars_2_test_avail.png
  ../../../CommonSrc/Resources/bars_3_net_avail.png
  ../../../CommonSrc/Resources/bars_4_relay_search.png
  ../../../CommonSrc/Resources/bars_5_onlie_with_relay.png
  ../../../CommonSrc/Resources/bars_6_direct.png
  ../../../CommonSrc/Resources/bars_7_hosted_direct.png
  ../../../CommonSrc/Resources/bars_7_hosted_with_relay.png
  ../../../CommonSrc/Resources/msg_state_fail_resend.png
  ../../AppSource/main.cpp
  GeneratedFiles/ui_AcceptCancelWidget.h
  GeneratedFiles/ui_ActivityAddRemoveLibraryFiles.h
  GeneratedFiles/ui_ActivityBase.h
  GeneratedFiles/ui_ActivityBrowseFiles.h
  GeneratedFiles/ui_ActivityChooseTestWebsiteUrl.h
  GeneratedFiles/ui_ActivityDownloadItemMenu.h
  GeneratedFiles/ui_ActivityFileSearch.h
  GeneratedFiles/ui_ActivityIsPortOpenTest.h
  GeneratedFiles/ui_ActivityMessageBox.h
  GeneratedFiles/ui_ActivityNetworkStartupTest.h
  GeneratedFiles/ui_ActivityNetworkTest.h
  GeneratedFiles/ui_ActivityPermissions.h
  GeneratedFiles/ui_ActivityPermissionsRelay.h
  GeneratedFiles/ui_ActivityScanPeopleSearch.h
  GeneratedFiles/ui_ActivityScanProfiles.h
  GeneratedFiles/ui_ActivityScanStoryBoards.h
  GeneratedFiles/ui_ActivityScanWebCams.h
  GeneratedFiles/ui_ActivitySessionOptions.h
  GeneratedFiles/ui_ActivityShowHelp.h
  GeneratedFiles/ui_ActivitySnapShot.h
  GeneratedFiles/ui_ActivitySoundOptions.h
  GeneratedFiles/ui_ActivityStoryBoard.h
  GeneratedFiles/ui_ActivityTimedMessage.h
  GeneratedFiles/ui_ActivityVideoOptions.h
  GeneratedFiles/ui_ActivityViewAudioFile.h
  GeneratedFiles/ui_ActivityViewLibraryFiles.h
  GeneratedFiles/ui_ActivityViewLog.h
  GeneratedFiles/ui_ActivityViewMySharedFiles.h
  GeneratedFiles/ui_ActivityViewVideoFile.h
  GeneratedFiles/ui_ActivityWebCamClient.h
  GeneratedFiles/ui_ActivityYesNo.h
  GeneratedFiles/ui_AppletBrowseFiles.h
  GeneratedFiles/ui_AppletKodi.h
  GeneratedFiles/ui_AppletLaunchWidget.h
  GeneratedFiles/ui_AppletPeerChangeFriendship.h
  GeneratedFiles/ui_AppletPeerTodGame.h
  GeneratedFiles/ui_AppletPeerVideoPhone.h
  GeneratedFiles/ui_AppletPeerViewSharedFiles.h
  GeneratedFiles/ui_AppletPeerVoicePhone.h
  GeneratedFiles/ui_AppletPlayerVideo.h
  GeneratedFiles/ui_AppletSharedContent.h
  GeneratedFiles/ui_AppletTheme.h
  GeneratedFiles/ui_AppletUserIdentity.h
  GeneratedFiles/ui_AssetFaceWidget.h
  GeneratedFiles/ui_AssetPhotoWidget.h
  GeneratedFiles/ui_AssetTextWidget.h
  GeneratedFiles/ui_AssetVideoWidget.h
  GeneratedFiles/ui_AssetVoiceWidget.h
  GeneratedFiles/ui_AvatarBarWidget.h
  GeneratedFiles/ui_BottomBarWidget.h
  GeneratedFiles/ui_CentralFrame.h
  GeneratedFiles/ui_CentralWidget.h
  GeneratedFiles/ui_ChatEntryWidget.h
  GeneratedFiles/ui_DialogConfirmRemoveMessage.h
  GeneratedFiles/ui_FileActionBarWidget.h
  GeneratedFiles/ui_FileActionMenu.h
  GeneratedFiles/ui_FileDisplayWidget.h
  GeneratedFiles/ui_FileItemWidget.h
  GeneratedFiles/ui_FileShareItemWidget.h
  GeneratedFiles/ui_FileXferWidget.h
  GeneratedFiles/ui_HistoryEntryWidget.h
  GeneratedFiles/ui_HomeWindow.h
  GeneratedFiles/ui_IdentWidget.h
  GeneratedFiles/ui_ImagePickBar.h
  GeneratedFiles/ui_InputAllWidget.h
  GeneratedFiles/ui_InputFaceWidget.h
  GeneratedFiles/ui_InputPhotoWidget.h
  GeneratedFiles/ui_InputTextWidget.h
  GeneratedFiles/ui_InputVideoWidget.h
  GeneratedFiles/ui_InputVoiceWidget.h
  GeneratedFiles/ui_InstMsgWidget.h
  GeneratedFiles/ui_OfferBarWidget.h
  GeneratedFiles/ui_OfferWidget.h
  GeneratedFiles/ui_PlayerGlWidget.h
  GeneratedFiles/ui_PopupMenuWidget.h
  GeneratedFiles/ui_SessionWidget.h
  GeneratedFiles/ui_SoundCtrlWidget.h
  GeneratedFiles/ui_StoryWidget.h
  GeneratedFiles/ui_TitleBarWidget.h
  GeneratedFiles/ui_TodGameWidget.h
  GeneratedFiles/ui_VidWidget.h
)
# Output file name
set_target_properties(_NoLimitConnectApp
  PROPERTIES
  OUTPUT_NAME_DEBUG NoLimitConnectD
)

# Additional include directories
set_property(TARGET _NoLimitConnectApp
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    .\GeneratedFiles\$ENV{ConfigurationName};
    .\GeneratedFiles;
    .;
    $ENV{QTDIR}\include;
    $ENV{QTDIR}\mkspecs\win64-msvc;
    $ENV{QTDIR}\include\QtCore;
    $ENV{QTDIR}\include\QtGui;
    $ENV{QTDIR}\include\QtANGLE;
    $ENV{QTDIR}\include\QtWidgets;
    $ENV{QTDIR}\include\QtSvg;
    $ENV{QTDIR}\include\QtMultimedia;
    $ENV{QTDIR}\include\QtOpenGL;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\PlayerCommon;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\libgotvptop;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\CommonSrc\QtSource;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\kodi_src\xbmc\xbmc;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\kodi_src\xbmc\xbmc\cores\AudioEngine\;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\ffmpeg;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\openssl-1.0.2p\include;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Platinum\Source\Devices\MediaRenderer;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Platinum\Source\Devices\MediaConnect;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Platinum\Source\Devices\MediaServer;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Platinum\Source\Platinum;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Platinum\Source\Core;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Platinum\Source\Extras;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Neptune\Source\Core;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libUPnP\Neptune\Source\System\Win32;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libpcre;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\Effects11\inc;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\DependLibs\libfreetype\include;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\;
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\GoTvCore;
    $ENV{QTDIR}\include\QtMultimediaWidgets>
)

# Preprocessor definitions
target_compile_definitions(_NoLimitConnectApp PRIVATE
  $<$<CONFIG:Debug>:_MBCS;CONSOLE;WIN64;_DEBUG;TARGET_OS_WINDOWS;LIB_STATIC;QT_MULTIMEDIAWIDGETS_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(_NoLimitConnectApp PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(_NoLimitConnectApp PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(_NoLimitConnectApp PRIVATE
    ""
  )
  set_property(SOURCE ../../../../app_precompiled_hdr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yc"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AccountDb.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AccountMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/CamLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/MessengerPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/DialogConfirmRemoveMessage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileActionBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileActionMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileItemInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileItemWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileListReplySession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileShareItemWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FriendList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InstMsgWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/MultiSessionState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingFileMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingFileShare.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingFriend.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingNotify.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingOfferToFriend.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingOptions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingScan.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingServers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/P2PThingMedia.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/PopupMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/PopupMenuDelegate.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RasterWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/TextChatLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ChatEntryWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputAllWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputBaseWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputFaceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputPhotoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputTextWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputVideoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InputVoiceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityAddRemoveLibraryFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityAppSetup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityBrowseFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityChooseTestWebsiteUrl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityCreateAccount.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityDownloadItemMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityFileSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityInformation.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityIsPortOpenTest.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityMessageBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityNetworkState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityPermissions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityScanPeopleSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityScanProfiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityScanStoryBoards.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityScanWebCams.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivitySessionOptions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityShowHelp.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivitySoundOptions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityTimedMessage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityViewAudioFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityViewLibraryFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityViewMySharedFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityViewVideoFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ActivityYesNoMsgBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AssetBaseWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AssetFaceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AssetPhotoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AssetTextWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AssetVideoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AssetVoiceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AcceptCancelWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AvatarBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/BottomBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ImageListEntry.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ImageListRow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ImageListWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ImagePickBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/InformationWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/NetAvailStatusWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/NetworkKeyWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/NetworkTestWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/PermissionListItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/PermissionWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/PlayerGlWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/PluginSettingsWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RelayServiceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ServiceSettingsWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxAspectFrame.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxComboBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxContextMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/IdentWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxFrame.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxMenuButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxMenuButtonBottom.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxMenuButtonTop.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxPlainTextEdit.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxPushButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxComboBoxFileFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxFlatButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxLabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxMyFileInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxSndInstance.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxResourceToRealFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxScrollArea.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxShredderButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxSpinProgress.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxTagLabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxTilePositioner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/SessionWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/SoundCtrlWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/StoryWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VidWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxWidgetBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/TodGameDefs.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/TodGameLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/TodGameWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPeerBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPeerChangeFriendship.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPeerTodGame.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPeerVideoPhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPeerViewSharedFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPeerVoicePhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletAboutApp.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletApplicationInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletChooseThumbnail.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletClientAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletClientBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletClientRandomConnect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletClientShareFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletClientShareWebCam.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletClientStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletEditAboutMe.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletEditAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletEditStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletGalleryImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletGalleryThumb.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletKodi.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletLibrary.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletLogSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletLogView.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletNetHostingPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletNetworkSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPermissionList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPersonalRecorder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPlayerBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSearchPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceAboutMe.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceBaseSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceChatRoom.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceConnectionTest.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceHostGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceHostNetwork.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceRandomConnect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceRandomConnectRelay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceRelay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceShareFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceShareWebCam.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletServiceStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsAboutMe.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsChatRoom.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsConnectionTest.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsFileXfer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsHostGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsHostNetwork.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsMessenger.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsRandomConnect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsRandomConnectRelay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsShareFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsShareWebCam.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsTruthOrDare.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsVideoPhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSettingsVoicePhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletShareServicesPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletSnapshot.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletTheme.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletPlayerVideo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletUserIdentity.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletLaunchWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppletLaunchPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppCommon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppCommonAudio.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppCommonEvents.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppCommonLogin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppCommonRender.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppDefs.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppGlobals.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppProfile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/AppSetup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/EventsQtToGoTv.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/GuiParams.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxAppStyle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxAppTheme.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/GuiHelpers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/HomeWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/VxAppDisplay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioInIo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioInThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioIoMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioMixer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioOutIo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioOutThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioTestGenerator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/soundio/AudioUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/MySndMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/NetClientGuiLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/NetHostGuiLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/GuiFileActionSession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/GuiFileXferSession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/GuiOfferSession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FileXferWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FriendListDelegate.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/FriendListWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/HistoryEntryWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/HistoryListWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/LogMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/LogWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/OfferBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/OfferSessionLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/OfferSessionState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/OfferMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/OfferWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/MyIcons.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/TitleBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ThumbnailChooseWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ThumbnailEditWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/ThumbnailViewWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlMatrix.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlOffScreenSurface.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlShaders.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlWidgetFonts.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlWidgetRender.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlWidgetShaderInterface.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderGlWidgetTextures.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderKodiThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderLogoShaders.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderShaderDefsGl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderShaderDefsGles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../CommonSrc/QtSource/RenderShaderQt.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../AppSource/main.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
endif ()

# Additional library directories
if (MSVC)
  target_link_options(_NoLimitConnectApp PRIVATE
    $<$<CONFIG:Debug>:
      /LIBPATH:F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\build-sharedlibs\Windows\x64\
      /LIBPATH:F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\build-staticlibs\Windows\x64\
      /LIBPATH:$ENV{QTDIR}\lib>
  )
else ()
  target_link_options(_NoLimitConnectApp PRIVATE
    $<$<CONFIG:Debug>:
      -LF:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\build-sharedlibs\Windows\x64\
      -LF:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\..\build-staticlibs\Windows\x64\
      -L$ENV{QTDIR}\lib>
  )
endif ()

# Link libraries
set_property(TARGET _NoLimitConnectApp
  APPEND PROPERTY LINK_LIBRARIES
  "$<$<CONFIG:Debug>:NoLimitConnectCoreD.lib;glu32.lib;opengl32.lib;libpthreadD.lib;ole32.lib;winmm.lib;Rpcrt4.lib;libkodiD.lib;libopenmpt-fullD.lib;libnfsD.lib;Vfw32.lib;Upnp2LibD.lib;VxVideoLibD.lib;libcurlD.lib;libexpatD.lib;libcdioD.lib;libzipD.lib;libopencore-amrD.lib;libceltD.lib;libfaacD.lib;libxavsD.lib;Secur32.lib;librtmpD.lib;libbento4D.lib;CoreLibD.lib;FfmpegAvCodecLibD.lib;FfmpegAvDeviceLibD.lib;FfmpegAvFilterLibD.lib;FfmpegAvFormatLibD.lib;FfmpegAvResampleLibD.lib;FfmpegAvUtilLibD.lib;FfmpegPostProcLibD.lib;FfmpegSwResampleLibD.lib;FfmpegSwScaleLibD.lib;glew64sd.lib;libarmrwbencD.lib;libasprintfD.lib;libassD.lib;libbzip2D.lib;libcpluffD.lib;libcrocoD.lib;libcrossguidD.lib;libcrystalhdD.lib;libdnssdD.lib;libexifD.lib;libflacD.lib;libfreetypeD.lib;libfribidiD.lib;libgnuD.lib;libgsmD.lib;libiconvD.lib;libilbcD.lib;libjpgD.lib;liblameD.lib;liblzmaD.lib;liblzoD.lib;libmicrohttpdD.lib;liboggD.lib;libopenjpegD.lib;libopusD.lib;libp8-platformD.lib;libpcreD.lib;libpngD.lib;python27_d.lib;libshairplayD.lib;libshineD.lib;libsnappyD.lib;libspeexD.lib;libsshD.lib;libsslD.lib;libtagD.lib;libtheoraD.lib;libtinyxmlD.lib;libtwolameD.lib;libUPnPD.lib;libvorbisD.lib;libvpxD.lib;libwavepackD.lib;libwebpD.lib;libwelsdecD.lib;libx264D.lib;libx265D.lib;libxml2D.lib;libxsltD.lib;libxvidD.lib;libyajlD.lib;libzlibD.lib;libzvbiD.lib;MediaToolsLibD.lib;NetLibD.lib;PktLibD.lib;d3d11.lib;DInput8.lib;DSound.lib;Mpr.lib;Iphlpapi.lib;PowrProf.lib;setupapi.lib;dwmapi.lib;dxguid.lib;WS2_32.lib;WSock32.lib;Crypt32.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5Multimediad.lib;F:/Qt/5.12.3/msvc2017_64/lib/qtmaind.lib;shell32.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5Svgd.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5OpenGLd.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5Widgetsd.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5Guid.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5Cored.lib;F:/Qt/5.12.3/msvc2017_64/lib/Qt5QuickWidgetsd.lib;Qt5MultimediaWidgetsd.lib>"
)
