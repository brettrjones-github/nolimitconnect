cmake_minimum_required(VERSION 3.12.2)

project(libcrossguid)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libcrossguid STATIC
  ../../guid.cpp
  ../../guid.h
)
# Output file name
set_target_properties(libcrossguid
  PROPERTIES
  OUTPUT_NAME_DEBUG libcrossguidD
)

# Additional include directories
set_property(TARGET libcrossguid
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\..\;
    ..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libcrossguid PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libcrossguid PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libcrossguid PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libcrossguid PRIVATE
    ""
  )
endif ()
