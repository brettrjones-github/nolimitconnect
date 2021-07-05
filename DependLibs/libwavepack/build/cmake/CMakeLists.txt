cmake_minimum_required(VERSION 3.12.2)

project(libwavepack)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libwavepack STATIC
  ../../config_libwavepack.h
  ../../src/common_utils_wavepack.c
  ../../src/decorr_tables.h
  ../../src/decorr_utils_wavepack.c
  ../../src/entropy_utils_wavepack.c
  ../../src/extra1_wavepack.c
  ../../src/extra2_wavepack.c
  ../../src/open_filename_wavepack.c
  ../../src/open_legacy_wavepack.c
  ../../src/open_raw_wavepack.c
  ../../src/open_utils_wavepack.c
  ../../src/pack_dns_wavepack.c
  ../../src/pack_dsd_wavepack.c
  ../../src/pack_floats_wavepack.c
  ../../src/pack_utils_wavepack.c
  ../../src/pack_wavepack.c
  ../../src/read_words_wavepack.c
  ../../src/tag_utils_wavepack.c
  ../../src/tags_wavepack.c
  ../../src/unpack_dsd_wavepack.c
  ../../src/unpack_floats_wavepack.c
  ../../src/unpack_seek_wavepack.c
  ../../src/unpack_utils_wavepack.c
  ../../src/unpack_wavepack.c
  ../../src/unpack3.h
  ../../src/unpack3_open_wavepack.c
  ../../src/unpack3_seek_wavepack.c
  ../../src/unpack3_wavepack.c
  ../../src/wavpack_local.h
  ../../src/wavpack_version.h
  ../../src/write_words_wavepack.c
)
# Output file name
set_target_properties(libwavepack
  PROPERTIES
  OUTPUT_NAME_DEBUG libwavepackD
)

# Additional include directories
set_property(TARGET libwavepack
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libwavepack\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libwavepack\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libwavepack\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libwavepack\build\VS2017\..\..\..\..\sysheaders;
    "C:\Program Files (x86)\Visual Leak Detector\include">
)

# Preprocessor definitions
target_compile_definitions(libwavepack PRIVATE
  $<$<CONFIG:Debug>:TARGET_OS_WINDOWS;WIN32;_DEBUG;_LIB;_CRT_SECURE_NO_DEPRECATE;ENABLE_DSD>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libwavepack PRIVATE
    "$<$<CONFIG:Debug>:/Gm>"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libwavepack PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libwavepack PRIVATE
    ""
  )
endif ()
