cmake_minimum_required(VERSION 3.12.2)

project(CoreLib)

set(CMAKE_CONFIGURATION_TYPES "Release"
    CACHE STRING "Configuration types" FORCE)

add_library(CoreLib STATIC
  ../../../../arpa/inet.h
  ../../../../asm/types.h
  ../../../../byteswap.h
  ../../../../dirent.h
  ../../../../dlfcn.h
  ../../../../AppCompilerConfig.h
  ../../../../AppCpuArchDefines.h
  ../../../../AppDependLibrariesConfig.h
  ../../../../AppTargetOsConfig.h
  ../../../../langinfo.h
  ../../../../linux/ioctl.h
  ../../../../linux/param.h
  ../../../../netdb.h
  ../../../../netinet/in.h
  ../../../../netinet/tcp.h
  ../../../../strings.h
  ../../../../sys/ioctl.h
  ../../../../sys/mman.h
  ../../../../sys/param.h
  ../../../../sys/select.h
  ../../../../sys/signal.h
  ../../../../sys/socket.h
  ../../../../sys/syslog.h
  ../../../../sys/time.h
  ../../../../sys/times.h
  ../../../../sys/un.h
  ../../../../sys/utsname.h
  ../../../../syslog.h
  ../../../../unistd.h
  ../../AppDefines.h
  ../../AppErr.h
  ../../AppErr_corelib.cpp
  ../../AssetDefs.cpp
  ../../AssetDefs.h
  ../../BinaryBlob.cpp
  ../../BinaryBlob.h
  ../../Blowfish.h
  ../../Blowfish_corelib.cpp
  ../../config_corelib.h
  ../../DbBase.h
  ../../DbBase_corelib.cpp
  ../../DbDataset.h
  ../../DbDataset_corelib.cpp
  ../../DbQueryData.h
  ../../DbQueryData_corelib.cpp
  ../../dirent_sim.cpp
  ../../dirent_sim.h
  ../../IsBigEndianCpu.h
  ../../md5.h
  ../../md5_corelib.cpp
  ../../MediaCallbackInterface.h
  ../../ObjectCommon.h
  ../../ObjectCommon_corelib.cpp
  ../../OsDetect.h
  ../../OsDetect_corelib.cpp
  ../../rsa.h
  ../../rsa_eay_corelib.c
  ../../rsa_gen_corelib.c
  ../../SHA1.h
  ../../SHA1_corelib.cpp
  ../../sqlite3.h
  ../../sqlite3_corelib.c
  ../../sqlite3ext.h
  ../../sqlitedataset.h
  ../../sqlitedataset_corelib.cpp
  ../../StdMinMaxForWindows.h
  ../../StdOutRedirect.h
  ../../StdOutRedirect_corelib.cpp
  ../../StringListBinary.cpp
  ../../StringListBinary.h
  ../../VarToString.h
  ../../VxAppInfo.cpp
  ../../VxAppInfo.h
  ../../VxBuffer.cpp
  ../../VxBuffer.h
  ../../VxChop.cpp
  ../../VxChop.h
  ../../VxChopMap.cpp
  ../../VxChopMap.h
  ../../VxColorUtil.cpp
  ../../VxColorUtil.h
  ../../VxCrypto.cpp
  ../../VxCrypto.h
  ../../VxDebug.cpp
  ../../VxDebug.h
  ../../VxDefs.cpp
  ../../VxDefs.h
  ../../VxFileCopier.cpp
  ../../VxFileCopier.h
  ../../VxFileCrc32.cpp
  ../../VxFileCrc32.h
  ../../VxFileInfo.cpp
  ../../VxFileInfo.h
  ../../VxFileIsTypeFunctions.cpp
  ../../VxFileIsTypeFunctions.h
  ../../VxFileLists.cpp
  ../../VxFileLists.h
  ../../VxFileShredder.cpp
  ../../VxFileShredder.h
  ../../VxFileShredderDb.cpp
  ../../VxFileShredderDb.h
  ../../VxFileTypeMasks.h
  ../../VxFileUtil.cpp
  ../../VxFileUtil.h
  ../../VxFunctionsMissingInAndroid.cpp
  ../../VxFunctionsMissingInPosix.cpp
  ../../VxFunctionsMissingInWindows.cpp
  ../../VxGlobals.cpp
  ../../VxGlobals.h
  ../../VxGUID.cpp
  ../../VxGUID.h
  ../../VxGUIDList.cpp
  ../../VxGUIDList.h
  ../../VxJava.cpp
  ../../VxJava.h
  ../../VxKeyDefs.h
  ../../VxLinuxOnly.cpp
  ../../VxLinuxOnly.h
  ../../VxMacros.h
  ../../VxMathDef.cpp
  ../../VxMathDef.h
  ../../VxMathVec2f.h
  ../../VxMathVec3f.cpp
  ../../VxMathVec3f.h
  ../../VxMathVec4f.h
  ../../VxMutex.cpp
  ../../VxMutex.h
  ../../VxParse.cpp
  ../../VxParse.h
  ../../VxProfile.cpp
  ../../VxProfile.h
  ../../VxRefCount.cpp
  ../../VxRefCount.h
  ../../VxSemaphore.cpp
  ../../VxSemaphore.h
  ../../VxSettings.cpp
  ../../VxSettings.h
  ../../VxSha1Hash.cpp
  ../../VxSha1Hash.h
  ../../VxShortcut.cpp
  ../../VxShortcut.h
  ../../VxSingleton.h
  ../../VxSupportedFiles.h
  ../../VxTextStreamReader.cpp
  ../../VxTextStreamReader.h
  ../../VxThread.cpp
  ../../VxThread.h
  ../../VxTime.cpp
  ../../VxTime.h
  ../../VxTimeDefs.h
  ../../VxTimer.cpp
  ../../VxTimer.h
  ../../VxTimeUtil.cpp
  ../../VxTimeUtil.h
  ../../VxUrl.cpp
  ../../VxUrl.h
  ../../VxValuePair.cpp
  ../../VxValuePair.h
  ../../VxXml.cpp
  ../../VxXml.h
)
# Additional include directories
set_property(TARGET CoreLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Release>:
    J:\NlcJunk\DependLibs\CoreLib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\CoreLib\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(CoreLib PRIVATE
  $<$<CONFIG:Release>:_MBCS;NDEBUG;TARGET_OS_WINDOWS;WIN64;LIB_STATIC;HAVE_CONFIG_H>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(CoreLib PRIVATE
    "$<$<CONFIG:Release>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(CoreLib PRIVATE
    ""
  )
endif ()
