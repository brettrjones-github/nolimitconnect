cmake_minimum_required(VERSION 3.12.2)

project(FfmpegAvResampleLib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(FfmpegAvResampleLib STATIC
  ../../../libswresample/dither_swresample.c
  ../../../libswresample/options_swresample.c
  ../../../libswresample/resample_swresample.c
  ../../audio_convert.c
  ../../audio_convert.h
  ../../audio_data.c
  ../../audio_data.h
  ../../audio_mix.c
  ../../audio_mix.h
  ../../audio_mix_matrix.c
  ../../avresample.h
  ../../avresample_internal.h
  ../../dither.h
  ../../resample.h
  ../../resample_template.c
  ../../utils.c
  ../../version.h
)
# Output file name
set_target_properties(FfmpegAvResampleLib
  PROPERTIES
  OUTPUT_NAME_DEBUG FfmpegAvResampleLibD
)

# Additional include directories
set_property(TARGET FfmpegAvResampleLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\..\..\..\sysheaders\;
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\..\..\..\GoTvCore\xbmc\xbmc;
    J:\NlcJunk\DependLibs\ffmpeg\libavresample\build\VS2017\..\..\..\..\openssl-1.0.2p\include\>
)

# Preprocessor definitions
target_compile_definitions(FfmpegAvResampleLib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(FfmpegAvResampleLib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(FfmpegAvResampleLib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(FfmpegAvResampleLib PRIVATE
    ""
  )
endif ()
