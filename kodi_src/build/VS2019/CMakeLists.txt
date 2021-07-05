cmake_minimum_required(VERSION 3.12.2)

project(libkodi)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libkodi STATIC
  ../../xbmc/xbmc/addons/Addon.h
  ../../xbmc/xbmc/addons/Addon2.cpp
  ../../xbmc/xbmc/addons/AddonBuilder.cpp
  ../../xbmc/xbmc/addons/AddonBuilder.h
  ../../xbmc/xbmc/addons/AddonDatabase.cpp
  ../../xbmc/xbmc/addons/AddonDatabase.h
  ../../xbmc/xbmc/addons/AddonEvents.h
  ../../xbmc/xbmc/addons/AddonInfo.cpp
  ../../xbmc/xbmc/addons/AddonInfo.h
  ../../xbmc/xbmc/addons/AddonInstaller.cpp
  ../../xbmc/xbmc/addons/AddonInstaller.h
  ../../xbmc/xbmc/addons/AddonManager.cpp
  ../../xbmc/xbmc/addons/AddonManager.h
  ../../xbmc/xbmc/addons/AddonProvider.h
  ../../xbmc/xbmc/addons/AddonStatusHandler.cpp
  ../../xbmc/xbmc/addons/AddonStatusHandler.h
  ../../xbmc/xbmc/addons/AddonSystemSettings.cpp
  ../../xbmc/xbmc/addons/AddonSystemSettings.h
  ../../xbmc/xbmc/addons/AddonVersion.cpp
  ../../xbmc/xbmc/addons/AddonVersion.h
  ../../xbmc/xbmc/addons/AudioDecoder.h
  ../../xbmc/xbmc/addons/AudioDecoder2.cpp
  ../../xbmc/xbmc/addons/AudioEncoder.cpp
  ../../xbmc/xbmc/addons/AudioEncoder.h
  ../../xbmc/xbmc/addons/BinaryAddonCache.cpp
  ../../xbmc/xbmc/addons/BinaryAddonCache.h
  ../../xbmc/xbmc/addons/binary-addons/AddonDll.cpp
  ../../xbmc/xbmc/addons/binary-addons/AddonDll.h
  ../../xbmc/xbmc/addons/binary-addons/AddonInstanceHandler.cpp
  ../../xbmc/xbmc/addons/binary-addons/AddonInstanceHandler.h
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonBase.cpp
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonBase.h
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonExtensions.cpp
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonExtensions.h
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonManager.cpp
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonManager.h
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonType.cpp
  ../../xbmc/xbmc/addons/binary-addons/BinaryAddonType.h
  ../../xbmc/xbmc/addons/binary-addons/DllAddon.h
  ../../xbmc/xbmc/addons/ContextMenuAddon.cpp
  ../../xbmc/xbmc/addons/ContextMenuAddon.h
  ../../xbmc/xbmc/addons/ContextMenus.h
  ../../xbmc/xbmc/addons/ContextMenus2.cpp
  ../../xbmc/xbmc/addons/DllLibCPluff.cpp
  ../../xbmc/xbmc/addons/DllLibCPluff.h
  ../../xbmc/xbmc/addons/FilesystemInstaller.cpp
  ../../xbmc/xbmc/addons/FilesystemInstaller.h
  ../../xbmc/xbmc/addons/FontResource.cpp
  ../../xbmc/xbmc/addons/FontResource.h
  ../../xbmc/xbmc/addons/GameResource.cpp
  ../../xbmc/xbmc/addons/GameResource.h
  ../../xbmc/xbmc/addons/GUIDialogAddonInfo.cpp
  ../../xbmc/xbmc/addons/GUIDialogAddonInfo.h
  ../../xbmc/xbmc/addons/GUIViewStateAddonBrowser.cpp
  ../../xbmc/xbmc/addons/GUIViewStateAddonBrowser.h
  ../../xbmc/xbmc/addons/GUIWindowAddonBrowser.cpp
  ../../xbmc/xbmc/addons/GUIWindowAddonBrowser.h
  ../../xbmc/xbmc/addons/IAddon.h
  ../../xbmc/xbmc/addons/ImageDecoder.cpp
  ../../xbmc/xbmc/addons/ImageDecoder.h
  ../../xbmc/xbmc/addons/ImageResource.cpp
  ../../xbmc/xbmc/addons/ImageResource.h
  ../../xbmc/xbmc/addons/interfaces/Addon/AddonCallbacksAddon.cpp
  ../../xbmc/xbmc/addons/interfaces/Addon/AddonCallbacksAddon.h
  ../../xbmc/xbmc/addons/interfaces/AddonInterfaces.cpp
  ../../xbmc/xbmc/addons/interfaces/AddonInterfaces.h
  ../../xbmc/xbmc/addons/interfaces/Filesystem.cpp
  ../../xbmc/xbmc/addons/interfaces/Filesystem.h
  ../../xbmc/xbmc/addons/interfaces/General.cpp
  ../../xbmc/xbmc/addons/interfaces/General.h
  ../../xbmc/xbmc/addons/interfaces/GUI/AddonCallbacksGUI.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/AddonCallbacksGUI.h
  ../../xbmc/xbmc/addons/interfaces/GUI/AddonGUIRenderingControl.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/AddonGUIRenderingControl.h
  ../../xbmc/xbmc/addons/interfaces/GUI/AddonGUIWindow.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/AddonGUIWindow.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Button.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Button.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Edit.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Edit.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/FadeLabel.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/FadeLabel.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Image.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Image.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Label.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Label.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Progress.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Progress3.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/RadioButton.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/RadioButton.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Rendering.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Rendering.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/SettingsSlider.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/SettingsSlider.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Slider.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Slider.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Spin.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/Spin.h
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/TextBox.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/controls/TextBox.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/ContextMenu.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/ContextMenu.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/ExtendedProgressBar.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/ExtendedProgressBar.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/FileBrowser.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/FileBrowser.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Keyboard.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Keyboard2.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Numeric.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Numeric.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/OK.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/OK.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Progress.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Progress2.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Select.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Select.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/TextViewer.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/TextViewer.h
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/YesNo.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/YesNo.h
  ../../xbmc/xbmc/addons/interfaces/GUI/General.h
  ../../xbmc/xbmc/addons/interfaces/GUI/General2.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/ListItem.h
  ../../xbmc/xbmc/addons/interfaces/GUI/ListItem2.cpp
  ../../xbmc/xbmc/addons/interfaces/GUI/Window.h
  ../../xbmc/xbmc/addons/interfaces/GUI/Window2.cpp
  ../../xbmc/xbmc/addons/interfaces/Network.h
  ../../xbmc/xbmc/addons/interfaces/Network2.cpp
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/AddonBase.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/AudioDecoder.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/AudioDSP.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/AudioEncoder.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/ImageDecoder.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/Inputstream.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/Peripheral.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/PeripheralUtils.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/Screensaver.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/VFS.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/VideoCodec.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/addon-instance/Visualization.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/AudioEngine.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/Filesystem.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/General.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Button.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Edit.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/FadeLabel.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Image.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Label.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Progress.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/RadioButton.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Rendering.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/SettingsSlider.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Slider.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/Spin.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/controls/TextBox.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/definitions.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/ContextMenu.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/ExtendedProgress.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/FileBrowser.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/Keyboard.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/Numeric.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/OK.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/Progress.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/Select.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/TextViewer.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/dialogs/YesNo.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/General.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/ListItem.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/gui/Window.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/kodi_game_dll.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/kodi_game_types.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/kodi_vfs_types.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/libKODI_game.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/libKODI_guilib.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/libXBMC_addon.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/libXBMC_pvr.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/Network.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/StreamCodec.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/StreamCrypto.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/tools/DllHelper.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/versions.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/xbmc_addon_dll.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/xbmc_addon_types.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/xbmc_epg_types.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/xbmc_pvr_dll.h
  ../../xbmc/xbmc/addons/kodi-addon-dev-kit/include/kodi/xbmc_pvr_types.h
  ../../xbmc/xbmc/addons/LanguageResource.cpp
  ../../xbmc/xbmc/addons/LanguageResource.h
  ../../xbmc/xbmc/addons/PluginSource.cpp
  ../../xbmc/xbmc/addons/PluginSource.h
  ../../xbmc/xbmc/addons/PVRClient.cpp
  ../../xbmc/xbmc/addons/PVRClient.h
  ../../xbmc/xbmc/addons/PVRClientMenuHooks.cpp
  ../../xbmc/xbmc/addons/PVRClientMenuHooks.h
  ../../xbmc/xbmc/addons/Repository.cpp
  ../../xbmc/xbmc/addons/Repository.h
  ../../xbmc/xbmc/addons/RepositoryUpdater.cpp
  ../../xbmc/xbmc/addons/RepositoryUpdater.h
  ../../xbmc/xbmc/addons/Resource.h
  ../../xbmc/xbmc/addons/Scraper.cpp
  ../../xbmc/xbmc/addons/Scraper.h
  ../../xbmc/xbmc/addons/ScreenSaver.cpp
  ../../xbmc/xbmc/addons/ScreenSaver.h
  ../../xbmc/xbmc/addons/Service.cpp
  ../../xbmc/xbmc/addons/Service.h
  ../../xbmc/xbmc/addons/settings/AddonSettings.cpp
  ../../xbmc/xbmc/addons/settings/AddonSettings.h
  ../../xbmc/xbmc/addons/settings/GUIDialogAddonSettings.cpp
  ../../xbmc/xbmc/addons/settings/GUIDialogAddonSettings.h
  ../../xbmc/xbmc/addons/settings/SettingUrlEncodedString.cpp
  ../../xbmc/xbmc/addons/settings/SettingUrlEncodedString.h
  ../../xbmc/xbmc/addons/Skin.cpp
  ../../xbmc/xbmc/addons/Skin.h
  ../../xbmc/xbmc/addons/UISoundsResource.cpp
  ../../xbmc/xbmc/addons/UISoundsResource.h
  ../../xbmc/xbmc/addons/VFSEntry.cpp
  ../../xbmc/xbmc/addons/VFSEntry.h
  ../../xbmc/xbmc/addons/Visualization.cpp
  ../../xbmc/xbmc/addons/Visualization.h
  ../../xbmc/xbmc/addons/Webinterface.cpp
  ../../xbmc/xbmc/addons/Webinterface.h
  ../../xbmc/xbmc/AppInboundProtocol.cpp
  ../../xbmc/xbmc/AppInboundProtocol.h
  ../../xbmc/xbmc/Application.cpp
  ../../xbmc/xbmc/Application.h
  ../../xbmc/xbmc/ApplicationEvents.cpp
  ../../xbmc/xbmc/ApplicationPlayer.cpp
  ../../xbmc/xbmc/ApplicationPlayer.h
  ../../xbmc/xbmc/ApplicationStackHelper.cpp
  ../../xbmc/xbmc/ApplicationStackHelper.h
  ../../xbmc/xbmc/AppParamParser.cpp
  ../../xbmc/xbmc/AppParamParser.h
  ../../xbmc/xbmc/Autorun.cpp
  ../../xbmc/xbmc/Autorun.h
  ../../xbmc/xbmc/AutoSwitch.cpp
  ../../xbmc/xbmc/AutoSwitch.h
  ../../xbmc/xbmc/BackgroundInfoLoader.cpp
  ../../xbmc/xbmc/BackgroundInfoLoader.h
  ../../xbmc/xbmc/cdrip/CDDARipJob.cpp
  ../../xbmc/xbmc/cdrip/CDDARipJob.h
  ../../xbmc/xbmc/cdrip/CDDARipper.cpp
  ../../xbmc/xbmc/cdrip/CDDARipper.h
  ../../xbmc/xbmc/cdrip/Encoder.cpp
  ../../xbmc/xbmc/cdrip/Encoder.h
  ../../xbmc/xbmc/cdrip/EncoderFFmpeg.cpp
  ../../xbmc/xbmc/cdrip/EncoderFFmpeg.h
  ../../xbmc/xbmc/cdrip/IEncoder.h
  ../../xbmc/xbmc/commons/Buffer.h
  ../../xbmc/xbmc/commons/Exception.cpp
  ../../xbmc/xbmc/commons/Exception.h
  ../../xbmc/xbmc/commons/ilog.cpp
  ../../xbmc/xbmc/commons/ilog.h
  ../../xbmc/xbmc/CompileInfo.cpp
  ../../xbmc/xbmc/CompileInfo.h
  ../../xbmc/xbmc/ContextMenuItem.cpp
  ../../xbmc/xbmc/ContextMenuItem.h
  ../../xbmc/xbmc/ContextMenuManager.cpp
  ../../xbmc/xbmc/ContextMenuManager.h
  ../../xbmc/xbmc/ContextMenus.cpp
  ../../xbmc/xbmc/ContextMenus.h
  ../../xbmc/xbmc/contrib/kissfft/_kiss_fft_guts.h
  ../../xbmc/xbmc/contrib/kissfft/kiss_fft.c
  ../../xbmc/xbmc/contrib/kissfft/kiss_fft.h
  ../../xbmc/xbmc/contrib/kissfft/kiss_fftr.c
  ../../xbmc/xbmc/contrib/kissfft/kiss_fftr.h
  ../../xbmc/xbmc/cores/AudioEngine/AEResampleFactory.cpp
  ../../xbmc/xbmc/cores/AudioEngine/AEResampleFactory.h
  ../../xbmc/xbmc/cores/AudioEngine/AESinkFactory.cpp
  ../../xbmc/xbmc/cores/AudioEngine/AESinkFactory.h
  ../../xbmc/xbmc/cores/AudioEngine/Encoders/AEEncoderFFmpeg.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Encoders/AEEncoderFFmpeg.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAE.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAE.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEBuffer.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEBuffer.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEFilter.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEFilter.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEResampleFFMPEG.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEResampleFFMPEG.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESettings.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESettings.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESink.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESink.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESound.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESound.h
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEStream.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEStream.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/AE.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/AEEncoder.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/AEResample.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/AESink.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/AESound.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/AEStream.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/IAudioCallback.h
  ../../xbmc/xbmc/cores/AudioEngine/Interfaces/ThreadedAE.h
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkDirectSound.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkDirectSound.h
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkQt.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkQt.h
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkWASAPI.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkWASAPI.h
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/windows/AESinkFactoryWin.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/windows/AESinkFactoryWin.h
  ../../xbmc/xbmc/cores/AudioEngine/Sinks/windows/AESinkFactoryWin32.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEAudioFormat.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEBitstreamPacker.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEBitstreamPacker.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEChannelData.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEChannelInfo.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEChannelInfo.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEDeviceInfo.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEDeviceInfo.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AELimiter.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AELimiter.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEPackIEC61937.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEPackIEC61937.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AERingBuffer.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEStreamData.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEStreamInfo.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEStreamInfo.h
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEUtil.cpp
  ../../xbmc/xbmc/cores/AudioEngine/Utils/AEUtil.h
  ../../xbmc/xbmc/cores/DataCacheCore.cpp
  ../../xbmc/xbmc/cores/DataCacheCore.h
  ../../xbmc/xbmc/cores/DllLoader/coff.cpp
  ../../xbmc/xbmc/cores/DllLoader/coff.h
  ../../xbmc/xbmc/cores/DllLoader/coffldr.h
  ../../xbmc/xbmc/cores/DllLoader/dll.cpp
  ../../xbmc/xbmc/cores/DllLoader/dll.h
  ../../xbmc/xbmc/cores/DllLoader/dll_tracker.cpp
  ../../xbmc/xbmc/cores/DllLoader/dll_tracker.h
  ../../xbmc/xbmc/cores/DllLoader/dll_tracker_file.cpp
  ../../xbmc/xbmc/cores/DllLoader/dll_tracker_file.h
  ../../xbmc/xbmc/cores/DllLoader/dll_tracker_library.cpp
  ../../xbmc/xbmc/cores/DllLoader/dll_tracker_library.h
  ../../xbmc/xbmc/cores/DllLoader/dll_util.cpp
  ../../xbmc/xbmc/cores/DllLoader/dll_util.h
  ../../xbmc/xbmc/cores/DllLoader/DllLoader.cpp
  ../../xbmc/xbmc/cores/DllLoader/DllLoader.h
  ../../xbmc/xbmc/cores/DllLoader/DllLoaderContainer.cpp
  ../../xbmc/xbmc/cores/DllLoader/DllLoaderContainer.h
  ../../xbmc/xbmc/cores/DllLoader/exports/emu_dummy.cpp
  ../../xbmc/xbmc/cores/DllLoader/exports/emu_dummy.h
  ../../xbmc/xbmc/cores/DllLoader/exports/emu_msvcrt.cpp
  ../../xbmc/xbmc/cores/DllLoader/exports/emu_msvcrt.h
  ../../xbmc/xbmc/cores/DllLoader/exports/util/EmuFileWrapper.cpp
  ../../xbmc/xbmc/cores/DllLoader/exports/util/EmuFileWrapper.h
  ../../xbmc/xbmc/cores/DllLoader/LibraryLoader.cpp
  ../../xbmc/xbmc/cores/DllLoader/LibraryLoader.h
  ../../xbmc/xbmc/cores/DllLoader/Win32DllLoader.cpp
  ../../xbmc/xbmc/cores/DllLoader/Win32DllLoader.h
  ../../xbmc/xbmc/cores/ExternalPlayer/ExternalPlayer.cpp
  ../../xbmc/xbmc/cores/ExternalPlayer/ExternalPlayer.h
  ../../xbmc/xbmc/cores/FFmpeg.h
  ../../xbmc/xbmc/cores/IPlayer.h
  ../../xbmc/xbmc/cores/IPlayerCallback.h
  ../../xbmc/xbmc/cores/paplayer/AudioDecoder.cpp
  ../../xbmc/xbmc/cores/paplayer/AudioDecoder.h
  ../../xbmc/xbmc/cores/paplayer/CachingCodec.h
  ../../xbmc/xbmc/cores/paplayer/CodecFactory.cpp
  ../../xbmc/xbmc/cores/paplayer/CodecFactory.h
  ../../xbmc/xbmc/cores/paplayer/ICodec.h
  ../../xbmc/xbmc/cores/paplayer/PAPlayer.cpp
  ../../xbmc/xbmc/cores/paplayer/PAPlayer.h
  ../../xbmc/xbmc/cores/paplayer/VideoPlayerCodec.cpp
  ../../xbmc/xbmc/cores/paplayer/VideoPlayerCodec.h
  ../../xbmc/xbmc/cores/playercorefactory/PlayerCoreConfig.h
  ../../xbmc/xbmc/cores/playercorefactory/PlayerCoreFactory.cpp
  ../../xbmc/xbmc/cores/playercorefactory/PlayerCoreFactory.h
  ../../xbmc/xbmc/cores/playercorefactory/PlayerSelectionRule.cpp
  ../../xbmc/xbmc/cores/playercorefactory/PlayerSelectionRule.h
  ../../xbmc/xbmc/cores/RetroPlayer/audio/AudioTranslator.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/audio/AudioTranslator.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/BaseRenderBuffer.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/BaseRenderBuffer.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/BaseRenderBufferPool.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/BaseRenderBufferPool.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/IRenderBuffer.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/IRenderBufferPool.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/RenderBufferManager.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/RenderBufferManager.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/video/RenderBufferGuiTexture.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/video/RenderBufferGuiTexture.h
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/video/RenderBufferSysMem.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/buffers/video/RenderBufferSysMem.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameRenderManager.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameRenderManager.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameSettings.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameSettings.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameSettingsHandle.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameSettingsHandle.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameVideoHandle.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameVideoHandle.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderHandle.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderHandle.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderTarget.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderTarget.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderTargetFactory.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderTargetFactory.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/IGameCallback.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/IGUIRenderSettings.h
  ../../xbmc/xbmc/cores/RetroPlayer/guibridge/IRenderCallback.h
  ../../xbmc/xbmc/cores/RetroPlayer/guicontrols/GUIGameControl.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guicontrols/GUIGameControl.h
  ../../xbmc/xbmc/cores/RetroPlayer/guicontrols/GUIRenderSettings.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guicontrols/GUIRenderSettings.h
  ../../xbmc/xbmc/cores/RetroPlayer/guiplayback/GUIPlaybackControl.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guiplayback/GUIPlaybackControl.h
  ../../xbmc/xbmc/cores/RetroPlayer/guiwindows/GameWindowFullScreen.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guiwindows/GameWindowFullScreen.h
  ../../xbmc/xbmc/cores/RetroPlayer/guiwindows/GameWindowFullScreenText.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/guiwindows/GameWindowFullScreenText.h
  ../../xbmc/xbmc/cores/RetroPlayer/playback/GameLoop.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/playback/GameLoop.h
  ../../xbmc/xbmc/cores/RetroPlayer/playback/IPlayback.h
  ../../xbmc/xbmc/cores/RetroPlayer/playback/IPlaybackControl.h
  ../../xbmc/xbmc/cores/RetroPlayer/playback/RealtimePlayback.h
  ../../xbmc/xbmc/cores/RetroPlayer/playback/ReversiblePlayback.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/playback/ReversiblePlayback.h
  ../../xbmc/xbmc/cores/RetroPlayer/process/qt/RPProcessInfoQt.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/process/qt/RPProcessInfoQt.h
  ../../xbmc/xbmc/cores/RetroPlayer/process/RPProcessInfo.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/process/RPProcessInfo.h
  ../../xbmc/xbmc/cores/RetroPlayer/process/windows/RPProcessInfoWin.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/process/windows/RPProcessInfoWin.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/IRenderManager.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderContext.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderContext.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderSettings.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderSettings.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderTranslator.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderTranslator.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderUtils.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderUtils.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderVideoSettings.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderVideoSettings.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RPRenderManager.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/RPRenderManager.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPBaseRenderer.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPBaseRenderer.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPRendererGuiTexture.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPRendererGuiTexture.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPWinRenderer.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPWinRenderer.h
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoShaders/windows/RPWinOutputShader.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoShaders/windows/RPWinOutputShader.h
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayer.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayer.h
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerAutoSave.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerAutoSave.h
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerInput.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerInput.h
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerTypes.h
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerUtils.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerUtils.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/ISavestate.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateDatabase.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateDatabase.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateDefines.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateFlatBuffer.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateFlatBuffer.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateTranslator.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateTypes.h
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateUtils.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/savestates/SavestateUtils.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/IRetroPlayerStream.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/IStreamManager.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/BasicMemoryStream.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/BasicMemoryStream.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/DeltaPairMemoryStream.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/DeltaPairMemoryStream.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/IMemoryStream.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/LinearMemoryStream.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/memory/LinearMemoryStream.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RetroPlayerAudio.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RetroPlayerAudio.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RetroPlayerStreamTypes.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RetroPlayerStreamTypes.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RetroPlayerVideo.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RetroPlayerVideo.h
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RPStreamManager.cpp
  ../../xbmc/xbmc/cores/RetroPlayer/streams/RPStreamManager.h
  ../../xbmc/xbmc/cores/VideoPlayer/AudioSinkAE.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/AudioSinkAE.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDClock.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDClock.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodec.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecFFmpeg.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecFFmpeg.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecPassthrough.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecPassthrough.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDCodecs.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDCodecUtils.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDCodecUtils.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDFactoryCodec.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDFactoryCodec.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/contrib/cc_decoder.c
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/contrib/cc_decoder.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/contrib/cc_decoder708.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/contrib/cc_decoder708.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlay.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodec.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodec.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecFFmpeg.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecFFmpeg.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecSSA.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecSSA.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecText.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecText.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecTX3G.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecTX3G.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayImage.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlaySpu.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlaySSA.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayText.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/AddonVideoCodec.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/AddonVideoCodec.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodec.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodec.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodecFFmpeg.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodecFFmpeg.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoPPFFmpeg.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoPPFFmpeg.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DXVA.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DXVA.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DemuxMultiSource.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DemuxMultiSource.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemux.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemux.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxBXA.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxBXA.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxCC.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxCC.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxCDDA.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxCDDA.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxClient.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxClient.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxFFmpeg.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxFFmpeg.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxUtils.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxUtils.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxVobsub.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxVobsub.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDFactoryDemuxer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDFactoryDemuxer.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxSPU.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxSPU.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDFileInfo.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDFileInfo.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DllDvdNav.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DllDvdNav.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDFactoryInputStream.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDFactoryInputStream.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStream.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStream.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamBluray.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamBluray.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamFFmpeg.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamFFmpeg.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamFile.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamFile.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamMemory.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamMemory.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamNavigator.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamNavigator.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamStack.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamStack.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDStateSerializer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDStateSerializer.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamAddon.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamAddon.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamMultiSource.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamMultiSource.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamMultiStreams.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamPVRBase.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamPVRBase.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamPVRChannel.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamPVRChannel.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamPVRRecording.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamPVRRecording.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDMessage.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDMessage.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDMessageQueue.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDMessageQueue.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDOverlayContainer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDOverlayContainer.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDResource.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDStreamInfo.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDStreamInfo.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DllLibass.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DllLibass.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDFactorySubtitle.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDFactorySubtitle.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleLineCollection.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleLineCollection.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParser.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserMicroDVD.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserMicroDVD.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserMPL2.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserMPL2.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSami.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSami.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSSA.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSSA.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSubrip.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSubrip.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserVplayer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserVplayer.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitlesLibass.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitlesLibass.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleStream.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleStream.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleTagMicroDVD.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleTagMicroDVD.h
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleTagSami.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleTagSami.h
  ../../xbmc/xbmc/cores/VideoPlayer/Edl.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/Edl.h
  ../../xbmc/xbmc/cores/VideoPlayer/Interface/Addon/DemuxCrypto.h
  ../../xbmc/xbmc/cores/VideoPlayer/Interface/Addon/DemuxPacket.h
  ../../xbmc/xbmc/cores/VideoPlayer/Interface/Addon/TimingConstants.h
  ../../xbmc/xbmc/cores/VideoPlayer/Interface/StreamInfo.h
  ../../xbmc/xbmc/cores/VideoPlayer/IVideoPlayer.h
  ../../xbmc/xbmc/cores/VideoPlayer/Process/ProcessInfo.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/Process/ProcessInfo.h
  ../../xbmc/xbmc/cores/VideoPlayer/Process/VideoBuffer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/Process/VideoBuffer.h
  ../../xbmc/xbmc/cores/VideoPlayer/Process/windows/ProcessInfoWin.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/Process/windows/ProcessInfoWin.h
  ../../xbmc/xbmc/cores/VideoPlayer/PTSTracker.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/PTSTracker.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayer.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerAudio.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerAudio.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerRadioRDS.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerRadioRDS.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerSubtitle.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerSubtitle.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerTeletext.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerTeletext.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerVideo.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerVideo.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoReferenceClock.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoReferenceClock.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/BaseRenderer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/BaseRenderer.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/ColorManager.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/ColorManager.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/DebugRenderer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/DebugRenderer.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/FrameBufferObject.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/FrameBufferObject.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/HwDecRender/DXVAHD.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/HwDecRender/DXVAHD.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRenderer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRenderer.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererDX.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererDX.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererGUI.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererGUI.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererQt.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererQt.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererUtil.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererUtil.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderCapture.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderCapture.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RendererQt.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RendererQt.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderFactory.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderFactory.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderFlags.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderFlags.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderInfo.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderManager.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderManager.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ConversionMatrix.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ConversionMatrix.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ConvolutionKernels.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ConvolutionKernels.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/dither.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ShaderFormats.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/VideoFilterShaderQt.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/VideoFilterShaderQt.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/WinVideoFilter.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/WinVideoFilter.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/YUV2RGBShaderQt.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/YUV2RGBShaderQt.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/YUVMatrix.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/YUVMatrix.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/WinRenderBuffer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/WinRenderBuffer.h
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/WinRenderer.cpp
  ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/WinRenderer.h
  ../../xbmc/xbmc/cores/VideoSettings.cpp
  ../../xbmc/xbmc/cores/VideoSettings.h
  ../../xbmc/xbmc/CueDocument.cpp
  ../../xbmc/xbmc/CueDocument.h
  ../../xbmc/xbmc/DatabaseManager.cpp
  ../../xbmc/xbmc/DatabaseManager.h
  ../../xbmc/xbmc/DbUrl.cpp
  ../../xbmc/xbmc/DbUrl.h
  ../../xbmc/xbmc/dbwrappers/Database.cpp
  ../../xbmc/xbmc/dbwrappers/Database.h
  ../../xbmc/xbmc/dbwrappers/DatabaseQuery.cpp
  ../../xbmc/xbmc/dbwrappers/DatabaseQuery.h
  ../../xbmc/xbmc/dbwrappers/dataset.cpp
  ../../xbmc/xbmc/dbwrappers/dataset.h
  ../../xbmc/xbmc/dbwrappers/mysqldataset.cpp
  ../../xbmc/xbmc/dbwrappers/mysqldataset.h
  ../../xbmc/xbmc/dbwrappers/qry_dat.cpp
  ../../xbmc/xbmc/dbwrappers/qry_dat.h
  ../../xbmc/xbmc/dbwrappers/sqlitedataset.cpp
  ../../xbmc/xbmc/dbwrappers/sqlitedataset.h
  ../../xbmc/xbmc/dialogs/GUIDialogBoxBase.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogBoxBase.h
  ../../xbmc/xbmc/dialogs/GUIDialogBusy.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogBusy.h
  ../../xbmc/xbmc/dialogs/GUIDialogBusyNoCancel.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogBusyNoCancel.h
  ../../xbmc/xbmc/dialogs/GUIDialogButtonMenu.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogButtonMenu.h
  ../../xbmc/xbmc/dialogs/GUIDialogCache.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogCache.h
  ../../xbmc/xbmc/dialogs/GUIDialogContextMenu.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogContextMenu.h
  ../../xbmc/xbmc/dialogs/GUIDialogExtendedProgressBar.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogExtendedProgressBar.h
  ../../xbmc/xbmc/dialogs/GUIDialogFileBrowser.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogFileBrowser.h
  ../../xbmc/xbmc/dialogs/GUIDialogGamepad.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogGamepad.h
  ../../xbmc/xbmc/dialogs/GUIDialogKaiToast.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogKaiToast.h
  ../../xbmc/xbmc/dialogs/GUIDialogKeyboardGeneric.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogKeyboardGeneric.h
  ../../xbmc/xbmc/dialogs/GUIDialogKeyboardTouch.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogKeyboardTouch.h
  ../../xbmc/xbmc/dialogs/GUIDialogMediaFilter.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogMediaFilter.h
  ../../xbmc/xbmc/dialogs/GUIDialogMediaSource.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogMediaSource.h
  ../../xbmc/xbmc/dialogs/GUIDialogNumeric.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogNumeric.h
  ../../xbmc/xbmc/dialogs/GUIDialogOK.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogOK.h
  ../../xbmc/xbmc/dialogs/GUIDialogPlayEject.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogPlayEject.h
  ../../xbmc/xbmc/dialogs/GUIDialogPlayerControls.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogPlayerControls.h
  ../../xbmc/xbmc/dialogs/GUIDialogPlayerProcessInfo.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogPlayerProcessInfo.h
  ../../xbmc/xbmc/dialogs/GUIDialogProgress.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogProgress.h
  ../../xbmc/xbmc/dialogs/GUIDialogSeekBar.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSeekBar.h
  ../../xbmc/xbmc/dialogs/GUIDialogSelect.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSelect.h
  ../../xbmc/xbmc/dialogs/GUIDialogSimpleMenu.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSimpleMenu.h
  ../../xbmc/xbmc/dialogs/GUIDialogSlider.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSlider.h
  ../../xbmc/xbmc/dialogs/GUIDialogSmartPlaylistEditor.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSmartPlaylistEditor.h
  ../../xbmc/xbmc/dialogs/GUIDialogSmartPlaylistRule.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSmartPlaylistRule.h
  ../../xbmc/xbmc/dialogs/GUIDialogSubMenu.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogSubMenu.h
  ../../xbmc/xbmc/dialogs/GUIDialogTextViewer.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogTextViewer.h
  ../../xbmc/xbmc/dialogs/GUIDialogVolumeBar.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogVolumeBar.h
  ../../xbmc/xbmc/dialogs/GUIDialogYesNo.cpp
  ../../xbmc/xbmc/dialogs/GUIDialogYesNo.h
  ../../xbmc/xbmc/dialogs/IGUIVolumeBarCallback.h
  ../../xbmc/xbmc/DllPaths.h
  ../../xbmc/xbmc/DllPaths_win32.h
  ../../xbmc/xbmc/drm/CryptoSession.cpp
  ../../xbmc/xbmc/drm/CryptoSession.h
  ../../xbmc/xbmc/DynamicDll.cpp
  ../../xbmc/xbmc/DynamicDll.h
  ../../xbmc/xbmc/events/AddonEvent.cpp
  ../../xbmc/xbmc/events/AddonEvent.h
  ../../xbmc/xbmc/events/AddonManagementEvent.cpp
  ../../xbmc/xbmc/events/AddonManagementEvent.h
  ../../xbmc/xbmc/events/BaseEvent.cpp
  ../../xbmc/xbmc/events/BaseEvent.h
  ../../xbmc/xbmc/events/EventLog.cpp
  ../../xbmc/xbmc/events/EventLog.h
  ../../xbmc/xbmc/events/EventLogManager.cpp
  ../../xbmc/xbmc/events/EventLogManager.h
  ../../xbmc/xbmc/events/IEvent.h
  ../../xbmc/xbmc/events/MediaLibraryEvent.cpp
  ../../xbmc/xbmc/events/MediaLibraryEvent.h
  ../../xbmc/xbmc/events/NotificationEvent.h
  ../../xbmc/xbmc/events/UniqueEvent.h
  ../../xbmc/xbmc/events/windows/GUIViewStateEventLog.cpp
  ../../xbmc/xbmc/events/windows/GUIViewStateEventLog.h
  ../../xbmc/xbmc/events/windows/GUIWindowEventLog.cpp
  ../../xbmc/xbmc/events/windows/GUIWindowEventLog.h
  ../../xbmc/xbmc/favourites/ContextMenus.h
  ../../xbmc/xbmc/favourites/ContextMenus3.cpp
  ../../xbmc/xbmc/favourites/FavouritesService.cpp
  ../../xbmc/xbmc/favourites/GUIDialogFavourites.cpp
  ../../xbmc/xbmc/favourites/GUIDialogFavourites.h
  ../../xbmc/xbmc/FileItem.cpp
  ../../xbmc/xbmc/FileItem.h
  ../../xbmc/xbmc/FileItemListModification.cpp
  ../../xbmc/xbmc/FileItemListModification.h
  ../../xbmc/xbmc/filesystem/AddonsDirectory.cpp
  ../../xbmc/xbmc/filesystem/AddonsDirectory.h
  ../../xbmc/xbmc/filesystem/AndroidAppDirectory.cpp
  ../../xbmc/xbmc/filesystem/AndroidAppDirectory.h
  ../../xbmc/xbmc/filesystem/AndroidAppFile.cpp
  ../../xbmc/xbmc/filesystem/AndroidAppFile.h
  ../../xbmc/xbmc/filesystem/APKDirectory.cpp
  ../../xbmc/xbmc/filesystem/APKDirectory.h
  ../../xbmc/xbmc/filesystem/APKFile.cpp
  ../../xbmc/xbmc/filesystem/APKFile.h
  ../../xbmc/xbmc/filesystem/AudioBookFileDirectory.cpp
  ../../xbmc/xbmc/filesystem/AudioBookFileDirectory.h
  ../../xbmc/xbmc/filesystem/BlurayCallback.cpp
  ../../xbmc/xbmc/filesystem/BlurayCallback.h
  ../../xbmc/xbmc/filesystem/BlurayDirectory.cpp
  ../../xbmc/xbmc/filesystem/BlurayDirectory.h
  ../../xbmc/xbmc/filesystem/BlurayFile.cpp
  ../../xbmc/xbmc/filesystem/BlurayFile.h
  ../../xbmc/xbmc/filesystem/CacheStrategy.cpp
  ../../xbmc/xbmc/filesystem/CacheStrategy.h
  ../../xbmc/xbmc/filesystem/CDDADirectory.cpp
  ../../xbmc/xbmc/filesystem/CDDADirectory.h
  ../../xbmc/xbmc/filesystem/CDDAFile.cpp
  ../../xbmc/xbmc/filesystem/CDDAFile.h
  ../../xbmc/xbmc/filesystem/CircularCache.cpp
  ../../xbmc/xbmc/filesystem/CircularCache.h
  ../../xbmc/xbmc/filesystem/CurlFile.cpp
  ../../xbmc/xbmc/filesystem/CurlFile.h
  ../../xbmc/xbmc/filesystem/DAVCommon.cpp
  ../../xbmc/xbmc/filesystem/DAVCommon.h
  ../../xbmc/xbmc/filesystem/DAVDirectory.cpp
  ../../xbmc/xbmc/filesystem/DAVDirectory.h
  ../../xbmc/xbmc/filesystem/DAVFile.cpp
  ../../xbmc/xbmc/filesystem/DAVFile.h
  ../../xbmc/xbmc/filesystem/Directorization.h
  ../../xbmc/xbmc/filesystem/Directory.cpp
  ../../xbmc/xbmc/filesystem/Directory.h
  ../../xbmc/xbmc/filesystem/DirectoryCache.cpp
  ../../xbmc/xbmc/filesystem/DirectoryCache.h
  ../../xbmc/xbmc/filesystem/DirectoryFactory.cpp
  ../../xbmc/xbmc/filesystem/DirectoryFactory.h
  ../../xbmc/xbmc/filesystem/DirectoryHistory.cpp
  ../../xbmc/xbmc/filesystem/DirectoryHistory.h
  ../../xbmc/xbmc/filesystem/DllLibCurl.cpp
  ../../xbmc/xbmc/filesystem/DllLibCurl.h
  ../../xbmc/xbmc/filesystem/DllLibNfs.cpp
  ../../xbmc/xbmc/filesystem/DllLibNfs.h
  ../../xbmc/xbmc/filesystem/EventsDirectory.cpp
  ../../xbmc/xbmc/filesystem/EventsDirectory.h
  ../../xbmc/xbmc/filesystem/FavouritesDirectory.cpp
  ../../xbmc/xbmc/filesystem/FavouritesDirectory.h
  ../../xbmc/xbmc/filesystem/File.h
  ../../xbmc/xbmc/filesystem/File2.cpp
  ../../xbmc/xbmc/filesystem/FileCache.cpp
  ../../xbmc/xbmc/filesystem/FileCache.h
  ../../xbmc/xbmc/filesystem/FileDirectoryFactory.cpp
  ../../xbmc/xbmc/filesystem/FileDirectoryFactory.h
  ../../xbmc/xbmc/filesystem/FileFactory.cpp
  ../../xbmc/xbmc/filesystem/FileFactory.h
  ../../xbmc/xbmc/filesystem/FTPDirectory.cpp
  ../../xbmc/xbmc/filesystem/FTPDirectory.h
  ../../xbmc/xbmc/filesystem/FTPParse.cpp
  ../../xbmc/xbmc/filesystem/FTPParse.h
  ../../xbmc/xbmc/filesystem/HTTPDirectory.cpp
  ../../xbmc/xbmc/filesystem/HTTPDirectory.h
  ../../xbmc/xbmc/filesystem/IDirectory.cpp
  ../../xbmc/xbmc/filesystem/IDirectory.h
  ../../xbmc/xbmc/filesystem/IFile.cpp
  ../../xbmc/xbmc/filesystem/IFile.h
  ../../xbmc/xbmc/filesystem/IFileDirectory.h
  ../../xbmc/xbmc/filesystem/IFileTypes.h
  ../../xbmc/xbmc/filesystem/ImageFile.cpp
  ../../xbmc/xbmc/filesystem/ImageFile.h
  ../../xbmc/xbmc/filesystem/iso9660.cpp
  ../../xbmc/xbmc/filesystem/iso9660.h
  ../../xbmc/xbmc/filesystem/ISO9660Directory.cpp
  ../../xbmc/xbmc/filesystem/ISO9660Directory.h
  ../../xbmc/xbmc/filesystem/ISOFile.cpp
  ../../xbmc/xbmc/filesystem/ISOFile.h
  ../../xbmc/xbmc/filesystem/LibraryDirectory.cpp
  ../../xbmc/xbmc/filesystem/LibraryDirectory.h
  ../../xbmc/xbmc/filesystem/MultiPathDirectory.cpp
  ../../xbmc/xbmc/filesystem/MultiPathDirectory.h
  ../../xbmc/xbmc/filesystem/MultiPathFile.cpp
  ../../xbmc/xbmc/filesystem/MultiPathFile.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNode.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNode2.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbum.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbum.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumCompilations.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumCompilations.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumCompilationsSongs.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumCompilationsSongs.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyAdded.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyAdded.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyAddedSong.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyAddedSong.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyPlayed.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyPlayed.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyPlayedSong.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyPlayedSong.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumTop100.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumTop100.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumTop100Song.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumTop100Song.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeArtist.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeArtist.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeGrouped.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeGroupedMusic.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeMusic.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeMusic.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeOverview.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeOverviewMusic.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeRoot.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSingles.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSingles.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSong.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSong.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSongTop100.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSongTop100.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeTop100.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeTop100.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeYearAlbum.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeYearAlbum.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeYearSong.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeYearSong.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/QueryParamsMusic.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/QueryParamsMusic.h
  ../../xbmc/xbmc/filesystem/MusicDatabaseFile.cpp
  ../../xbmc/xbmc/filesystem/MusicDatabaseFile.h
  ../../xbmc/xbmc/filesystem/MusicFileDirectory.cpp
  ../../xbmc/xbmc/filesystem/MusicFileDirectory.h
  ../../xbmc/xbmc/filesystem/MusicSearchDirectory.cpp
  ../../xbmc/xbmc/filesystem/MusicSearchDirectory.h
  ../../xbmc/xbmc/filesystem/NFSDirectory.cpp
  ../../xbmc/xbmc/filesystem/NFSDirectory.h
  ../../xbmc/xbmc/filesystem/NFSFile.cpp
  ../../xbmc/xbmc/filesystem/NFSFile.h
  ../../xbmc/xbmc/filesystem/NptXbmcFile.cpp
  ../../xbmc/xbmc/filesystem/OverrideDirectory.cpp
  ../../xbmc/xbmc/filesystem/OverrideDirectory.h
  ../../xbmc/xbmc/filesystem/OverrideFile.cpp
  ../../xbmc/xbmc/filesystem/OverrideFile.h
  ../../xbmc/xbmc/filesystem/PipeFile.cpp
  ../../xbmc/xbmc/filesystem/PipeFile.h
  ../../xbmc/xbmc/filesystem/PipesManager.cpp
  ../../xbmc/xbmc/filesystem/PipesManager.h
  ../../xbmc/xbmc/filesystem/PlaylistDirectory.cpp
  ../../xbmc/xbmc/filesystem/PlaylistDirectory.h
  ../../xbmc/xbmc/filesystem/PlaylistFileDirectory.cpp
  ../../xbmc/xbmc/filesystem/PlaylistFileDirectory.h
  ../../xbmc/xbmc/filesystem/PluginDirectory.cpp
  ../../xbmc/xbmc/filesystem/PluginDirectory.h
  ../../xbmc/xbmc/filesystem/PVRDirectory.cpp
  ../../xbmc/xbmc/filesystem/PVRDirectory.h
  ../../xbmc/xbmc/filesystem/ResourceDirectory.cpp
  ../../xbmc/xbmc/filesystem/ResourceDirectory.h
  ../../xbmc/xbmc/filesystem/ResourceFile.cpp
  ../../xbmc/xbmc/filesystem/ResourceFile.h
  ../../xbmc/xbmc/filesystem/RSSDirectory.cpp
  ../../xbmc/xbmc/filesystem/RSSDirectory.h
  ../../xbmc/xbmc/filesystem/ShoutcastFile.cpp
  ../../xbmc/xbmc/filesystem/ShoutcastFile.h
  ../../xbmc/xbmc/filesystem/SmartPlaylistDirectory.cpp
  ../../xbmc/xbmc/filesystem/SmartPlaylistDirectory.h
  ../../xbmc/xbmc/filesystem/SMBDirectory.h
  ../../xbmc/xbmc/filesystem/SMBFile.h
  ../../xbmc/xbmc/filesystem/SourcesDirectory.cpp
  ../../xbmc/xbmc/filesystem/SourcesDirectory.h
  ../../xbmc/xbmc/filesystem/SpecialProtocol.cpp
  ../../xbmc/xbmc/filesystem/SpecialProtocol.h
  ../../xbmc/xbmc/filesystem/SpecialProtocolDirectory.cpp
  ../../xbmc/xbmc/filesystem/SpecialProtocolDirectory.h
  ../../xbmc/xbmc/filesystem/SpecialProtocolFile.cpp
  ../../xbmc/xbmc/filesystem/SpecialProtocolFile.h
  ../../xbmc/xbmc/filesystem/StackDirectory.cpp
  ../../xbmc/xbmc/filesystem/StackDirectory.h
  ../../xbmc/xbmc/filesystem/udf25.cpp
  ../../xbmc/xbmc/filesystem/udf25.h
  ../../xbmc/xbmc/filesystem/UDFDirectory.cpp
  ../../xbmc/xbmc/filesystem/UDFDirectory.h
  ../../xbmc/xbmc/filesystem/UDFFile.cpp
  ../../xbmc/xbmc/filesystem/UDFFile.h
  ../../xbmc/xbmc/filesystem/UPnPDirectory.cpp
  ../../xbmc/xbmc/filesystem/UPnPDirectory.h
  ../../xbmc/xbmc/filesystem/UPnPFile.cpp
  ../../xbmc/xbmc/filesystem/UPnPFile.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeEpisodes.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeEpisodes.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeGrouped.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeGroupedVideo.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeInProgressTvShows.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeInProgressTvShows.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeMoviesOverview.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeMoviesOverview.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeMusicVideosOverview.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeMusicVideosOverview.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeOverviewVideo.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeOverviewVideo.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedEpisodes.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedEpisodes.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedMovies.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedMovies.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedMusicVideos.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedMusicVideos.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRoot.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRoot.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeSeasons.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeSeasons.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleMovies.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleMovies.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleMusicVideos.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleMusicVideos.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleTvShows.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleTvShows.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTvShowsOverview.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTvShowsOverview.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeVideo.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeVideo.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/QueryParamsVideo.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/QueryParamsVideo.h
  ../../xbmc/xbmc/filesystem/VideoDatabaseFile.cpp
  ../../xbmc/xbmc/filesystem/VideoDatabaseFile.h
  ../../xbmc/xbmc/filesystem/VirtualDirectory.cpp
  ../../xbmc/xbmc/filesystem/VirtualDirectory.h
  ../../xbmc/xbmc/filesystem/XbtDirectory.cpp
  ../../xbmc/xbmc/filesystem/XbtDirectory.h
  ../../xbmc/xbmc/filesystem/XbtFile.cpp
  ../../xbmc/xbmc/filesystem/XbtFile.h
  ../../xbmc/xbmc/filesystem/XbtManager.cpp
  ../../xbmc/xbmc/filesystem/XbtManager.h
  ../../xbmc/xbmc/filesystem/ZeroconfDirectory.cpp
  ../../xbmc/xbmc/filesystem/ZeroconfDirectory.h
  ../../xbmc/xbmc/filesystem/ZipDirectory.cpp
  ../../xbmc/xbmc/filesystem/ZipDirectory.h
  ../../xbmc/xbmc/filesystem/ZipFile.cpp
  ../../xbmc/xbmc/filesystem/ZipFile.h
  ../../xbmc/xbmc/filesystem/ZipManager.cpp
  ../../xbmc/xbmc/filesystem/ZipManager.h
  ../../xbmc/xbmc/fmt/format.cpp
  ../../xbmc/xbmc/fmt/format.h
  ../../xbmc/xbmc/fmt/locale.h
  ../../xbmc/xbmc/fmt/ostream.cpp
  ../../xbmc/xbmc/fmt/ostream.h
  ../../xbmc/xbmc/fmt/posix.cpp
  ../../xbmc/xbmc/fmt/posix.h
  ../../xbmc/xbmc/fmt/time.h
  ../../xbmc/xbmc/games/addons/GameClient.cpp
  ../../xbmc/xbmc/games/addons/GameClient.h
  ../../xbmc/xbmc/games/addons/GameClientCallbacks.h
  ../../xbmc/xbmc/games/addons/GameClientInGameSaves.cpp
  ../../xbmc/xbmc/games/addons/GameClientInGameSaves.h
  ../../xbmc/xbmc/games/addons/GameClientProperties.cpp
  ../../xbmc/xbmc/games/addons/GameClientProperties.h
  ../../xbmc/xbmc/games/addons/GameClientSubsystem.cpp
  ../../xbmc/xbmc/games/addons/GameClientSubsystem.h
  ../../xbmc/xbmc/games/addons/GameClientTranslator.cpp
  ../../xbmc/xbmc/games/addons/GameClientTranslator.h
  ../../xbmc/xbmc/games/addons/input/GameClientController.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientController.h
  ../../xbmc/xbmc/games/addons/input/GameClientDevice.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientDevice.h
  ../../xbmc/xbmc/games/addons/input/GameClientHardware.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientHardware.h
  ../../xbmc/xbmc/games/addons/input/GameClientInput.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientInput.h
  ../../xbmc/xbmc/games/addons/input/GameClientJoystick.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientJoystick.h
  ../../xbmc/xbmc/games/addons/input/GameClientKeyboard.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientKeyboard.h
  ../../xbmc/xbmc/games/addons/input/GameClientMouse.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientMouse.h
  ../../xbmc/xbmc/games/addons/input/GameClientPort.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientPort.h
  ../../xbmc/xbmc/games/addons/input/GameClientTopology.cpp
  ../../xbmc/xbmc/games/addons/input/GameClientTopology.h
  ../../xbmc/xbmc/games/addons/streams/GameClientStreamAudio.cpp
  ../../xbmc/xbmc/games/addons/streams/GameClientStreamAudio.h
  ../../xbmc/xbmc/games/addons/streams/GameClientStreams.cpp
  ../../xbmc/xbmc/games/addons/streams/GameClientStreams.h
  ../../xbmc/xbmc/games/addons/streams/GameClientStreamSwFramebuffer.cpp
  ../../xbmc/xbmc/games/addons/streams/GameClientStreamSwFramebuffer.h
  ../../xbmc/xbmc/games/addons/streams/GameClientStreamVideo.cpp
  ../../xbmc/xbmc/games/addons/streams/GameClientStreamVideo.h
  ../../xbmc/xbmc/games/addons/streams/IGameClientStream.h
  ../../xbmc/xbmc/games/controllers/Controller.cpp
  ../../xbmc/xbmc/games/controllers/Controller.h
  ../../xbmc/xbmc/games/controllers/ControllerDefinitions.h
  ../../xbmc/xbmc/games/controllers/ControllerFeature.cpp
  ../../xbmc/xbmc/games/controllers/ControllerFeature.h
  ../../xbmc/xbmc/games/controllers/ControllerIDs.h
  ../../xbmc/xbmc/games/controllers/ControllerLayout.cpp
  ../../xbmc/xbmc/games/controllers/ControllerLayout.h
  ../../xbmc/xbmc/games/controllers/ControllerManager.cpp
  ../../xbmc/xbmc/games/controllers/ControllerManager.h
  ../../xbmc/xbmc/games/controllers/ControllerTopology.cpp
  ../../xbmc/xbmc/games/controllers/ControllerTopology.h
  ../../xbmc/xbmc/games/controllers/ControllerTranslator.cpp
  ../../xbmc/xbmc/games/controllers/ControllerTranslator.h
  ../../xbmc/xbmc/games/controllers/ControllerTypes.h
  ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogAxisDetection.cpp
  ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogAxisDetection.h
  ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogButtonCapture.cpp
  ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogButtonCapture.h
  ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogIgnoreInput.cpp
  ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogIgnoreInput.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUICardinalFeatureButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUICardinalFeatureButton.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIControllerButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIControllerButton.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIControlTypes.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureButton.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureControls.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureControls.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureFactory.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureFactory.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureTranslator.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureTranslator.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIGameController.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIGameController.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIScalarFeatureButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIScalarFeatureButton.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUISelectKeyButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUISelectKeyButton.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIThrottleButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIThrottleButton.h
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIWheelButton.cpp
  ../../xbmc/xbmc/games/controllers/guicontrols/GUIWheelButton.h
  ../../xbmc/xbmc/games/controllers/types/ControllerGrid.cpp
  ../../xbmc/xbmc/games/controllers/types/ControllerGrid.h
  ../../xbmc/xbmc/games/controllers/types/ControllerTree.cpp
  ../../xbmc/xbmc/games/controllers/types/ControllerTree.h
  ../../xbmc/xbmc/games/controllers/windows/ControllerInstaller.cpp
  ../../xbmc/xbmc/games/controllers/windows/ControllerInstaller.h
  ../../xbmc/xbmc/games/controllers/windows/GUIConfigurationWizard.cpp
  ../../xbmc/xbmc/games/controllers/windows/GUIConfigurationWizard.h
  ../../xbmc/xbmc/games/controllers/windows/GUIControllerDefines.h
  ../../xbmc/xbmc/games/controllers/windows/GUIControllerList.cpp
  ../../xbmc/xbmc/games/controllers/windows/GUIControllerList.h
  ../../xbmc/xbmc/games/controllers/windows/GUIControllerWindow.cpp
  ../../xbmc/xbmc/games/controllers/windows/GUIControllerWindow.h
  ../../xbmc/xbmc/games/controllers/windows/GUIFeatureList.cpp
  ../../xbmc/xbmc/games/controllers/windows/GUIFeatureList.h
  ../../xbmc/xbmc/games/controllers/windows/IConfigurationWindow.h
  ../../xbmc/xbmc/games/dialogs/GUIDialogSelectGameClient.cpp
  ../../xbmc/xbmc/games/dialogs/GUIDialogSelectGameClient.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameAdvancedSettings.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameAdvancedSettings.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameOSD.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameOSD.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameOSDHelp.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameOSDHelp.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameStretchMode.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameStretchMode.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoFilter.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoFilter.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoRotation.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoRotation.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoSelect.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoSelect.h
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVolume.cpp
  ../../xbmc/xbmc/games/dialogs/osd/DialogGameVolume.h
  ../../xbmc/xbmc/games/GameServices.cpp
  ../../xbmc/xbmc/games/GameServices.h
  ../../xbmc/xbmc/games/GameSettings.h
  ../../xbmc/xbmc/games/GameSettings2.cpp
  ../../xbmc/xbmc/games/GameTypes.h
  ../../xbmc/xbmc/games/GameUtils.cpp
  ../../xbmc/xbmc/games/GameUtils.h
  ../../xbmc/xbmc/games/ports/InputSink.cpp
  ../../xbmc/xbmc/games/ports/InputSink.h
  ../../xbmc/xbmc/games/ports/Port.cpp
  ../../xbmc/xbmc/games/ports/Port.h
  ../../xbmc/xbmc/games/tags/GameInfoTag.cpp
  ../../xbmc/xbmc/games/tags/GameInfoTag.h
  ../../xbmc/xbmc/games/windows/GUIViewStateWindowGames.cpp
  ../../xbmc/xbmc/games/windows/GUIViewStateWindowGames.h
  ../../xbmc/xbmc/games/windows/GUIWindowGames.cpp
  ../../xbmc/xbmc/games/windows/GUIWindowGames.h
  ../../xbmc/xbmc/GoTvCoreUtil.cpp
  ../../xbmc/xbmc/GoTvCoreUtil.h
  ../../xbmc/xbmc/GoTvUrl.cpp
  ../../xbmc/xbmc/GoTvUrl.h
  ../../xbmc/xbmc/guiinfo/GUIInfoLabels.h
  ../../xbmc/xbmc/GUIInfoManager.cpp
  ../../xbmc/xbmc/GUIInfoManager.h
  ../../xbmc/xbmc/GUILargeTextureManager.cpp
  ../../xbmc/xbmc/GUILargeTextureManager.h
  ../../xbmc/xbmc/guilib/D3DResource.cpp
  ../../xbmc/xbmc/guilib/D3DResource.h
  ../../xbmc/xbmc/guilib/DDSImage.cpp
  ../../xbmc/xbmc/guilib/DDSImage.h
  ../../xbmc/xbmc/guilib/DirectXGraphics.cpp
  ../../xbmc/xbmc/guilib/DirectXGraphics.h
  ../../xbmc/xbmc/guilib/DirtyRegion.h
  ../../xbmc/xbmc/guilib/DirtyRegionSolvers.cpp
  ../../xbmc/xbmc/guilib/DirtyRegionSolvers.h
  ../../xbmc/xbmc/guilib/DirtyRegionTracker.cpp
  ../../xbmc/xbmc/guilib/DirtyRegionTracker.h
  ../../xbmc/xbmc/guilib/DispResource.h
  ../../xbmc/xbmc/guilib/FFmpegImage.cpp
  ../../xbmc/xbmc/guilib/FFmpegImage.h
  ../../xbmc/xbmc/guilib/gui3d.h
  ../../xbmc/xbmc/guilib/GUIAction.cpp
  ../../xbmc/xbmc/guilib/GUIAction.h
  ../../xbmc/xbmc/guilib/GUIAudioManager.cpp
  ../../xbmc/xbmc/guilib/GUIAudioManager.h
  ../../xbmc/xbmc/guilib/GUIBaseContainer.cpp
  ../../xbmc/xbmc/guilib/GUIBaseContainer.h
  ../../xbmc/xbmc/guilib/GUIBorderedImage.cpp
  ../../xbmc/xbmc/guilib/GUIBorderedImage.h
  ../../xbmc/xbmc/guilib/GUIButtonControl.cpp
  ../../xbmc/xbmc/guilib/GUIButtonControl.h
  ../../xbmc/xbmc/guilib/GUICallback.h
  ../../xbmc/xbmc/guilib/GUIColorManager.cpp
  ../../xbmc/xbmc/guilib/GUIColorManager.h
  ../../xbmc/xbmc/guilib/GUIComponent.cpp
  ../../xbmc/xbmc/guilib/GUIComponent.h
  ../../xbmc/xbmc/guilib/GUIControl.cpp
  ../../xbmc/xbmc/guilib/GUIControl.h
  ../../xbmc/xbmc/guilib/GUIControlFactory.cpp
  ../../xbmc/xbmc/guilib/GUIControlFactory.h
  ../../xbmc/xbmc/guilib/GUIControlGroup.cpp
  ../../xbmc/xbmc/guilib/GUIControlGroup.h
  ../../xbmc/xbmc/guilib/GUIControlGroupList.cpp
  ../../xbmc/xbmc/guilib/GUIControlGroupList.h
  ../../xbmc/xbmc/guilib/GUIControlLookup.cpp
  ../../xbmc/xbmc/guilib/GUIControlLookup.h
  ../../xbmc/xbmc/guilib/GUIControlProfiler.cpp
  ../../xbmc/xbmc/guilib/GUIControlProfiler.h
  ../../xbmc/xbmc/guilib/GUIDialog.cpp
  ../../xbmc/xbmc/guilib/GUIDialog.h
  ../../xbmc/xbmc/guilib/GUIEditControl.cpp
  ../../xbmc/xbmc/guilib/GUIEditControl.h
  ../../xbmc/xbmc/guilib/GUIFadeLabelControl.cpp
  ../../xbmc/xbmc/guilib/GUIFadeLabelControl.h
  ../../xbmc/xbmc/guilib/GUIFixedListContainer.cpp
  ../../xbmc/xbmc/guilib/GUIFixedListContainer.h
  ../../xbmc/xbmc/guilib/GUIFont.cpp
  ../../xbmc/xbmc/guilib/GUIFont.h
  ../../xbmc/xbmc/guilib/GUIFontCache.cpp
  ../../xbmc/xbmc/guilib/GUIFontCache.h
  ../../xbmc/xbmc/guilib/GUIFontManager.cpp
  ../../xbmc/xbmc/guilib/GUIFontManager.h
  ../../xbmc/xbmc/guilib/GUIFontTTF.cpp
  ../../xbmc/xbmc/guilib/GUIFontTTF.h
  ../../xbmc/xbmc/guilib/GUIFontTTFDX.cpp
  ../../xbmc/xbmc/guilib/GUIFontTTFDX.h
  ../../xbmc/xbmc/guilib/GUIFontTTFGL.cpp
  ../../xbmc/xbmc/guilib/GUIFontTTFGL.h
  ../../xbmc/xbmc/guilib/GUIFontTTFQt.cpp
  ../../xbmc/xbmc/guilib/GUIFontTTFQt.h
  ../../xbmc/xbmc/guilib/GUIImage.cpp
  ../../xbmc/xbmc/guilib/GUIImage.h
  ../../xbmc/xbmc/guilib/GUIIncludes.cpp
  ../../xbmc/xbmc/guilib/GUIIncludes.h
  ../../xbmc/xbmc/guilib/guiinfo/AddonsGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/AddonsGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/GamesGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GamesGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIControlsGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIControlsGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoBool.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoBool.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoColor.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoColor.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoHelper.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoHelper.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoLabel.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoLabel.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoLabels.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoProvider.h
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoProviders.cpp
  ../../xbmc/xbmc/guilib/guiinfo/GUIInfoProviders.h
  ../../xbmc/xbmc/guilib/guiinfo/IGUIInfoProvider.h
  ../../xbmc/xbmc/guilib/guiinfo/LibraryGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/LibraryGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/MusicGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/MusicGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/PicturesGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/PicturesGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/PlayerGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/PlayerGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/SkinGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/SkinGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/SystemGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/SystemGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/VideoGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/VideoGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/VisualisationGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/VisualisationGUIInfo.h
  ../../xbmc/xbmc/guilib/guiinfo/WeatherGUIInfo.cpp
  ../../xbmc/xbmc/guilib/guiinfo/WeatherGUIInfo.h
  ../../xbmc/xbmc/guilib/GUIKeyboard.h
  ../../xbmc/xbmc/guilib/GUIKeyboardFactory.cpp
  ../../xbmc/xbmc/guilib/GUIKeyboardFactory.h
  ../../xbmc/xbmc/guilib/GUILabel.cpp
  ../../xbmc/xbmc/guilib/GUILabel.h
  ../../xbmc/xbmc/guilib/GUILabelControl.cpp
  ../../xbmc/xbmc/guilib/GUILabelControl.h
  ../../xbmc/xbmc/guilib/GUIListContainer.cpp
  ../../xbmc/xbmc/guilib/GUIListContainer.h
  ../../xbmc/xbmc/guilib/GUIListGroup.cpp
  ../../xbmc/xbmc/guilib/GUIListGroup.h
  ../../xbmc/xbmc/guilib/GUIListItem.cpp
  ../../xbmc/xbmc/guilib/GUIListItem.h
  ../../xbmc/xbmc/guilib/GUIListItemLayout.cpp
  ../../xbmc/xbmc/guilib/GUIListItemLayout.h
  ../../xbmc/xbmc/guilib/GUIListLabel.cpp
  ../../xbmc/xbmc/guilib/GUIListLabel.h
  ../../xbmc/xbmc/guilib/GUIMessage.cpp
  ../../xbmc/xbmc/guilib/GUIMessage.h
  ../../xbmc/xbmc/guilib/GUIMoverControl.cpp
  ../../xbmc/xbmc/guilib/GUIMoverControl.h
  ../../xbmc/xbmc/guilib/GUIMultiImage.cpp
  ../../xbmc/xbmc/guilib/GUIMultiImage.h
  ../../xbmc/xbmc/guilib/GUIPanelContainer.cpp
  ../../xbmc/xbmc/guilib/GUIPanelContainer.h
  ../../xbmc/xbmc/guilib/GUIProgressControl.cpp
  ../../xbmc/xbmc/guilib/GUIProgressControl.h
  ../../xbmc/xbmc/guilib/GUIRadioButtonControl.cpp
  ../../xbmc/xbmc/guilib/GUIRadioButtonControl.h
  ../../xbmc/xbmc/guilib/GUIRenderingControl.cpp
  ../../xbmc/xbmc/guilib/GUIRenderingControl.h
  ../../xbmc/xbmc/guilib/GUIResizeControl.cpp
  ../../xbmc/xbmc/guilib/GUIResizeControl.h
  ../../xbmc/xbmc/guilib/GUIRSSControl.cpp
  ../../xbmc/xbmc/guilib/GUIRSSControl.h
  ../../xbmc/xbmc/guilib/GUIScrollBarControl.cpp
  ../../xbmc/xbmc/guilib/GUIScrollBarControl.h
  ../../xbmc/xbmc/guilib/GUISettingsSliderControl.cpp
  ../../xbmc/xbmc/guilib/GUISettingsSliderControl.h
  ../../xbmc/xbmc/guilib/GUIShaderDX.cpp
  ../../xbmc/xbmc/guilib/GUIShaderDX.h
  ../../xbmc/xbmc/guilib/GUISliderControl.cpp
  ../../xbmc/xbmc/guilib/GUISliderControl.h
  ../../xbmc/xbmc/guilib/GUISpinControl.cpp
  ../../xbmc/xbmc/guilib/GUISpinControl.h
  ../../xbmc/xbmc/guilib/GUISpinControlEx.cpp
  ../../xbmc/xbmc/guilib/GUISpinControlEx.h
  ../../xbmc/xbmc/guilib/GUIStaticItem.cpp
  ../../xbmc/xbmc/guilib/GUIStaticItem.h
  ../../xbmc/xbmc/guilib/GUITextBox.cpp
  ../../xbmc/xbmc/guilib/GUITextBox.h
  ../../xbmc/xbmc/guilib/GUITextLayout.cpp
  ../../xbmc/xbmc/guilib/GUITextLayout.h
  ../../xbmc/xbmc/guilib/GUITexture.cpp
  ../../xbmc/xbmc/guilib/GUITexture.h
  ../../xbmc/xbmc/guilib/GUITextureD3D.cpp
  ../../xbmc/xbmc/guilib/GUITextureD3D.h
  ../../xbmc/xbmc/guilib/GUITextureGL.cpp
  ../../xbmc/xbmc/guilib/GUITextureGL.h
  ../../xbmc/xbmc/guilib/GUITextureGLES.cpp
  ../../xbmc/xbmc/guilib/GUITextureGLES.h
  ../../xbmc/xbmc/guilib/GUITextureQt.cpp
  ../../xbmc/xbmc/guilib/GUITextureQt.h
  ../../xbmc/xbmc/guilib/GUIToggleButtonControl.cpp
  ../../xbmc/xbmc/guilib/GUIToggleButtonControl.h
  ../../xbmc/xbmc/guilib/GUIVideoControl.cpp
  ../../xbmc/xbmc/guilib/GUIVideoControl.h
  ../../xbmc/xbmc/guilib/GUIVisualisationControl.cpp
  ../../xbmc/xbmc/guilib/GUIVisualisationControl.h
  ../../xbmc/xbmc/guilib/GUIWindow.cpp
  ../../xbmc/xbmc/guilib/GUIWindow.h
  ../../xbmc/xbmc/guilib/GUIWindowManager.cpp
  ../../xbmc/xbmc/guilib/GUIWindowManager.h
  ../../xbmc/xbmc/guilib/GUIWrappingListContainer.cpp
  ../../xbmc/xbmc/guilib/GUIWrappingListContainer.h
  ../../xbmc/xbmc/guilib/IAudioDeviceChangedCallback.h
  ../../xbmc/xbmc/guilib/IDirtyRegionSolver.h
  ../../xbmc/xbmc/guilib/IGUIContainer.h
  ../../xbmc/xbmc/guilib/iimage.h
  ../../xbmc/xbmc/guilib/imagefactory.cpp
  ../../xbmc/xbmc/guilib/imagefactory.h
  ../../xbmc/xbmc/guilib/IMsgTargetCallback.h
  ../../xbmc/xbmc/guilib/IRenderingCallback.h
  ../../xbmc/xbmc/guilib/ISliderCallback.h
  ../../xbmc/xbmc/guilib/IWindowManagerCallback.cpp
  ../../xbmc/xbmc/guilib/IWindowManagerCallback.h
  ../../xbmc/xbmc/guilib/LocalizeStrings.cpp
  ../../xbmc/xbmc/guilib/LocalizeStrings.h
  ../../xbmc/xbmc/guilib/Shader.cpp
  ../../xbmc/xbmc/guilib/Shader.h
  ../../xbmc/xbmc/guilib/StereoscopicsManager.cpp
  ../../xbmc/xbmc/guilib/StereoscopicsManager.h
  ../../xbmc/xbmc/guilib/Texture.cpp
  ../../xbmc/xbmc/guilib/Texture.h
  ../../xbmc/xbmc/guilib/TextureBundle.cpp
  ../../xbmc/xbmc/guilib/TextureBundle.h
  ../../xbmc/xbmc/guilib/TextureBundleXBT.cpp
  ../../xbmc/xbmc/guilib/TextureBundleXBT.h
  ../../xbmc/xbmc/guilib/TextureDX.cpp
  ../../xbmc/xbmc/guilib/TextureDX.h
  ../../xbmc/xbmc/guilib/TextureFormats.h
  ../../xbmc/xbmc/guilib/TextureGL.cpp
  ../../xbmc/xbmc/guilib/TextureGL.h
  ../../xbmc/xbmc/guilib/TextureManager.cpp
  ../../xbmc/xbmc/guilib/TextureManager.h
  ../../xbmc/xbmc/guilib/TexturePi.cpp
  ../../xbmc/xbmc/guilib/TexturePi.h
  ../../xbmc/xbmc/guilib/TextureQt.cpp
  ../../xbmc/xbmc/guilib/TextureQt.h
  ../../xbmc/xbmc/guilib/TransformMatrix.h
  ../../xbmc/xbmc/guilib/Tween.h
  ../../xbmc/xbmc/guilib/VisibleEffect.cpp
  ../../xbmc/xbmc/guilib/VisibleEffect.h
  ../../xbmc/xbmc/guilib/WindowIDs.h
  ../../xbmc/xbmc/guilib/XBTF.cpp
  ../../xbmc/xbmc/guilib/XBTF.h
  ../../xbmc/xbmc/guilib/XBTFReader.cpp
  ../../xbmc/xbmc/guilib/XBTFReader.h
  ../../xbmc/xbmc/GUIPassword.cpp
  ../../xbmc/xbmc/GUIPassword.h
  ../../xbmc/xbmc/GUIUserMessages.h
  ../../xbmc/xbmc/IFileItemListModifier.h
  ../../xbmc/xbmc/InfoScanner.cpp
  ../../xbmc/xbmc/InfoScanner.h
  ../../xbmc/xbmc/input/actions/Action.cpp
  ../../xbmc/xbmc/input/actions/Action.h
  ../../xbmc/xbmc/input/actions/ActionIDs.h
  ../../xbmc/xbmc/input/actions/ActionTranslator.cpp
  ../../xbmc/xbmc/input/actions/ActionTranslator.h
  ../../xbmc/xbmc/input/AppTranslator.cpp
  ../../xbmc/xbmc/input/AppTranslator.h
  ../../xbmc/xbmc/input/ButtonTranslator.cpp
  ../../xbmc/xbmc/input/ButtonTranslator.h
  ../../xbmc/xbmc/input/CustomControllerTranslator.cpp
  ../../xbmc/xbmc/input/CustomControllerTranslator.h
  ../../xbmc/xbmc/input/GamepadTranslator.cpp
  ../../xbmc/xbmc/input/GamepadTranslator.h
  ../../xbmc/xbmc/input/hardware/IHardwareInput.h
  ../../xbmc/xbmc/input/IButtonMapper.h
  ../../xbmc/xbmc/input/IKeymap.h
  ../../xbmc/xbmc/input/IKeymapEnvironment.h
  ../../xbmc/xbmc/input/InertialScrollingHandler.cpp
  ../../xbmc/xbmc/input/InertialScrollingHandler.h
  ../../xbmc/xbmc/input/InputCodingTable.h
  ../../xbmc/xbmc/input/InputCodingTableBaiduPY.cpp
  ../../xbmc/xbmc/input/InputCodingTableBaiduPY.h
  ../../xbmc/xbmc/input/InputCodingTableBasePY.cpp
  ../../xbmc/xbmc/input/InputCodingTableBasePY.h
  ../../xbmc/xbmc/input/InputCodingTableFactory.cpp
  ../../xbmc/xbmc/input/InputCodingTableFactory.h
  ../../xbmc/xbmc/input/InputCodingTableKorean.cpp
  ../../xbmc/xbmc/input/InputCodingTableKorean.h
  ../../xbmc/xbmc/input/InputManager.cpp
  ../../xbmc/xbmc/input/InputManager.h
  ../../xbmc/xbmc/input/InputTranslator.cpp
  ../../xbmc/xbmc/input/InputTranslator.h
  ../../xbmc/xbmc/input/InputTypes.h
  ../../xbmc/xbmc/input/IRTranslator.cpp
  ../../xbmc/xbmc/input/IRTranslator.h
  ../../xbmc/xbmc/input/JoystickMapper.cpp
  ../../xbmc/xbmc/input/JoystickMapper.h
  ../../xbmc/xbmc/input/joysticks/DeadzoneFilter.cpp
  ../../xbmc/xbmc/input/joysticks/DeadzoneFilter.h
  ../../xbmc/xbmc/input/joysticks/dialogs/GUIDialogNewJoystick.cpp
  ../../xbmc/xbmc/input/joysticks/dialogs/GUIDialogNewJoystick.h
  ../../xbmc/xbmc/input/joysticks/DriverPrimitive.cpp
  ../../xbmc/xbmc/input/joysticks/DriverPrimitive.h
  ../../xbmc/xbmc/input/joysticks/generic/ButtonMapping.cpp
  ../../xbmc/xbmc/input/joysticks/generic/ButtonMapping.h
  ../../xbmc/xbmc/input/joysticks/generic/DriverReceiving.cpp
  ../../xbmc/xbmc/input/joysticks/generic/DriverReceiving.h
  ../../xbmc/xbmc/input/joysticks/generic/FeatureHandling.cpp
  ../../xbmc/xbmc/input/joysticks/generic/FeatureHandling.h
  ../../xbmc/xbmc/input/joysticks/generic/InputHandling.cpp
  ../../xbmc/xbmc/input/joysticks/generic/InputHandling.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IButtonMap.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IButtonMapCallback.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IButtonMapper.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IButtonSequence.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IDriverHandler.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IDriverReceiver.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IInputHandler.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IInputProvider.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IInputReceiver.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IKeyHandler.h
  ../../xbmc/xbmc/input/joysticks/interfaces/IKeymapHandler.h
  ../../xbmc/xbmc/input/joysticks/JoystickEasterEgg.cpp
  ../../xbmc/xbmc/input/joysticks/JoystickEasterEgg.h
  ../../xbmc/xbmc/input/joysticks/JoystickIDs.h
  ../../xbmc/xbmc/input/joysticks/JoystickMonitor.cpp
  ../../xbmc/xbmc/input/joysticks/JoystickMonitor.h
  ../../xbmc/xbmc/input/joysticks/JoystickTranslator.cpp
  ../../xbmc/xbmc/input/joysticks/JoystickTranslator.h
  ../../xbmc/xbmc/input/joysticks/JoystickTypes.h
  ../../xbmc/xbmc/input/joysticks/JoystickUtils.cpp
  ../../xbmc/xbmc/input/joysticks/JoystickUtils.h
  ../../xbmc/xbmc/input/joysticks/keymaps/KeyHandler.cpp
  ../../xbmc/xbmc/input/joysticks/keymaps/KeyHandler.h
  ../../xbmc/xbmc/input/joysticks/keymaps/KeymapHandler.cpp
  ../../xbmc/xbmc/input/joysticks/keymaps/KeymapHandler.h
  ../../xbmc/xbmc/input/joysticks/keymaps/KeymapHandling.cpp
  ../../xbmc/xbmc/input/joysticks/keymaps/KeymapHandling.h
  ../../xbmc/xbmc/input/joysticks/RumbleGenerator.cpp
  ../../xbmc/xbmc/input/joysticks/RumbleGenerator.h
  ../../xbmc/xbmc/input/Key.cpp
  ../../xbmc/xbmc/input/Key.h
  ../../xbmc/xbmc/input/keyboard/generic/KeyboardInputHandling.cpp
  ../../xbmc/xbmc/input/keyboard/generic/KeyboardInputHandling.h
  ../../xbmc/xbmc/input/keyboard/interfaces/IActionMap.h
  ../../xbmc/xbmc/input/keyboard/interfaces/IKeyboardDriverHandler.h
  ../../xbmc/xbmc/input/keyboard/interfaces/IKeyboardInputHandler.h
  ../../xbmc/xbmc/input/keyboard/interfaces/IKeyboardInputProvider.h
  ../../xbmc/xbmc/input/keyboard/KeyboardEasterEgg.cpp
  ../../xbmc/xbmc/input/keyboard/KeyboardEasterEgg.h
  ../../xbmc/xbmc/input/keyboard/KeyboardTypes.h
  ../../xbmc/xbmc/input/keyboard/KeymapActionMap.cpp
  ../../xbmc/xbmc/input/keyboard/KeymapActionMap.h
  ../../xbmc/xbmc/input/KeyboardLayout.cpp
  ../../xbmc/xbmc/input/KeyboardLayout.h
  ../../xbmc/xbmc/input/KeyboardLayoutManager.cpp
  ../../xbmc/xbmc/input/KeyboardLayoutManager.h
  ../../xbmc/xbmc/input/KeyboardStat.cpp
  ../../xbmc/xbmc/input/KeyboardStat.h
  ../../xbmc/xbmc/input/KeyboardTranslator.cpp
  ../../xbmc/xbmc/input/KeyboardTranslator.h
  ../../xbmc/xbmc/input/Keymap.cpp
  ../../xbmc/xbmc/input/Keymap.h
  ../../xbmc/xbmc/input/KeymapEnvironment.cpp
  ../../xbmc/xbmc/input/KeymapEnvironment.h
  ../../xbmc/xbmc/input/mouse/generic/MouseInputHandling.cpp
  ../../xbmc/xbmc/input/mouse/generic/MouseInputHandling.h
  ../../xbmc/xbmc/input/mouse/interfaces/IMouseDriverHandler.h
  ../../xbmc/xbmc/input/mouse/interfaces/IMouseInputHandler.h
  ../../xbmc/xbmc/input/mouse/interfaces/IMouseInputProvider.h
  ../../xbmc/xbmc/input/mouse/MouseStat.cpp
  ../../xbmc/xbmc/input/mouse/MouseStat.h
  ../../xbmc/xbmc/input/mouse/MouseTranslator.cpp
  ../../xbmc/xbmc/input/mouse/MouseTranslator.h
  ../../xbmc/xbmc/input/mouse/MouseTypes.h
  ../../xbmc/xbmc/input/remote/IRemoteControl.h
  ../../xbmc/xbmc/input/remote/IRRemote.h
  ../../xbmc/xbmc/input/touch/generic/GenericTouchActionHandler.cpp
  ../../xbmc/xbmc/input/touch/generic/GenericTouchActionHandler.h
  ../../xbmc/xbmc/input/touch/generic/GenericTouchInputHandler.cpp
  ../../xbmc/xbmc/input/touch/generic/GenericTouchInputHandler.h
  ../../xbmc/xbmc/input/touch/generic/GenericTouchPinchDetector.cpp
  ../../xbmc/xbmc/input/touch/generic/GenericTouchPinchDetector.h
  ../../xbmc/xbmc/input/touch/generic/GenericTouchRotateDetector.cpp
  ../../xbmc/xbmc/input/touch/generic/GenericTouchRotateDetector.h
  ../../xbmc/xbmc/input/touch/generic/GenericTouchSwipeDetector.cpp
  ../../xbmc/xbmc/input/touch/generic/GenericTouchSwipeDetector.h
  ../../xbmc/xbmc/input/touch/generic/IGenericTouchGestureDetector.h
  ../../xbmc/xbmc/input/touch/ITouchActionHandler.h
  ../../xbmc/xbmc/input/touch/ITouchInputHandler.h
  ../../xbmc/xbmc/input/touch/ITouchInputHandling.cpp
  ../../xbmc/xbmc/input/touch/ITouchInputHandling.h
  ../../xbmc/xbmc/input/touch/TouchTypes.h
  ../../xbmc/xbmc/input/TouchTranslator.cpp
  ../../xbmc/xbmc/input/TouchTranslator.h
  ../../xbmc/xbmc/input/WindowKeymap.cpp
  ../../xbmc/xbmc/input/WindowKeymap.h
  ../../xbmc/xbmc/input/WindowTranslator.cpp
  ../../xbmc/xbmc/input/WindowTranslator.h
  ../../xbmc/xbmc/input/XBMC_keyboard.h
  ../../xbmc/xbmc/input/XBMC_keysym.h
  ../../xbmc/xbmc/input/XBMC_keytable.cpp
  ../../xbmc/xbmc/input/XBMC_keytable.h
  ../../xbmc/xbmc/input/XBMC_vkeys.h
  ../../xbmc/xbmc/interfaces/AnnouncementManager.h
  ../../xbmc/xbmc/interfaces/AnnouncementManager3.cpp
  ../../xbmc/xbmc/interfaces/builtins/AddonBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/AddonBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/AndroidBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/ApplicationBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/ApplicationBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/Builtins.cpp
  ../../xbmc/xbmc/interfaces/builtins/Builtins.h
  ../../xbmc/xbmc/interfaces/builtins/CECBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/CECBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/GUIBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/GUIBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/GUIContainerBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/GUIContainerBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/GUIControlBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/GUIControlBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/LibraryBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/LibraryBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/OpticalBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/OpticalBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/PictureBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/PictureBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/PlayerBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/PlayerBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/ProfileBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/ProfileBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/PVRBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/PVRBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/SkinBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/SkinBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/SystemBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/SystemBuiltins.h
  ../../xbmc/xbmc/interfaces/builtins/WeatherBuiltins.cpp
  ../../xbmc/xbmc/interfaces/builtins/WeatherBuiltins.h
  ../../xbmc/xbmc/interfaces/generic/ILanguageInvocationHandler.h
  ../../xbmc/xbmc/interfaces/generic/ILanguageInvoker.cpp
  ../../xbmc/xbmc/interfaces/generic/ILanguageInvoker.h
  ../../xbmc/xbmc/interfaces/generic/LanguageInvokerThread.cpp
  ../../xbmc/xbmc/interfaces/generic/LanguageInvokerThread.h
  ../../xbmc/xbmc/interfaces/generic/ScriptInvocationManager.cpp
  ../../xbmc/xbmc/interfaces/generic/ScriptInvocationManager.h
  ../../xbmc/xbmc/interfaces/IActionListener.h
  ../../xbmc/xbmc/interfaces/IAnnouncer.h
  ../../xbmc/xbmc/interfaces/info/InfoBool.cpp
  ../../xbmc/xbmc/interfaces/info/InfoBool.h
  ../../xbmc/xbmc/interfaces/info/InfoExpression.cpp
  ../../xbmc/xbmc/interfaces/info/InfoExpression.h
  ../../xbmc/xbmc/interfaces/info/SkinVariable.cpp
  ../../xbmc/xbmc/interfaces/info/SkinVariable.h
  ../../xbmc/xbmc/interfaces/json-rpc/AddonsOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/AddonsOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/ApplicationOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/ApplicationOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/AudioLibrary.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/AudioLibrary.h
  ../../xbmc/xbmc/interfaces/json-rpc/FavouritesOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/FavouritesOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/FileItemHandler.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/FileItemHandler.h
  ../../xbmc/xbmc/interfaces/json-rpc/FileOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/FileOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/GUIOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/GUIOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/IClient.h
  ../../xbmc/xbmc/interfaces/json-rpc/IJSONRPCAnnouncer.h
  ../../xbmc/xbmc/interfaces/json-rpc/InputOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/InputOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/ITransportLayer.h
  ../../xbmc/xbmc/interfaces/json-rpc/JSONRPC.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/JSONRPC.h
  ../../xbmc/xbmc/interfaces/json-rpc/JSONRPCUtils.h
  ../../xbmc/xbmc/interfaces/json-rpc/JSONServiceDescription.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/JSONServiceDescription.h
  ../../xbmc/xbmc/interfaces/json-rpc/JSONUtils.h
  ../../xbmc/xbmc/interfaces/json-rpc/PlayerOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/PlayerOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/PlaylistOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/PlaylistOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/ProfilesOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/ProfilesOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/PVROperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/PVROperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/SettingsOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/SettingsOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/SystemOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/SystemOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/TextureOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/TextureOperations.h
  ../../xbmc/xbmc/interfaces/json-rpc/VideoLibrary.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/VideoLibrary.h
  ../../xbmc/xbmc/interfaces/json-rpc/XBMCOperations.cpp
  ../../xbmc/xbmc/interfaces/json-rpc/XBMCOperations.h
  ../../xbmc/xbmc/interfaces/legacy/Addon.h
  ../../xbmc/xbmc/interfaces/legacy/Addon3.cpp
  ../../xbmc/xbmc/interfaces/legacy/AddonCallback.h
  ../../xbmc/xbmc/interfaces/legacy/AddonCallback3.cpp
  ../../xbmc/xbmc/interfaces/legacy/AddonClass.h
  ../../xbmc/xbmc/interfaces/legacy/AddonClass3.cpp
  ../../xbmc/xbmc/interfaces/legacy/AddonString.h
  ../../xbmc/xbmc/interfaces/legacy/AddonUtil3s.cpp
  ../../xbmc/xbmc/interfaces/legacy/AddonUtils.h
  ../../xbmc/xbmc/interfaces/legacy/Alternative.h
  ../../xbmc/xbmc/interfaces/legacy/aojsonrpc.h
  ../../xbmc/xbmc/interfaces/legacy/CallbackFunction.h
  ../../xbmc/xbmc/interfaces/legacy/CallbackFunction3.cpp
  ../../xbmc/xbmc/interfaces/legacy/CallbackHandler.h
  ../../xbmc/xbmc/interfaces/legacy/CallbackHandler3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Control.h
  ../../xbmc/xbmc/interfaces/legacy/Control3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Dialog.h
  ../../xbmc/xbmc/interfaces/legacy/Dialog3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Dictionary.h
  ../../xbmc/xbmc/interfaces/legacy/DrmCryptoSession.h
  ../../xbmc/xbmc/interfaces/legacy/DrmCryptoSession3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Exception.h
  ../../xbmc/xbmc/interfaces/legacy/File.h
  ../../xbmc/xbmc/interfaces/legacy/File3.cpp
  ../../xbmc/xbmc/interfaces/legacy/InfoTagMusic.h
  ../../xbmc/xbmc/interfaces/legacy/InfoTagMusic3.cpp
  ../../xbmc/xbmc/interfaces/legacy/InfoTagRadioRDS.h
  ../../xbmc/xbmc/interfaces/legacy/InfoTagRadioRDS3.cpp
  ../../xbmc/xbmc/interfaces/legacy/InfoTagVideo.h
  ../../xbmc/xbmc/interfaces/legacy/InfoTagVideo3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Keyboard.h
  ../../xbmc/xbmc/interfaces/legacy/Keyboard3.cpp
  ../../xbmc/xbmc/interfaces/legacy/LanguageHook.h
  ../../xbmc/xbmc/interfaces/legacy/LanguageHook3.cpp
  ../../xbmc/xbmc/interfaces/legacy/List.h
  ../../xbmc/xbmc/interfaces/legacy/ListItem.h
  ../../xbmc/xbmc/interfaces/legacy/ListItem3.cpp
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmc.h
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmc3.cpp
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmcgui.h
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmcgui3.cpp
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmcplugin.h
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmcplugin3.cpp
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmcvfs.h
  ../../xbmc/xbmc/interfaces/legacy/ModuleXbmcvfs3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Monitor.h
  ../../xbmc/xbmc/interfaces/legacy/Monitor3.cpp
  ../../xbmc/xbmc/interfaces/legacy/Player.h
  ../../xbmc/xbmc/interfaces/legacy/Player3.cpp
  ../../xbmc/xbmc/interfaces/legacy/PlayList.h
  ../../xbmc/xbmc/interfaces/legacy/PlayList3.cpp
  ../../xbmc/xbmc/interfaces/legacy/RenderCapture.h
  ../../xbmc/xbmc/interfaces/legacy/Stat.h
  ../../xbmc/xbmc/interfaces/legacy/String3.cpp
  ../../xbmc/xbmc/interfaces/legacy/swighelper.h
  ../../xbmc/xbmc/interfaces/legacy/Tuple.h
  ../../xbmc/xbmc/interfaces/legacy/Window.h
  ../../xbmc/xbmc/interfaces/legacy/Window3.cpp
  ../../xbmc/xbmc/interfaces/legacy/WindowDialog.h
  ../../xbmc/xbmc/interfaces/legacy/WindowDialog3.cpp
  ../../xbmc/xbmc/interfaces/legacy/WindowDialogMixin.h
  ../../xbmc/xbmc/interfaces/legacy/WindowDialogMixin3.cpp
  ../../xbmc/xbmc/interfaces/legacy/WindowException.h
  ../../xbmc/xbmc/interfaces/legacy/WindowInterceptor.h
  ../../xbmc/xbmc/interfaces/legacy/WindowXML.h
  ../../xbmc/xbmc/interfaces/legacy/WindowXML3.cpp
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiErrorStream.cpp
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiErrorStream.h
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiInputStream.cpp
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiInputStream.h
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiResponse.cpp
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiResponse.h
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiResponseBody.cpp
  ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiResponseBody.h
  ../../xbmc/xbmc/interfaces/python/AddonPythonInvoker.cpp
  ../../xbmc/xbmc/interfaces/python/AddonPythonInvoker.h
  ../../xbmc/xbmc/interfaces/python/CallbackHandler.cpp
  ../../xbmc/xbmc/interfaces/python/CallbackHandler.h
  ../../xbmc/xbmc/interfaces/python/ContextItemAddonInvoker.cpp
  ../../xbmc/xbmc/interfaces/python/ContextItemAddonInvoker.h
  ../../xbmc/xbmc/interfaces/python/generated/AddonModuleXbmcwsgi.cpp
  ../../xbmc/xbmc/interfaces/python/LanguageHook.cpp
  ../../xbmc/xbmc/interfaces/python/LanguageHook.h
  ../../xbmc/xbmc/interfaces/python/preamble.h
  ../../xbmc/xbmc/interfaces/python/PyContext.cpp
  ../../xbmc/xbmc/interfaces/python/PyContext.h
  ../../xbmc/xbmc/interfaces/python/PythonInvoker.cpp
  ../../xbmc/xbmc/interfaces/python/PythonInvoker.h
  ../../xbmc/xbmc/interfaces/python/pythreadstate.h
  ../../xbmc/xbmc/interfaces/python/swig.cpp
  ../../xbmc/xbmc/interfaces/python/swig.h
  ../../xbmc/xbmc/interfaces/python/XBPython.cpp
  ../../xbmc/xbmc/interfaces/python/XBPython.h
  ../../xbmc/xbmc/IProgressCallback.h
  ../../xbmc/xbmc/LangInfoKodi.cpp
  ../../xbmc/xbmc/LangInfoKodi.h
  ../../xbmc/xbmc/listproviders/DirectoryProvider.cpp
  ../../xbmc/xbmc/listproviders/DirectoryProvider.h
  ../../xbmc/xbmc/listproviders/IListProvider.cpp
  ../../xbmc/xbmc/listproviders/IListProvider.h
  ../../xbmc/xbmc/listproviders/MultiProvider.cpp
  ../../xbmc/xbmc/listproviders/MultiProvider.h
  ../../xbmc/xbmc/listproviders/StaticProvider.cpp
  ../../xbmc/xbmc/listproviders/StaticProvider.h
  ../../xbmc/xbmc/media/MediaType.cpp
  ../../xbmc/xbmc/media/MediaType.h
  ../../xbmc/xbmc/MediaSource.cpp
  ../../xbmc/xbmc/MediaSource.h
  ../../xbmc/xbmc/messaging/ApplicationMessenger.cpp
  ../../xbmc/xbmc/messaging/ApplicationMessenger.h
  ../../xbmc/xbmc/messaging/helpers/DialogHelper.cpp
  ../../xbmc/xbmc/messaging/helpers/DialogHelper.h
  ../../xbmc/xbmc/messaging/helpers/DialogOKHelper.cpp
  ../../xbmc/xbmc/messaging/helpers/DialogOKHelper.h
  ../../xbmc/xbmc/messaging/IMessageTarget.h
  ../../xbmc/xbmc/messaging/ThreadMessage.h
  ../../xbmc/xbmc/music/Album.cpp
  ../../xbmc/xbmc/music/Album.h
  ../../xbmc/xbmc/music/Artist.cpp
  ../../xbmc/xbmc/music/Artist.h
  ../../xbmc/xbmc/music/ContextMenus.h
  ../../xbmc/xbmc/music/ContextMenus4.cpp
  ../../xbmc/xbmc/music/dialogs/GUIDialogInfoProviderSettings.cpp
  ../../xbmc/xbmc/music/dialogs/GUIDialogInfoProviderSettings.h
  ../../xbmc/xbmc/music/dialogs/GUIDialogMusicInfo.cpp
  ../../xbmc/xbmc/music/dialogs/GUIDialogMusicInfo.h
  ../../xbmc/xbmc/music/dialogs/GUIDialogMusicOSD.cpp
  ../../xbmc/xbmc/music/dialogs/GUIDialogMusicOSD.h
  ../../xbmc/xbmc/music/dialogs/GUIDialogSongInfo.cpp
  ../../xbmc/xbmc/music/dialogs/GUIDialogSongInfo.h
  ../../xbmc/xbmc/music/dialogs/GUIDialogVisualisationPresetList.cpp
  ../../xbmc/xbmc/music/dialogs/GUIDialogVisualisationPresetList.h
  ../../xbmc/xbmc/music/GUIViewStateMusic.cpp
  ../../xbmc/xbmc/music/GUIViewStateMusic.h
  ../../xbmc/xbmc/music/infoscanner/MusicAlbumInfo.cpp
  ../../xbmc/xbmc/music/infoscanner/MusicAlbumInfo.h
  ../../xbmc/xbmc/music/infoscanner/MusicArtistInfo.cpp
  ../../xbmc/xbmc/music/infoscanner/MusicArtistInfo.h
  ../../xbmc/xbmc/music/infoscanner/MusicInfoScanner.cpp
  ../../xbmc/xbmc/music/infoscanner/MusicInfoScanner.h
  ../../xbmc/xbmc/music/infoscanner/MusicInfoScraper.cpp
  ../../xbmc/xbmc/music/infoscanner/MusicInfoScraper.h
  ../../xbmc/xbmc/music/jobs/MusicLibraryCleaningJob.cpp
  ../../xbmc/xbmc/music/jobs/MusicLibraryCleaningJob.h
  ../../xbmc/xbmc/music/jobs/MusicLibraryExportJob.cpp
  ../../xbmc/xbmc/music/jobs/MusicLibraryExportJob.h
  ../../xbmc/xbmc/music/jobs/MusicLibraryJob.cpp
  ../../xbmc/xbmc/music/jobs/MusicLibraryJob.h
  ../../xbmc/xbmc/music/jobs/MusicLibraryProgressJob.cpp
  ../../xbmc/xbmc/music/jobs/MusicLibraryProgressJob.h
  ../../xbmc/xbmc/music/jobs/MusicLibraryScanningJob.cpp
  ../../xbmc/xbmc/music/jobs/MusicLibraryScanningJob.h
  ../../xbmc/xbmc/music/MusicDatabase.cpp
  ../../xbmc/xbmc/music/MusicDatabase.h
  ../../xbmc/xbmc/music/MusicDbUrl.cpp
  ../../xbmc/xbmc/music/MusicDbUrl.h
  ../../xbmc/xbmc/music/MusicInfoLoader.cpp
  ../../xbmc/xbmc/music/MusicInfoLoader.h
  ../../xbmc/xbmc/music/MusicLibraryQueue.cpp
  ../../xbmc/xbmc/music/MusicLibraryQueue.h
  ../../xbmc/xbmc/music/MusicThumbLoader.cpp
  ../../xbmc/xbmc/music/MusicThumbLoader.h
  ../../xbmc/xbmc/music/MusicUtils.cpp
  ../../xbmc/xbmc/music/MusicUtils.h
  ../../xbmc/xbmc/music/Song.cpp
  ../../xbmc/xbmc/music/Song.h
  ../../xbmc/xbmc/music/tags/ImusicInfoTagLoader.h
  ../../xbmc/xbmc/music/tags/MusicInfoTag.cpp
  ../../xbmc/xbmc/music/tags/MusicInfoTag.h
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderCDDA.cpp
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderCDDA.h
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderDatabase.cpp
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderDatabase.h
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderFactory.cpp
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderFactory.h
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderFFmpeg.cpp
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderFFmpeg.h
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderShn.cpp
  ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderShn.h
  ../../xbmc/xbmc/music/tags/ReplayGain.cpp
  ../../xbmc/xbmc/music/tags/ReplayGain.h
  ../../xbmc/xbmc/music/tags/TagLibVFSStream.cpp
  ../../xbmc/xbmc/music/tags/TagLibVFSStream.h
  ../../xbmc/xbmc/music/tags/TagLoaderTagLib.cpp
  ../../xbmc/xbmc/music/tags/TagLoaderTagLib.h
  ../../xbmc/xbmc/music/windows/GUIWindowMusicBase.cpp
  ../../xbmc/xbmc/music/windows/GUIWindowMusicBase.h
  ../../xbmc/xbmc/music/windows/GUIWindowMusicNav.cpp
  ../../xbmc/xbmc/music/windows/GUIWindowMusicNav.h
  ../../xbmc/xbmc/music/windows/GUIWindowMusicPlaylist.cpp
  ../../xbmc/xbmc/music/windows/GUIWindowMusicPlaylist.h
  ../../xbmc/xbmc/music/windows/GUIWindowMusicPlaylistEditor.cpp
  ../../xbmc/xbmc/music/windows/GUIWindowMusicPlaylistEditor.h
  ../../xbmc/xbmc/music/windows/GUIWindowVisualisation.cpp
  ../../xbmc/xbmc/music/windows/GUIWindowVisualisation.h
  ../../xbmc/xbmc/music/windows/MusicFileItemListModifier.cpp
  ../../xbmc/xbmc/music/windows/MusicFileItemListModifier.h
  ../../xbmc/xbmc/network/AirPlayServer.cpp
  ../../xbmc/xbmc/network/AirPlayServer.h
  ../../xbmc/xbmc/network/AirTunesServer.cpp
  ../../xbmc/xbmc/network/AirTunesServer.h
  ../../xbmc/xbmc/network/cddb.cpp
  ../../xbmc/xbmc/network/cddb.h
  ../../xbmc/xbmc/network/dacp/dacp.cpp
  ../../xbmc/xbmc/network/dacp/dacp.h
  ../../xbmc/xbmc/network/DllLibPlist.cpp
  ../../xbmc/xbmc/network/DllLibPlist.h
  ../../xbmc/xbmc/network/DllLibShairplay.cpp
  ../../xbmc/xbmc/network/DllLibShairplay.h
  ../../xbmc/xbmc/network/DNSNameCache.cpp
  ../../xbmc/xbmc/network/DNSNameCache.h
  ../../xbmc/xbmc/network/EventClient.cpp
  ../../xbmc/xbmc/network/EventClient.h
  ../../xbmc/xbmc/network/EventPacket.cpp
  ../../xbmc/xbmc/network/EventPacket.h
  ../../xbmc/xbmc/network/EventServer.cpp
  ../../xbmc/xbmc/network/EventServer.h
  ../../xbmc/xbmc/network/GUIDialogAccessPoints.cpp
  ../../xbmc/xbmc/network/GUIDialogAccessPoints.h
  ../../xbmc/xbmc/network/GUIDialogNetworkSetup.cpp
  ../../xbmc/xbmc/network/GUIDialogNetworkSetup.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPFileHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPFileHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPImageHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPImageHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPImageTransformationHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPImageTransformationHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPJsonRpcHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPJsonRpcHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPPythonHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPPythonHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPRequestHandlerUtils.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPRequestHandlerUtils.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPVfsHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPVfsHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPWebinterfaceAddonsHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPWebinterfaceAddonsHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/HTTPWebinterfaceHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/HTTPWebinterfaceHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/IHTTPRequestHandler.cpp
  ../../xbmc/xbmc/network/httprequesthandler/IHTTPRequestHandler.h
  ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonInvoker.cpp
  ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonInvoker.h
  ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonRequest.h
  ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonWsgiInvoker.cpp
  ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonWsgiInvoker.h
  ../../xbmc/xbmc/network/mdns/ZeroconfBrowserMDNS.cpp
  ../../xbmc/xbmc/network/mdns/ZeroconfBrowserMDNS.h
  ../../xbmc/xbmc/network/mdns/ZeroconfMDNS.cpp
  ../../xbmc/xbmc/network/mdns/ZeroconfMDNS.h
  ../../xbmc/xbmc/network/Network.cpp
  ../../xbmc/xbmc/network/Network.h
  ../../xbmc/xbmc/network/NetworkServices.cpp
  ../../xbmc/xbmc/network/NetworkServices.h
  ../../xbmc/xbmc/network/Socket.cpp
  ../../xbmc/xbmc/network/Socket.h
  ../../xbmc/xbmc/network/TCPServer.cpp
  ../../xbmc/xbmc/network/TCPServer.h
  ../../xbmc/xbmc/network/UdpClient.cpp
  ../../xbmc/xbmc/network/UdpClient.h
  ../../xbmc/xbmc/network/upnp/UPnP.cpp
  ../../xbmc/xbmc/network/upnp/UPnP.h
  ../../xbmc/xbmc/network/upnp/UPnPInternal.cpp
  ../../xbmc/xbmc/network/upnp/UPnPInternal.h
  ../../xbmc/xbmc/network/upnp/UPnPPlayer.cpp
  ../../xbmc/xbmc/network/upnp/UPnPPlayer.h
  ../../xbmc/xbmc/network/upnp/UPnPRenderer.cpp
  ../../xbmc/xbmc/network/upnp/UPnPRenderer.h
  ../../xbmc/xbmc/network/upnp/UPnPServer.cpp
  ../../xbmc/xbmc/network/upnp/UPnPServer.h
  ../../xbmc/xbmc/network/upnp/UPnPSettings.cpp
  ../../xbmc/xbmc/network/upnp/UPnPSettings.h
  ../../xbmc/xbmc/network/WakeOnAccess.cpp
  ../../xbmc/xbmc/network/WakeOnAccess.h
  ../../xbmc/xbmc/network/WebServer.cpp
  ../../xbmc/xbmc/network/WebServer.h
  ../../xbmc/xbmc/network/websocket/sha1.hpp
  ../../xbmc/xbmc/network/websocket/WebSocket.cpp
  ../../xbmc/xbmc/network/websocket/WebSocket.h
  ../../xbmc/xbmc/network/websocket/WebSocketManager.cpp
  ../../xbmc/xbmc/network/websocket/WebSocketManager.h
  ../../xbmc/xbmc/network/websocket/WebSocketV13.cpp
  ../../xbmc/xbmc/network/websocket/WebSocketV13.h
  ../../xbmc/xbmc/network/websocket/WebSocketV8.cpp
  ../../xbmc/xbmc/network/websocket/WebSocketV8.h
  ../../xbmc/xbmc/network/Zeroconf.cpp
  ../../xbmc/xbmc/network/Zeroconf.h
  ../../xbmc/xbmc/network/ZeroconfBrowser.cpp
  ../../xbmc/xbmc/network/ZeroconfBrowser.h
  ../../xbmc/xbmc/NfoFile.cpp
  ../../xbmc/xbmc/NfoFile.h
  ../../xbmc/xbmc/PartyModeManager.cpp
  ../../xbmc/xbmc/PartyModeManager.h
  ../../xbmc/xbmc/PasswordManager.cpp
  ../../xbmc/xbmc/PasswordManager.h
  ../../xbmc/xbmc/peripherals/addons/AddonButtonMap.cpp
  ../../xbmc/xbmc/peripherals/addons/AddonButtonMap.h
  ../../xbmc/xbmc/peripherals/addons/AddonButtonMapping.cpp
  ../../xbmc/xbmc/peripherals/addons/AddonButtonMapping.h
  ../../xbmc/xbmc/peripherals/addons/AddonInputHandling.cpp
  ../../xbmc/xbmc/peripherals/addons/AddonInputHandling.h
  ../../xbmc/xbmc/peripherals/addons/PeripheralAddon.cpp
  ../../xbmc/xbmc/peripherals/addons/PeripheralAddon.h
  ../../xbmc/xbmc/peripherals/addons/PeripheralAddonTranslator.cpp
  ../../xbmc/xbmc/peripherals/addons/PeripheralAddonTranslator.h
  ../../xbmc/xbmc/peripherals/bus/PeripheralBus.cpp
  ../../xbmc/xbmc/peripherals/bus/PeripheralBus.h
  ../../xbmc/xbmc/peripherals/bus/PeripheralBusUSB.h
  ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusAddon.cpp
  ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusAddon.h
  ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusApplication.cpp
  ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusApplication.h
  ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusCEC.cpp
  ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusCEC.h
  ../../xbmc/xbmc/peripherals/devices/Peripheral.cpp
  ../../xbmc/xbmc/peripherals/devices/Peripheral.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralBluetooth.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralBluetooth.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralCecAdapter.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralCecAdapter.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralDisk.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralDisk.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralHID.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralHID.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralImon.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralImon.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralJoystick.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralJoystick.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralKeyboard.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralKeyboard.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralMouse.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralMouse.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralNIC.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralNIC.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralNyxboard.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralNyxboard.h
  ../../xbmc/xbmc/peripherals/devices/PeripheralTuner.cpp
  ../../xbmc/xbmc/peripherals/devices/PeripheralTuner.h
  ../../xbmc/xbmc/peripherals/dialogs/GUIDialogPeripherals.cpp
  ../../xbmc/xbmc/peripherals/dialogs/GUIDialogPeripherals.h
  ../../xbmc/xbmc/peripherals/dialogs/GUIDialogPeripheralSettings.cpp
  ../../xbmc/xbmc/peripherals/dialogs/GUIDialogPeripheralSettings.h
  ../../xbmc/xbmc/peripherals/EventLockHandle.cpp
  ../../xbmc/xbmc/peripherals/EventLockHandle.h
  ../../xbmc/xbmc/peripherals/EventPollHandle.cpp
  ../../xbmc/xbmc/peripherals/EventPollHandle.h
  ../../xbmc/xbmc/peripherals/EventScanner.cpp
  ../../xbmc/xbmc/peripherals/EventScanner.h
  ../../xbmc/xbmc/peripherals/IEventScannerCallback.h
  ../../xbmc/xbmc/peripherals/Peripherals.cpp
  ../../xbmc/xbmc/peripherals/Peripherals.h
  ../../xbmc/xbmc/peripherals/PeripheralTypes.h
  ../../xbmc/xbmc/pictures/ExifParse.cpp
  ../../xbmc/xbmc/pictures/GUIDialogPictureInfo.cpp
  ../../xbmc/xbmc/pictures/GUIDialogPictureInfo.h
  ../../xbmc/xbmc/pictures/GUIViewStatePictures.cpp
  ../../xbmc/xbmc/pictures/GUIViewStatePictures.h
  ../../xbmc/xbmc/pictures/GUIWindowPictures.cpp
  ../../xbmc/xbmc/pictures/GUIWindowPictures.h
  ../../xbmc/xbmc/pictures/GUIWindowSlideShow.cpp
  ../../xbmc/xbmc/pictures/GUIWindowSlideShow.h
  ../../xbmc/xbmc/pictures/IptcParse.cpp
  ../../xbmc/xbmc/pictures/JpegParse.cpp
  ../../xbmc/xbmc/pictures/libexif.cpp
  ../../xbmc/xbmc/pictures/Picture.cpp
  ../../xbmc/xbmc/pictures/Picture.h
  ../../xbmc/xbmc/pictures/PictureInfoLoader.cpp
  ../../xbmc/xbmc/pictures/PictureInfoLoader.h
  ../../xbmc/xbmc/pictures/PictureInfoTag.cpp
  ../../xbmc/xbmc/pictures/PictureInfoTag.h
  ../../xbmc/xbmc/pictures/PictureScalingAlgorithm.cpp
  ../../xbmc/xbmc/pictures/PictureScalingAlgorithm.h
  ../../xbmc/xbmc/pictures/PictureThumbLoader.cpp
  ../../xbmc/xbmc/pictures/PictureThumbLoader.h
  ../../xbmc/xbmc/pictures/SlideShowPicture.cpp
  ../../xbmc/xbmc/pictures/SlideShowPicture.h
  ../../xbmc/xbmc/platform/Environment.cpp
  ../../xbmc/xbmc/platform/Environment.h
  ../../xbmc/xbmc/platform/Filesystem.h
  ../../xbmc/xbmc/platform/MessagePrinter.h
  ../../xbmc/xbmc/platform/Platform.cpp
  ../../xbmc/xbmc/platform/Platform.h
  ../../xbmc/xbmc/platform/qt/KodiQtApp.cpp
  ../../xbmc/xbmc/platform/qt/KodiQtApp.h
  ../../xbmc/xbmc/platform/win32/CharsetConverter.h
  ../../xbmc/xbmc/platform/win32/CharsetConverterWin32.cpp
  ../../xbmc/xbmc/platform/win32/crts_caller.cpp
  ../../xbmc/xbmc/platform/win32/crts_caller.h
  ../../xbmc/xbmc/platform/win32/dirent.h
  ../../xbmc/xbmc/platform/win32/dxerr.cpp
  ../../xbmc/xbmc/platform/win32/dxerr.h
  ../../xbmc/xbmc/platform/win32/EnvironmentWin32.cpp
  ../../xbmc/xbmc/platform/win32/filesystem/Win32Directory.cpp
  ../../xbmc/xbmc/platform/win32/filesystem/Win32Directory.h
  ../../xbmc/xbmc/platform/win32/filesystem/Win32File.cpp
  ../../xbmc/xbmc/platform/win32/filesystem/Win32File.h
  ../../xbmc/xbmc/platform/win32/filesystem/Win32SMBDirectory.cpp
  ../../xbmc/xbmc/platform/win32/filesystem/Win32SMBDirectory.h
  ../../xbmc/xbmc/platform/win32/filesystem/Win32SMBFile.cpp
  ../../xbmc/xbmc/platform/win32/filesystem/Win32SMBFile.h
  ../../xbmc/xbmc/platform/win32/FilesystemWin32.cpp
  ../../xbmc/xbmc/platform/win32/IMMNotificationClient.h
  ../../xbmc/xbmc/platform/win32/input/IRServerSuite.cpp
  ../../xbmc/xbmc/platform/win32/input/IRServerSuite.h
  ../../xbmc/xbmc/platform/win32/input/IrssMessage.cpp
  ../../xbmc/xbmc/platform/win32/input/IrssMessage.h
  ../../xbmc/xbmc/platform/win32/MessagePrinterWin32.cpp
  ../../xbmc/xbmc/platform/win32/my_ntddcdrm.h
  ../../xbmc/xbmc/platform/win32/my_ntddscsi.h
  ../../xbmc/xbmc/platform/win32/netdb.h
  ../../xbmc/xbmc/platform/win32/network/NetworkWin32.cpp
  ../../xbmc/xbmc/platform/win32/network/NetworkWin32.h
  ../../xbmc/xbmc/platform/win32/pch.cpp
  ../../xbmc/xbmc/platform/win32/pch.h
  ../../xbmc/xbmc/platform/win32/peripherals/PeripheralBusUSB.cpp
  ../../xbmc/xbmc/platform/win32/peripherals/PeripheralBusUSB.h
  ../../xbmc/xbmc/platform/win32/PlatformDefs.h
  ../../xbmc/xbmc/platform/win32/PlatformInclude.h
  ../../xbmc/xbmc/platform/win32/powermanagement/Win32PowerSyscall.cpp
  ../../xbmc/xbmc/platform/win32/powermanagement/Win32PowerSyscall.h
  ../../xbmc/xbmc/platform/win32/powermanagement/WinIdleTimer.cpp
  ../../xbmc/xbmc/platform/win32/resource.h
  ../../xbmc/xbmc/platform/win32/stat_utf8.cpp
  ../../xbmc/xbmc/platform/win32/stat_utf8.h
  ../../xbmc/xbmc/platform/win32/stdio_utf8.cpp
  ../../xbmc/xbmc/platform/win32/stdio_utf8.h
  ../../xbmc/xbmc/platform/win32/storage/Win32StorageProvider.cpp
  ../../xbmc/xbmc/platform/win32/storage/Win32StorageProvider.h
  ../../xbmc/xbmc/platform/win32/strverscmp.cpp
  ../../xbmc/xbmc/platform/win32/unistd.h
  ../../xbmc/xbmc/platform/win32/utils/gpu_memcpy_sse4.h
  ../../xbmc/xbmc/platform/win32/utils/memcpy_sse2.h
  ../../xbmc/xbmc/platform/win32/utils/Win32InterfaceForCLog.cpp
  ../../xbmc/xbmc/platform/win32/utils/Win32InterfaceForCLog.h
  ../../xbmc/xbmc/platform/win32/WIN32Util.cpp
  ../../xbmc/xbmc/platform/win32/WIN32Util.h
  ../../xbmc/xbmc/platform/win32/WindowHelper.cpp
  ../../xbmc/xbmc/platform/win32/WindowHelper.h
  ../../xbmc/xbmc/platform/win32/WinMain.cpp
  ../../xbmc/xbmc/platform/win32/WMIInterface.cpp
  ../../xbmc/xbmc/platform/win32/WMIInterface.h
  ../../xbmc/xbmc/platform/xbmc.cpp
  ../../xbmc/xbmc/platform/xbmc.h
  ../../xbmc/xbmc/PlayListPlayer.cpp
  ../../xbmc/xbmc/PlayListPlayer.h
  ../../xbmc/xbmc/playlists/PlayList.cpp
  ../../xbmc/xbmc/playlists/PlayList.h
  ../../xbmc/xbmc/playlists/PlayListB4S.cpp
  ../../xbmc/xbmc/playlists/PlayListB4S.h
  ../../xbmc/xbmc/playlists/PlayListFactory.cpp
  ../../xbmc/xbmc/playlists/PlayListFactory.h
  ../../xbmc/xbmc/playlists/PlayListM3U.cpp
  ../../xbmc/xbmc/playlists/PlayListM3U.h
  ../../xbmc/xbmc/playlists/PlayListPLS.cpp
  ../../xbmc/xbmc/playlists/PlayListPLS.h
  ../../xbmc/xbmc/playlists/PlayListURL.cpp
  ../../xbmc/xbmc/playlists/PlayListURL.h
  ../../xbmc/xbmc/playlists/PlayListWPL.cpp
  ../../xbmc/xbmc/playlists/PlayListWPL.h
  ../../xbmc/xbmc/playlists/PlayListXML.cpp
  ../../xbmc/xbmc/playlists/PlayListXML.h
  ../../xbmc/xbmc/playlists/PlayListXSPF.cpp
  ../../xbmc/xbmc/playlists/PlayListXSPF.h
  ../../xbmc/xbmc/playlists/SmartPlayList.cpp
  ../../xbmc/xbmc/playlists/SmartPlayList.h
  ../../xbmc/xbmc/playlists/SmartPlaylistFileItemListModifier.cpp
  ../../xbmc/xbmc/playlists/SmartPlaylistFileItemListModifier.h
  ../../xbmc/xbmc/powermanagement/DPMSSupport.cpp
  ../../xbmc/xbmc/powermanagement/DPMSSupport.h
  ../../xbmc/xbmc/powermanagement/IPowerSyscall.cpp
  ../../xbmc/xbmc/powermanagement/IPowerSyscall.h
  ../../xbmc/xbmc/powermanagement/PowerManager.cpp
  ../../xbmc/xbmc/powermanagement/PowerManager.h
  ../../xbmc/xbmc/powermanagement/WinIdleTimer.h
  ../../xbmc/xbmc/profiles/dialogs/GUIDialogLockSettings.cpp
  ../../xbmc/xbmc/profiles/dialogs/GUIDialogLockSettings.h
  ../../xbmc/xbmc/profiles/dialogs/GUIDialogProfileSettings.cpp
  ../../xbmc/xbmc/profiles/dialogs/GUIDialogProfileSettings.h
  ../../xbmc/xbmc/profiles/Profile.cpp
  ../../xbmc/xbmc/profiles/Profile.h
  ../../xbmc/xbmc/profiles/ProfileManager.cpp
  ../../xbmc/xbmc/profiles/ProfileManager.h
  ../../xbmc/xbmc/profiles/windows/GUIWindowSettingsProfile.cpp
  ../../xbmc/xbmc/profiles/windows/GUIWindowSettingsProfile.h
  ../../xbmc/xbmc/programs/GUIViewStatePrograms.cpp
  ../../xbmc/xbmc/programs/GUIViewStatePrograms.h
  ../../xbmc/xbmc/programs/GUIWindowPrograms.cpp
  ../../xbmc/xbmc/programs/GUIWindowPrograms.h
  ../../xbmc/xbmc/pvr/addons/PVRClients.cpp
  ../../xbmc/xbmc/pvr/addons/PVRClients.h
  ../../xbmc/xbmc/pvr/channels/PVRChannel.cpp
  ../../xbmc/xbmc/pvr/channels/PVRChannel.h
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroup.cpp
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroup.h
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroupInternal.cpp
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroupInternal.h
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroups.cpp
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroups.h
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroupsContainer.cpp
  ../../xbmc/xbmc/pvr/channels/PVRChannelGroupsContainer.h
  ../../xbmc/xbmc/pvr/channels/PVRChannelNumber.cpp
  ../../xbmc/xbmc/pvr/channels/PVRChannelNumber.h
  ../../xbmc/xbmc/pvr/channels/PVRRadioRDSInfoTag.cpp
  ../../xbmc/xbmc/pvr/channels/PVRRadioRDSInfoTag.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelGuide.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelGuide.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelManager.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelManager.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelsOSD.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelsOSD.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRClientPriorities.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRClientPriorities.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGroupManager.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGroupManager.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGuideInfo.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGuideInfo.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGuideSearch.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGuideSearch.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRItemsViewBase.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRItemsViewBase.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRadioRDSInfo.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRadioRDSInfo.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRecordingInfo.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRecordingInfo.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRecordingSettings.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRecordingSettings.h
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRTimerSettings.cpp
  ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRTimerSettings.h
  ../../xbmc/xbmc/pvr/epg/Epg.cpp
  ../../xbmc/xbmc/pvr/epg/Epg.h
  ../../xbmc/xbmc/pvr/epg/EpgContainer.cpp
  ../../xbmc/xbmc/pvr/epg/EpgContainer.h
  ../../xbmc/xbmc/pvr/epg/EpgDatabase.cpp
  ../../xbmc/xbmc/pvr/epg/EpgDatabase.h
  ../../xbmc/xbmc/pvr/epg/EpgInfoTag.cpp
  ../../xbmc/xbmc/pvr/epg/EpgInfoTag.h
  ../../xbmc/xbmc/pvr/epg/EpgSearchFilter.cpp
  ../../xbmc/xbmc/pvr/epg/EpgSearchFilter.h
  ../../xbmc/xbmc/pvr/PVRActionListener.cpp
  ../../xbmc/xbmc/pvr/PVRActionListener.h
  ../../xbmc/xbmc/pvr/PVRChannelNumberInputHandler.cpp
  ../../xbmc/xbmc/pvr/PVRChannelNumberInputHandler.h
  ../../xbmc/xbmc/pvr/PVRContextMenus.cpp
  ../../xbmc/xbmc/pvr/PVRContextMenus.h
  ../../xbmc/xbmc/pvr/PVRDatabase.cpp
  ../../xbmc/xbmc/pvr/PVRDatabase.h
  ../../xbmc/xbmc/pvr/PVREvent.h
  ../../xbmc/xbmc/pvr/PVRGUIActions.cpp
  ../../xbmc/xbmc/pvr/PVRGUIActions.h
  ../../xbmc/xbmc/pvr/PVRGUIChannelNavigator.cpp
  ../../xbmc/xbmc/pvr/PVRGUIChannelNavigator.h
  ../../xbmc/xbmc/pvr/PVRGUIInfo.cpp
  ../../xbmc/xbmc/pvr/PVRGUIInfo.h
  ../../xbmc/xbmc/pvr/PVRGUIProgressHandler.cpp
  ../../xbmc/xbmc/pvr/PVRGUIProgressHandler.h
  ../../xbmc/xbmc/pvr/PVRGUITimerInfo.cpp
  ../../xbmc/xbmc/pvr/PVRGUITimerInfo.h
  ../../xbmc/xbmc/pvr/PVRGUITimesInfo.cpp
  ../../xbmc/xbmc/pvr/PVRGUITimesInfo.h
  ../../xbmc/xbmc/pvr/PVRItem.cpp
  ../../xbmc/xbmc/pvr/PVRItem.h
  ../../xbmc/xbmc/pvr/PVRJobs.cpp
  ../../xbmc/xbmc/pvr/PVRJobs.h
  ../../xbmc/xbmc/pvr/PVRManager.cpp
  ../../xbmc/xbmc/pvr/PVRManager.h
  ../../xbmc/xbmc/pvr/PVRSettings.cpp
  ../../xbmc/xbmc/pvr/PVRSettings.h
  ../../xbmc/xbmc/pvr/PVRTypes.h
  ../../xbmc/xbmc/pvr/recordings/PVRRecording.cpp
  ../../xbmc/xbmc/pvr/recordings/PVRRecording.h
  ../../xbmc/xbmc/pvr/recordings/PVRRecordings.cpp
  ../../xbmc/xbmc/pvr/recordings/PVRRecordings.h
  ../../xbmc/xbmc/pvr/recordings/PVRRecordingsPath.cpp
  ../../xbmc/xbmc/pvr/recordings/PVRRecordingsPath.h
  ../../xbmc/xbmc/pvr/timers/PVRTimerInfoTag.cpp
  ../../xbmc/xbmc/pvr/timers/PVRTimerInfoTag.h
  ../../xbmc/xbmc/pvr/timers/PVRTimers.cpp
  ../../xbmc/xbmc/pvr/timers/PVRTimers.h
  ../../xbmc/xbmc/pvr/timers/PVRTimerType.cpp
  ../../xbmc/xbmc/pvr/timers/PVRTimerType.h
  ../../xbmc/xbmc/pvr/windows/GUIEPGGridContainer.cpp
  ../../xbmc/xbmc/pvr/windows/GUIEPGGridContainer.h
  ../../xbmc/xbmc/pvr/windows/GUIEPGGridContainerModel.cpp
  ../../xbmc/xbmc/pvr/windows/GUIEPGGridContainerModel.h
  ../../xbmc/xbmc/pvr/windows/GUIViewStatePVR.cpp
  ../../xbmc/xbmc/pvr/windows/GUIViewStatePVR.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRBase.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRBase.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRChannels.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRChannels.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRGuide.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRGuide.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRRecordings.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRRecordings.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRSearch.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRSearch.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimerRules.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimerRules.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimers.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimers.h
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimersBase.cpp
  ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimersBase.h
  ../../xbmc/xbmc/rendering/dx/DeviceResources.cpp
  ../../xbmc/xbmc/rendering/dx/DeviceResources.h
  ../../xbmc/xbmc/rendering/dx/DirectXHelper.h
  ../../xbmc/xbmc/rendering/dx/RenderContext.h
  ../../xbmc/xbmc/rendering/dx/RenderSystemDX.cpp
  ../../xbmc/xbmc/rendering/dx/RenderSystemDX.h
  ../../xbmc/xbmc/rendering/gles/GLESShader.cpp
  ../../xbmc/xbmc/rendering/gles/GLESShader.h
  ../../xbmc/xbmc/rendering/gles/RenderSystemGLES.cpp
  ../../xbmc/xbmc/rendering/gles/RenderSystemGLES.h
  ../../xbmc/xbmc/rendering/qt/RenderSystemQt.cpp
  ../../xbmc/xbmc/rendering/qt/RenderSystemQt.h
  ../../xbmc/xbmc/rendering/RenderSystem.cpp
  ../../xbmc/xbmc/rendering/RenderSystem.h
  ../../xbmc/xbmc/rendering/RenderSystemTypes.h
  ../../xbmc/xbmc/SectionLoader.cpp
  ../../xbmc/xbmc/SectionLoader.h
  ../../xbmc/xbmc/SeekHandler.cpp
  ../../xbmc/xbmc/SeekHandler.h
  ../../xbmc/xbmc/ServiceBroker.cpp
  ../../xbmc/xbmc/ServiceBroker.h
  ../../xbmc/xbmc/ServiceDescription.h
  ../../xbmc/xbmc/ServiceManager.cpp
  ../../xbmc/xbmc/ServiceManager.h
  ../../xbmc/xbmc/settings/AdvancedSettings.cpp
  ../../xbmc/xbmc/settings/AdvancedSettings.h
  ../../xbmc/xbmc/settings/dialogs/GUIDialogContentSettings.cpp
  ../../xbmc/xbmc/settings/dialogs/GUIDialogContentSettings.h
  ../../xbmc/xbmc/settings/dialogs/GUIDialogLibExportSettings.cpp
  ../../xbmc/xbmc/settings/dialogs/GUIDialogLibExportSettings.h
  ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsBase.cpp
  ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsBase.h
  ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsManagerBase.cpp
  ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsManagerBase.h
  ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsManualBase.cpp
  ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsManualBase.h
  ../../xbmc/xbmc/settings/DiscSettings.cpp
  ../../xbmc/xbmc/settings/DiscSettings.h
  ../../xbmc/xbmc/settings/DisplaySettings.cpp
  ../../xbmc/xbmc/settings/DisplaySettings.h
  ../../xbmc/xbmc/settings/GameSettings.cpp
  ../../xbmc/xbmc/settings/GameSettings.h
  ../../xbmc/xbmc/settings/lib/ISetting.cpp
  ../../xbmc/xbmc/settings/lib/ISetting.h
  ../../xbmc/xbmc/settings/lib/ISettingCallback.h
  ../../xbmc/xbmc/settings/lib/ISettingControl.cpp
  ../../xbmc/xbmc/settings/lib/ISettingControl.h
  ../../xbmc/xbmc/settings/lib/ISettingControlCreator.h
  ../../xbmc/xbmc/settings/lib/ISettingCreator.h
  ../../xbmc/xbmc/settings/lib/ISettingsHandler.h
  ../../xbmc/xbmc/settings/lib/ISubSettings.h
  ../../xbmc/xbmc/settings/lib/Setting.cpp
  ../../xbmc/xbmc/settings/lib/Setting.h
  ../../xbmc/xbmc/settings/lib/SettingCategoryAccess.cpp
  ../../xbmc/xbmc/settings/lib/SettingCategoryAccess.h
  ../../xbmc/xbmc/settings/lib/SettingConditions.h
  ../../xbmc/xbmc/settings/lib/SettingConditions2.cpp
  ../../xbmc/xbmc/settings/lib/SettingDefinitions.h
  ../../xbmc/xbmc/settings/lib/SettingDependency.cpp
  ../../xbmc/xbmc/settings/lib/SettingDependency.h
  ../../xbmc/xbmc/settings/lib/SettingLevel.h
  ../../xbmc/xbmc/settings/lib/SettingRequirement.cpp
  ../../xbmc/xbmc/settings/lib/SettingRequirement.h
  ../../xbmc/xbmc/settings/lib/SettingSection.cpp
  ../../xbmc/xbmc/settings/lib/SettingSection.h
  ../../xbmc/xbmc/settings/lib/SettingsManager.cpp
  ../../xbmc/xbmc/settings/lib/SettingsManager.h
  ../../xbmc/xbmc/settings/lib/SettingType.h
  ../../xbmc/xbmc/settings/lib/SettingUpdate.cpp
  ../../xbmc/xbmc/settings/lib/SettingUpdate.h
  ../../xbmc/xbmc/settings/LibExportSettings.cpp
  ../../xbmc/xbmc/settings/LibExportSettings.h
  ../../xbmc/xbmc/settings/MediaSettings.cpp
  ../../xbmc/xbmc/settings/MediaSettings.h
  ../../xbmc/xbmc/settings/MediaSourceSettings.cpp
  ../../xbmc/xbmc/settings/MediaSourceSettings.h
  ../../xbmc/xbmc/settings/SettingAddon.cpp
  ../../xbmc/xbmc/settings/SettingAddon.h
  ../../xbmc/xbmc/settings/SettingConditions.cpp
  ../../xbmc/xbmc/settings/SettingConditions.h
  ../../xbmc/xbmc/settings/SettingControl.cpp
  ../../xbmc/xbmc/settings/SettingControl.h
  ../../xbmc/xbmc/settings/SettingCreator.cpp
  ../../xbmc/xbmc/settings/SettingCreator.h
  ../../xbmc/xbmc/settings/SettingDateTime.cpp
  ../../xbmc/xbmc/settings/SettingDateTime.h
  ../../xbmc/xbmc/settings/SettingPath.cpp
  ../../xbmc/xbmc/settings/SettingPath.h
  ../../xbmc/xbmc/settings/Settings.cpp
  ../../xbmc/xbmc/settings/Settings.h
  ../../xbmc/xbmc/settings/SettingsBase.cpp
  ../../xbmc/xbmc/settings/SettingsBase.h
  ../../xbmc/xbmc/settings/SettingsComponent.cpp
  ../../xbmc/xbmc/settings/SettingsComponent.h
  ../../xbmc/xbmc/settings/SettingUtils.cpp
  ../../xbmc/xbmc/settings/SettingUtils.h
  ../../xbmc/xbmc/settings/SkinSettings.cpp
  ../../xbmc/xbmc/settings/SkinSettings.h
  ../../xbmc/xbmc/settings/windows/GUIControlSettings.cpp
  ../../xbmc/xbmc/settings/windows/GUIControlSettings.h
  ../../xbmc/xbmc/settings/windows/GUIWindowSettings.cpp
  ../../xbmc/xbmc/settings/windows/GUIWindowSettings.h
  ../../xbmc/xbmc/settings/windows/GUIWindowSettingsCategory.cpp
  ../../xbmc/xbmc/settings/windows/GUIWindowSettingsCategory.h
  ../../xbmc/xbmc/settings/windows/GUIWindowSettingsScreenCalibration.cpp
  ../../xbmc/xbmc/settings/windows/GUIWindowSettingsScreenCalibration.h
  ../../xbmc/xbmc/SortFileItem.h
  ../../xbmc/xbmc/StaticDll.cpp
  ../../xbmc/xbmc/StaticDll.h
  ../../xbmc/xbmc/storage/AutorunMediaJob.cpp
  ../../xbmc/xbmc/storage/AutorunMediaJob.h
  ../../xbmc/xbmc/storage/cdioSupport.cpp
  ../../xbmc/xbmc/storage/cdioSupport.h
  ../../xbmc/xbmc/storage/DetectDVDType.cpp
  ../../xbmc/xbmc/storage/DetectDVDType.h
  ../../xbmc/xbmc/storage/IoSupport.cpp
  ../../xbmc/xbmc/storage/IoSupport.h
  ../../xbmc/xbmc/storage/IStorageProvider.h
  ../../xbmc/xbmc/storage/MediaManager.cpp
  ../../xbmc/xbmc/storage/MediaManager.h
  ../../xbmc/xbmc/swig/AddonModuleXbmc.i.cpp
  ../../xbmc/xbmc/swig/AddonModuleXbmcaddon.i.cpp
  ../../xbmc/xbmc/swig/AddonModuleXbmcdrm.i.cpp
  ../../xbmc/xbmc/swig/AddonModuleXbmcgui.i.cpp
  ../../xbmc/xbmc/swig/AddonModuleXbmcplugin.i.cpp
  ../../xbmc/xbmc/swig/AddonModuleXbmcvfs.i.cpp
  ../../xbmc/xbmc/swig/AddonModuleXbmcwsgi.i.cpp
  ../../xbmc/xbmc/system_gl.h
  ../../xbmc/xbmc/SystemGlobals.cpp
  ../../xbmc/xbmc/TextureCache.cpp
  ../../xbmc/xbmc/TextureCache.h
  ../../xbmc/xbmc/TextureCacheJob.cpp
  ../../xbmc/xbmc/TextureCacheJob.h
  ../../xbmc/xbmc/TextureDatabase.cpp
  ../../xbmc/xbmc/TextureDatabase.h
  ../../xbmc/xbmc/threads/Atomics.cpp
  ../../xbmc/xbmc/threads/Atomics.h
  ../../xbmc/xbmc/threads/Condition.h
  ../../xbmc/xbmc/threads/CriticalSection.h
  ../../xbmc/xbmc/threads/Event.cpp
  ../../xbmc/xbmc/threads/Event.h
  ../../xbmc/xbmc/threads/Helpers.h
  ../../xbmc/xbmc/threads/IRunnable.h
  ../../xbmc/xbmc/threads/Lockables.h
  ../../xbmc/xbmc/threads/MipsAtomics.h
  ../../xbmc/xbmc/threads/platform/win/ThreadImpl.h
  ../../xbmc/xbmc/threads/platform/win/ThreadImplWin32.cpp
  ../../xbmc/xbmc/threads/platform/win/ThreadImplWin32.h
  ../../xbmc/xbmc/threads/platform/win/Win32Exception.cpp
  ../../xbmc/xbmc/threads/platform/win/Win32Exception.h
  ../../xbmc/xbmc/threads/SharedSection.h
  ../../xbmc/xbmc/threads/SingleLock.h
  ../../xbmc/xbmc/threads/SystemClock.cpp
  ../../xbmc/xbmc/threads/SystemClock.h
  ../../xbmc/xbmc/threads/Thread.cpp
  ../../xbmc/xbmc/threads/Thread.h
  ../../xbmc/xbmc/threads/ThreadImpl.h
  ../../xbmc/xbmc/threads/Timer.cpp
  ../../xbmc/xbmc/threads/Timer.h
  ../../xbmc/xbmc/ThumbLoader.cpp
  ../../xbmc/xbmc/ThumbLoader.h
  ../../xbmc/xbmc/utils/ActorProtocol.cpp
  ../../xbmc/xbmc/utils/ActorProtocol.h
  ../../xbmc/xbmc/utils/AlarmClock.cpp
  ../../xbmc/xbmc/utils/AlarmClock.h
  ../../xbmc/xbmc/utils/AliasShortcutUtils.cpp
  ../../xbmc/xbmc/utils/AliasShortcutUtils.h
  ../../xbmc/xbmc/utils/AMLUtils.cpp
  ../../xbmc/xbmc/utils/AMLUtils.h
  ../../xbmc/xbmc/utils/Archive.cpp
  ../../xbmc/xbmc/utils/Archive.h
  ../../xbmc/xbmc/utils/auto_buffer.cpp
  ../../xbmc/xbmc/utils/auto_buffer.h
  ../../xbmc/xbmc/utils/Base64.cpp
  ../../xbmc/xbmc/utils/Base64.h
  ../../xbmc/xbmc/utils/BitstreamConverter.cpp
  ../../xbmc/xbmc/utils/BitstreamConverter.h
  ../../xbmc/xbmc/utils/BitstreamReader.cpp
  ../../xbmc/xbmc/utils/BitstreamReader.h
  ../../xbmc/xbmc/utils/BitstreamStats.cpp
  ../../xbmc/xbmc/utils/BitstreamStats.h
  ../../xbmc/xbmc/utils/BitstreamWriter.cpp
  ../../xbmc/xbmc/utils/BitstreamWriter.h
  ../../xbmc/xbmc/utils/BooleanLogic.cpp
  ../../xbmc/xbmc/utils/BooleanLogic.h
  ../../xbmc/xbmc/utils/CharsetConverter.cpp
  ../../xbmc/xbmc/utils/CharsetConverter.h
  ../../xbmc/xbmc/utils/CharsetDetection.cpp
  ../../xbmc/xbmc/utils/CharsetDetection.h
  ../../xbmc/xbmc/utils/Color.h
  ../../xbmc/xbmc/utils/ColorUtils.cpp
  ../../xbmc/xbmc/utils/ColorUtils.h
  ../../xbmc/xbmc/utils/CPUInfo.cpp
  ../../xbmc/xbmc/utils/CPUInfo.h
  ../../xbmc/xbmc/utils/Crc32.cpp
  ../../xbmc/xbmc/utils/Crc32.h
  ../../xbmc/xbmc/utils/CryptThreading.cpp
  ../../xbmc/xbmc/utils/CryptThreading.h
  ../../xbmc/xbmc/utils/DatabaseUtils.cpp
  ../../xbmc/xbmc/utils/DatabaseUtils.h
  ../../xbmc/xbmc/utils/Digest.cpp
  ../../xbmc/xbmc/utils/Digest.h
  ../../xbmc/xbmc/utils/EmbeddedArt.cpp
  ../../xbmc/xbmc/utils/EmbeddedArt.h
  ../../xbmc/xbmc/utils/EndianSwap.cpp
  ../../xbmc/xbmc/utils/endianswap.h
  ../../xbmc/xbmc/utils/EventStream.h
  ../../xbmc/xbmc/utils/EventStreamDetail.h
  ../../xbmc/xbmc/utils/Fanart.cpp
  ../../xbmc/xbmc/utils/Fanart.h
  ../../xbmc/xbmc/utils/FileExtensionProvider.cpp
  ../../xbmc/xbmc/utils/FileExtensionProvider.h
  ../../xbmc/xbmc/utils/FileOperationJob.cpp
  ../../xbmc/xbmc/utils/FileOperationJob.h
  ../../xbmc/xbmc/utils/FileUtils.cpp
  ../../xbmc/xbmc/utils/FileUtils.h
  ../../xbmc/xbmc/utils/fstrcmp.c
  ../../xbmc/xbmc/utils/fstrcmp.h
  ../../xbmc/xbmc/utils/Geometry.h
  ../../xbmc/xbmc/utils/GlobalsHandling.h
  ../../xbmc/xbmc/utils/GLUtils.cpp
  ../../xbmc/xbmc/utils/GLUtils.h
  ../../xbmc/xbmc/utils/GroupUtils.cpp
  ../../xbmc/xbmc/utils/GroupUtils.h
  ../../xbmc/xbmc/utils/HTMLUtil.cpp
  ../../xbmc/xbmc/utils/HTMLUtil.h
  ../../xbmc/xbmc/utils/HttpHeader.cpp
  ../../xbmc/xbmc/utils/HttpHeader.h
  ../../xbmc/xbmc/utils/HttpParser.cpp
  ../../xbmc/xbmc/utils/HttpParser.h
  ../../xbmc/xbmc/utils/HttpRangeUtils.cpp
  ../../xbmc/xbmc/utils/HttpRangeUtils.h
  ../../xbmc/xbmc/utils/HttpResponse.cpp
  ../../xbmc/xbmc/utils/HttpResponse.h
  ../../xbmc/xbmc/utils/IArchivable.h
  ../../xbmc/xbmc/utils/IBufferObject.h
  ../../xbmc/xbmc/utils/ILocalizer.h
  ../../xbmc/xbmc/utils/InfoLoader.cpp
  ../../xbmc/xbmc/utils/InfoLoader.h
  ../../xbmc/xbmc/utils/IRssObserver.h
  ../../xbmc/xbmc/utils/ISerializable.h
  ../../xbmc/xbmc/utils/ISortable.h
  ../../xbmc/xbmc/utils/IXmlDeserializable.h
  ../../xbmc/xbmc/utils/Job.h
  ../../xbmc/xbmc/utils/JobManager.cpp
  ../../xbmc/xbmc/utils/JobManager.h
  ../../xbmc/xbmc/utils/JSONVariantParser.cpp
  ../../xbmc/xbmc/utils/JSONVariantParser.h
  ../../xbmc/xbmc/utils/JSONVariantWriter.cpp
  ../../xbmc/xbmc/utils/JSONVariantWriter.h
  ../../xbmc/xbmc/utils/LabelFormatter.cpp
  ../../xbmc/xbmc/utils/LabelFormatter.h
  ../../xbmc/xbmc/utils/LangCodeExpander.cpp
  ../../xbmc/xbmc/utils/LangCodeExpander.h
  ../../xbmc/xbmc/utils/LegacyPathTranslation.cpp
  ../../xbmc/xbmc/utils/LegacyPathTranslation.h
  ../../xbmc/xbmc/utils/Literals.h
  ../../xbmc/xbmc/utils/Locale.cpp
  ../../xbmc/xbmc/utils/Locale.h
  ../../xbmc/xbmc/utils/log.cpp
  ../../xbmc/xbmc/utils/log.h
  ../../xbmc/xbmc/utils/MathUtils.h
  ../../xbmc/xbmc/utils/Mime.cpp
  ../../xbmc/xbmc/utils/Mime.h
  ../../xbmc/xbmc/utils/Observer.cpp
  ../../xbmc/xbmc/utils/Observer.h
  ../../xbmc/xbmc/utils/params_check_macros.h
  ../../xbmc/xbmc/utils/POUtils.cpp
  ../../xbmc/xbmc/utils/POUtils.h
  ../../xbmc/xbmc/utils/ProgressJob.cpp
  ../../xbmc/xbmc/utils/ProgressJob.h
  ../../xbmc/xbmc/utils/Random.h
  ../../xbmc/xbmc/utils/RecentlyAddedJob.cpp
  ../../xbmc/xbmc/utils/RecentlyAddedJob.h
  ../../xbmc/xbmc/utils/RegExp.cpp
  ../../xbmc/xbmc/utils/RegExp.h
  ../../xbmc/xbmc/utils/rfft.cpp
  ../../xbmc/xbmc/utils/rfft.h
  ../../xbmc/xbmc/utils/RingBuffer.cpp
  ../../xbmc/xbmc/utils/RingBuffer.h
  ../../xbmc/xbmc/utils/RssManager.cpp
  ../../xbmc/xbmc/utils/RssManager.h
  ../../xbmc/xbmc/utils/RssReader.cpp
  ../../xbmc/xbmc/utils/RssReader.h
  ../../xbmc/xbmc/utils/SaveFileStateJob.cpp
  ../../xbmc/xbmc/utils/SaveFileStateJob.h
  ../../xbmc/xbmc/utils/ScopeGuard.h
  ../../xbmc/xbmc/utils/ScraperParser.cpp
  ../../xbmc/xbmc/utils/ScraperParser.h
  ../../xbmc/xbmc/utils/ScraperUrl.cpp
  ../../xbmc/xbmc/utils/ScraperUrl.h
  ../../xbmc/xbmc/utils/Screenshot.cpp
  ../../xbmc/xbmc/utils/Screenshot.h
  ../../xbmc/xbmc/utils/ScreenshotAML.cpp
  ../../xbmc/xbmc/utils/ScreenshotAML.h
  ../../xbmc/xbmc/utils/sha1.hpp
  ../../xbmc/xbmc/utils/SortUtils.cpp
  ../../xbmc/xbmc/utils/SortUtils.h
  ../../xbmc/xbmc/utils/Speed.cpp
  ../../xbmc/xbmc/utils/Speed.h
  ../../xbmc/xbmc/utils/Stopwatch.cpp
  ../../xbmc/xbmc/utils/Stopwatch.h
  ../../xbmc/xbmc/utils/StreamDetails.cpp
  ../../xbmc/xbmc/utils/StreamDetails.h
  ../../xbmc/xbmc/utils/StreamUtils.cpp
  ../../xbmc/xbmc/utils/StreamUtils.h
  ../../xbmc/xbmc/utils/StringUtils.h
  ../../xbmc/xbmc/utils/StringUtils2.cpp
  ../../xbmc/xbmc/utils/StringValidation.cpp
  ../../xbmc/xbmc/utils/StringValidation.h
  ../../xbmc/xbmc/utils/SysfsUtils.cpp
  ../../xbmc/xbmc/utils/SysfsUtils.h
  ../../xbmc/xbmc/utils/SystemInfo.cpp
  ../../xbmc/xbmc/utils/SystemInfo.h
  ../../xbmc/xbmc/utils/Temperature.cpp
  ../../xbmc/xbmc/utils/Temperature.h
  ../../xbmc/xbmc/utils/TextSearch.cpp
  ../../xbmc/xbmc/utils/TextSearch.h
  ../../xbmc/xbmc/utils/TimeUtils.h
  ../../xbmc/xbmc/utils/TimeUtils2.cpp
  ../../xbmc/xbmc/utils/TransformMatrix.h
  ../../xbmc/xbmc/utils/URIUtils.cpp
  ../../xbmc/xbmc/utils/URIUtils.h
  ../../xbmc/xbmc/utils/UrlOptions.cpp
  ../../xbmc/xbmc/utils/UrlOptions.h
  ../../xbmc/xbmc/utils/Utf8Utils.cpp
  ../../xbmc/xbmc/utils/Utf8Utils.h
  ../../xbmc/xbmc/utils/uXstrings.h
  ../../xbmc/xbmc/utils/Variant.cpp
  ../../xbmc/xbmc/utils/Variant.h
  ../../xbmc/xbmc/utils/VC1BitstreamParser.cpp
  ../../xbmc/xbmc/utils/VC1BitstreamParser.h
  ../../xbmc/xbmc/utils/Vector.cpp
  ../../xbmc/xbmc/utils/Vector.h
  ../../xbmc/xbmc/utils/XBMCTinyXML.cpp
  ../../xbmc/xbmc/utils/XBMCTinyXML.h
  ../../xbmc/xbmc/utils/XMLUtils.cpp
  ../../xbmc/xbmc/utils/XMLUtils.h
  ../../xbmc/xbmc/utils/XSLTUtils.cpp
  ../../xbmc/xbmc/utils/XSLTUtils.h
  ../../xbmc/xbmc/video/Bookmark.cpp
  ../../xbmc/xbmc/video/Bookmark.h
  ../../xbmc/xbmc/video/ContextMenus.h
  ../../xbmc/xbmc/video/ContextMenusVideo.cpp
  ../../xbmc/xbmc/video/ContextMenusVideo.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogAudioSettings.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogAudioSettings.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogCMSSettings.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogCMSSettings.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogFullScreenInfo.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogFullScreenInfo.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogSubtitles.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogSubtitles.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogSubtitleSettings.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogSubtitleSettings.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogTeletext.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogTeletext.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoBookmarks.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoBookmarks.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoInfo.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoInfo.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoOSD.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoOSD.h
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoSettings.cpp
  ../../xbmc/xbmc/video/dialogs/GUIDialogVideoSettings.h
  ../../xbmc/xbmc/video/Episode.h
  ../../xbmc/xbmc/video/GUIViewStateVideo.cpp
  ../../xbmc/xbmc/video/GUIViewStateVideo.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryCleaningJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryCleaningJob.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryJob.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryMarkWatchedJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryMarkWatchedJob.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryProgressJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryProgressJob.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryRefreshingJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryRefreshingJob.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryResetResumePointJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryResetResumePointJob.h
  ../../xbmc/xbmc/video/jobs/VideoLibraryScanningJob.cpp
  ../../xbmc/xbmc/video/jobs/VideoLibraryScanningJob.h
  ../../xbmc/xbmc/video/PlayerController.cpp
  ../../xbmc/xbmc/video/PlayerController.h
  ../../xbmc/xbmc/video/tags/IVideoInfoTagLoader.h
  ../../xbmc/xbmc/video/tags/VideoInfoTagLoaderFactory.cpp
  ../../xbmc/xbmc/video/tags/VideoInfoTagLoaderFactory.h
  ../../xbmc/xbmc/video/tags/VideoTagLoaderFFmpeg.cpp
  ../../xbmc/xbmc/video/tags/VideoTagLoaderFFmpeg.h
  ../../xbmc/xbmc/video/tags/VideoTagLoaderNFO.cpp
  ../../xbmc/xbmc/video/tags/VideoTagLoaderNFO.h
  ../../xbmc/xbmc/video/tags/VideoTagLoaderPlugin.cpp
  ../../xbmc/xbmc/video/tags/VideoTagLoaderPlugin.h
  ../../xbmc/xbmc/video/Teletext.cpp
  ../../xbmc/xbmc/video/Teletext.h
  ../../xbmc/xbmc/video/TeletextDefines.h
  ../../xbmc/xbmc/video/VideoDatabase.cpp
  ../../xbmc/xbmc/video/VideoDatabase.h
  ../../xbmc/xbmc/video/VideoDbUrl.cpp
  ../../xbmc/xbmc/video/VideoDbUrl.h
  ../../xbmc/xbmc/video/VideoInfoDownloader.cpp
  ../../xbmc/xbmc/video/VideoInfoDownloader.h
  ../../xbmc/xbmc/video/VideoInfoScanner.cpp
  ../../xbmc/xbmc/video/VideoInfoScanner.h
  ../../xbmc/xbmc/video/VideoInfoTag.cpp
  ../../xbmc/xbmc/video/VideoInfoTag.h
  ../../xbmc/xbmc/video/VideoLibraryQueue.cpp
  ../../xbmc/xbmc/video/VideoLibraryQueue.h
  ../../xbmc/xbmc/video/VideoThumbLoader.cpp
  ../../xbmc/xbmc/video/VideoThumbLoader.h
  ../../xbmc/xbmc/video/ViewModeSettings.cpp
  ../../xbmc/xbmc/video/ViewModeSettings.h
  ../../xbmc/xbmc/video/windows/GUIWindowFullScreen.cpp
  ../../xbmc/xbmc/video/windows/GUIWindowFullScreen.h
  ../../xbmc/xbmc/video/windows/GUIWindowFullScreenDefines.h
  ../../xbmc/xbmc/video/windows/GUIWindowVideoBase.cpp
  ../../xbmc/xbmc/video/windows/GUIWindowVideoBase.h
  ../../xbmc/xbmc/video/windows/GUIWindowVideoNav.cpp
  ../../xbmc/xbmc/video/windows/GUIWindowVideoNav.h
  ../../xbmc/xbmc/video/windows/GUIWindowVideoPlaylist.cpp
  ../../xbmc/xbmc/video/windows/GUIWindowVideoPlaylist.h
  ../../xbmc/xbmc/video/windows/VideoFileItemListModifier.cpp
  ../../xbmc/xbmc/video/windows/VideoFileItemListModifier.h
  ../../xbmc/xbmc/view/GUIViewControl.cpp
  ../../xbmc/xbmc/view/GUIViewControl.h
  ../../xbmc/xbmc/view/GUIViewState.cpp
  ../../xbmc/xbmc/view/GUIViewState.h
  ../../xbmc/xbmc/view/ViewDatabase.cpp
  ../../xbmc/xbmc/view/ViewDatabase.h
  ../../xbmc/xbmc/view/ViewState.h
  ../../xbmc/xbmc/view/ViewStateSettings.cpp
  ../../xbmc/xbmc/view/ViewStateSettings.h
  ../../xbmc/xbmc/weather/GUIWindowWeather.cpp
  ../../xbmc/xbmc/weather/GUIWindowWeather.h
  ../../xbmc/xbmc/weather/WeatherJob.cpp
  ../../xbmc/xbmc/weather/WeatherJob.h
  ../../xbmc/xbmc/weather/WeatherManager.cpp
  ../../xbmc/xbmc/weather/WeatherManager.h
  ../../xbmc/xbmc/windowing/GraphicContext.cpp
  ../../xbmc/xbmc/windowing/GraphicContext.h
  ../../xbmc/xbmc/windowing/OSScreenSaver.cpp
  ../../xbmc/xbmc/windowing/OSScreenSaver.h
  ../../xbmc/xbmc/windowing/qt/OSScreenSaverQt.cpp
  ../../xbmc/xbmc/windowing/qt/OSScreenSaverQt.h
  ../../xbmc/xbmc/windowing/qt/QtUtils.cpp
  ../../xbmc/xbmc/windowing/qt/QtUtils.h
  ../../xbmc/xbmc/windowing/qt/VideoSyncQt.cpp
  ../../xbmc/xbmc/windowing/qt/VideoSyncQt.h
  ../../xbmc/xbmc/windowing/qt/WinEventsQt.cpp
  ../../xbmc/xbmc/windowing/qt/WinEventsQt.h
  ../../xbmc/xbmc/windowing/qt/WinKeyMapQt.h
  ../../xbmc/xbmc/windowing/qt/WinSystemQt.cpp
  ../../xbmc/xbmc/windowing/qt/WinSystemQt.h
  ../../xbmc/xbmc/windowing/qt/WinSystemQtContext.cpp
  ../../xbmc/xbmc/windowing/qt/WinSystemQtContext.h
  ../../xbmc/xbmc/windowing/Resolution.cpp
  ../../xbmc/xbmc/windowing/Resolution.h
  ../../xbmc/xbmc/windowing/VideoSync.h
  ../../xbmc/xbmc/windowing/windows/VideoSyncD3D.cpp
  ../../xbmc/xbmc/windowing/windows/VideoSyncD3D.h
  ../../xbmc/xbmc/windowing/windows/WinEventsWin32.cpp
  ../../xbmc/xbmc/windowing/windows/WinEventsWin32.h
  ../../xbmc/xbmc/windowing/windows/WinKeyMap.h
  ../../xbmc/xbmc/windowing/windows/WinSystemWin32.cpp
  ../../xbmc/xbmc/windowing/windows/WinSystemWin32.h
  ../../xbmc/xbmc/windowing/windows/WinSystemWin32DX.cpp
  ../../xbmc/xbmc/windowing/windows/WinSystemWin32DX.h
  ../../xbmc/xbmc/windowing/WinEvents.h
  ../../xbmc/xbmc/windowing/WinSystem.cpp
  ../../xbmc/xbmc/windowing/WinSystem.h
  ../../xbmc/xbmc/windowing/XBMC_events.h
  ../../xbmc/xbmc/windows/GUIMediaWindow.cpp
  ../../xbmc/xbmc/windows/GUIMediaWindow.h
  ../../xbmc/xbmc/windows/GUIWindowDebugInfo.cpp
  ../../xbmc/xbmc/windows/GUIWindowDebugInfo.h
  ../../xbmc/xbmc/windows/GUIWindowFileManager.cpp
  ../../xbmc/xbmc/windows/GUIWindowFileManager.h
  ../../xbmc/xbmc/windows/GUIWindowHome.cpp
  ../../xbmc/xbmc/windows/GUIWindowHome.h
  ../../xbmc/xbmc/windows/GUIWindowLoginScreen.cpp
  ../../xbmc/xbmc/windows/GUIWindowLoginScreen.h
  ../../xbmc/xbmc/windows/GUIWindowPointer.cpp
  ../../xbmc/xbmc/windows/GUIWindowPointer.h
  ../../xbmc/xbmc/windows/GUIWindowScreensaver.cpp
  ../../xbmc/xbmc/windows/GUIWindowScreensaver.h
  ../../xbmc/xbmc/windows/GUIWindowScreensaverDim.cpp
  ../../xbmc/xbmc/windows/GUIWindowScreensaverDim.h
  ../../xbmc/xbmc/windows/GUIWindowSplash.cpp
  ../../xbmc/xbmc/windows/GUIWindowSplash.h
  ../../xbmc/xbmc/windows/GUIWindowStartup.cpp
  ../../xbmc/xbmc/windows/GUIWindowStartup.h
  ../../xbmc/xbmc/windows/GUIWindowSystemInfo.cpp
  ../../xbmc/xbmc/windows/GUIWindowSystemInfo.h
  ../../xbmc/xbmc/XBApplicationEx.cpp
  ../../xbmc/xbmc/XBApplicationEx.h
  ../../xbmc/xbmc/XBDateTime.cpp
  ../../xbmc/xbmc/XBDateTime.h
)
# Output file name
set_target_properties(libkodi
  PROPERTIES
  OUTPUT_NAME_DEBUG libkodiD
)

