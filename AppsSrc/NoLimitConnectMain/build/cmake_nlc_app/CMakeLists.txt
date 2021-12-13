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
  ../../../GoTvCommon/QtSource/AcceptCancelWidget.cpp
  ../../../GoTvCommon/QtSource/AcceptCancelWidget.h
  ../../../GoTvCommon/QtSource/AccountDb.cpp
  ../../../GoTvCommon/QtSource/AccountDb.h
  ../../../GoTvCommon/QtSource/AccountMgr.cpp
  ../../../GoTvCommon/QtSource/AccountMgr.h
  ../../../GoTvCommon/QtSource/ActivityAddRemoveLibraryFiles.cpp
  ../../../GoTvCommon/QtSource/ActivityAddRemoveLibraryFiles.h
  ../../../GoTvCommon/QtSource/ActivityAppSetup.cpp
  ../../../GoTvCommon/QtSource/ActivityAppSetup.h
  ../../../GoTvCommon/QtSource/ActivityBase.cpp
  ../../../GoTvCommon/QtSource/ActivityBase.h
  ../../../GoTvCommon/QtSource/ActivityBrowseFiles.cpp
  ../../../GoTvCommon/QtSource/ActivityBrowseFiles.h
  ../../../GoTvCommon/QtSource/ActivityChooseTestWebsiteUrl.cpp
  ../../../GoTvCommon/QtSource/ActivityChooseTestWebsiteUrl.h
  ../../../GoTvCommon/QtSource/ActivityCreateAccount.cpp
  ../../../GoTvCommon/QtSource/ActivityCreateAccount.h
  ../../../GoTvCommon/QtSource/ActivityDownloadItemMenu.cpp
  ../../../GoTvCommon/QtSource/ActivityDownloadItemMenu.h
  ../../../GoTvCommon/QtSource/ActivityFileSearch.cpp
  ../../../GoTvCommon/QtSource/ActivityFileSearch.h
  ../../../GoTvCommon/QtSource/ActivityHelpSignal.cpp
  ../../../GoTvCommon/QtSource/ActivityHelpSignal.h
  ../../../GoTvCommon/QtSource/ActivityInformation.cpp
  ../../../GoTvCommon/QtSource/ActivityInformation.h
  ../../../GoTvCommon/QtSource/ActivityIsPortOpenTest.cpp
  ../../../GoTvCommon/QtSource/ActivityIsPortOpenTest.h
  ../../../GoTvCommon/QtSource/ActivityMessageBox.cpp
  ../../../GoTvCommon/QtSource/ActivityMessageBox.h
  ../../../GoTvCommon/QtSource/ActivityNetworkState.cpp
  ../../../GoTvCommon/QtSource/ActivityNetworkState.h
  ../../../GoTvCommon/QtSource/ActivityPermissions.cpp
  ../../../GoTvCommon/QtSource/ActivityPermissions.h
  ../../../GoTvCommon/QtSource/ActivityScanPeopleSearch.cpp
  ../../../GoTvCommon/QtSource/ActivityScanPeopleSearch.h
  ../../../GoTvCommon/QtSource/ActivityScanProfiles.cpp
  ../../../GoTvCommon/QtSource/ActivityScanProfiles.h
  ../../../GoTvCommon/QtSource/ActivityScanStoryBoards.cpp
  ../../../GoTvCommon/QtSource/ActivityScanStoryBoards.h
  ../../../GoTvCommon/QtSource/ActivityScanWebCams.cpp
  ../../../GoTvCommon/QtSource/ActivityScanWebCams.h
  ../../../GoTvCommon/QtSource/ActivitySessionOptions.cpp
  ../../../GoTvCommon/QtSource/ActivitySessionOptions.h
  ../../../GoTvCommon/QtSource/ActivityShowHelp.cpp
  ../../../GoTvCommon/QtSource/ActivityShowHelp.h
  ../../../GoTvCommon/QtSource/ActivitySnapShot.cpp
  ../../../GoTvCommon/QtSource/ActivitySnapShot.h
  ../../../GoTvCommon/QtSource/ActivitySoundOptions.cpp
  ../../../GoTvCommon/QtSource/ActivitySoundOptions.h
  ../../../GoTvCommon/QtSource/ActivityTimedMessage.cpp
  ../../../GoTvCommon/QtSource/ActivityTimedMessage.h
  ../../../GoTvCommon/QtSource/ActivityViewAudioFile.cpp
  ../../../GoTvCommon/QtSource/ActivityViewAudioFile.h
  ../../../GoTvCommon/QtSource/ActivityViewLibraryFiles.cpp
  ../../../GoTvCommon/QtSource/ActivityViewLibraryFiles.h
  ../../../GoTvCommon/QtSource/ActivityViewMySharedFiles.cpp
  ../../../GoTvCommon/QtSource/ActivityViewMySharedFiles.h
  ../../../GoTvCommon/QtSource/ActivityViewVideoFile.cpp
  ../../../GoTvCommon/QtSource/ActivityViewVideoFile.h
  ../../../GoTvCommon/QtSource/ActivityYesNoMsgBox.cpp
  ../../../GoTvCommon/QtSource/ActivityYesNoMsgBox.h
  ../../../GoTvCommon/QtSource/AppCommon.cpp
  ../../../GoTvCommon/QtSource/AppCommon.h
  ../../../GoTvCommon/QtSource/AppCommonAudio.cpp
  ../../../GoTvCommon/QtSource/AppCommonEvents.cpp
  ../../../GoTvCommon/QtSource/AppCommonLogin.cpp
  ../../../GoTvCommon/QtSource/AppCommonRender.cpp
  ../../../GoTvCommon/QtSource/AppDefs.cpp
  ../../../GoTvCommon/QtSource/AppDefs.h
  ../../../GoTvCommon/QtSource/AppGlobals.cpp
  ../../../GoTvCommon/QtSource/AppGlobals.h
  ../../../GoTvCommon/QtSource/AppletAboutApp.cpp
  ../../../GoTvCommon/QtSource/AppletAboutApp.h
  ../../../GoTvCommon/QtSource/AppletApplicationInfo.cpp
  ../../../GoTvCommon/QtSource/AppletApplicationInfo.h
  ../../../GoTvCommon/QtSource/AppletBase.cpp
  ../../../GoTvCommon/QtSource/AppletBase.h
  ../../../GoTvCommon/QtSource/AppletBrowseFiles.cpp
  ../../../GoTvCommon/QtSource/AppletBrowseFiles.h
  ../../../GoTvCommon/QtSource/AppletCallList.cpp
  ../../../GoTvCommon/QtSource/AppletCallList.h
  ../../../GoTvCommon/QtSource/AppletCamSettings.cpp
  ../../../GoTvCommon/QtSource/AppletCamSettings.h
  ../../../GoTvCommon/QtSource/AppletChatRoomClient.cpp
  ../../../GoTvCommon/QtSource/AppletChatRoomClient.h
  ../../../GoTvCommon/QtSource/AppletChatRoomJoin.cpp
  ../../../GoTvCommon/QtSource/AppletChatRoomJoin.h
  ../../../GoTvCommon/QtSource/AppletChatRoomJoinSearch.cpp
  ../../../GoTvCommon/QtSource/AppletChatRoomJoinSearch.h
  ../../../GoTvCommon/QtSource/AppletChooseThumbnail.cpp
  ../../../GoTvCommon/QtSource/AppletChooseThumbnail.h
  ../../../GoTvCommon/QtSource/AppletClientAvatarImage.cpp
  ../../../GoTvCommon/QtSource/AppletClientAvatarImage.h
  ../../../GoTvCommon/QtSource/AppletClientBase.cpp
  ../../../GoTvCommon/QtSource/AppletClientBase.h
  ../../../GoTvCommon/QtSource/AppletClientRandomConnect.cpp
  ../../../GoTvCommon/QtSource/AppletClientRandomConnect.h
  ../../../GoTvCommon/QtSource/AppletClientShareFiles.cpp
  ../../../GoTvCommon/QtSource/AppletClientShareFiles.h
  ../../../GoTvCommon/QtSource/AppletClientShareWebCam.cpp
  ../../../GoTvCommon/QtSource/AppletClientShareWebCam.h
  ../../../GoTvCommon/QtSource/AppletClientStoryboard.cpp
  ../../../GoTvCommon/QtSource/AppletClientStoryboard.h
  ../../../GoTvCommon/QtSource/AppletDownloads.cpp
  ../../../GoTvCommon/QtSource/AppletDownloads.h
  ../../../GoTvCommon/QtSource/AppletEditAboutMe.cpp
  ../../../GoTvCommon/QtSource/AppletEditAboutMe.h
  ../../../GoTvCommon/QtSource/AppletEditAvatarImage.cpp
  ../../../GoTvCommon/QtSource/AppletEditAvatarImage.h
  ../../../GoTvCommon/QtSource/AppletEditStoryboard.cpp
  ../../../GoTvCommon/QtSource/AppletEditStoryboard.h
  ../../../GoTvCommon/QtSource/AppletGalleryImage.cpp
  ../../../GoTvCommon/QtSource/AppletGalleryImage.h
  ../../../GoTvCommon/QtSource/AppletGalleryThumb.cpp
  ../../../GoTvCommon/QtSource/AppletGalleryThumb.h
  ../../../GoTvCommon/QtSource/AppletGroupJoin.cpp
  ../../../GoTvCommon/QtSource/AppletGroupJoin.h
  ../../../GoTvCommon/QtSource/AppletGroupJoinSearch.cpp
  ../../../GoTvCommon/QtSource/AppletGroupJoinSearch.h
  ../../../GoTvCommon/QtSource/AppletJoinBase.cpp
  ../../../GoTvCommon/QtSource/AppletJoinBase.h
  ../../../GoTvCommon/QtSource/AppletKodi.cpp
  ../../../GoTvCommon/QtSource/AppletKodi.h
  ../../../GoTvCommon/QtSource/AppletLaunchPage.cpp
  ../../../GoTvCommon/QtSource/AppletLaunchPage.h
  ../../../GoTvCommon/QtSource/AppletLaunchWidget.cpp
  ../../../GoTvCommon/QtSource/AppletLaunchWidget.h
  ../../../GoTvCommon/QtSource/AppletLibrary.cpp
  ../../../GoTvCommon/QtSource/AppletLibrary.h
  ../../../GoTvCommon/QtSource/AppletLog.cpp
  ../../../GoTvCommon/QtSource/AppletLog.h
  ../../../GoTvCommon/QtSource/AppletLogSettings.cpp
  ../../../GoTvCommon/QtSource/AppletLogSettings.h
  ../../../GoTvCommon/QtSource/AppletLogView.cpp
  ../../../GoTvCommon/QtSource/AppletLogView.h
  ../../../GoTvCommon/QtSource/AppletMgr.cpp
  ../../../GoTvCommon/QtSource/AppletMgr.h
  ../../../GoTvCommon/QtSource/AppletMultiMessenger.cpp
  ../../../GoTvCommon/QtSource/AppletMultiMessenger.h
  ../../../GoTvCommon/QtSource/AppletNetHostingPage.cpp
  ../../../GoTvCommon/QtSource/AppletNetHostingPage.h
  ../../../GoTvCommon/QtSource/AppletNetworkSettings.cpp
  ../../../GoTvCommon/QtSource/AppletNetworkSettings.h
  ../../../GoTvCommon/QtSource/AppletOfferList.cpp
  ../../../GoTvCommon/QtSource/AppletOfferList.h
  ../../../GoTvCommon/QtSource/AppletPeerBase.cpp
  ../../../GoTvCommon/QtSource/AppletPeerBase.h
  ../../../GoTvCommon/QtSource/AppletPeerChangeFriendship.cpp
  ../../../GoTvCommon/QtSource/AppletPeerChangeFriendship.h
  ../../../GoTvCommon/QtSource/AppletPeerReplyFileOffer.cpp
  ../../../GoTvCommon/QtSource/AppletPeerReplyFileOffer.h
  ../../../GoTvCommon/QtSource/AppletPeerSelectFileToSend.cpp
  ../../../GoTvCommon/QtSource/AppletPeerSelectFileToSend.h
  ../../../GoTvCommon/QtSource/AppletPeerSessionFileOffer.cpp
  ../../../GoTvCommon/QtSource/AppletPeerSessionFileOffer.h
  ../../../GoTvCommon/QtSource/AppletPeerTodGame.cpp
  ../../../GoTvCommon/QtSource/AppletPeerTodGame.h
  ../../../GoTvCommon/QtSource/AppletPeerVideoPhone.cpp
  ../../../GoTvCommon/QtSource/AppletPeerVideoPhone.h
  ../../../GoTvCommon/QtSource/AppletPeerViewSharedFiles.cpp
  ../../../GoTvCommon/QtSource/AppletPeerViewSharedFiles.h
  ../../../GoTvCommon/QtSource/AppletPeerVoicePhone.cpp
  ../../../GoTvCommon/QtSource/AppletPeerVoicePhone.h
  ../../../GoTvCommon/QtSource/AppletPermissionList.cpp
  ../../../GoTvCommon/QtSource/AppletPermissionList.h
  ../../../GoTvCommon/QtSource/AppletPersonalRecorder.cpp
  ../../../GoTvCommon/QtSource/AppletPersonalRecorder.h
  ../../../GoTvCommon/QtSource/AppletPlayerBase.cpp
  ../../../GoTvCommon/QtSource/AppletPlayerBase.h
  ../../../GoTvCommon/QtSource/AppletPlayerVideo.cpp
  ../../../GoTvCommon/QtSource/AppletPlayerVideo.h
  ../../../GoTvCommon/QtSource/AppletRandomConnectJoin.cpp
  ../../../GoTvCommon/QtSource/AppletRandomConnectJoin.h
  ../../../GoTvCommon/QtSource/AppletRandomConnectJoinSearch.cpp
  ../../../GoTvCommon/QtSource/AppletRandomConnectJoinSearch.h
  ../../../GoTvCommon/QtSource/AppletSearchPage.cpp
  ../../../GoTvCommon/QtSource/AppletSearchPage.h
  ../../../GoTvCommon/QtSource/AppletServiceAboutMe.cpp
  ../../../GoTvCommon/QtSource/AppletServiceAboutMe.h
  ../../../GoTvCommon/QtSource/AppletServiceAvatarImage.cpp
  ../../../GoTvCommon/QtSource/AppletServiceAvatarImage.h
  ../../../GoTvCommon/QtSource/AppletServiceBase.cpp
  ../../../GoTvCommon/QtSource/AppletServiceBase.h
  ../../../GoTvCommon/QtSource/AppletServiceBaseSettings.cpp
  ../../../GoTvCommon/QtSource/AppletServiceBaseSettings.h
  ../../../GoTvCommon/QtSource/AppletServiceChatRoom.cpp
  ../../../GoTvCommon/QtSource/AppletServiceChatRoom.h
  ../../../GoTvCommon/QtSource/AppletServiceConnectionTest.cpp
  ../../../GoTvCommon/QtSource/AppletServiceConnectionTest.h
  ../../../GoTvCommon/QtSource/AppletServiceHostGroup.cpp
  ../../../GoTvCommon/QtSource/AppletServiceHostGroup.h
  ../../../GoTvCommon/QtSource/AppletServiceHostNetwork.cpp
  ../../../GoTvCommon/QtSource/AppletServiceHostNetwork.h
  ../../../GoTvCommon/QtSource/AppletServiceRandomConnect.cpp
  ../../../GoTvCommon/QtSource/AppletServiceRandomConnect.h
  ../../../GoTvCommon/QtSource/AppletServiceRandomConnectRelay.cpp
  ../../../GoTvCommon/QtSource/AppletServiceRandomConnectRelay.h
  ../../../GoTvCommon/QtSource/AppletServiceRelay.cpp
  ../../../GoTvCommon/QtSource/AppletServiceRelay.h
  ../../../GoTvCommon/QtSource/AppletServiceShareFiles.cpp
  ../../../GoTvCommon/QtSource/AppletServiceShareFiles.h
  ../../../GoTvCommon/QtSource/AppletServiceShareWebCam.cpp
  ../../../GoTvCommon/QtSource/AppletServiceShareWebCam.h
  ../../../GoTvCommon/QtSource/AppletServiceStoryboard.cpp
  ../../../GoTvCommon/QtSource/AppletServiceStoryboard.h
  ../../../GoTvCommon/QtSource/AppletSettingsAboutMe.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsAboutMe.h
  ../../../GoTvCommon/QtSource/AppletSettingsAvatarImage.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsAvatarImage.h
  ../../../GoTvCommon/QtSource/AppletSettingsBase.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsBase.h
  ../../../GoTvCommon/QtSource/AppletSettingsChatRoom.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsChatRoom.h
  ../../../GoTvCommon/QtSource/AppletSettingsConnectionTest.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsConnectionTest.h
  ../../../GoTvCommon/QtSource/AppletSettingsFileXfer.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsFileXfer.h
  ../../../GoTvCommon/QtSource/AppletSettingsHostBase.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsHostBase.h
  ../../../GoTvCommon/QtSource/AppletSettingsHostChatRoom.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsHostChatRoom.h
  ../../../GoTvCommon/QtSource/AppletSettingsHostGroup.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsHostGroup.h
  ../../../GoTvCommon/QtSource/AppletSettingsHostNetwork.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsHostNetwork.h
  ../../../GoTvCommon/QtSource/AppletSettingsHostRandomConnect.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsHostRandomConnect.h
  ../../../GoTvCommon/QtSource/AppletSettingsMessenger.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsMessenger.h
  ../../../GoTvCommon/QtSource/AppletSettingsPage.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsPage.h
  ../../../GoTvCommon/QtSource/AppletSettingsRandomConnect.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsRandomConnect.h
  ../../../GoTvCommon/QtSource/AppletSettingsRandomConnectRelay.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsRandomConnectRelay.h
  ../../../GoTvCommon/QtSource/AppletSettingsShareFiles.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsShareFiles.h
  ../../../GoTvCommon/QtSource/AppletSettingsShareWebCam.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsShareWebCam.h
  ../../../GoTvCommon/QtSource/AppletSettingsStoryboard.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsStoryboard.h
  ../../../GoTvCommon/QtSource/AppletSettingsTruthOrDare.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsTruthOrDare.h
  ../../../GoTvCommon/QtSource/AppletSettingsVideoPhone.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsVideoPhone.h
  ../../../GoTvCommon/QtSource/AppletSettingsVoicePhone.cpp
  ../../../GoTvCommon/QtSource/AppletSettingsVoicePhone.h
  ../../../GoTvCommon/QtSource/AppletShareServicesPage.cpp
  ../../../GoTvCommon/QtSource/AppletShareServicesPage.h
  ../../../GoTvCommon/QtSource/AppletSnapshot.cpp
  ../../../GoTvCommon/QtSource/AppletSnapshot.h
  ../../../GoTvCommon/QtSource/AppletTestAndDebug.cpp
  ../../../GoTvCommon/QtSource/AppletTestAndDebug.h
  ../../../GoTvCommon/QtSource/AppletTheme.cpp
  ../../../GoTvCommon/QtSource/AppletTheme.h
  ../../../GoTvCommon/QtSource/AppletUploads.cpp
  ../../../GoTvCommon/QtSource/AppletUploads.h
  ../../../GoTvCommon/QtSource/AppletUserIdentity.cpp
  ../../../GoTvCommon/QtSource/AppletUserIdentity.h
  ../../../GoTvCommon/QtSource/AppProfile.cpp
  ../../../GoTvCommon/QtSource/AppProfile.h
  ../../../GoTvCommon/QtSource/AppSettings.cpp
  ../../../GoTvCommon/QtSource/AppSettings.h
  ../../../GoTvCommon/QtSource/AppSetup.cpp
  ../../../GoTvCommon/QtSource/AppSetup.h
  ../../../GoTvCommon/QtSource/AppTranslate.cpp
  ../../../GoTvCommon/QtSource/AppTranslate.h
  ../../../GoTvCommon/QtSource/AssetBaseWidget.cpp
  ../../../GoTvCommon/QtSource/AssetBaseWidget.h
  ../../../GoTvCommon/QtSource/AssetFaceWidget.cpp
  ../../../GoTvCommon/QtSource/AssetFaceWidget.h
  ../../../GoTvCommon/QtSource/AssetPhotoWidget.cpp
  ../../../GoTvCommon/QtSource/AssetPhotoWidget.h
  ../../../GoTvCommon/QtSource/AssetTextWidget.cpp
  ../../../GoTvCommon/QtSource/AssetTextWidget.h
  ../../../GoTvCommon/QtSource/AssetVideoWidget.cpp
  ../../../GoTvCommon/QtSource/AssetVideoWidget.h
  ../../../GoTvCommon/QtSource/AssetVoiceWidget.cpp
  ../../../GoTvCommon/QtSource/AssetVoiceWidget.h
  ../../../GoTvCommon/QtSource/AvatarBar.cpp
  ../../../GoTvCommon/QtSource/AvatarBar.h
  ../../../GoTvCommon/QtSource/BottomBarWidget.cpp
  ../../../GoTvCommon/QtSource/BottomBarWidget.h
  ../../../GoTvCommon/QtSource/CallListItem.cpp
  ../../../GoTvCommon/QtSource/CallListItem.h
  ../../../GoTvCommon/QtSource/CallListWidget.cpp
  ../../../GoTvCommon/QtSource/CallListWidget.h
  ../../../GoTvCommon/QtSource/CamLogic.cpp
  ../../../GoTvCommon/QtSource/CamLogic.h
  ../../../GoTvCommon/QtSource/CamTitleBarPreview.cpp
  ../../../GoTvCommon/QtSource/CamTitleBarPreview.h
  ../../../GoTvCommon/QtSource/ChatEntryWidget.cpp
  ../../../GoTvCommon/QtSource/ChatEntryWidget.h
  ../../../GoTvCommon/QtSource/ChatRoomWidget.cpp
  ../../../GoTvCommon/QtSource/ChatRoomWidget.h
  ../../../GoTvCommon/QtSource/DialogConfirmRemoveMessage.cpp
  ../../../GoTvCommon/QtSource/DialogConfirmRemoveMessage.h
  ../../../GoTvCommon/QtSource/EventsQtToGoTv.cpp
  ../../../GoTvCommon/QtSource/EventsQtToGoTv.h
  ../../../GoTvCommon/QtSource/FileActionBarWidget.cpp
  ../../../GoTvCommon/QtSource/FileActionBarWidget.h
  ../../../GoTvCommon/QtSource/FileActionMenu.cpp
  ../../../GoTvCommon/QtSource/FileActionMenu.h
  ../../../GoTvCommon/QtSource/FileItemInfo.cpp
  ../../../GoTvCommon/QtSource/FileItemInfo.h
  ../../../GoTvCommon/QtSource/FileItemWidget.cpp
  ../../../GoTvCommon/QtSource/FileItemWidget.h
  ../../../GoTvCommon/QtSource/FileListReplySession.cpp
  ../../../GoTvCommon/QtSource/FileListReplySession.h
  ../../../GoTvCommon/QtSource/FileShareItemWidget.cpp
  ../../../GoTvCommon/QtSource/FileShareItemWidget.h
  ../../../GoTvCommon/QtSource/FileXferWidget.cpp
  ../../../GoTvCommon/QtSource/FileXferWidget.h
  ../../../GoTvCommon/QtSource/FriendList.cpp
  ../../../GoTvCommon/QtSource/FriendList.h
  ../../../GoTvCommon/QtSource/FriendListDelegate.cpp
  ../../../GoTvCommon/QtSource/FriendListDelegate.h
  ../../../GoTvCommon/QtSource/FriendListEntryWidget.cpp
  ../../../GoTvCommon/QtSource/FriendListEntryWidget.h
  ../../../GoTvCommon/QtSource/FriendListWidget.cpp
  ../../../GoTvCommon/QtSource/FriendListWidget.h
  ../../../GoTvCommon/QtSource/GoTvP2P.ico
  ../../../GoTvCommon/QtSource/GoTvP2P.rc
  ../../../GoTvCommon/QtSource/GuiConnectMgr.cpp
  ../../../GoTvCommon/QtSource/GuiConnectMgr.h
  ../../../GoTvCommon/QtSource/GuiFileActionSession.cpp
  ../../../GoTvCommon/QtSource/GuiFileActionSession.h
  ../../../GoTvCommon/QtSource/GuiFileXferSession.cpp
  ../../../GoTvCommon/QtSource/GuiFileXferSession.h
  ../../../GoTvCommon/QtSource/GuiHelpers.cpp
  ../../../GoTvCommon/QtSource/GuiHelpers.h
  ../../../GoTvCommon/QtSource/GuiHostSession.cpp
  ../../../GoTvCommon/QtSource/GuiHostSession.h
  ../../../GoTvCommon/QtSource/GuiOfferMgr.cpp
  ../../../GoTvCommon/QtSource/GuiOfferMgr.h
  ../../../GoTvCommon/QtSource/GuiOfferSession.cpp
  ../../../GoTvCommon/QtSource/GuiOfferSession.h
  ../../../GoTvCommon/QtSource/GuiParams.cpp
  ../../../GoTvCommon/QtSource/GuiThumb.cpp
  ../../../GoTvCommon/QtSource/GuiThumb.h
  ../../../GoTvCommon/QtSource/GuiThumbList.cpp
  ../../../GoTvCommon/QtSource/GuiThumbList.h
  ../../../GoTvCommon/QtSource/GuiThumbMgr.cpp
  ../../../GoTvCommon/QtSource/GuiThumbMgr.h
  ../../../GoTvCommon/QtSource/GuiUser.cpp
  ../../../GoTvCommon/QtSource/GuiUser.h
  ../../../GoTvCommon/QtSource/GuiUserJoin.cpp
  ../../../GoTvCommon/QtSource/GuiUserJoin.h
  ../../../GoTvCommon/QtSource/GuiUserJoinList.cpp
  ../../../GoTvCommon/QtSource/GuiUserJoinList.h
  ../../../GoTvCommon/QtSource/GuiUserJoinMgr.cpp
  ../../../GoTvCommon/QtSource/GuiUserJoinMgr.h
  ../../../GoTvCommon/QtSource/GuiUserJoinSession.cpp
  ../../../GoTvCommon/QtSource/GuiUserJoinSession.h
  ../../../GoTvCommon/QtSource/GuiUserMgr.cpp
  ../../../GoTvCommon/QtSource/GuiUserMgr.h
  ../../../GoTvCommon/QtSource/GuiUserSessionBase.cpp
  ../../../GoTvCommon/QtSource/GuiUserSessionBase.h
  ../../../GoTvCommon/QtSource/HelpBrowser.cpp
  ../../../GoTvCommon/QtSource/HelpBrowser.h
  ../../../GoTvCommon/QtSource/HistoryEntryWidget.cpp
  ../../../GoTvCommon/QtSource/HistoryEntryWidget.h
  ../../../GoTvCommon/QtSource/HistoryListWidget.cpp
  ../../../GoTvCommon/QtSource/HistoryListWidget.h
  ../../../GoTvCommon/QtSource/HomeWindow.cpp
  ../../../GoTvCommon/QtSource/HomeWindow.h
  ../../../GoTvCommon/QtSource/HostListItem.cpp
  ../../../GoTvCommon/QtSource/HostListItem.h
  ../../../GoTvCommon/QtSource/HostListWidget.cpp
  ../../../GoTvCommon/QtSource/HostListWidget.h
  ../../../GoTvCommon/QtSource/IdentWidget.cpp
  ../../../GoTvCommon/QtSource/IdentWidget.h
  ../../../GoTvCommon/QtSource/ImageListEntry.cpp
  ../../../GoTvCommon/QtSource/ImageListEntry.h
  ../../../GoTvCommon/QtSource/ImageListRow.cpp
  ../../../GoTvCommon/QtSource/ImageListRow.h
  ../../../GoTvCommon/QtSource/ImageListWidget.cpp
  ../../../GoTvCommon/QtSource/ImageListWidget.h
  ../../../GoTvCommon/QtSource/ImagePickBar.cpp
  ../../../GoTvCommon/QtSource/ImagePickBar.h
  ../../../GoTvCommon/QtSource/InformationWidget.cpp
  ../../../GoTvCommon/QtSource/InformationWidget.h
  ../../../GoTvCommon/QtSource/InputAllWidget.cpp
  ../../../GoTvCommon/QtSource/InputAllWidget.h
  ../../../GoTvCommon/QtSource/InputBaseWidget.cpp
  ../../../GoTvCommon/QtSource/InputBaseWidget.h
  ../../../GoTvCommon/QtSource/InputFaceWidget.cpp
  ../../../GoTvCommon/QtSource/InputFaceWidget.h
  ../../../GoTvCommon/QtSource/InputPhotoWidget.cpp
  ../../../GoTvCommon/QtSource/InputPhotoWidget.h
  ../../../GoTvCommon/QtSource/InputTextWidget.cpp
  ../../../GoTvCommon/QtSource/InputTextWidget.h
  ../../../GoTvCommon/QtSource/InputVideoWidget.cpp
  ../../../GoTvCommon/QtSource/InputVideoWidget.h
  ../../../GoTvCommon/QtSource/InputVoiceWidget.cpp
  ../../../GoTvCommon/QtSource/InputVoiceWidget.h
  ../../../GoTvCommon/QtSource/InstMsgWidget.cpp
  ../../../GoTvCommon/QtSource/InstMsgWidget.h
  ../../../GoTvCommon/QtSource/LogMgr.cpp
  ../../../GoTvCommon/QtSource/LogMgr.h
  ../../../GoTvCommon/QtSource/LogWidget.cpp
  ../../../GoTvCommon/QtSource/LogWidget.h
  ../../../GoTvCommon/QtSource/MenuBase.cpp
  ../../../GoTvCommon/QtSource/MenuBase.h
  ../../../GoTvCommon/QtSource/MenuCam.cpp
  ../../../GoTvCommon/QtSource/MenuCam.h
  ../../../GoTvCommon/QtSource/MenuDefs.h
  ../../../GoTvCommon/QtSource/MessengerPage.cpp
  ../../../GoTvCommon/QtSource/MessengerPage.h
  ../../../GoTvCommon/QtSource/MultiSessionState.cpp
  ../../../GoTvCommon/QtSource/MultiSessionState.h
  ../../../GoTvCommon/QtSource/MyIcons.cpp
  ../../../GoTvCommon/QtSource/MyIcons.h
  ../../../GoTvCommon/QtSource/MyIconsDefs.h
  ../../../GoTvCommon/QtSource/MySndMgr.cpp
  ../../../GoTvCommon/QtSource/MySndMgr.h
  ../../../GoTvCommon/QtSource/NetAvailStatusWidget.cpp
  ../../../GoTvCommon/QtSource/NetAvailStatusWidget.h
  ../../../GoTvCommon/QtSource/NetClientGuiLogic.cpp
  ../../../GoTvCommon/QtSource/NetClientGuiLogic.h
  ../../../GoTvCommon/QtSource/NetHostGuiLogic.cpp
  ../../../GoTvCommon/QtSource/NetHostGuiLogic.h
  ../../../GoTvCommon/QtSource/NetworkKeyWidget.cpp
  ../../../GoTvCommon/QtSource/NetworkKeyWidget.h
  ../../../GoTvCommon/QtSource/NetworkTestWidget.cpp
  ../../../GoTvCommon/QtSource/NetworkTestWidget.h
  ../../../GoTvCommon/QtSource/OfferBarWidget.cpp
  ../../../GoTvCommon/QtSource/OfferBarWidget.h
  ../../../GoTvCommon/QtSource/OfferListItem.cpp
  ../../../GoTvCommon/QtSource/OfferListItem.h
  ../../../GoTvCommon/QtSource/OfferListWidget.cpp
  ../../../GoTvCommon/QtSource/OfferListWidget.h
  ../../../GoTvCommon/QtSource/OfferMgr.cpp
  ../../../GoTvCommon/QtSource/OfferMgr.h
  ../../../GoTvCommon/QtSource/OfferSessionCallbackInterface.h
  ../../../GoTvCommon/QtSource/OfferSessionLogic.cpp
  ../../../GoTvCommon/QtSource/OfferSessionLogic.h
  ../../../GoTvCommon/QtSource/OfferSessionState.cpp
  ../../../GoTvCommon/QtSource/OfferSessionState.h
  ../../../GoTvCommon/QtSource/OfferWidget.cpp
  ../../../GoTvCommon/QtSource/OfferWidget.h
  ../../../GoTvCommon/QtSource/P2PThingFileMenu.cpp
  ../../../GoTvCommon/QtSource/P2PThingFileShare.cpp
  ../../../GoTvCommon/QtSource/P2PThingFriend.cpp
  ../../../GoTvCommon/QtSource/P2PThingMedia.cpp
  ../../../GoTvCommon/QtSource/P2PThingNotify.cpp
  ../../../GoTvCommon/QtSource/P2PThingOfferToFriend.cpp
  ../../../GoTvCommon/QtSource/P2PThingOptions.cpp
  ../../../GoTvCommon/QtSource/P2PThingScan.cpp
  ../../../GoTvCommon/QtSource/P2PThingSearch.cpp
  ../../../GoTvCommon/QtSource/P2PThingServers.cpp
  ../../../GoTvCommon/QtSource/PermissionListItem.cpp
  ../../../GoTvCommon/QtSource/PermissionListItem.h
  ../../../GoTvCommon/QtSource/PermissionWidget.cpp
  ../../../GoTvCommon/QtSource/PermissionWidget.h
  ../../../GoTvCommon/QtSource/PlayerGlWidget.cpp
  ../../../GoTvCommon/QtSource/PlayerGlWidget.h
  ../../../GoTvCommon/QtSource/PluginSettingsWidget.cpp
  ../../../GoTvCommon/QtSource/PluginSettingsWidget.h
  ../../../GoTvCommon/QtSource/PopupMenu.cpp
  ../../../GoTvCommon/QtSource/PopupMenu.h
  ../../../GoTvCommon/QtSource/PopupMenuDelegate.cpp
  ../../../GoTvCommon/QtSource/PopupMenuDelegate.h
  ../../../GoTvCommon/QtSource/RasterWindow.cpp
  ../../../GoTvCommon/QtSource/RasterWindow.h
  ../../../GoTvCommon/QtSource/RelayServiceWidget.cpp
  ../../../GoTvCommon/QtSource/RelayServiceWidget.h
  ../../../GoTvCommon/QtSource/RenderGlLogic.cpp
  ../../../GoTvCommon/QtSource/RenderGlLogic.h
  ../../../GoTvCommon/QtSource/RenderGlMatrix.cpp
  ../../../GoTvCommon/QtSource/RenderGlOffScreenSurface.cpp
  ../../../GoTvCommon/QtSource/RenderGlOffScreenSurface.h
  ../../../GoTvCommon/QtSource/RenderGlShaders.cpp
  ../../../GoTvCommon/QtSource/RenderGlShaders.h
  ../../../GoTvCommon/QtSource/RenderGlWidget.cpp
  ../../../GoTvCommon/QtSource/RenderGlWidget.h
  ../../../GoTvCommon/QtSource/RenderGlWidgetFonts.cpp
  ../../../GoTvCommon/QtSource/RenderGlWidgetRender.cpp
  ../../../GoTvCommon/QtSource/RenderGlWidgetShaderInterface.cpp
  ../../../GoTvCommon/QtSource/RenderGlWidgetTextures.cpp
  ../../../GoTvCommon/QtSource/RenderKodiThread.cpp
  ../../../GoTvCommon/QtSource/RenderKodiThread.h
  ../../../GoTvCommon/QtSource/RenderLogoShaders.cpp
  ../../../GoTvCommon/QtSource/RenderLogoShaders.h
  ../../../GoTvCommon/QtSource/RenderShaderDefsGl.cpp
  ../../../GoTvCommon/QtSource/RenderShaderDefsGles.cpp
  ../../../GoTvCommon/QtSource/RenderShaderQt.cpp
  ../../../GoTvCommon/QtSource/RenderShaderQt.h
  ../../../GoTvCommon/QtSource/SearchParamsWidget.cpp
  ../../../GoTvCommon/QtSource/SearchParamsWidget.h
  ../../../GoTvCommon/QtSource/ServiceSettingsWidget.cpp
  ../../../GoTvCommon/QtSource/ServiceSettingsWidget.h
  ../../../GoTvCommon/QtSource/SessionWidget.cpp
  ../../../GoTvCommon/QtSource/SessionWidget.h
  ../../../GoTvCommon/QtSource/SoundCtrlWidget.cpp
  ../../../GoTvCommon/QtSource/SoundCtrlWidget.h
  ../../../GoTvCommon/QtSource/SoundDefs.h
  ../../../GoTvCommon/QtSource/soundio/AudioDefs.h
  ../../../GoTvCommon/QtSource/soundio/AudioInIo.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioInIo.h
  ../../../GoTvCommon/QtSource/soundio/AudioInThread.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioInThread.h
  ../../../GoTvCommon/QtSource/soundio/AudioIoMgr.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioIoMgr.h
  ../../../GoTvCommon/QtSource/soundio/AudioMixer.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioMixer.h
  ../../../GoTvCommon/QtSource/soundio/AudioOutIo.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioOutIo.h
  ../../../GoTvCommon/QtSource/soundio/AudioOutThread.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioOutThread.h
  ../../../GoTvCommon/QtSource/soundio/AudioTestGenerator.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioTestGenerator.h
  ../../../GoTvCommon/QtSource/soundio/AudioUtils.cpp
  ../../../GoTvCommon/QtSource/soundio/AudioUtils.h
  ../../../GoTvCommon/QtSource/soundio/IAudioInterface.h
  ../../../GoTvCommon/QtSource/StoryWidget.cpp
  ../../../GoTvCommon/QtSource/StoryWidget.h
  ../../../GoTvCommon/QtSource/TextChatLogic.cpp
  ../../../GoTvCommon/QtSource/TextChatLogic.h
  ../../../GoTvCommon/QtSource/ThumbnailChooseWidget.cpp
  ../../../GoTvCommon/QtSource/ThumbnailChooseWidget.h
  ../../../GoTvCommon/QtSource/ThumbnailEditWidget.cpp
  ../../../GoTvCommon/QtSource/ThumbnailEditWidget.h
  ../../../GoTvCommon/QtSource/ThumbnailViewWidget.cpp
  ../../../GoTvCommon/QtSource/ThumbnailViewWidget.h
  ../../../GoTvCommon/QtSource/TitleBarWidget.cpp
  ../../../GoTvCommon/QtSource/TitleBarWidget.h
  ../../../GoTvCommon/QtSource/TodGameDefs.cpp
  ../../../GoTvCommon/QtSource/TodGameDefs.h
  ../../../GoTvCommon/QtSource/TodGameLogic.cpp
  ../../../GoTvCommon/QtSource/TodGameLogic.h
  ../../../GoTvCommon/QtSource/TodGameWidget.cpp
  ../../../GoTvCommon/QtSource/TodGameWidget.h
  ../../../GoTvCommon/QtSource/ToGuiActivityClient.cpp
  ../../../GoTvCommon/QtSource/ToGuiActivityClient.h
  ../../../GoTvCommon/QtSource/ToGuiActivityInterface.h
  ../../../GoTvCommon/QtSource/ToGuiFileXferClient.cpp
  ../../../GoTvCommon/QtSource/ToGuiFileXferClient.h
  ../../../GoTvCommon/QtSource/ToGuiFileXferInterface.h
  ../../../GoTvCommon/QtSource/ToGuiHardwareControlInterface.h
  ../../../GoTvCommon/QtSource/ToGuiHardwareCtrlClient.cpp
  ../../../GoTvCommon/QtSource/ToGuiHardwareCtrlClient.h
  ../../../GoTvCommon/QtSource/ToGuiOfferInterface.h
  ../../../GoTvCommon/QtSource/ToGuiThumbUpdateClient.cpp
  ../../../GoTvCommon/QtSource/ToGuiThumbUpdateClient.h
  ../../../GoTvCommon/QtSource/ToGuiThumbUpdateInterface.h
  ../../../GoTvCommon/QtSource/ToGuiUserUpdateClient.cpp
  ../../../GoTvCommon/QtSource/ToGuiUserUpdateClient.h
  ../../../GoTvCommon/QtSource/ToGuiUserUpdateInterface.h
  ../../../GoTvCommon/QtSource/UserJoinListItem.cpp
  ../../../GoTvCommon/QtSource/UserJoinListItem.h
  ../../../GoTvCommon/QtSource/UserJoinListWidget.cpp
  ../../../GoTvCommon/QtSource/UserJoinListWidget.h
  ../../../GoTvCommon/QtSource/UserListItem.cpp
  ../../../GoTvCommon/QtSource/UserListItem.h
  ../../../GoTvCommon/QtSource/UserListWidget.cpp
  ../../../GoTvCommon/QtSource/UserListWidget.h
  ../../../GoTvCommon/QtSource/UserProfile.h
  ../../../GoTvCommon/QtSource/VidWidget.cpp
  ../../../GoTvCommon/QtSource/VidWidget.h
  ../../../GoTvCommon/QtSource/VxAppDisplay.cpp
  ../../../GoTvCommon/QtSource/VxAppDisplay.h
  ../../../GoTvCommon/QtSource/VxAppStyle.cpp
  ../../../GoTvCommon/QtSource/VxAppStyle.h
  ../../../GoTvCommon/QtSource/VxAppStyle2.cpp
  ../../../GoTvCommon/QtSource/VxAppStyle2.h
  ../../../GoTvCommon/QtSource/VxAppTheme.cpp
  ../../../GoTvCommon/QtSource/VxAppTheme.h
  ../../../GoTvCommon/QtSource/VxAspectFrame.cpp
  ../../../GoTvCommon/QtSource/VxAspectFrame.h
  ../../../GoTvCommon/QtSource/VxAvatarImage.cpp
  ../../../GoTvCommon/QtSource/VxAvatarImage.h
  ../../../GoTvCommon/QtSource/VxComboBox.cpp
  ../../../GoTvCommon/QtSource/VxComboBox.h
  ../../../GoTvCommon/QtSource/VxComboBoxFileFilter.cpp
  ../../../GoTvCommon/QtSource/VxComboBoxFileFilter.h
  ../../../GoTvCommon/QtSource/VxContextMenu.cpp
  ../../../GoTvCommon/QtSource/VxContextMenu.h
  ../../../GoTvCommon/QtSource/VxFlatButton.cpp
  ../../../GoTvCommon/QtSource/VxFlatButton.h
  ../../../GoTvCommon/QtSource/VxFrame.cpp
  ../../../GoTvCommon/QtSource/VxFrame.h
  ../../../GoTvCommon/QtSource/VxLabel.cpp
  ../../../GoTvCommon/QtSource/VxLabel.h
  ../../../GoTvCommon/QtSource/VxMenu.cpp
  ../../../GoTvCommon/QtSource/VxMenu.h
  ../../../GoTvCommon/QtSource/VxMenuButton.cpp
  ../../../GoTvCommon/QtSource/VxMenuButton.h
  ../../../GoTvCommon/QtSource/VxMenuButtonBottom.cpp
  ../../../GoTvCommon/QtSource/VxMenuButtonBottom.h
  ../../../GoTvCommon/QtSource/VxMenuButtonTop.cpp
  ../../../GoTvCommon/QtSource/VxMenuButtonTop.h
  ../../../GoTvCommon/QtSource/VxMyFileInfo.cpp
  ../../../GoTvCommon/QtSource/VxMyFileInfo.h
  ../../../GoTvCommon/QtSource/VxPlainTextEdit.cpp
  ../../../GoTvCommon/QtSource/VxPlainTextEdit.h
  ../../../GoTvCommon/QtSource/VxProgressBar.cpp
  ../../../GoTvCommon/QtSource/VxProgressBar.h
  ../../../GoTvCommon/QtSource/VxPushButton.cpp
  ../../../GoTvCommon/QtSource/VxPushButton.h
  ../../../GoTvCommon/QtSource/VxResourceToRealFile.cpp
  ../../../GoTvCommon/QtSource/VxResourceToRealFile.h
  ../../../GoTvCommon/QtSource/VxScrollArea.cpp
  ../../../GoTvCommon/QtSource/VxScrollArea.h
  ../../../GoTvCommon/QtSource/VxShredderButton.cpp
  ../../../GoTvCommon/QtSource/VxShredderButton.h
  ../../../GoTvCommon/QtSource/VxSndInstance.cpp
  ../../../GoTvCommon/QtSource/VxSndInstance.h
  ../../../GoTvCommon/QtSource/VxSpinProgress.cpp
  ../../../GoTvCommon/QtSource/VxSpinProgress.h
  ../../../GoTvCommon/QtSource/VxTagLabel.cpp
  ../../../GoTvCommon/QtSource/VxTagLabel.h
  ../../../GoTvCommon/QtSource/VxTilePositioner.cpp
  ../../../GoTvCommon/QtSource/VxTilePositioner.h
  ../../../GoTvCommon/QtSource/VxWidgetBase.cpp
  ../../../GoTvCommon/QtSource/VxWidgetBase.h
  ../../../GoTvCommon/Resources/bars_1_internet.png
  ../../../GoTvCommon/Resources/bars_2_test_avail.png
  ../../../GoTvCommon/Resources/bars_3_net_avail.png
  ../../../GoTvCommon/Resources/bars_4_relay_search.png
  ../../../GoTvCommon/Resources/bars_5_onlie_with_relay.png
  ../../../GoTvCommon/Resources/bars_6_direct.png
  ../../../GoTvCommon/Resources/bars_7_hosted_direct.png
  ../../../GoTvCommon/Resources/bars_7_hosted_with_relay.png
  ../../../GoTvCommon/Resources/msg_state_fail_resend.png
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
    F:\NoLimitConnectWindows\AppsSrc\GoTvPtoP\build\VS2017\..\..\..\GoTvCommon\QtSource;
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
  set_property(SOURCE ../../../GoTvCommon/QtSource/AccountDb.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AccountMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/CamLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/MessengerPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/DialogConfirmRemoveMessage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileActionBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileActionMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileItemInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileItemWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileListReplySession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileShareItemWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FriendList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InstMsgWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/MultiSessionState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingFileMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingFileShare.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingFriend.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingNotify.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingOfferToFriend.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingOptions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingScan.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingServers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/P2PThingMedia.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/PopupMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/PopupMenuDelegate.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RasterWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/TextChatLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ChatEntryWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputAllWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputBaseWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputFaceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputPhotoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputTextWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputVideoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InputVoiceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityAddRemoveLibraryFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityAppSetup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityBrowseFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityChooseTestWebsiteUrl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityCreateAccount.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityDownloadItemMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityFileSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityInformation.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityIsPortOpenTest.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityMessageBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityNetworkState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityPermissions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityScanPeopleSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityScanProfiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityScanStoryBoards.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityScanWebCams.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivitySessionOptions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityShowHelp.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivitySoundOptions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityTimedMessage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityViewAudioFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityViewLibraryFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityViewMySharedFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityViewVideoFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ActivityYesNoMsgBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AssetBaseWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AssetFaceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AssetPhotoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AssetTextWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AssetVideoWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AssetVoiceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AcceptCancelWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AvatarBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/BottomBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ImageListEntry.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ImageListRow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ImageListWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ImagePickBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/InformationWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/NetAvailStatusWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/NetworkKeyWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/NetworkTestWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/PermissionListItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/PermissionWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/PlayerGlWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/PluginSettingsWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RelayServiceWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ServiceSettingsWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxAspectFrame.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxComboBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxContextMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/IdentWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxFrame.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxMenuButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxMenuButtonBottom.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxMenuButtonTop.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxPlainTextEdit.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxPushButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxComboBoxFileFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxFlatButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxLabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxMyFileInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxSndInstance.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxResourceToRealFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxScrollArea.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxShredderButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxSpinProgress.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxTagLabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxTilePositioner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/SessionWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/SoundCtrlWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/StoryWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VidWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxWidgetBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/TodGameDefs.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/TodGameLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/TodGameWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPeerBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPeerChangeFriendship.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPeerTodGame.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPeerVideoPhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPeerViewSharedFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPeerVoicePhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletAboutApp.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletApplicationInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletChooseThumbnail.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletClientAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletClientBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletClientRandomConnect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletClientShareFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletClientShareWebCam.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletClientStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletEditAboutMe.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletEditAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletEditStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletGalleryImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletGalleryThumb.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletKodi.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletLibrary.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletLogSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletLogView.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletNetHostingPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletNetworkSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPermissionList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPersonalRecorder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPlayerBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSearchPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceAboutMe.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceBaseSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceChatRoom.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceConnectionTest.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceHostGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceHostNetwork.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceRandomConnect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceRandomConnectRelay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceRelay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceShareFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceShareWebCam.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletServiceStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsAboutMe.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsAvatarImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsChatRoom.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsConnectionTest.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsFileXfer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsHostGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsHostNetwork.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsMessenger.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsRandomConnect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsRandomConnectRelay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsShareFiles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsShareWebCam.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsStoryboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsTruthOrDare.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsVideoPhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSettingsVoicePhone.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletShareServicesPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletSnapshot.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletTheme.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletPlayerVideo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletUserIdentity.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletLaunchWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppletLaunchPage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppCommon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppCommonAudio.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppCommonEvents.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppCommonLogin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppCommonRender.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppDefs.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppGlobals.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppProfile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/AppSetup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/EventsQtToGoTv.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/GuiParams.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxAppStyle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxAppTheme.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/GuiHelpers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/HomeWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/VxAppDisplay.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioInIo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioInThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioIoMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioMixer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioOutIo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioOutThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioTestGenerator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/soundio/AudioUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/MySndMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/NetClientGuiLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/NetHostGuiLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/GuiFileActionSession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/GuiFileXferSession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/GuiOfferSession.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FileXferWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FriendListDelegate.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/FriendListWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/HistoryEntryWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/HistoryListWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/LogMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/LogWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/OfferBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/OfferSessionLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/OfferSessionState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/OfferMgr.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/OfferWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/MyIcons.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/TitleBarWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ThumbnailChooseWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ThumbnailEditWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/ThumbnailViewWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlLogic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlMatrix.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlOffScreenSurface.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlShaders.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlWidget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlWidgetFonts.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlWidgetRender.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlWidgetShaderInterface.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderGlWidgetTextures.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderKodiThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderLogoShaders.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderShaderDefsGl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderShaderDefsGles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"app_precompiled_hdr.h" /Fp"$(ProjectDir)/../../../app_precompiled_hdr.pch">")
  set_property(SOURCE ../../../GoTvCommon/QtSource/RenderShaderQt.cpp
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
