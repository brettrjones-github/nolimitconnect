INCLUDEPATH += $$PWD/../../xbmc/xbmc/
INCLUDEPATH += $$PWD/../../../DependLibs/
INCLUDEPATH += $$PWD/../../../DependLibs/openssl-1.0.2o/inc32


HEADERS += 	$$PWD/../../../GoTvCompilerConfig.h \
	$$PWD/../../../GoTvCpuArchDefines.h \
	$$PWD/../../../GoTvDependLibrariesConfig.h \
	$$PWD/../../../GoTvInterface/IDefs.h \
	$$PWD/../../../GoTvInterface/IFromGui.h \
	$$PWD/../../../GoTvInterface/IGoTv.h \
	$$PWD/../../../GoTvInterface/IGoTvDefs.h \
	$$PWD/../../../GoTvInterface/ILog.h \
	$$PWD/../../../GoTvInterface/IScan.h \
	$$PWD/../../../GoTvInterface/IToGui.h \
	$$PWD/../../../GoTvInterface/config_libgotvptop.h \
	$$PWD/../../../GoTvTargetOsConfig.h \
	$$PWD/../../GoTvP2P/Anchor/AnchorDb.h \
	$$PWD/../../GoTvP2P/Anchor/AnchorDefs.h \
	$$PWD/../../GoTvP2P/Anchor/AnchorList.h \
	$$PWD/../../GoTvP2P/Anchor/AnchorListEntry.h \
	$$PWD/../../GoTvP2P/Anchor/PhoneShakeEntry.h \
	$$PWD/../../GoTvP2P/AnchorTest/AnchorTest.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetCallbackInterface.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetInfo.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetInfoDb.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetMgr.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetMgrBase.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetRxSession.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetTxSession.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetXferDb.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetXferMgr.h \
	$$PWD/../../GoTvP2P/AssetMgr/AssetXferSession.h \
	$$PWD/../../GoTvP2P/BigListLib/BigList.h \
	$$PWD/../../GoTvP2P/BigListLib/BigListDb.h \
	$$PWD/../../GoTvP2P/BigListLib/BigListInfo.h \
	$$PWD/../../GoTvP2P/BigListLib/BigListLib.h \
	$$PWD/../../GoTvP2P/BigListLib/BigListMgr.h \
	$$PWD/../../GoTvP2P/EchoCancel/WebRtcTypeDefs.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_common.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_core.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_core_internal.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_logging.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_logging_file_handling.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_rdft.h \
	$$PWD/../../GoTvP2P/EchoCancel/aec_resampler.h \
	$$PWD/../../GoTvP2P/EchoCancel/array_view.h \
	$$PWD/../../GoTvP2P/EchoCancel/arraysize.h \
	$$PWD/../../GoTvP2P/EchoCancel/atomic32.h \
	$$PWD/../../GoTvP2P/EchoCancel/atomicops.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device_buffer.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device_config.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device_defines.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device_generic.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_processing.h \
	$$PWD/../../GoTvP2P/EchoCancel/audio_util.h \
	$$PWD/../../GoTvP2P/EchoCancel/base64.h \
	$$PWD/../../GoTvP2P/EchoCancel/basictypes.h \
	$$PWD/../../GoTvP2P/EchoCancel/block_mean_calculator.h \
	$$PWD/../../GoTvP2P/EchoCancel/byteorder.h \
	$$PWD/../../GoTvP2P/EchoCancel/checks.h \
	$$PWD/../../GoTvP2P/EchoCancel/common_types.h \
	$$PWD/../../GoTvP2P/EchoCancel/constructormagic.h \
	$$PWD/../../GoTvP2P/EchoCancel/cpu_features_wrapper.h \
	$$PWD/../../GoTvP2P/EchoCancel/critical_section_wrapper.h \
	$$PWD/../../GoTvP2P/EchoCancel/criticalsection.h \
	$$PWD/../../GoTvP2P/EchoCancel/delay_estimator.h \
	$$PWD/../../GoTvP2P/EchoCancel/delay_estimator_internal.h \
	$$PWD/../../GoTvP2P/EchoCancel/delay_estimator_wrapper.h \
	$$PWD/../../GoTvP2P/EchoCancel/echo_cancellation.h \
	$$PWD/../../GoTvP2P/EchoCancel/echo_cancellation_internal.h \
	$$PWD/../../GoTvP2P/EchoCancel/echo_common.h \
	$$PWD/../../GoTvP2P/EchoCancel/event.h \
	$$PWD/../../GoTvP2P/EchoCancel/event_timer_posix.h \
	$$PWD/../../GoTvP2P/EchoCancel/event_timer_win.h \
	$$PWD/../../GoTvP2P/EchoCancel/event_wrapper.h \
	$$PWD/../../GoTvP2P/EchoCancel/fake_audio_device.h \
	$$PWD/../../GoTvP2P/EchoCancel/fileutils.h \
	$$PWD/../../GoTvP2P/EchoCancel/fine_audio_buffer.h \
	$$PWD/../../GoTvP2P/EchoCancel/format_macros.h \
	$$PWD/../../GoTvP2P/EchoCancel/logging.h \
	$$PWD/../../GoTvP2P/EchoCancel/module.h \
	$$PWD/../../GoTvP2P/EchoCancel/module_common_types.h \
	$$PWD/../../GoTvP2P/EchoCancel/pathutils.h \
	$$PWD/../../GoTvP2P/EchoCancel/platform_file.h \
	$$PWD/../../GoTvP2P/EchoCancel/platform_thread.h \
	$$PWD/../../GoTvP2P/EchoCancel/platform_thread_types.h \
	$$PWD/../../GoTvP2P/EchoCancel/random.h \
	$$PWD/../../GoTvP2P/EchoCancel/real_fft.h \
	$$PWD/../../GoTvP2P/EchoCancel/ring_buffer.h \
	$$PWD/../../GoTvP2P/EchoCancel/safe_conversions.h \
	$$PWD/../../GoTvP2P/EchoCancel/safe_conversions_impl.h \
	$$PWD/../../GoTvP2P/EchoCancel/scoped_ptr.h \
	$$PWD/../../GoTvP2P/EchoCancel/scoped_ref_ptr.h \
	$$PWD/../../GoTvP2P/EchoCancel/scopedptrcollection.h \
	$$PWD/../../GoTvP2P/EchoCancel/sec_buffer.h \
	$$PWD/../../GoTvP2P/EchoCancel/signal_processing_library.h \
	$$PWD/../../GoTvP2P/EchoCancel/spl_inl.h \
	$$PWD/../../GoTvP2P/EchoCancel/spl_inl_armv7.h \
	$$PWD/../../GoTvP2P/EchoCancel/spl_inl_mips.h \
	$$PWD/../../GoTvP2P/EchoCancel/stringencode.h \
	$$PWD/../../GoTvP2P/EchoCancel/stringutils.h \
	$$PWD/../../GoTvP2P/EchoCancel/template_util.h \
	$$PWD/../../GoTvP2P/EchoCancel/thread_annotations.h \
	$$PWD/../../GoTvP2P/EchoCancel/thread_checker.h \
	$$PWD/../../GoTvP2P/EchoCancel/thread_checker_impl.h \
	$$PWD/../../GoTvP2P/EchoCancel/tick_util.h \
	$$PWD/../../GoTvP2P/EchoCancel/timeutils.h \
	$$PWD/../../GoTvP2P/EchoCancel/urlencode.h \
	$$PWD/../../GoTvP2P/EchoCancel/wav_file.h \
	$$PWD/../../GoTvP2P/EchoCancel/wav_header.h \
	$$PWD/../../GoTvP2P/EchoCancel/win32.h \
	$$PWD/../../GoTvP2P/EchoCancel/win32filesystem.h \
	$$PWD/../../GoTvP2P/Groups/GroupDefs.h \
	$$PWD/../../GoTvP2P/IsPortOpenTest/IsPortOpenTest.h \
	$$PWD/../../GoTvP2P/IsPortOpenTest/PingResponseServer.h \
	$$PWD/../../GoTvP2P/MediaProcessor/AudioUtil.h \
	$$PWD/../../GoTvP2P/MediaProcessor/EchoCancel.h \
	$$PWD/../../GoTvP2P/MediaProcessor/MediaClient.h \
	$$PWD/../../GoTvP2P/MediaProcessor/MediaProcessor.h \
	$$PWD/../../GoTvP2P/MediaProcessor/RawAudio.h \
	$$PWD/../../GoTvP2P/MediaProcessor/RawVideo.h \
	$$PWD/../../GoTvP2P/NetServices/NetActionAnnounce.h \
	$$PWD/../../GoTvP2P/NetServices/NetActionBase.h \
	$$PWD/../../GoTvP2P/NetServices/NetActionIdle.h \
	$$PWD/../../GoTvP2P/NetServices/NetActionIsMyPortOpen.h \
	$$PWD/../../GoTvP2P/NetServices/NetActionPhoneShake.h \
	$$PWD/../../GoTvP2P/NetServices/NetActionQueryMyIp.h \
	$$PWD/../../GoTvP2P/NetServices/NetServiceAnchor.h \
	$$PWD/../../GoTvP2P/NetServices/NetServiceDefs.h \
	$$PWD/../../GoTvP2P/NetServices/NetServiceHdr.h \
	$$PWD/../../GoTvP2P/NetServices/NetServiceUtils.h \
	$$PWD/../../GoTvP2P/NetServices/NetServicesMgr.h \
	$$PWD/../../GoTvP2P/Network/ConnectRequest.h \
	$$PWD/../../GoTvP2P/Network/DirectConnectTester.h \
	$$PWD/../../GoTvP2P/Network/Firewall.h \
	$$PWD/../../GoTvP2P/Network/NetConnector.h \
	$$PWD/../../GoTvP2P/Network/NetPortForward.h \
	$$PWD/../../GoTvP2P/Network/NetworkDefs.h \
	$$PWD/../../GoTvP2P/Network/NetworkEventAvail.h \
	$$PWD/../../GoTvP2P/Network/NetworkEventBase.h \
	$$PWD/../../GoTvP2P/Network/NetworkEventLost.h \
	$$PWD/../../GoTvP2P/Network/NetworkMgr.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateAnnounce.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateAvail.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateBase.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateGetRelayList.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateLost.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateMachine.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateOnlineDirect.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateOnlineThroughRelay.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateRelaySearch.h \
	$$PWD/../../GoTvP2P/Network/NetworkStateTestConnection.h \
	$$PWD/../../GoTvP2P/Network/RcMulticastBase.h \
	$$PWD/../../GoTvP2P/Network/RcMulticastBroadcast.h \
	$$PWD/../../GoTvP2P/Network/RcMulticastListen.h \
	$$PWD/../../GoTvP2P/NetworkMonitor/NetworkMonitor.h \
	$$PWD/../../GoTvP2P/P2PEngine/BrowserHttpConnection.h \
	$$PWD/../../GoTvP2P/P2PEngine/ContactList.h \
	$$PWD/../../GoTvP2P/P2PEngine/EngineParams.h \
	$$PWD/../../GoTvP2P/P2PEngine/EngineSettings.h \
	$$PWD/../../GoTvP2P/P2PEngine/EngineSettingsDefaultValues.h \
	$$PWD/../../GoTvP2P/P2PEngine/FileShareSettings.h \
	$$PWD/../../GoTvP2P/P2PEngine/P2PConnectList.h \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngine.h \
	$$PWD/../../GoTvP2P/Plugins/AudioJitterBuffer.h \
	$$PWD/../../GoTvP2P/Plugins/FileLibraryDb.h \
	$$PWD/../../GoTvP2P/Plugins/FileLibraryMgr.h \
	$$PWD/../../GoTvP2P/Plugins/FileRxSession.h \
	$$PWD/../../GoTvP2P/Plugins/FileShareXferMgr.h \
	$$PWD/../../GoTvP2P/Plugins/FileShareXferSession.h \
	$$PWD/../../GoTvP2P/Plugins/FileToXfer.h \
	$$PWD/../../GoTvP2P/Plugins/FileTxSession.h \
	$$PWD/../../GoTvP2P/Plugins/FileXferBaseMgr.h \
	$$PWD/../../GoTvP2P/Plugins/FileXferMgr.h \
	$$PWD/../../GoTvP2P/Plugins/FileXferPluginSession.h \
	$$PWD/../../GoTvP2P/Plugins/LibraryFileInfo.h \
	$$PWD/../../GoTvP2P/Plugins/P2PSession.h \
	$$PWD/../../GoTvP2P/Plugins/PluginBase.h \
	$$PWD/../../GoTvP2P/Plugins/PluginCamServer.h \
	$$PWD/../../GoTvP2P/Plugins/PluginFileOffer.h \
	$$PWD/../../GoTvP2P/Plugins/PluginFileShare.h \
	$$PWD/../../GoTvP2P/Plugins/PluginInvalid.h \
	$$PWD/../../GoTvP2P/Plugins/PluginMgr.h \
	$$PWD/../../GoTvP2P/Plugins/PluginMultiSession.h \
	$$PWD/../../GoTvP2P/Plugins/PluginNetServices.h \
	$$PWD/../../GoTvP2P/Plugins/PluginRelay.h \
	$$PWD/../../GoTvP2P/Plugins/PluginSessionBase.h \
	$$PWD/../../GoTvP2P/Plugins/PluginSessionMgr.h \
	$$PWD/../../GoTvP2P/Plugins/PluginStoryBoard.h \
	$$PWD/../../GoTvP2P/Plugins/PluginTruthOrDare.h \
	$$PWD/../../GoTvP2P/Plugins/PluginVideoPhone.h \
	$$PWD/../../GoTvP2P/Plugins/PluginVoicePhone.h \
	$$PWD/../../GoTvP2P/Plugins/PluginWebServer.h \
	$$PWD/../../GoTvP2P/Plugins/RcWebProxy.h \
	$$PWD/../../GoTvP2P/Plugins/Relay.h \
	$$PWD/../../GoTvP2P/Plugins/RelayClientSession.h \
	$$PWD/../../GoTvP2P/Plugins/RelayClientTestSession.h \
	$$PWD/../../GoTvP2P/Plugins/RelayServerSession.h \
	$$PWD/../../GoTvP2P/Plugins/RelayServerTestSession.h \
	$$PWD/../../GoTvP2P/Plugins/RelaySession.h \
	$$PWD/../../GoTvP2P/Plugins/RxSession.h \
	$$PWD/../../GoTvP2P/Plugins/SessionMgrBase.h \
	$$PWD/../../GoTvP2P/Plugins/SharedFileInfo.h \
	$$PWD/../../GoTvP2P/Plugins/SharedFilesDb.h \
	$$PWD/../../GoTvP2P/Plugins/SharedFilesFinder.h \
	$$PWD/../../GoTvP2P/Plugins/SharedFilesMgr.h \
	$$PWD/../../GoTvP2P/Plugins/TodGameSession.h \
	$$PWD/../../GoTvP2P/Plugins/TxSession.h \
	$$PWD/../../GoTvP2P/Plugins/VideoFeedMgr.h \
	$$PWD/../../GoTvP2P/Plugins/VoiceFeedMgr.h \
	$$PWD/../../GoTvP2P/Search/RcScan.h \
	$$PWD/../../GoTvP2P/Search/RcScanAction.h \
	$$PWD/../../GoTvP2P/Search/RcScanMatchedConnection.h \
	$$PWD/../../GoTvP2P/Search/RcScanPic.h \
	$$PWD/../../GoTvP2P/WebServer/RcWebPageBuilder.h \
	$$PWD/../../GoTvP2P/WebServer/RcWebPageSettings.h \
	$$PWD/../../GoTvP2P/WebServer/RcWebServer.h \
	$$PWD/../../GoTvP2P/WebServer/RcWebServerLib.h \
	$$PWD/../../GoTvP2P/WebServer/RcWebSkt.h \
	$$PWD/../../config_gotvcore.h

