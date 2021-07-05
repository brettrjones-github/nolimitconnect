cmake_minimum_required(VERSION 3.12.2)

project(libgsm)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libgsm STATIC
  ../../inc/config_gsm.h
  ../../inc/gsm.h
  ../../inc/private.h
  ../../inc/proto.h
  ../../inc/toast.h
  ../../inc/unproto.h
  ../../src/add.c
  ../../src/code.c
  ../../src/debug.c
  ../../src/decode.c
  ../../src/gsm_create.c
  ../../src/gsm_decode.c
  ../../src/gsm_destroy.c
  ../../src/gsm_encode.c
  ../../src/gsm_explode.c
  ../../src/gsm_implode.c
  ../../src/gsm_option.c
  ../../src/gsm_print.c
  ../../src/long_term.c
  ../../src/lpc.c
  ../../src/preprocess.c
  ../../src/rpe.c
  ../../src/short_term.c
  ../../src/table.c
  ../../src/toast.c
  ../../src/toast_alaw.c
  ../../src/toast_audio.c
  ../../src/toast_lin.c
  ../../src/toast_ulaw.c
)
# Output file name
set_target_properties(libgsm
  PROPERTIES
  OUTPUT_NAME_DEBUG libgsmD
)

# Additional include directories
set_property(TARGET libgsm
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libgsm\build\VS2017\..\..\inc;
    J:\NlcJunk\DependLibs\libgsm\build\VS2017\..\..\src;
    J:\NlcJunk\DependLibs\libgsm\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libgsm\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libgsm\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libgsm\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libgsm PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN32;_DEBUG;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libgsm PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libgsm PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libgsm PRIVATE
    ""
  )
endif ()
