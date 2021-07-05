cmake_minimum_required(VERSION 3.12.2)

project(PktLib)

set(CMAKE_CONFIGURATION_TYPES "Release"
    CACHE STRING "Configuration types" FORCE)

add_library(PktLib STATIC
  ../../AllPkts.h
  ../../MatchParams.cpp
  ../../MatchParams.h
  ../../PktAnnList.cpp
  ../../PktAnnList.h
  ../../PktAnnounce.cpp
  ../../PktAnnounce.h
  ../../PktBlobEntry.cpp
  ../../PktBlobEntry.h
  ../../PktChatReq.cpp
  ../../PktChatReq.h
  ../../PktDebugHelpers.cpp
  ../../PktDebugHelpers.h
  ../../PktHostAnnounce.cpp
  ../../PktHostAnnounce.h
  ../../PktLogReply.cpp
  ../../PktLogReply.h
  ../../PktLogReq.cpp
  ../../PktLogReq.h
  ../../PktPluginHandlerBase.cpp
  ../../PktPluginHandlerBase.h
  ../../PktRequestErrors.h
  ../../PktsAssetXfer.cpp
  ../../PktsAssetXfer.h
  ../../PktsBaseXfer.cpp
  ../../PktsBaseXfer.h
  ../../PktsBlobXfer.cpp
  ../../PktsBlobXfer.h
  ../../PktsFileList.cpp
  ../../PktsFileList.h
  ../../PktsFileShare.cpp
  ../../PktsFileShare.h
  ../../PktsFriendOffer.cpp
  ../../PktsFriendOffer.h
  ../../PktsHostJoin.cpp
  ../../PktsHostJoin.h
  ../../PktsHostSearch.cpp
  ../../PktsHostSearch.h
  ../../PktsImAlive.cpp
  ../../PktsImAlive.h
  ../../PktsMultiSession.cpp
  ../../PktsMultiSession.h
  ../../PktsOfferXfer.cpp
  ../../PktsOfferXfer.h
  ../../PktsPing.cpp
  ../../PktsPing.h
  ../../PktsPluginOffer.cpp
  ../../PktsPluginOffer.h
  ../../PktsPluginSetting.cpp
  ../../PktsPluginSetting.h
  ../../PktsRelay.cpp
  ../../PktsRelay.h
  ../../PktsScan.cpp
  ../../PktsScan.h
  ../../PktsSearch.cpp
  ../../PktsSearch.h
  ../../PktsSession.cpp
  ../../PktsSession.h
  ../../PktsStoryBoard.cpp
  ../../PktsStoryBoard.h
  ../../PktsThumb.cpp
  ../../PktsThumb.h
  ../../PktsThumbXfer.cpp
  ../../PktsThumbXfer.h
  ../../PktsTodGame.cpp
  ../../PktsTodGame.h
  ../../PktsVideoFeed.cpp
  ../../PktsVideoFeed.h
  ../../PktsWebServer.cpp
  ../../PktsWebServer.h
  ../../PktSysHandlerBase.cpp
  ../../PktSysHandlerBase.h
  ../../PktTcpPunch.cpp
  ../../PktTcpPunch.h
  ../../PktTypes.h
  ../../PktVoiceReply.cpp
  ../../PktVoiceReply.h
  ../../PktVoiceReq.cpp
  ../../PktVoiceReq.h
  ../../PluginId.cpp
  ../../PluginId.h
  ../../PluginIdList.cpp
  ../../PluginIdList.h
  ../../SearchParams.cpp
  ../../SearchParams.h
  ../../VxCommon.cpp
  ../../VxCommon.h
  ../../VxConnectId.cpp
  ../../VxConnectId.h
  ../../VxConnectInfo.cpp
  ../../VxConnectInfo.h
  ../../VxFriendMatch.cpp
  ../../VxFriendMatch.h
  ../../VxNetIdentBase.cpp
  ../../VxNetIdentBase.h
  ../../VxOnlineStatusFlags.cpp
  ../../VxOnlineStatusFlags.h
  ../../VxPktHdr.cpp
  ../../VxPktHdr.h
  ../../VxRelayFlags.cpp
  ../../VxRelayFlags.h
  ../../VxSearchDefs.h
  ../../VxSearchFlags.cpp
  ../../VxSearchFlags.h
)
# Additional include directories
set_property(TARGET PktLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Release>:
    J:\NlcJunk\DependLibs\PktLib\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\PktLib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\PktLib\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\PktLib\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(PktLib PRIVATE
  $<$<CONFIG:Release>:_MBCS;NDEBUG;HAVE_CONFIG_H;WIN32;_LIB;TARGET_OS_WINDOWS;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(PktLib PRIVATE
    "$<$<CONFIG:Release>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(PktLib PRIVATE
    ""
  )
endif ()
