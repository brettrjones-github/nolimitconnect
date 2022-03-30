cmake_minimum_required(VERSION 3.12.2)

project(libasprintf)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libasprintf STATIC
  ../../../../AppCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../AppCpuArchDefines.h
  ../../../../AppDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../AppTargetOsConfig.h
  ../../alloca.c
  ../../alloca.in.h
  ../../asnprintf.c
  ../../asprintf.c
  ../../autosprintf.cc
  ../../autosprintf.h
  ../../config_libasprintf.h
  ../../errno.in.h
  ../../lib-asprintf.c
  ../../lib-asprintf.h
  ../../printf-args.c
  ../../printf-args.h
  ../../printf-parse.c
  ../../printf-parse.h
  ../../scripts/pngwin.rc
  ../../size_max.h
  ../../vasnprintf.c
  ../../vasnprintf.h
  ../../vasprintf.c
  ../../vasprintf.h
  ../../verify.h
  ../../xsize_asprintf.c
  ../../xsize_asprintf.h
)
# Output file name
set_target_properties(libasprintf
  PROPERTIES
  OUTPUT_NAME_DEBUG libasprintfD
)

# Additional include directories
set_property(TARGET libasprintf
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\;
    ..\..\..\zlib;
    ..\..\..\;
    ..\..\..\..\;
    ..\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libasprintf PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;_DEBUG;DEBUG;PNG_DEBUG=1;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libasprintf PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libasprintf PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libasprintf PRIVATE
    ""
  )
endif ()
