cmake_minimum_required(VERSION 3.12.2)

project(libsnappy)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libsnappy STATIC
  ../../config_libsnappy.h
  ../../snappy.cc
  ../../snappy.h
  ../../snappy-c.cc
  ../../snappy-c.h
  ../../snappy-internal.h
  ../../snappy-sinksource.cc
  ../../snappy-sinksource.h
  ../../snappy-stubs-internal.cc
  ../../snappy-stubs-internal.h
  ../../snappy-stubs-public.h
  ../../snappy-test.h
)
# Output file name
set_target_properties(libsnappy
  PROPERTIES
  OUTPUT_NAME_DEBUG libsnappyD
)

# Additional include directories
set_property(TARGET libsnappy
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libsnappy\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libsnappy\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libsnappy\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libsnappy PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libsnappy PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libsnappy PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libsnappy PRIVATE
    ""
  )
endif ()
