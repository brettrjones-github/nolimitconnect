cmake_minimum_required(VERSION 3.12.2)

project(libogg)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libogg STATIC
  ../../include/ogg/ogg.h
  ../../include/ogg/os_types.h
  ../../src/bitwise_ogg.c
  ../../src/framing_ogg.c
)
# Output file name
set_target_properties(libogg
  PROPERTIES
  OUTPUT_NAME_DEBUG liboggD
)

# Additional include directories
set_property(TARGET libogg
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\include;
    J:\NlcJunk\DependLibs\libogg\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libogg\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libogg\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libogg\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libogg PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;_DEBUG;_WINDOWS;_LIB;LIBOGG_EXPORTS>
)

# SDL check
target_compile_options(libogg PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libogg PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libogg PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libogg PRIVATE
    ""
  )
endif ()