# Additional include directories
set_property(TARGET libkodi
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\sysheaders;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libpthread\;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\xbmc;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\xbmc\cores\AudioEngine\;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\Python-2.7.14\Include;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\openssl-1.0.2p\include;
    ${CMAKE_CURRENT_SOURCE_DIR};
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\ffmpeg;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Platinum\Source\Devices\MediaRenderer;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Platinum\Source\Devices\MediaConnect;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Platinum\Source\Devices\MediaServer;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Platinum\Source\Platinum;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Platinum\Source\Core;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Platinum\Source\Extras;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Neptune\Source\Core;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libUPnP\Neptune\Source\System\Win32;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libpcre;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\Effects11\inc;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\libfreetype\include;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\..\DependLibs\PlatformLib\platform\win32;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\addons\library.kodi.guilib;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\addons\library.xbmc.addon;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\addons\library.xbmc.pvr;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\addons\library.GoTv.adsp;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\addons\library.xbmc.codec;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\gtest\include;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\xbmc\platform\win32;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\xbmc\cores\VideoPlayer;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\build;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\project\BuildDependencies\win32\include;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\project\BuildDependencies\mingwlibs\win32\include;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\build\libdvd\include;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\project\BuildDependencies\win32\include\lzo;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\project\BuildDependencies\win32\include\python;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\xbmc\cores\AudioEngine;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Platinum\Source\Core;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Platinum\Source\Platinum;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Platinum\Source\Devices\MediaConnect;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Platinum\Source\Devices\MediaRenderer;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Platinum\Source\Devices\MediaServer;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Platinum\Source\Extras;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Neptune\Source\System\Posix;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\libUPnP\Neptune\Source\Core;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\build\guilib;
    F:\NoLimitConnectWindows\GoTvCore\build\VS2017\..\..\xbmc\lib\win32\Effects11\inc>
)

