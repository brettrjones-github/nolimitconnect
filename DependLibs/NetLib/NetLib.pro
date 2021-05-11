#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T19:53:30
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = NetLib
TEMPLATE = lib
CONFIG += staticlib

NetLib.depends += CoreLib
NetLib.depends += PktLib

win32 {
    CharacterSet=2
    DEFINES -= UNICODE
    DEFINES += "TARGET_OS_WINDOWS"
    DEFINES += "WIN32"
    DEFINES += "_WINDOWS"
    DEFINES += "_CRT_SECURE_NO_WARNINGS"
    debug {
        DEFINES += "DEBUG"
    }
    release {
        DEFINES += "RELEASE"
    }
}

#unix:!android {
#    QMAKE_CXXFLAGS += -Wno-unused-parameter
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

INCLUDEPATH += ../
 
HEADERS += ./AnchorSetting.h \
	InetAddress.h \
	NetSettings.h \
	VxAnyRandomPort.h \ 
	VxBaseSktInfo.h \
	VxClientMgr.h \
	VxSktBase.h \
	VxSktBaseMgr.h \
	VxSktBuf.h \
    VxSktWaitingList.h \
    VxSktUtil.h \
    VxSktUdpBroadcast.h \
    VxSktUdp.h \
    VxSktThrottle.h \
    VxSktRandomData.h \
    VxSktInfo.h \
    VxSktDefs.h \
    VxSktCrypto.h \
    VxSktConnectSimple.h \
    VxSktConnect.h \
    VxSktAccept.h \
    VxServerMgr.h \
    VxResolveHost.h \
    VxProxy.h \
    VxPeerMgr.h \
    VxParseUrlComponents.h \
    VxHttpValuePair.h \
    VxHttpRequest.h \
    VxHttpParse.h \
    VxGetRandomPort.h \
    VxGetExternalIps.h \
    VxFileXferInfo.h \
	VxUrl.h

SOURCES += ./AnchorSetting.cpp \
    InetAddress.cpp \
	NetSettings.cpp \
	VxBaseSktInfo.cpp \
    VxClientMgr.cpp \
	VxFileXferInfo.cpp \
	VxSktBase.cpp \
	VxSktBaseMgr.cpp \
	VxSktBuf.cpp \
    VxSktWaitingList.cpp \
    VxSktUtil.cpp \
    VxSktUdpBroadcast.cpp \
    VxSktUdp.cpp \
    VxSktThrottle.cpp \
    VxSktCrypto.cpp \
    VxSktConnectSimple.cpp \
    VxSktConnect.cpp \
    VxSktAccept.cpp \
    VxServerMgr.cpp \
    VxResolveHost.cpp \
    VxProxy.cpp \
    VxPeerMgr.cpp \
    VxParseUrlComponents.cpp \
    VxHttpValuePair.cpp \
    VxHttpRequest.cpp \
    VxHttpParse.cpp \
    VxGetRandomPort.cpp \
    VxGetExternalIps.cpp \
    VxFileXferInfo.cpp \
	VxUrl.cpp
