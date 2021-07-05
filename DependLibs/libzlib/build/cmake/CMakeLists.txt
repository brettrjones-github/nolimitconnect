cmake_minimum_required(VERSION 3.12.2)

project(libzlib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libzlib STATIC
  ../../adler32_zlib.c
  ../../compress_zlib.c
  ../../contrib/minizip/crypt.h
  ../../contrib/minizip/ioapi.c
  ../../contrib/minizip/ioapi.h
  ../../contrib/minizip/iowin32.c
  ../../contrib/minizip/iowin32.h
  ../../contrib/minizip/miniunz.c
  ../../contrib/minizip/minizip.c
  ../../contrib/minizip/mztools.c
  ../../contrib/minizip/mztools.h
  ../../contrib/minizip/unzip.c
  ../../contrib/minizip/unzip.h
  ../../contrib/minizip/zip.c
  ../../contrib/minizip/zip.h
  ../../crc32.h
  ../../crc32_zlib.c
  ../../deflate.h
  ../../deflate_zlib.c
  ../../example.c
  ../../gzclose_zlib.c
  ../../gzlib_zlib.c
  ../../gzread_zlib.c
  ../../gzwrite_zlib.c
  ../../infback_zlib.c
  ../../inffast.h
  ../../inffast_zlib.c
  ../../inffixed.h
  ../../inflate.h
  ../../inflate_zlib.c
  ../../inftrees.h
  ../../inftrees_zlib.c
  ../../minigzip_zlib.c
  ../../trees.h
  ../../trees_zlib.c
  ../../uncompr_zlib.c
  ../../zconf_libzlib.h
  ../../zlib.h
  ../../zutil.h
  ../../zutil_zlib.c
)
# Output file name
set_target_properties(libzlib
  PROPERTIES
  OUTPUT_NAME_DEBUG libzlibD
)

# Additional include directories
set_property(TARGET libzlib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libzlib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libzlib\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libzlib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;NO_MSCVER_FILE64_FUNC;NOCRYPT;WIN64;_DEBUG;_LIB;PRJ_USE_AUTOGEN_CONFIG=<DEBUG_VTPOOLUPLOAD;NOMINMAX;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libzlib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libzlib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libzlib PRIVATE
    ""
  )
endif ()
