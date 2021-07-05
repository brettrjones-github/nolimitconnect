cmake_minimum_required(VERSION 3.12.2)

project(libflac)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libflac STATIC
  ../../bitmath.c
  ../../bitreader.c
  ../../bitwriter.c
  ../../config_libflac.h
  ../../cpu.c
  ../../crc.c
  ../../fixed.c
  ../../fixed_intrin_sse2.c
  ../../fixed_intrin_ssse3.c
  ../../float.c
  ../../format.c
  ../../include/all.h
  ../../include/assert.h
  ../../include/callback.h
  ../../include/export.h
  ../../include/format.h
  ../../include/metadata.h
  ../../include/ordinals.h
  ../../include/stream_decoder.h
  ../../include/stream_encoder.h
  ../../lpc.c
  ../../lpc_intrin_avx2.c
  ../../lpc_intrin_sse.c
  ../../lpc_intrin_sse2.c
  ../../lpc_intrin_sse41.c
  ../../md5.c
  ../../memory.c
  ../../metadata_iterators.c
  ../../metadata_object.c
  ../../ogg_decoder_aspect.c
  ../../ogg_encoder_aspect.c
  ../../ogg_helper.c
  ../../ogg_mapping.c
  ../../stream_decoder.c
  ../../stream_encoder.c
  ../../stream_encoder_framing.c
  ../../stream_encoder_intrin_avx2.c
  ../../stream_encoder_intrin_sse2.c
  ../../stream_encoder_intrin_ssse3.c
  ../../window.c
  ../../windows_unicode_filenames.c
)
# Output file name
set_target_properties(libflac
  PROPERTIES
  OUTPUT_NAME_DEBUG libflacD
)

# Additional include directories
set_property(TARGET libflac
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libflac\build\VS2017\..\..\include;
    J:\NlcJunk\DependLibs\libflac\build\VS2017\..\..\FLAC;
    J:\NlcJunk\DependLibs\libflac\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libflac\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libflac\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libflac\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libflac PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN32;_DEBUG;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libflac PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libflac PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libflac PRIVATE
    ""
  )
endif ()
