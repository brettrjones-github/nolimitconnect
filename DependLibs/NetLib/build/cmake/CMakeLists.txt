cmake_minimum_required(VERSION 3.12.2)

project(NetLib)

set(CMAKE_CONFIGURATION_TYPES "Release"
    CACHE STRING "Configuration types" FORCE)

add_library(NetLib STATIC
  ../../InetAddress.cpp
  ../../InetAddress.h
  ../../NetHostSetting.cpp
  ../../NetHostSetting.h
  ../../NetSettings.cpp
  ../../NetSettings.h
  ../../VxAnyRandomPort.h
  ../../VxBaseSktInfo.cpp
  ../../VxBaseSktInfo.h
  ../../VxClientMgr.cpp
  ../../VxClientMgr.h
  ../../VxFileXferInfo.cpp
  ../../VxFileXferInfo.h
  ../../VxGetExternalIps.cpp
  ../../VxGetExternalIps.h
  ../../VxGetRandomPort.cpp
  ../../VxGetRandomPort.h
  ../../VxHttpParse.cpp
  ../../VxHttpParse.h
  ../../VxHttpRequest.cpp
  ../../VxHttpRequest.h
  ../../VxHttpValuePair.cpp
  ../../VxHttpValuePair.h
  ../../VxParseUrlComponents.cpp
  ../../VxParseUrlComponents.h
  ../../VxPeerMgr.cpp
  ../../VxPeerMgr.h
  ../../VxProxy.cpp
  ../../VxProxy.h
  ../../VxResolveHost.cpp
  ../../VxResolveHost.h
  ../../VxServerMgr.cpp
  ../../VxServerMgr.h
  ../../VxSktAccept.cpp
  ../../VxSktAccept.h
  ../../VxSktBase.cpp
  ../../VxSktBase.h
  ../../VxSktBaseMgr.cpp
  ../../VxSktBaseMgr.h
  ../../VxSktBuf.cpp
  ../../VxSktBuf.h
  ../../VxSktConnect.cpp
  ../../VxSktConnect.h
  ../../VxSktConnectSimple.cpp
  ../../VxSktConnectSimple.h
  ../../VxSktCrypto.cpp
  ../../VxSktCrypto.h
  ../../VxSktDefs.h
  ../../VxSktInfo.h
  ../../VxSktRandomData.h
  ../../VxSktThrottle.cpp
  ../../VxSktThrottle.h
  ../../VxSktUdp.cpp
  ../../VxSktUdp.h
  ../../VxSktUdpBroadcast.cpp
  ../../VxSktUdpBroadcast.h
  ../../VxSktUtil.cpp
  ../../VxSktUtil.h
  ../../VxSktWaitingList.cpp
  ../../VxSktWaitingList.h
  ../../VxXferDefs.cpp
  ../../VxXferDefs.h
)
# Additional include directories
set_property(TARGET NetLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Release>:
    J:\NlcJunk\DependLibs\NetLib\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\NetLib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\NetLib\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\NetLib\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(NetLib PRIVATE
  $<$<CONFIG:Release>:_MBCS;NDEBUG;WIN64;HAVE_CONFIG;TARGET_OS_WINDOWS;_CRT_SECURE_NO_WARNINGS;_LIB;%(PreprocessorDefinitions)UG;WIN32;HAVE_CONFIG;TARGET_OS_WINDOWS;_CRT_SECURE_NO_WARNINGS;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(NetLib PRIVATE
    "$<$<CONFIG:Release>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(NetLib PRIVATE
    ""
  )
endif ()
