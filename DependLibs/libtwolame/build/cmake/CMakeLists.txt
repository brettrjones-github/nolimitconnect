cmake_minimum_required(VERSION 3.12.2)

project(libtwolame)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libtwolame STATIC
  ../../ath.h
  ../../ath_twolame.c
  ../../availbits.h
  ../../availbits_twolame.c
  ../../bitbuffer.h
  ../../bitbuffer_inline.h
  ../../bitbuffer_twolame.c
  ../../common.h
  ../../config_libtwolame.h
  ../../config_libtwolamewin.h
  ../../crc.h
  ../../crc_twolame.c
  ../../dab.h
  ../../dab_twolame.c
  ../../encode.h
  ../../encode_twolame.c
  ../../energy.h
  ../../energy_twolame.c
  ../../enwindow.h
  ../../fft.h
  ../../fft_twolame.c
  ../../get_set_twolame.c
  ../../mem.h
  ../../mem_twolame.c
  ../../psycho_0.h
  ../../psycho_0_twolame.c
  ../../psycho_1.h
  ../../psycho_1_critband.h
  ../../psycho_1_freqtable.h
  ../../psycho_1_twolame.c
  ../../psycho_2.h
  ../../psycho_2_absthr.h
  ../../psycho_2_twolame.c
  ../../psycho_3.h
  ../../psycho_3_twolame.c
  ../../psycho_4.h
  ../../psycho_4_twolame.c
  ../../psycho_n1.h
  ../../psycho_n1_twolame.c
  ../../subband.h
  ../../subband_twolame.c
  ../../twolame.h
  ../../twolame_twolame.c
  ../../util.h
  ../../util_twolame.c
  ../../winutil.h
)
# Output file name
set_target_properties(libtwolame
  PROPERTIES
  OUTPUT_NAME_DEBUG libtwolameD
)

# Additional include directories
set_property(TARGET libtwolame
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libtwolame\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libtwolame\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libtwolame\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libtwolame PRIVATE
  $<$<CONFIG:Debug>:WIN32;_DEBUG;_LIB;LIBTWOLAME_STATIC>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libtwolame PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libtwolame PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libtwolame PRIVATE
    ""
  )
endif ()