# Preprocessor definitions
target_compile_definitions(libkodi PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;DEBUG;TARGET_OS_WINDOWS;_LIB;LIB_STATIC;NOMINMAX;__STDC_CONSTANT_MACROS;WIN64;_WINDOWS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libkodi PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libkodi PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libkodi PRIVATE
    ""
  )
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonBuilder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/BinaryAddonCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonInstaller.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonStatusHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonSystemSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AddonVersion.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/AudioEncoder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/ContextMenuAddon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/FontResource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/FilesystemInstaller.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/GameResource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/GUIDialogAddonInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/GUIViewStateAddonBrowser.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/GUIWindowAddonBrowser.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/ImageDecoder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/ImageResource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/LanguageResource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/PluginSource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/PVRClient.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/Repository.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/RepositoryUpdater.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/Scraper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/ScreenSaver.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/Service.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/Skin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/UISoundsResource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/VFSEntry.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/Visualization.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/Webinterface.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/binary-addons/BinaryAddonManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/binary-addons/AddonDll.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/binary-addons/AddonInstanceHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/binary-addons/BinaryAddonBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/binary-addons/BinaryAddonExtensions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/binary-addons/BinaryAddonType.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/AddonInterfaces.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/General.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/Filesystem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/Addon/AddonCallbacksAddon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/AddonCallbacksGUI.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/AddonGUIRenderingControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/AddonGUIWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Button.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Edit.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/FadeLabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Image.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Label.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/RadioButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Rendering.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/SettingsSlider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Slider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/Spin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/controls/TextBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/ContextMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/ExtendedProgressBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/FileBrowser.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Numeric.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/Select.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/TextViewer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/interfaces/GUI/dialogs/YesNo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/settings/AddonSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/settings/GUIDialogAddonSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/addons/settings/SettingUrlEncodedString.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cdrip/CDDARipJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cdrip/Encoder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cdrip/EncoderFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cdrip/CDDARipper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/contrib/kissfft/kiss_fft.c
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/contrib/kissfft/kiss_fftr.c
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DataCacheCore.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/AEResampleFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/AESinkFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Encoders/AEEncoderFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAE.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEBuffer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESink.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESound.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAESettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Engines/ActiveAE/ActiveAEResampleFFMPEG.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkWASAPI.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Sinks/AESinkDirectSound.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Sinks/windows/AESinkFactoryWin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Sinks/windows/AESinkFactoryWin32.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AEBitstreamPacker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AEChannelInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AEDeviceInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AELimiter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AEPackIEC61937.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AEStreamInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/AudioEngine/Utils/AEUtil.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/coff.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/dll.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/DllLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/DllLoaderContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/dll_tracker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/dll_tracker_file.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/dll_tracker_library.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/dll_util.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/LibraryLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/Win32DllLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/exports/emu_dummy.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/exports/emu_msvcrt.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/DllLoader/exports/util/EmuFileWrapper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/ExternalPlayer/ExternalPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerAutoSave.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerInput.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/RetroPlayerUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameRenderManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIGameVideoHandle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderHandle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderTarget.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guibridge/GUIRenderTargetFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guicontrols/GUIGameControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guicontrols/GUIRenderSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guiwindows/GameWindowFullScreen.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/guiwindows/GameWindowFullScreenText.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/process/windows/RPProcessInfoWin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderContext.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/RenderVideoSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/RPRenderManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPBaseRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPRendererGuiTexture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoRenderers/RPWinRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/RetroPlayer/rendering/VideoShaders/windows/RPWinOutputShader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/AudioSinkAE.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDClock.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxSPU.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDFileInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDMessage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDMessageQueue.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDOverlayContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDStreamInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/PTSTracker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/Edl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerAudio.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerRadioRDS.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerSubtitle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerTeletext.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoPlayerVideo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoReferenceClock.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDCodecUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/DVDFactoryCodec.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Audio/DVDAudioCodecPassthrough.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodec.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecSSA.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecText.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayCodecTX3G.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/contrib/cc_decoder.c
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Overlay/contrib/cc_decoder708.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/AddonVideoCodec.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodec.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoCodecFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DVDVideoPPFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDCodecs/Video/DXVA.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DemuxMultiSource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemux.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxBXA.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxCC.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxCDDA.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxClient.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDDemuxVobsub.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDDemuxers/DVDFactoryDemuxer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDFactoryInputStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamMemory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamNavigator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamStack.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDStateSerializer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamAddon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/InputStreamMultiSource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDInputStreams/DVDInputStreamBluray.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDFactorySubtitle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleLineCollection.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserMicroDVD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserMPL2.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSami.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSubrip.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserVplayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitlesLibass.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleParserSSA.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleTagMicroDVD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/DVDSubtitles/DVDSubtitleTagSami.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/Process/ProcessInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/Process/VideoBuffer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/Process/windows/ProcessInfoWin.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/BaseRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/ColorManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererGUI.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererUtil.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderCapture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderFlags.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/RenderManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/DebugRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/WinRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/WinRenderBuffer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/OverlayRendererDX.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/HwDecRender/DXVAHD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ConvolutionKernels.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/ConversionMatrix.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/VideoPlayer/VideoRenderers/VideoShaders/WinVideoFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/paplayer/AudioDecoder.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/paplayer/CodecFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/paplayer/PAPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/paplayer/VideoPlayerCodec.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/playercorefactory/PlayerCoreFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/cores/playercorefactory/PlayerSelectionRule.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dbwrappers/Database.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dbwrappers/DatabaseQuery.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dbwrappers/dataset.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dbwrappers/qry_dat.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dbwrappers/sqlitedataset.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dbwrappers/mysqldataset.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogBoxBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogBusy.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogButtonMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogContextMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogExtendedProgressBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogFileBrowser.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogGamepad.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogKaiToast.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogKeyboardGeneric.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogKeyboardTouch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogMediaFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogMediaSource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogNumeric.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogOK.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogPlayEject.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogPlayerControls.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogPlayerProcessInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogProgress.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSeekBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSelect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSimpleMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSlider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSmartPlaylistEditor.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSmartPlaylistRule.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogSubMenu.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogTextViewer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogVolumeBar.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/dialogs/GUIDialogYesNo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/AddonEvent.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/AddonManagementEvent.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/BaseEvent.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/EventLog.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/EventLogManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/MediaLibraryEvent.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/windows/GUIViewStateEventLog.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/events/windows/GUIWindowEventLog.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/favourites/GUIDialogFavourites.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/favourites/FavouritesService.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/AddonsDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/AudioBookFileDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/CacheStrategy.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/CircularCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/CurlFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DAVCommon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DAVDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DAVFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DirectoryCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/Directory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DirectoryFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DirectoryHistory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/DllLibCurl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/EventsDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/FavouritesDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/FileCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/FileDirectoryFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/FileFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/FTPDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/FTPParse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/HTTPDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/IDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/IFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ImageFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/iso9660.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ISO9660Directory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ISOFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/LibraryDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MultiPathDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MultiPathFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicFileDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicSearchDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/OverrideDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/OverrideFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/PipeFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/PipesManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/PlaylistDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/PlaylistFileDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/PluginDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/PVRDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ResourceDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ResourceFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/RSSDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ShoutcastFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/SmartPlaylistDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/SourcesDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/SpecialProtocol.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/SpecialProtocolDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/SpecialProtocolFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/StackDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/udf25.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/UDFDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/UDFFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VirtualDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/XbtDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/XbtFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/XbtManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ZeroconfDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ZipDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ZipFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/ZipManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/BlurayDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/BlurayFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/CDDADirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/CDDAFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/NFSDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/NFSFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/NptXbmcFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/UPnPDirectory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/UPnPFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumCompilations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumCompilationsSongs.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbum.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyAdded.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyAddedSong.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyPlayed.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumRecentlyPlayedSong.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumTop100.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeAlbumTop100Song.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeArtist.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSingles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSong.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeSongTop100.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeTop100.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeYearAlbum.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/MusicDatabaseDirectory/DirectoryNodeYearSong.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeEpisodes.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeInProgressTvShows.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeMoviesOverview.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeMusicVideosOverview.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedEpisodes.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedMovies.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRecentlyAddedMusicVideos.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeRoot.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeSeasons.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleMovies.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleMusicVideos.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTitleTvShows.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/filesystem/VideoDatabaseDirectory/DirectoryNodeTvShowsOverview.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/GameServices.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/GameUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/GameClient.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/GameClientProperties.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/GameClientSubsystem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/GameClientTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientDevice.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientHardware.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientInput.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientJoystick.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientKeyboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientMouse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientPort.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/addons/input/GameClientTopology.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/Controller.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/ControllerFeature.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/ControllerLayout.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/ControllerManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/ControllerTopology.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/ControllerTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogAxisDetection.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogButtonCapture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/dialogs/GUIDialogIgnoreInput.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUICardinalFeatureButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIControllerButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureControls.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIFeatureTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIGameController.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIScalarFeatureButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUISelectKeyButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIThrottleButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/guicontrols/GUIWheelButton.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/types/ControllerGrid.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/types/ControllerTree.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/windows/GUIConfigurationWizard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/windows/GUIControllerList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/windows/GUIControllerWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/controllers/windows/GUIFeatureList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/dialogs/GUIDialogSelectGameClient.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/dialogs/osd/DialogGameOSD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/dialogs/osd/DialogGameVideoSelect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/dialogs/osd/DialogGameVolume.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/ports/InputSink.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/ports/Port.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/tags/GameInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/windows/GUIViewStateWindowGames.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/games/windows/GUIWindowGames.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/DDSImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/DirtyRegionSolvers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/DirtyRegionTracker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/FFmpegImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIAction.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIAudioManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIBaseContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIBorderedImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIButtonControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIColorManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIControlFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIControlGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIControlGroupList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIControlLookup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIControlProfiler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIDialog.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIEditControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFadeLabelControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFixedListContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFont.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFontCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFontManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFontTTF.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIIncludes.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIKeyboardFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUILabelControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUILabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIListContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIListGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIListItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIListItemLayout.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIListLabel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIMessage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIMoverControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIMultiImage.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIPanelContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIProgressControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIRadioButtonControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIRenderingControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIResizeControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIRSSControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIScrollBarControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUISettingsSliderControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUISliderControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUISpinControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUISpinControlEx.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIStaticItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUITextBox.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUITextLayout.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUITexture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIToggleButtonControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIVideoControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIVisualisationControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIWindowManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIWrappingListContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/imagefactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/IWindowManagerCallback.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/LocalizeStrings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/StereoscopicsManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/TextureBundle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/TextureBundleXBT.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/Texture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/TextureManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/VisibleEffect.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/XBTF.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/XBTFReader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/D3DResource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/DirectXGraphics.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIFontTTFDX.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUIShaderDX.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/GUITextureD3D.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/guilib/TextureDX.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/AppTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/ButtonTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/CustomControllerTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/GamepadTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InertialScrollingHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InputCodingTableBaiduPY.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InputCodingTableBasePY.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InputCodingTableFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InputCodingTableKorean.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InputManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/InputTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/IRTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/JoystickMapper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/Key.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/KeyboardLayout.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/KeyboardLayoutManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/KeyboardStat.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/KeyboardTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/Keymap.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/KeymapEnvironment.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/TouchTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/WindowKeymap.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/WindowTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/XBMC_keytable.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/DeadzoneFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/DriverPrimitive.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/JoystickEasterEgg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/JoystickMonitor.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/JoystickTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/JoystickUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/RumbleGenerator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/dialogs/GUIDialogNewJoystick.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/generic/ButtonMapping.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/generic/DriverReceiving.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/generic/FeatureHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/generic/InputHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/keymaps/KeyHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/keymaps/KeymapHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/joysticks/keymaps/KeymapHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/keyboard/KeyboardEasterEgg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/keyboard/KeymapActionMap.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/keyboard/generic/KeyboardInputHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/mouse/MouseStat.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/mouse/MouseTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/mouse/generic/MouseInputHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/touch/ITouchInputHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/touch/generic/GenericTouchActionHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/touch/generic/GenericTouchInputHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/touch/generic/GenericTouchPinchDetector.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/touch/generic/GenericTouchRotateDetector.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/input/touch/generic/GenericTouchSwipeDetector.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/AddonBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/ApplicationBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/Builtins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/CECBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/GUIBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/GUIControlBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/GUIContainerBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/LibraryBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/OpticalBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/PictureBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/PlayerBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/ProfileBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/PVRBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/SkinBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/SystemBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/builtins/WeatherBuiltins.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/generic/ILanguageInvoker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/generic/LanguageInvokerThread.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/generic/ScriptInvocationManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/info/InfoBool.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/info/InfoExpression.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/info/SkinVariable.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/AddonsOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/ApplicationOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/AudioLibrary.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/FavouritesOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/FileItemHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/FileOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/GUIOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/InputOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/JSONRPC.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/JSONServiceDescription.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/PlayerOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/PlaylistOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/ProfilesOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/PVROperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/SettingsOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/SystemOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/TextureOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/VideoLibrary.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/json-rpc/XBMCOperations.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiErrorStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiInputStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiResponseBody.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/legacy/wsgi/WsgiResponse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/AddonPythonInvoker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/CallbackHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/ContextItemAddonInvoker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/LanguageHook.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/PythonInvoker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/XBPython.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/swig.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/interfaces/python/PyContext.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/listproviders/DirectoryProvider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/listproviders/IListProvider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/listproviders/MultiProvider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/listproviders/StaticProvider.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/media/MediaType.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/messaging/ApplicationMessenger.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/messaging/helpers/DialogHelper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/messaging/helpers/DialogOKHelper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/Album.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/Artist.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/GUIViewStateMusic.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/MusicDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/MusicDbUrl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/MusicInfoLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/MusicLibraryQueue.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/MusicThumbLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/Song.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/dialogs/GUIDialogInfoProviderSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/dialogs/GUIDialogMusicInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/dialogs/GUIDialogMusicOSD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/dialogs/GUIDialogSongInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/dialogs/GUIDialogVisualisationPresetList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/infoscanner/MusicAlbumInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/infoscanner/MusicArtistInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/infoscanner/MusicInfoScanner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/infoscanner/MusicInfoScraper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/jobs/MusicLibraryJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/jobs/MusicLibraryProgressJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/jobs/MusicLibraryCleaningJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/jobs/MusicLibraryExportJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/jobs/MusicLibraryScanningJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/MusicInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderCDDA.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/MusicInfoTagLoaderShn.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/ReplayGain.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/TagLibVFSStream.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/tags/TagLoaderTagLib.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/windows/GUIWindowMusicBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/windows/GUIWindowMusicNav.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/windows/GUIWindowMusicPlaylist.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/windows/GUIWindowMusicPlaylistEditor.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/windows/GUIWindowVisualisation.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/music/windows/MusicFileItemListModifier.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/DNSNameCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/EventClient.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/EventPacket.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/EventServer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/GUIDialogAccessPoints.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/GUIDialogNetworkSetup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/Network.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/NetworkServices.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/Socket.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/TCPServer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/UdpClient.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/WakeOnAccess.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/ZeroconfBrowser.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/Zeroconf.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/cddb.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/AirPlayServer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/AirTunesServer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/WebServer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/dacp/dacp.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPFileHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPImageHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPImageTransformationHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPJsonRpcHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPRequestHandlerUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPVfsHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPWebinterfaceAddonsHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPWebinterfaceHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/IHTTPRequestHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/HTTPPythonHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonInvoker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/httprequesthandler/python/HTTPPythonWsgiInvoker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/mdns/ZeroconfBrowserMDNS.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/mdns/ZeroconfMDNS.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/upnp/UPnP.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/upnp/UPnPInternal.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/upnp/UPnPPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/upnp/UPnPRenderer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/upnp/UPnPServer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/upnp/UPnPSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/websocket/WebSocket.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/websocket/WebSocketManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/websocket/WebSocketV13.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/network/websocket/WebSocketV8.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/EventPollHandle.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/EventScanner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/Peripherals.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/addons/AddonButtonMap.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/addons/AddonButtonMapping.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/addons/AddonInputHandling.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/addons/PeripheralAddon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/addons/PeripheralAddonTranslator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/bus/PeripheralBus.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusAddon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusApplication.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/bus/virtual/PeripheralBusCEC.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/Peripheral.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralBluetooth.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralDisk.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralHID.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralImon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralJoystick.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralKeyboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralMouse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralNIC.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralNyxboard.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralTuner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/devices/PeripheralCecAdapter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/dialogs/GUIDialogPeripherals.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/peripherals/dialogs/GUIDialogPeripheralSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/ExifParse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/GUIDialogPictureInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/GUIViewStatePictures.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/GUIWindowPictures.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/GUIWindowSlideShow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/IptcParse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/JpegParse.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/libexif.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/Picture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/PictureInfoLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/PictureInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/PictureScalingAlgorithm.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/PictureThumbLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pictures/SlideShowPicture.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListB4S.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListM3U.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListPLS.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListURL.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListWPL.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/PlayListXML.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/SmartPlayList.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/playlists/SmartPlaylistFileItemListModifier.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/profiles/Profile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/profiles/dialogs/GUIDialogLockSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/profiles/dialogs/GUIDialogProfileSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/profiles/windows/GUIWindowSettingsProfile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/programs/GUIViewStatePrograms.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/programs/GUIWindowPrograms.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRActionListener.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRGUIInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRContextMenus.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRGUIActions.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRChannelNumberInputHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRJobs.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRGUIChannelNavigator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/PVRGUIProgressHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRChannel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRChannelGroup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRChannelGroupInternal.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRChannelGroups.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRChannelGroupsContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRChannelNumber.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/channels/PVRRadioRDSInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelsOSD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGroupManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGuideInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRChannelGuide.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRGuideSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRadioRDSInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRecordingInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRRecordingSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRTimerSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/dialogs/GUIDialogPVRClientPriorities.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/epg/EpgContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/epg/Epg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/epg/EpgDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/epg/EpgInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/epg/EpgSearchFilter.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/recordings/PVRRecording.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/recordings/PVRRecordings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/recordings/PVRRecordingsPath.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/timers/PVRTimerInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/timers/PVRTimers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/timers/PVRTimerType.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIViewStatePVR.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRChannels.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRGuide.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRRecordings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRSearch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimers.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimersBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIWindowPVRTimerRules.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIEPGGridContainer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/pvr/windows/GUIEPGGridContainerModel.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/rendering/RenderSystem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/rendering/dx/DeviceResources.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/rendering/dx/RenderSystemDX.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/AdvancedSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/DisplaySettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/GameSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/LibExportSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/MediaSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/MediaSourceSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingAddon.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingCreator.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingDateTime.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingPath.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/Settings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingsBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SettingUtils.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/SkinSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/DiscSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/dialogs/GUIDialogContentSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/dialogs/GUIDialogLibExportSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsManagerBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/dialogs/GUIDialogSettingsManualBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/windows/GUIControlSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/windows/GUIWindowSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/windows/GUIWindowSettingsCategory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/settings/windows/GUIWindowSettingsScreenCalibration.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/Bookmark.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/GUIViewStateVideo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/PlayerController.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/Teletext.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoDbUrl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoInfoDownloader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoInfoScanner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoInfoTag.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoLibraryQueue.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/VideoThumbLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/ViewModeSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogFullScreenInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogSubtitles.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogTeletext.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogVideoBookmarks.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogVideoInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogVideoOSD.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogVideoSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/dialogs/GUIDialogCMSSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryCleaningJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryMarkWatchedJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryProgressJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryRefreshingJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryScanningJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/jobs/VideoLibraryResetResumePointJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/tags/VideoInfoTagLoaderFactory.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/tags/VideoTagLoaderFFmpeg.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/tags/VideoTagLoaderNFO.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/windows/GUIWindowFullScreen.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/windows/GUIWindowVideoBase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/windows/GUIWindowVideoNav.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/windows/GUIWindowVideoPlaylist.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/video/windows/VideoFileItemListModifier.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/view/GUIViewControl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/view/GUIViewState.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/view/ViewDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/view/ViewStateSettings.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/weather/GUIWindowWeather.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/weather/WeatherJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/weather/WeatherManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windowing/OSScreenSaver.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windowing/WinSystem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windowing/windows/VideoSyncD3D.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windowing/windows/WinEventsWin32.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windowing/windows/WinSystemWin32.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windowing/windows/WinSystemWin32DX.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIMediaWindow.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowDebugInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowFileManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowHome.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowLoginScreen.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowPointer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowScreensaver.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowScreensaverDim.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowSplash.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowStartup.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/windows/GUIWindowSystemInfo.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/Application.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ApplicationPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ApplicationStackHelper.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/AppParamParser.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/AutoSwitch.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/BackgroundInfoLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ContextMenuItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ContextMenuManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ContextMenus.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/CueDocument.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/DatabaseManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/DbUrl.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/DynamicDll.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/FileItem.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/FileItemListModification.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/GUIInfoManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/GUILargeTextureManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/GUIPassword.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/InfoScanner.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/MediaSource.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/NfoFile.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/PasswordManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/PlayListPlayer.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/PartyModeManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/SectionLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/SeekHandler.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ServiceBroker.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ServiceManager.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/SystemGlobals.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/TextureCache.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/TextureCacheJob.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/TextureDatabase.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/ThumbLoader.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/XBApplicationEx.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/XBDateTime.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
  set_property(SOURCE ../../xbmc/xbmc/Autorun.cpp
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:>")
endif ()
