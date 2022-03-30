cmake_minimum_required(VERSION 3.12.2)

project(libmicrohttpd)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libmicrohttpd STATIC
  ../../../../AppCompilerConfig.h
  ../../base64_microhttpd.c
  ../../base64_microhttpd.h
  ../../basicauth.c
  ../../config_microhttpd.h
  ../../connection.c
  ../../connection.h
  ../../connection_https.c
  ../../connection_https.h
  ../../daemon.c
  ../../digestauth.c
  ../../internal_microhttpd.c
  ../../internal_microhttpd.h
  ../../md5.c
  ../../md5.h
  ../../memorypool.c
  ../../memorypool.h
  ../../mhd_byteorder.h
  ../../mhd_limits.h
  ../../mhd_mono_clock.c
  ../../mhd_mono_clock.h
  ../../mhd_str.c
  ../../mhd_str.h
  ../../microhttpd.h
  ../../platform_interface.h
  ../../platform_microhttpd.h
  ../../postprocessor.c
  ../../reason_phrase.c
  ../../response.c
  ../../response.h
  ../../sysfdsetsize.c
  ../../sysfdsetsize.h
  ../../tsearch.c
  ../../tsearch.h
  ../../w32functions.c
  ../../w32functions.h
)
# Output file name
set_target_properties(libmicrohttpd
  PROPERTIES
  OUTPUT_NAME_DEBUG libmicrohttpdD
)

# Additional include directories
set_property(TARGET libmicrohttpd
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libmicrohttpd\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libmicrohttpd\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libmicrohttpd\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libmicrohttpd\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libmicrohttpd PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;WINDOWS;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libmicrohttpd PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libmicrohttpd PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libmicrohttpd PRIVATE
    ""
  )
endif ()
