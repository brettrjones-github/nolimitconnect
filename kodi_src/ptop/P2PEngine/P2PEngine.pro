#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T19:53:30
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = P2PEngine
TEMPLATE = lib
CONFIG += staticlib

P2PEngine.depends += CoreLib
P2PEngine.depends += NetLib
P2PEngine.depends += PktLib
P2PEngine.depends += Upnp2Lib
P2PEngine.depends += JpgLib
P2PEngine.depends += SpeexLib

DEFINES += "OUTSIDE_SPEEX"
win32 {
    CharacterSet=2
    DEFINES -= UNICODE
    DEFINES += "TARGET_OS_WINDOWS"
    DEFINES += "WIN32"
    DEFINES += "_WINDOWS"
    DEFINES += "_CRT_SECURE_NO_WARNINGS"
    DEFINES += "CPU_ARCH_X86_FAMILY"
    debug {
        DEFINES += "DEBUG"
    }
    release {
        DEFINES += "RELEASE"
    }
}

unix:!symbian {
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    debug {
        DEFINES += "DEBUG"
    }
    release {
        DEFINES += "RELEASE"
    }
    INSTALLS += target
}

INCLUDEPATH += ../
INCLUDEPATH += ../../../
INCLUDEPATH += ../../../MediaToolsLib
INCLUDEPATH += ../../../MediaToolsLib/include

SOURCES += ../Anchor/AnchorDb.cpp \
    ../Anchor/PhoneShakeEntry.cpp \
    ../Anchor/AnchorListEntry.cpp \
    ../Anchor/AnchorList.cpp \
    ../AnchorTest/AnchorTest.cpp \
    ../BigListLib/BigListMgr.cpp \
    ../BigListLib/BigListInfo.cpp \
    ../BigListLib/BigListDb.cpp \
    ../BigListLib/BigList.cpp \
    ../IP2PEngine/IToGui.cpp \
    ../IsPortOpenTest/PingResponseServer.cpp \
    ../IsPortOpenTest/IsPortOpenTest.cpp \
    ../NetServices/NetServiceUtils.cpp \
    ../NetServices/NetServicesMgr.cpp \
    ../NetServices/NetServiceDefs.cpp \
    ../NetServices/NetServiceAnchor.cpp \
    ../NetServices/NetActionQueryMyIp.cpp \
    ../NetServices/NetActionPhoneShake.cpp \
    ../NetServices/NetActionIsMyPortOpen.cpp \
    ../NetServices/NetActionIdle.cpp \
    ../NetServices/NetActionBase.cpp \
    ../NetServices/NetActionAnnounce.cpp \
    ../Network/RcMulticastListen.cpp \
    ../Network/RcMulticastBroadcast.cpp \
    ../Network/RcMulticastBase.cpp \
    ../Network/NetworkStateTestConnection.cpp \
    ../Network/NetworkStateRelaySearch.cpp \
	../Network/NetworkStateOnlineDirect.cpp \
	../Network/NetworkStateOnlineThroughRelay.cpp \
	../Network/NetworkStateMachine.cpp \
    ../Network/NetworkStateLost.cpp \
    ../Network/NetworkStateBase.cpp \
    ../Network/NetworkStateAvail.cpp \
    ../Network/NetworkStateAnnounce.cpp \
    ../Network/NetworkMgr.cpp \
    ../Network/NetworkEventLost.cpp \
    ../Network/NetworkEventBase.cpp \
    ../Network/NetworkEventAvail.cpp \
    ../Network/NetPortForward.cpp \
    ../Network/NetConnector.cpp \
    ../Network/Firewall.cpp \
    ../Network/DirectConnectTester.cpp \
	../Network/ConnectRequest.cpp \
    ../NetworkMonitor/NetworkMonitor.cpp \
	../Plugins/FileLibraryDb.cpp \
	../Plugins/FileLibraryMgr.cpp \
	../Plugins/FileRxSession.cpp \
	../Plugins/FileShareXferMgr.cpp \
	../Plugins/FileShareXferSession.cpp \
	../Plugins/FileToXfer.cpp \
	../Plugins/FileXferBaseMgr.cpp \
	../Plugins/FileXferMgr.cpp \
	../Plugins/FileTxSession.cpp \
	../Plugins/FileXferPluginSession.cpp \
	../Plugins/LibraryFileInfo.cpp \
	../Plugins/P2PSession.cpp \
	../Plugins/PluginWebServerPktHandlers.cpp \
	../Plugins/PluginWebServerFiles.cpp \
	../Plugins/PluginWebServer.cpp \
	../Plugins/PluginVoicePhone.cpp \
	../Plugins/PluginVideoPhone.cpp \
	../Plugins/PluginTruthOrDare.cpp \
	../Plugins/PluginStoryBoard.cpp \
	../Plugins/PluginSessionMgr.cpp \
	../Plugins/PluginSessionBase.cpp \
	../Plugins/PluginRelayTests.cpp \
	../Plugins/PluginRelayPktHandlers.cpp \
	../Plugins/PluginRelay.cpp \
	../Plugins/PluginNetServices.cpp \
	../Plugins/PluginMgrSktApi.cpp \
	../Plugins/PluginMgr.cpp \
	../Plugins/PluginInvalid.cpp \
	../Plugins/PluginFileShare.cpp \
	../Plugins/PluginFileOfferPktHandlers.cpp \
	../Plugins/PluginFileOffer.cpp \
	../Plugins/PluginCamServer.cpp \
	../Plugins/PluginBase.cpp \
	../Plugins/VoiceFeedMgr.cpp \
    ../Plugins/VideoFeedMgr.cpp \
    ../Plugins/TxSession.cpp \
    ../Plugins/TodGameSession.cpp \
    ../Plugins/SharedFilesFinder.cpp \
    ../Plugins/SharedFileInfo.cpp \
	../Plugins/SharedFilesDb.cpp \
	../Plugins/SharedFilesMgr.cpp \
    ../Plugins/SessionMgrBase.cpp \
    ../Plugins/RxSession.cpp \
    ../Plugins/RelaySession.cpp \
    ../Plugins/RelayServerSession.cpp \
    ../Plugins/RelayClientSession.cpp \
    ../Plugins/Relay.cpp \
    ../Plugins/RcWebProxy.cpp \
    ../Search/RcScanMatchedConnection.cpp \
    ../Search/RcScanAction.cpp \
    ../Search/RcScan.cpp \
    ../WebServer/RcWebSkt.cpp \
    ../WebServer/RcWebServerHttp.cpp \
    ../WebServer/RcWebServer.cpp \
    ../WebServer/RcWebPageSettings.cpp \
    ../WebServer/RcWebPageBuilder.cpp \
    BrowserHttpConnection.cpp \
    ContactList.cpp \
	EngineParams.cpp \
	EngineSettings.cpp \
    FileShareSettings.cpp \
    P2PConnectList.cpp \
    P2PEngine.cpp \
    P2PEngineAssetMgrCallbacks.cpp \
    P2PEngineContact.cpp \
    P2PEngineConnections.cpp \
    P2PEngineConnect.cpp \
    P2PEngineIgnoredAndHackers.cpp \
    P2PEngineFromGui.cpp \
    P2PEngineMediaProcessorCallbacks.cpp \
    P2PEnginePktHandlers.cpp \
    P2PEnginePktAnnChanged.cpp \
    P2PEngineSktCallback.cpp \
    P2PEngineSearch.cpp \
    P2PEngineScan.cpp \
    P2PEngineRelay.cpp \
    ../Network/NetworkStateGetRelayList.cpp \
    ../AssetMgr/AssetInfo.cpp \
    ../AssetMgr/AssetInfoDb.cpp \
    ../AssetMgr/AssetMgr.cpp \
    ../AssetMgr/AssetMgrBase.cpp \    
    ../AssetMgr/AssetXferMgr.cpp \
    ../AssetMgr/AssetXferDb.cpp \
    ../AssetMgr/AssetTxSession.cpp \
    ../AssetMgr/AssetRxSession.cpp \
    ../AssetMgr/AssetXferSession.cpp \  
    ../MediaProcessor/MediaProcessor.cpp \
    ../Plugins/AudioJitterBuffer.cpp \
    ../Plugins/PluginMultiSession.cpp

 