SOURCES += 	$$PWD/../../GoTvP2P/Anchor/AnchorDb.cpp \
	$$PWD/../../GoTvP2P/Anchor/AnchorList.cpp \
	$$PWD/../../GoTvP2P/Anchor/AnchorListEntry.cpp \
	$$PWD/../../GoTvP2P/Anchor/PhoneShakeEntry.cpp \
	$$PWD/../../GoTvP2P/AnchorTest/AnchorTest.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetInfo.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetInfoDb.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetMgr.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetMgrBase.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetRxSession.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetTxSession.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetXferDb.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetXferMgr.cpp \
	$$PWD/../../GoTvP2P/AssetMgr/AssetXferSession.cpp \
	$$PWD/../../GoTvP2P/BigListLib/BigList.cpp \
	$$PWD/../../GoTvP2P/BigListLib/BigListDb.cpp \
	$$PWD/../../GoTvP2P/BigListLib/BigListInfo.cpp \
	$$PWD/../../GoTvP2P/BigListLib/BigListMgr.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_core.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_core_mips.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_core_neon.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_core_sse2.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_logging_file_handling.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_rdft.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_rdft_mips.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_rdft_neon.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_rdft_sse2.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/aec_resampler.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/atomic32_posix.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/atomic32_win.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device_buffer.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/audio_device_generic.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/audio_util.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/base64.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/block_mean_calculator.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/checks.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/common_types.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/cpu_features.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/cpu_features_android.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/cpu_features_linux.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/criticalsection.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/delay_estimator.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/delay_estimator_wrapper.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/echo_cancellation.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/event.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/event_timer_posix.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/event_timer_win.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/event_wrapper.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/fileutils.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/fine_audio_buffer.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/logging.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/pathutils.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/platform_file.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/platform_thread.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/random.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/randomization_functions.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/ring_buffer.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/stringencode.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/stringutils.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/thread_checker_impl.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/tick_util.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/timeutils.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/urlencode.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/wav_file.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/wav_header.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/win32.cpp \
	$$PWD/../../GoTvP2P/EchoCancel/win32filesystem.cpp \
	$$PWD/../../GoTvP2P/IsPortOpenTest/IsPortOpenTest.cpp \
	$$PWD/../../GoTvP2P/IsPortOpenTest/PingResponseServer.cpp \
	$$PWD/../../GoTvP2P/MediaProcessor/AudioUtil.cpp \
	$$PWD/../../GoTvP2P/MediaProcessor/EchoCancel.cpp \
	$$PWD/../../GoTvP2P/MediaProcessor/MediaProcessor.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetActionAnnounce.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetActionBase.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetActionIdle.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetActionIsMyPortOpen.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetActionPhoneShake.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetActionQueryMyIp.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetServiceAnchor.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetServiceDefs.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetServiceUtils.cpp \
	$$PWD/../../GoTvP2P/NetServices/NetServicesMgr.cpp \
	$$PWD/../../GoTvP2P/Network/ConnectRequest.cpp \
	$$PWD/../../GoTvP2P/Network/DirectConnectTester.cpp \
	$$PWD/../../GoTvP2P/Network/Firewall.cpp \
	$$PWD/../../GoTvP2P/Network/NetConnector.cpp \
	$$PWD/../../GoTvP2P/Network/NetPortForward.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkEventAvail.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkEventBase.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkEventLost.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkMgr.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateAnnounce.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateAvail.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateBase.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateGetRelayList.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateLost.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateMachine.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateOnlineDirect.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateOnlineThroughRelay.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateRelaySearch.cpp \
	$$PWD/../../GoTvP2P/Network/NetworkStateTestConnection.cpp \
	$$PWD/../../GoTvP2P/Network/RcMulticastBase.cpp \
	$$PWD/../../GoTvP2P/Network/RcMulticastBroadcast.cpp \
	$$PWD/../../GoTvP2P/Network/RcMulticastListen.cpp \
	$$PWD/../../GoTvP2P/NetworkMonitor/NetworkMonitor.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/BrowserHttpConnection.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/ContactList.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/EngineParams.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/EngineSettings.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/FileShareSettings.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PConnectList.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngine.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineAssetMgrCallbacks.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineConnect.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineConnections.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineContact.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineFromGui.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineIgnoredAndHackers.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineMediaProcessorCallbacks.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEnginePktAnnChanged.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEnginePktHandlers.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineRelay.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineScan.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineSearch.cpp \
	$$PWD/../../GoTvP2P/P2PEngine/P2PEngineSktCallback.cpp \
	$$PWD/../../GoTvP2P/Plugins/AudioJitterBuffer.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileLibraryDb.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileLibraryMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileRxSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileShareXferMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileShareXferSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileToXfer.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileTxSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileXferBaseMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileXferMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/FileXferPluginSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/LibraryFileInfo.cpp \
	$$PWD/../../GoTvP2P/Plugins/P2PSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginBase.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginCamServer.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginFileOffer.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginFileOfferPktHandlers.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginFileShare.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginInvalid.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginMgrSktApi.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginMultiSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginNetServices.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginRelay.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginRelayPktHandlers.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginRelayTests.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginSessionBase.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginSessionMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginStoryBoard.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginTruthOrDare.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginVideoPhone.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginVoicePhone.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginWebServer.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginWebServerFiles.cpp \
	$$PWD/../../GoTvP2P/Plugins/PluginWebServerPktHandlers.cpp \
	$$PWD/../../GoTvP2P/Plugins/RcWebProxy.cpp \
	$$PWD/../../GoTvP2P/Plugins/Relay.cpp \
	$$PWD/../../GoTvP2P/Plugins/RelayClientSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/RelayServerSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/RelaySession.cpp \
	$$PWD/../../GoTvP2P/Plugins/RxSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/SessionMgrBase.cpp \
	$$PWD/../../GoTvP2P/Plugins/SharedFileInfo.cpp \
	$$PWD/../../GoTvP2P/Plugins/SharedFilesDb.cpp \
	$$PWD/../../GoTvP2P/Plugins/SharedFilesFinder.cpp \
	$$PWD/../../GoTvP2P/Plugins/SharedFilesMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/TodGameSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/TxSession.cpp \
	$$PWD/../../GoTvP2P/Plugins/VideoFeedMgr.cpp \
	$$PWD/../../GoTvP2P/Plugins/VoiceFeedMgr.cpp \
	$$PWD/../../GoTvP2P/Search/RcScan.cpp \
	$$PWD/../../GoTvP2P/Search/RcScanAction.cpp \
	$$PWD/../../GoTvP2P/Search/RcScanMatchedConnection.cpp \
	$$PWD/../../GoTvP2P/WebServer/RcWebPageBuilder.cpp \
	$$PWD/../../GoTvP2P/WebServer/RcWebPageSettings.cpp \
	$$PWD/../../GoTvP2P/WebServer/RcWebServer.cpp \
	$$PWD/../../GoTvP2P/WebServer/RcWebServerHttp.cpp \
	$$PWD/../../GoTvP2P/WebServer/RcWebSkt.cpp
