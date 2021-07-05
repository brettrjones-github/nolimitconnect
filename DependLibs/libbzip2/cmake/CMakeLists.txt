cmake_minimum_required(VERSION 3.12.2)

project(libbzip2)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libbzip2 STATIC
  ../blocksort.c
  ../bzip2.c
  ../bzip2recover.c
  ../bzlib.c
  ../bzlib.h
  ../bzlib_private.h
  ../compress.c
  ../crctable.c
  ../decompress.c
  ../huffman.c
  ../mk251.c
  ../randtable.c
  ../spewG.c
)
# Output file name
set_target_properties(libbzip2
  PROPERTIES
  OUTPUT_NAME_DEBUG libbzip2D
)

# Additional include directories
set_property(TARGET libbzip2
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libbzip2\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libbzip2\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libbzip2\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libbzip2 PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN64;_DEBUG;_WINDOWS;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libbzip2 PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libbzip2 PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libbzip2 PRIVATE
    ""
  )
endif ()
