cmake_minimum_required(VERSION 3.12.2)

project(libvorbis)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libvorbis STATIC
  ../../config_libvorbis.h
  ../../include/vorbis/codec.h
  ../../include/vorbis/vorbisenc.h
  ../../include/vorbis/vorbisfile.h
  ../../lib/analysis_vorbis.c
  ../../lib/backends.h
  ../../lib/bitrate.h
  ../../lib/bitrate_vorbis.c
  ../../lib/block_vorbis.c
  ../../lib/books/coupled/res_books_stereo.h
  ../../lib/books/floor/floor_books.h
  ../../lib/books/uncoupled/res_books_uncoupled.h
  ../../lib/codebook.h
  ../../lib/codebook_vorbis.c
  ../../lib/codec_internal.h
  ../../lib/envelope.h
  ../../lib/envelope_vorbis.c
  ../../lib/floor0_vorbis.c
  ../../lib/floor1_vorbis.c
  ../../lib/highlevel.h
  ../../lib/info_vorbis.c
  ../../lib/lookup.h
  ../../lib/lookup_data.h
  ../../lib/lookup_vorbis.c
  ../../lib/lpc.h
  ../../lib/lpc_vorbis.c
  ../../lib/lsp.h
  ../../lib/lsp_vorbis.c
  ../../lib/mapping0_vorbis.c
  ../../lib/masking.h
  ../../lib/mdct.h
  ../../lib/mdct_vorbis.c
  ../../lib/misc.h
  ../../lib/modes/floor_all.h
  ../../lib/modes/psych_11.h
  ../../lib/modes/psych_16.h
  ../../lib/modes/psych_44.h
  ../../lib/modes/psych_8.h
  ../../lib/modes/residue_16.h
  ../../lib/modes/residue_44.h
  ../../lib/modes/residue_44u.h
  ../../lib/modes/residue_8.h
  ../../lib/modes/setup_11.h
  ../../lib/modes/setup_16.h
  ../../lib/modes/setup_22.h
  ../../lib/modes/setup_32.h
  ../../lib/modes/setup_44.h
  ../../lib/modes/setup_44u.h
  ../../lib/modes/setup_8.h
  ../../lib/modes/setup_X.h
  ../../lib/os.h
  ../../lib/psy.h
  ../../lib/psy_vorbis.c
  ../../lib/registry.h
  ../../lib/registry_vorbis.c
  ../../lib/res0_vorbis.c
  ../../lib/scales.h
  ../../lib/sharedbook_vorbis.c
  ../../lib/smallft.h
  ../../lib/smallft_vorbis.c
  ../../lib/synthesis_vorbis.c
  ../../lib/vorbisenc_vorbis.c
  ../../lib/vorbisfile.c
  ../../lib/window.h
  ../../lib/window_vorbis.c
)
# Output file name
set_target_properties(libvorbis
  PROPERTIES
  OUTPUT_NAME_DEBUG libvorbisD
)

# Additional include directories
set_property(TARGET libvorbis
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\include;
    ..\..\..\libogg\include;
    J:\NlcJunk\DependLibs\libvorbis\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libvorbis\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libvorbis\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libvorbis\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libvorbis PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;_DEBUG;_WINDOWS;_LIB;LIBVORBIS_EXPORTS>
)

# SDL check
target_compile_options(libvorbis PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libvorbis PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libvorbis PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libvorbis PRIVATE
    ""
  )
endif ()
