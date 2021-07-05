cmake_minimum_required(VERSION 3.12.2)

project(FfmpegPostProcLib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(FfmpegPostProcLib STATIC
  ../../postprocess.c
  ../../postprocess.h
  ../../postprocess_altivec_template.c
  ../../postprocess_internal.h
  ../../postprocess_template.c
  ../../version.h
)
# Output file name
set_target_properties(FfmpegPostProcLib
  PROPERTIES
  OUTPUT_NAME_DEBUG FfmpegPostProcLibD
)

# Additional include directories
set_property(TARGET FfmpegPostProcLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\..\..\..\sysheaders\;
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\..\..\..\GoTvCore\xbmc\xbmc;
    J:\NlcJunk\DependLibs\ffmpeg\libpostproc\build\VS2017\..\..\..\..\openssl-1.0.2p\include\>
)

# Preprocessor definitions
target_compile_definitions(FfmpegPostProcLib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(FfmpegPostProcLib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(FfmpegPostProcLib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(FfmpegPostProcLib PRIVATE
    ""
  )
endif ()
