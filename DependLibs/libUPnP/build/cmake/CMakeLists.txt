cmake_minimum_required(VERSION 3.12.2)

project(libUPnP)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libUPnP STATIC
  ../../../../base64.h
  ../../../../byteswap.h
  ../../../../dirent.h
  ../../../../dlfcn.h
  ../../../../AppCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../AppCpuArchDefines.h
  ../../../../AppDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../AppTargetOsConfig.h
  ../../../../langinfo.h
  ../../../../netdb.h
  ../../../../pwd.h
  ../../../../strings.h
  ../../../../syslog.h
  ../../../../unistd.h
  ../../../../utime.h
  ../../config_libupnp.h
  ../../Neptune/Source/Core/Neptune.cpp
  ../../Neptune/Source/Core/Neptune.h
  ../../Neptune/Source/Core/NptArray.h
  ../../Neptune/Source/Core/NptAutomaticCleaner.cpp
  ../../Neptune/Source/Core/NptAutomaticCleaner.h
  ../../Neptune/Source/Core/NptAutoreleasePool.h
  ../../Neptune/Source/Core/NptBase64.cpp
  ../../Neptune/Source/Core/NptBase64.h
  ../../Neptune/Source/Core/NptBufferedStreams.cpp
  ../../Neptune/Source/Core/NptBufferedStreams.h
  ../../Neptune/Source/Core/NptCommon.cpp
  ../../Neptune/Source/Core/NptCommon.h
  ../../Neptune/Source/Core/NptConfig.h
  ../../Neptune/Source/Core/NptConsole.cpp
  ../../Neptune/Source/Core/NptConsole.h
  ../../Neptune/Source/Core/NptConstants.h
  ../../Neptune/Source/Core/NptCrypto.cpp
  ../../Neptune/Source/Core/NptCrypto.h
  ../../Neptune/Source/Core/NptDataBuffer.cpp
  ../../Neptune/Source/Core/NptDataBuffer.h
  ../../Neptune/Source/Core/NptDebug.cpp
  ../../Neptune/Source/Core/NptDebug.h
  ../../Neptune/Source/Core/NptDefs.h
  ../../Neptune/Source/Core/NptDigest.cpp
  ../../Neptune/Source/Core/NptDigest.h
  ../../Neptune/Source/Core/NptDynamicCast.h
  ../../Neptune/Source/Core/NptDynamicLibraries.cpp
  ../../Neptune/Source/Core/NptDynamicLibraries.h
  ../../Neptune/Source/Core/NptFile.cpp
  ../../Neptune/Source/Core/NptFile.h
  ../../Neptune/Source/Core/NptHash.cpp
  ../../Neptune/Source/Core/NptHash.h
  ../../Neptune/Source/Core/NptHttp.cpp
  ../../Neptune/Source/Core/NptHttp.h
  ../../Neptune/Source/Core/NptInterfaces.h
  ../../Neptune/Source/Core/NptJson.cpp
  ../../Neptune/Source/Core/NptJson.h
  ../../Neptune/Source/Core/NptList.cpp
  ../../Neptune/Source/Core/NptList.h
  ../../Neptune/Source/Core/NptLogging.cpp
  ../../Neptune/Source/Core/NptLogging.h
  ../../Neptune/Source/Core/NptMap.h
  ../../Neptune/Source/Core/NptMessaging.cpp
  ../../Neptune/Source/Core/NptMessaging.h
  ../../Neptune/Source/Core/NptNetwork.cpp
  ../../Neptune/Source/Core/NptNetwork.h
  ../../Neptune/Source/Core/NptQueue.cpp
  ../../Neptune/Source/Core/NptQueue.h
  ../../Neptune/Source/Core/NptReferences.h
  ../../Neptune/Source/Core/NptResults.cpp
  ../../Neptune/Source/Core/NptResults.h
  ../../Neptune/Source/Core/NptRingBuffer.cpp
  ../../Neptune/Source/Core/NptRingBuffer.h
  ../../Neptune/Source/Core/NptSelectableMessageQueue.h
  ../../Neptune/Source/Core/NptSerialPort.h
  ../../Neptune/Source/Core/NptSimpleMessageQueue.cpp
  ../../Neptune/Source/Core/NptSimpleMessageQueue.h
  ../../Neptune/Source/Core/NptSockets.cpp
  ../../Neptune/Source/Core/NptSockets.h
  ../../Neptune/Source/Core/NptStack.h
  ../../Neptune/Source/Core/NptStreams.cpp
  ../../Neptune/Source/Core/NptStreams.h
  ../../Neptune/Source/Core/NptStrings.cpp
  ../../Neptune/Source/Core/NptStrings.h
  ../../Neptune/Source/Core/NptSystem.cpp
  ../../Neptune/Source/Core/NptSystem.h
  ../../Neptune/Source/Core/NptThreads.cpp
  ../../Neptune/Source/Core/NptThreads.h
  ../../Neptune/Source/Core/NptTime.cpp
  ../../Neptune/Source/Core/NptTime.h
  ../../Neptune/Source/Core/NptTls.cpp
  ../../Neptune/Source/Core/NptTls.h
  ../../Neptune/Source/Core/NptTlsDefaultTrustAnchorsBase.h
  ../../Neptune/Source/Core/NptTlsDefaultTrustAnchorsExtended.h
  ../../Neptune/Source/Core/NptTypes.h
  ../../Neptune/Source/Core/NptUri.cpp
  ../../Neptune/Source/Core/NptUri.h
  ../../Neptune/Source/Core/NptUtils.cpp
  ../../Neptune/Source/Core/NptUtils.h
  ../../Neptune/Source/Core/NptVersion.h
  ../../Neptune/Source/Core/NptXml.cpp
  ../../Neptune/Source/Core/NptXml.h
  ../../Neptune/Source/Core/NptZip.cpp
  ../../Neptune/Source/Core/NptZip.h
  ../../Neptune/Source/System/Bsd/NptBsdResolver.cpp
  ../../Neptune/Source/System/Bsd/NptBsdSockets.cpp
  ../../Neptune/Source/System/Posix/NptPosixEnvironment.cpp
  ../../Neptune/Source/System/StdC/NptStdcEnvironment.cpp
  ../../Neptune/Source/System/StdC/NptStdcFile.cpp
  ../../Neptune/Source/System/Win32/NptWin32Console.cpp
  ../../Neptune/Source/System/Win32/NptWin32Debug.cpp
  ../../Neptune/Source/System/Win32/NptWin32DynamicLibraries.cpp
  ../../Neptune/Source/System/Win32/NptWin32File.cpp
  ../../Neptune/Source/System/Win32/NptWin32MessageQueue.cpp
  ../../Neptune/Source/System/Win32/NptWin32MessageQueue.h
  ../../Neptune/Source/System/Win32/NptWin32Network.cpp
  ../../Neptune/Source/System/Win32/NptWin32Network.h
  ../../Neptune/Source/System/Win32/NptWin32Queue.cpp
  ../../Neptune/Source/System/Win32/NptWin32SerialPort.cpp
  ../../Neptune/Source/System/Win32/NptWin32System.cpp
  ../../Neptune/Source/System/Win32/NptWin32Threads.cpp
  ../../Neptune/Source/System/Win32/NptWin32Threads.h
  ../../Neptune/Source/System/Win32/NptWin32Time.cpp
  ../../Platinum/Source/Core/PltAction.cpp
  ../../Platinum/Source/Core/PltAction.h
  ../../Platinum/Source/Core/PltArgument.cpp
  ../../Platinum/Source/Core/PltArgument.h
  ../../Platinum/Source/Core/PltConstants.cpp
  ../../Platinum/Source/Core/PltConstants.h
  ../../Platinum/Source/Core/PltCtrlPoint.cpp
  ../../Platinum/Source/Core/PltCtrlPoint.h
  ../../Platinum/Source/Core/PltCtrlPointTask.cpp
  ../../Platinum/Source/Core/PltCtrlPointTask.h
  ../../Platinum/Source/Core/PltDatagramStream.cpp
  ../../Platinum/Source/Core/PltDatagramStream.h
  ../../Platinum/Source/Core/PltDeviceData.cpp
  ../../Platinum/Source/Core/PltDeviceData.h
  ../../Platinum/Source/Core/PltDeviceHost.cpp
  ../../Platinum/Source/Core/PltDeviceHost.h
  ../../Platinum/Source/Core/PltEvent.cpp
  ../../Platinum/Source/Core/PltEvent.h
  ../../Platinum/Source/Core/PltHttp.cpp
  ../../Platinum/Source/Core/PltHttp.h
  ../../Platinum/Source/Core/PltHttpClientTask.cpp
  ../../Platinum/Source/Core/PltHttpClientTask.h
  ../../Platinum/Source/Core/PltHttpServer.cpp
  ../../Platinum/Source/Core/PltHttpServer.h
  ../../Platinum/Source/Core/PltHttpServerListener.h
  ../../Platinum/Source/Core/PltHttpServerTask.cpp
  ../../Platinum/Source/Core/PltHttpServerTask.h
  ../../Platinum/Source/Core/PltIconsData.cpp
  ../../Platinum/Source/Core/PltMimeType.cpp
  ../../Platinum/Source/Core/PltMimeType.h
  ../../Platinum/Source/Core/PltProtocolInfo.cpp
  ../../Platinum/Source/Core/PltProtocolInfo.h
  ../../Platinum/Source/Core/PltService.cpp
  ../../Platinum/Source/Core/PltService.h
  ../../Platinum/Source/Core/PltSsdp.cpp
  ../../Platinum/Source/Core/PltSsdp.h
  ../../Platinum/Source/Core/PltSsdpListener.h
  ../../Platinum/Source/Core/PltStateVariable.cpp
  ../../Platinum/Source/Core/PltStateVariable.h
  ../../Platinum/Source/Core/PltTaskManager.cpp
  ../../Platinum/Source/Core/PltTaskManager.h
  ../../Platinum/Source/Core/PltThreadTask.cpp
  ../../Platinum/Source/Core/PltThreadTask.h
  ../../Platinum/Source/Core/PltUPnP.cpp
  ../../Platinum/Source/Core/PltUPnP.h
  ../../Platinum/Source/Core/PltXmlHelper.h
  ../../Platinum/Source/Devices/MediaConnect/PltMediaConnect.cpp
  ../../Platinum/Source/Devices/MediaConnect/PltMediaConnect.h
  ../../Platinum/Source/Devices/MediaConnect/PltXbox360.cpp
  ../../Platinum/Source/Devices/MediaConnect/PltXbox360.h
  ../../Platinum/Source/Devices/MediaConnect/X_MS_MediaReceiverRegistrarSCPD.cpp
  ../../Platinum/Source/Devices/MediaRenderer/AVTransportSCPD.cpp
  ../../Platinum/Source/Devices/MediaRenderer/PltMediaController.cpp
  ../../Platinum/Source/Devices/MediaRenderer/PltMediaController.h
  ../../Platinum/Source/Devices/MediaRenderer/PltMediaControllerListener.h
  ../../Platinum/Source/Devices/MediaRenderer/PltMediaRenderer.cpp
  ../../Platinum/Source/Devices/MediaRenderer/PltMediaRenderer.h
  ../../Platinum/Source/Devices/MediaRenderer/RdrConnectionManagerSCPD.cpp
  ../../Platinum/Source/Devices/MediaRenderer/RenderingControlSCPD.cpp
  ../../Platinum/Source/Devices/MediaServer/ConnectionManagerSCPD.cpp
  ../../Platinum/Source/Devices/MediaServer/ContentDirectorySCPD.cpp
  ../../Platinum/Source/Devices/MediaServer/ContentDirectorywSearchSCPD.cpp
  ../../Platinum/Source/Devices/MediaServer/PltDidl.cpp
  ../../Platinum/Source/Devices/MediaServer/PltDidl.h
  ../../Platinum/Source/Devices/MediaServer/PltFileMediaServer.cpp
  ../../Platinum/Source/Devices/MediaServer/PltFileMediaServer.h
  ../../Platinum/Source/Devices/MediaServer/PltMediaBrowser.cpp
  ../../Platinum/Source/Devices/MediaServer/PltMediaBrowser.h
  ../../Platinum/Source/Devices/MediaServer/PltMediaBrowserListener.h
  ../../Platinum/Source/Devices/MediaServer/PltMediaCache.cpp
  ../../Platinum/Source/Devices/MediaServer/PltMediaCache.h
  ../../Platinum/Source/Devices/MediaServer/PltMediaItem.cpp
  ../../Platinum/Source/Devices/MediaServer/PltMediaItem.h
  ../../Platinum/Source/Devices/MediaServer/PltMediaServer.cpp
  ../../Platinum/Source/Devices/MediaServer/PltMediaServer.h
  ../../Platinum/Source/Devices/MediaServer/PltSyncMediaBrowser.cpp
  ../../Platinum/Source/Devices/MediaServer/PltSyncMediaBrowser.h
  ../../Platinum/Source/Extras/PltDownloader.cpp
  ../../Platinum/Source/Extras/PltDownloader.h
  ../../Platinum/Source/Extras/PltFrameBuffer.cpp
  ../../Platinum/Source/Extras/PltFrameBuffer.h
  ../../Platinum/Source/Extras/PltFrameServer.cpp
  ../../Platinum/Source/Extras/PltFrameServer.h
  ../../Platinum/Source/Extras/PltFrameStream.cpp
  ../../Platinum/Source/Extras/PltFrameStream.h
  ../../Platinum/Source/Extras/PltLeaks.cpp
  ../../Platinum/Source/Extras/PltLeaks.h
  ../../Platinum/Source/Extras/PltMetadataHandler.cpp
  ../../Platinum/Source/Extras/PltMetadataHandler.h
  ../../Platinum/Source/Extras/PltRingBufferStream.cpp
  ../../Platinum/Source/Extras/PltRingBufferStream.h
  ../../Platinum/Source/Extras/PltStreamPump.cpp
  ../../Platinum/Source/Extras/PltStreamPump.h
  ../../Platinum/Source/Platinum/Platinum.h
  ../../Platinum/Source/Platinum/PltSvnVersion.h
  ../../Platinum/Source/Platinum/PltVersion.h
)
# Output file name
set_target_properties(libUPnP
  PROPERTIES
  OUTPUT_NAME_DEBUG libUPnPD
)

# Additional include directories
set_property(TARGET libUPnP
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\Platinum\Source\Platinum;
    ..\..\Platinum\Source\Core;
    ..\..\Platinum\Source\Extras;
    ..\..\Platinum\Source\Devices\MediaServer;
    ..\..\Platinum\Source\Devices\MediaRenderer;
    ..\..\Platinum\Source\Devices\MediaConnect;
    ..\..\Neptune\Source\Core;
    ..\..\Neptune\Source\System\Win32;
    J:\NlcJunk\DependLibs\libUPnP\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libUPnP\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libUPnP\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libUPnP\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libUPnP PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;WIN64;_DEBUG;_LIB;_CRT_SECURE_NO_DEPRECATE;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libUPnP PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libUPnP PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libUPnP PRIVATE
    ""
  )
endif ()
