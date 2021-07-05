cmake_minimum_required(VERSION 3.12.2)

project(librtmp)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(librtmp STATIC
  ../../config_librtmp.h
  ../../rtmp.c
  ../../rtmp.h
  ../../rtmp_amf.c
  ../../rtmp_amf.h
  ../../rtmp_bytes.h
  ../../rtmp_dh.h
  ../../rtmp_dhgroups.h
  ../../rtmp_handshake.h
  ../../rtmp_hashswf.c
  ../../rtmp_http.h
  ../../rtmp_log.c
  ../../rtmp_log.h
  ../../rtmp_parseurl.c
  ../../rtmp_sys.h
  ../../rtmp_thread.c
  ../../rtmp_thread.h
  ../../rtmpdump.c
  ../../rtmpgw.c
  ../../rtmpsrv.c
  ../../rtmpsuck.c
)
# Output file name
set_target_properties(librtmp
  PROPERTIES
  OUTPUT_NAME_DEBUG librtmpD
)

# Additional include directories
set_property(TARGET librtmp
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\librtmp\build\VS2017\..\..\..\openssl-1.0.2p;
    J:\NlcJunk\DependLibs\librtmp\build\VS2017\..\..\..\openssl-1.0.2p\include;
    J:\NlcJunk\DependLibs\librtmp\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\librtmp\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\librtmp\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\librtmp\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(librtmp PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(librtmp PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(librtmp PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(librtmp PRIVATE
    ""
  )
endif ()
