cmake_minimum_required(VERSION 3.12.2)

project(liblame)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(liblame STATIC
  ../../configMS.h
  ../../include/lame.h
  ../../libmp3lame/bitstream.h
  ../../libmp3lame/bitstream_lame.c
  ../../libmp3lame/encoder.h
  ../../libmp3lame/encoder_lame.c
  ../../libmp3lame/fft.h
  ../../libmp3lame/fft_lame.c
  ../../libmp3lame/gain_analysis.c
  ../../libmp3lame/gain_analysis.h
  ../../libmp3lame/i386/nasm.h
  ../../libmp3lame/id3tag.c
  ../../libmp3lame/id3tag.h
  ../../libmp3lame/l3side.h
  ../../libmp3lame/lame.c
  ../../libmp3lame/lame_global_flags.h
  ../../libmp3lame/lame-analysis.h
  ../../libmp3lame/lameerror.h
  ../../libmp3lame/machine.h
  ../../libmp3lame/mpglib_interface.c
  ../../libmp3lame/newmdct.c
  ../../libmp3lame/newmdct.h
  ../../libmp3lame/presets.c
  ../../libmp3lame/psymodel.c
  ../../libmp3lame/psymodel.h
  ../../libmp3lame/quantize.h
  ../../libmp3lame/quantize_lame.c
  ../../libmp3lame/quantize_pvt.c
  ../../libmp3lame/quantize_pvt.h
  ../../libmp3lame/reservoir.c
  ../../libmp3lame/reservoir.h
  ../../libmp3lame/set_get.c
  ../../libmp3lame/set_get.h
  ../../libmp3lame/tables.h
  ../../libmp3lame/tables_lame.c
  ../../libmp3lame/takehiro.c
  ../../libmp3lame/util.h
  ../../libmp3lame/util_lame.c
  ../../libmp3lame/vbrquantize.c
  ../../libmp3lame/vbrquantize.h
  ../../libmp3lame/VbrTag.c
  ../../libmp3lame/VbrTag.h
  ../../libmp3lame/vector/lame_intrin.h
  ../../libmp3lame/vector/xmm_quantize_sub.c
  ../../libmp3lame/version.h
  ../../libmp3lame/version_lame.c
  ../../mpglib/common.h
  ../../mpglib/common_lame.c
  ../../mpglib/dct64_i386.c
  ../../mpglib/dct64_i386.h
  ../../mpglib/decode_i386.c
  ../../mpglib/decode_i386.h
  ../../mpglib/huffman.h
  ../../mpglib/interface.c
  ../../mpglib/interface.h
  ../../mpglib/l2tables.h
  ../../mpglib/layer1.c
  ../../mpglib/layer1.h
  ../../mpglib/layer2.c
  ../../mpglib/layer2.h
  ../../mpglib/layer3.c
  ../../mpglib/layer3.h
  ../../mpglib/mpg123.h
  ../../mpglib/mpglib.h
  ../../mpglib/tabinit.c
  ../../mpglib/tabinit.h
)
# Output file name
set_target_properties(liblame
  PROPERTIES
  OUTPUT_NAME_DEBUG liblameD
)

# Additional include directories
set_property(TARGET liblame
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\mpglib;
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\include;
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\libmp3lame;
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\liblame\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(liblame PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;TARGET_OS_WINDOWS;_WINDOWS;HAVE_MPGLIB;WIN64;HAVE_CONFIG_H>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(liblame PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(liblame PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(liblame PRIVATE
    ""
  )
endif ()
