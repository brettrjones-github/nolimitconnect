cmake_minimum_required(VERSION 3.12.2)

project(libp8-platform)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libp8-platform STATIC
  ../../config_libp8platform.h
  ../../src/util/StringUtils.cpp
  ../../src/util/StringUtils.h
  ../../src/windows/dlfcn-win32.cpp
  ../../src/windows/dlfcn-win32.h
  ../../src/windows/inttypes.h
  ../../src/windows/os-socket.h
  ../../src/windows/os-threads.cpp
  ../../src/windows/os-threads.h
  ../../src/windows/os-types.h
  ../../src/windows/stdint.h
)
# Output file name
set_target_properties(libp8-platform
  PROPERTIES
  OUTPUT_NAME_DEBUG libp8-platformD
)

# Additional include directories
set_property(TARGET libp8-platform
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libp8platform\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libp8platform\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libp8platform\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libp8platform\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libp8-platform PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libp8-platform PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libp8-platform PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libp8-platform PRIVATE
    ""
  )
endif ()
