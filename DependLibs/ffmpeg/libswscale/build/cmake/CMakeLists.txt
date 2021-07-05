cmake_minimum_required(VERSION 3.12.2)

project(FfmpegSwScaleLib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(FfmpegSwScaleLib STATIC
  ../../alphablend.c
  ../../bayer_template.c
  ../../gamma.c
  ../../hscale.c
  ../../hscale_fast_bilinear.c
  ../../input.c
  ../../log2_tab.c
  ../../options_swoptions.c
  ../../output.c
  ../../rgb2rgb.c
  ../../rgb2rgb.h
  ../../rgb2rgb_template.c
  ../../slice.c
  ../../swscale.c
  ../../swscale.h
  ../../swscale_internal.h
  ../../swscale_unscaled.c
  ../../utils_swscale.c
  ../../version.h
  ../../vscale.c
  ../../yuv2rgb.c
)
# Output file name
set_target_properties(FfmpegSwScaleLib
  PROPERTIES
  OUTPUT_NAME_DEBUG FfmpegSwScaleLibD
)

# Additional include directories
set_property(TARGET FfmpegSwScaleLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\..\..\..\sysheaders\;
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\..\..\..\kodi_src\xbmc\xbmc;
    J:\NlcJunk\DependLibs\ffmpeg\libswscale\build\VS2017\..\..\..\..\openssl-1.0.2p\include\>
)

# Preprocessor definitions
target_compile_definitions(FfmpegSwScaleLib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;TARGET_OS_WINDOWS;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(FfmpegSwScaleLib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(FfmpegSwScaleLib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(FfmpegSwScaleLib PRIVATE
    ""
  )
endif ()