HEADERS += ../Anchor/AnchorDb.h \
    ../Anchor/PhoneShakeEntry.h \
    ../Anchor/AnchorListEntry.h \
    ../Anchor/AnchorList.h \
    ../Anchor/AnchorDefs.h \
    ../AnchorTest/AnchorTest.h \
    ../BigListLib/BigListMgr.h \
    ../BigListLib/BigListLib.h \
    ../BigListLib/BigListInfo.h \
    ../BigListLib/BigListDb.h \
    ../BigListLib/BigList.h \
    ../IP2PEngine/IToGui.h \
    ../IP2PEngine/IScan.h \
    ../GuiInterface/IFromGui.h \
    ../IP2PEngine/IDefs.h \
    ../IP2PEngine/IAppState.h \
    ../IsPortOpenTest/PingResponseServer.h \
    ../IsPortOpenTest/IsPortOpenTest.h \
    ../NetServices/NetServiceUtils.h \
    ../NetServices/NetServicesMgr.h \
    ../NetServices/NetServiceHdr.h \
    ../NetServices/NetServiceDefs.h \
    ../NetServices/NetServiceAnchor.h \
    ../NetServices/NetActionQueryMyIp.h \
    ../NetServices/NetActionPhoneShake.h \
    ../NetServices/NetActionIsMyPortOpen.h \
    ../NetServices/NetActionIdle.h \
    ../NetServices/NetActionBase.h \
    ../NetServices/NetActionAnnounce.h \
    ../Network/RcMulticastListen.h \
    ../Network/RcMulticastBroadcast.h \
    ../Network/RcMulticastBase.h \
    ../Network/NetworkStateTestConnection.h \
    ../Network/NetworkStateRelaySearch.h \
    ../Network/NetworkStateOnline.h \
    ../Network/NetworkStateMachine.h \
    ../Network/NetworkStateLost.h \
    ../Network/NetworkStateBase.h \
    ../Network/NetworkStateAvail.h \
    ../Network/NetworkStateAnnounce.h \
    ../Network/NetworkMgr.h \
    ../Network/NetworkEventLost.h \
    ../Network/NetworkEventBase.h \
    ../Network/NetworkEventAvail.h \
    ../Network/NetworkDefs.h \
    ../Network/NetPortForward.h \
    ../Network/NetConnector.h \
    ../Network/Firewall.h \
    ../Network/DirectConnectTester.h \
    ../Network/ConnectRequest.h \
    ../NetworkMonitor/NetworkMonitor.h \
    ../Plugins/VoiceFeedMgr.h \
    ../Plugins/VideoFeedMgr.h \
    ../Plugins/TxSession.h \
    ../Plugins/TodGameSession.h \
    ../Plugins/SharedFilesFinder.h \
    ../Plugins/SharedFileInfo.h \
	../Plugins/SharedFilesDb.h \
	../Plugins/SharedFilesMgr.h \
    ../Plugins/SessionMgrBase.h \
    ../Plugins/RxSession.h \
    ../Plugins/RelaySession.h \
    ../Plugins/RelayServerTestSession.h \
    ../Plugins/RelayServerSession.h \
    ../Plugins/RelayClientTestSession.h \
    ../Plugins/RelayClientSession.h \
    ../Plugins/Relay.h \
    ../Plugins/RcWebProxy.h \
    ../Plugins/PluginWebServer.h \
    ../Plugins/PluginVoicePhone.h \
    ../Plugins/PluginVideoPhone.h \
    ../Plugins/PluginTruthOrDare.h \
    ../Plugins/PluginStoryBoard.h \
    ../Plugins/PluginSessionMgr.h \
    ../Plugins/PluginSessionBase.h \
    ../Plugins/PluginSession.h \
    ../Plugins/PluginRelay.h \
    ../Plugins/PluginNetServices.h \
    ../Plugins/PluginMgr.h \
    ../Plugins/PluginInvalid.h \
    ../Plugins/PluginFileShare.h \
    ../Plugins/PluginFileOffer.h \
    ../Plugins/PluginCamServer.h \
    ../Plugins/PluginBase.h \
    ../Plugins/P2PSession.h \
    ../Plugins/FileXferPluginSession.h \
	../Plugins/FileXferBaseMgr.h \
    ../Plugins/FileXferMgr.h \
    ../Plugins/FileTxSession.h \
    ../Plugins/FileToXfer.h \
    ../Plugins/FileShareXferSession.h \
    ../Plugins/FileShareMgr.h \
    ../Plugins/FileRxSession.h \
    ../Search/RcScanMatchedConnection.h \
    ../Search/RcScanPic.h \
    ../Search/RcScanAction.h \
    ../Search/RcScan.h \
    ../WebServer/RcWebSkt.h \
    ../WebServer/RcWebServerLib.h \
    ../WebServer/RcWebServer.h \
    ../WebServer/RcWebPageSettings.h \
    ../WebServer/RcWebPageBuilder.h \
    BrowserHttpConnection.h \
    ContactList.h \
	EngineParams.h \
    EngineSettingsDefaultValues.h \
    EngineSettings.h \
    FileShareSettings.h \
    P2PEngine.h \
    P2PConnectList.h \
    ../Network/NetworkStateGetRelayList.h \
    ../AssetMgr/AssetCallbackInterface.h \
    ../AssetMgr/AssetInfo.h \
    ../AssetMgr/AssetInfoDb.h \
    ../AssetMgr/AssetMgr.h \
    ../AssetMgr/AssetMgrBase.h \
    ../AssetMgr/AssetXferMgr.cpp \
    ../AssetMgr/AssetXferDb.cpp \
    ../AssetMgr/AssetTxSession.cpp \
    ../AssetMgr/AssetRxSession.cpp \
    ../AssetMgr/AssetXferSession.cpp \    
    ../MediaProcessor/MediaClient.h \
    ../MediaProcessor/MediaProcessor.h \
    ../MediaProcessor/RawAudio.h \
    ../MediaProcessor/RawVideo.h \
    ../Plugins/AudioJitterBuffer.h \
    ../Plugins/FileLibraryDb.h \
    ../Plugins/FileLibraryMgr.h \
    ../Plugins/PluginMultiSession.h

 
