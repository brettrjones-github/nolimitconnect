cmake_minimum_required(VERSION 3.12.2)

project(bzlib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(bzlib STATIC
  ../../blocksort_bz2.c
  ../../bzip2.c
  ../../bzip2recover.c
  ../../bzlib.h
  ../../bzlib_bz2.c
  ../../bzlib_private.h
  ../../compress_bz2.c
  ../../crctable_bz2.c
  ../../decompress_bz2.c
  ../../dlltest.c
  ../../huffman_bz2.c
  ../../mk251.c
  ../../randtable_bz2.c
  ../../spewG.c
  ../../unzcrash.c
)
# Output file name
set_target_properties(bzlib
  PROPERTIES
  OUTPUT_NAME_DEBUG bzlibD
)

# Additional include directories
set_property(TARGET bzlib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\bzlib\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\bzlib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\bzlib\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\bzlib\build\VS2017\..\..\..\..\sysheaders\;
    J:\NlcJunk\DependLibs\bzlib\build\VS2017\..\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(bzlib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN64;_DEBUG;_WINDOWS;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(bzlib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(bzlib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(bzlib PRIVATE
    ""
  )
endif ()
