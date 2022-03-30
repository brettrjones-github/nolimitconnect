
INCLUDEPATH += $$PWD/DependLibs/libUPnP/
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Platinum/Source/Platinum
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Platinum/Source/Core
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Platinum/Source/Extras
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaConnect
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Neptune/Source/Core
win32{
INCLUDEPATH += $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32
}

HEADERS += 	$$PWD/AppCompilerConfig.h \
    $$PWD/AppCpuArchDefines.h \
    $$PWD/AppDependLibrariesConfig.h \
    $$PWD/AppTargetOsConfig.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/Neptune.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptArray.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptAutomaticCleaner.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptAutoreleasePool.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptBase64.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptBufferedStreams.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptCommon.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptConfig.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptConsole.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptConstants.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptCrypto.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDataBuffer.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDebug.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDefs.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDigest.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDynamicCast.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDynamicLibraries.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptFile.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptHash.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptHttp.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptInterfaces.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptJson.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptList.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptLogging.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptMap.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptMessaging.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptNetwork.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptQueue.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptReferences.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptResults.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptRingBuffer.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSelectableMessageQueue.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSerialPort.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSimpleMessageQueue.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSockets.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptStack.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptStreams.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptStrings.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSystem.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptThreads.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTime.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTls.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTlsDefaultTrustAnchorsBase.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTlsDefaultTrustAnchorsExtended.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTypes.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptUri.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptUtils.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptVersion.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptXml.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptZip.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32MessageQueue.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Network.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Threads.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltAction.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltArgument.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltConstants.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltCtrlPoint.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltCtrlPointTask.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltDatagramStream.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltDeviceData.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltDeviceHost.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltEvent.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttp.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttpClientTask.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttpServer.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttpServerTask.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltMimeType.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltProtocolInfo.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltService.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltSsdp.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltStateVariable.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltTaskManager.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltThreadTask.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltUPnP.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaConnect/PltMediaConnect.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaConnect/PltXbox360.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/PltMediaController.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/PltMediaRenderer.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltDidl.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltFileMediaServer.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaBrowser.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaCache.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaItem.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaServer.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltSyncMediaBrowser.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltDownloader.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltFrameBuffer.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltFrameServer.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltFrameStream.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltLeaks.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltMetadataHandler.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltRingBufferStream.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltStreamPump.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Platinum/Platinum.h \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Platinum/PltVersion.h \
    $$PWD/DependLibs/libUPnP/config_libupnp.h \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Bsd/NptBsdBlockerSocket.h

SOURCES += $$PWD/DependLibs/libUPnP/Neptune/Source/Core/Neptune.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptAutomaticCleaner.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptBase64.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptBufferedStreams.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptCommon.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptConsole.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptCrypto.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDataBuffer.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDebug.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDigest.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptDynamicLibraries.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptFile.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptHash.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptHttp.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptJson.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptList.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptLogging.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptMessaging.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptNetwork.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptQueue.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptResults.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptRingBuffer.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSimpleMessageQueue.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSockets.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptStreams.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptStrings.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptSystem.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptThreads.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTime.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptTls.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptUri.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptUtils.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptXml.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/Core/NptZip.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixEnvironment.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/StdC/NptStdcEnvironment.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/StdC/NptStdcFile.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltAction.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltArgument.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltConstants.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltCtrlPoint.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltCtrlPointTask.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltDatagramStream.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltDeviceData.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltDeviceHost.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltEvent.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttp.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttpClientTask.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttpServer.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltHttpServerTask.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltIconsData.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltMimeType.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltProtocolInfo.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltService.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltSsdp.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltStateVariable.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltTaskManager.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltThreadTask.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Core/PltUPnP.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaConnect/PltMediaConnect.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaConnect/PltXbox360.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaConnect/X_MS_MediaReceiverRegistrarSCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/AVTransportSCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/PltMediaController.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/PltMediaRenderer.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/RdrConnectionManagerSCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaRenderer/RenderingControlSCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/ConnectionManagerSCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/ContentDirectorySCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/ContentDirectorywSearchSCPD.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltDidl.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltFileMediaServer.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaBrowser.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaCache.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaItem.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltMediaServer.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Devices/MediaServer/PltSyncMediaBrowser.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltDownloader.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltFrameBuffer.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltFrameServer.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltFrameStream.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltLeaks.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltMetadataHandler.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltRingBufferStream.cpp \
    $$PWD/DependLibs/libUPnP/Platinum/Source/Extras/PltStreamPump.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/StdC/NptStdcConsole.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/StdC/NptStdcDebug.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixDynamicLibraries.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixFile.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixNetwork.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixQueue.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixSystem.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixThreads.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptPosixTime.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Posix/NptSelectableMessageQueue.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Null/NptNullAutoreleasePool.cpp

unix:{
SOURCES += $$PWD/DependLibs/libUPnP/Neptune/Source/System/Bsd/NptBsdNetwork.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Bsd/NptBsdResolver.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Bsd/NptBsdSockets.cpp

}

win32:{
SOURCES += $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Console.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Debug.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32DynamicLibraries.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32File.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32MessageQueue.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Network.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Queue.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32SerialPort.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32System.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Threads.cpp \
    $$PWD/DependLibs/libUPnP/Neptune/Source/System/Win32/NptWin32Time.cpp \
}

