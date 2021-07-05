cmake_minimum_required(VERSION 3.12.2)

project(libxavs)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libxavs STATIC
  ../../common/bs.h
  ../../common/common.c
  ../../common/common.h
  ../../common/cpu.c
  ../../common/cpu.h
  ../../common/csp.c
  ../../common/csp.h
  ../../common/dct.c
  ../../common/dct.h
  ../../common/frame.c
  ../../common/frame.h
  ../../common/macroblock.c
  ../../common/macroblock.h
  ../../common/mc.c
  ../../common/mc.h
  ../../common/mdate.c
  ../../common/osdep.h
  ../../common/pixel.c
  ../../common/pixel.h
  ../../common/predict.c
  ../../common/predict.h
  ../../common/quant.c
  ../../common/quant.h
  ../../common/set.c
  ../../common/set.h
  ../../common/vlc.h
  ../../config_libxavs.h
  ../../encoder/analyse.c
  ../../encoder/analyse.h
  ../../encoder/cavlc.c
  ../../encoder/encoder.c
  ../../encoder/eval.c
  ../../encoder/lookahead.c
  ../../encoder/macroblock.c
  ../../encoder/macroblock.h
  ../../encoder/me.c
  ../../encoder/me.h
  ../../encoder/ratecontrol.c
  ../../encoder/ratecontrol.h
  ../../encoder/set.c
  ../../encoder/set.h
  ../../extras/getopt.c
  ../../extras/getopt.h
  ../../extras/stdint.h
)
# Output file name
set_target_properties(libxavs
  PROPERTIES
  OUTPUT_NAME_DEBUG libxavsD
)

# Additional include directories
set_property(TARGET libxavs
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ../../common;
    ../../encoder;
    ../../extras;
    ../..;
    J:\NlcJunk\DependLibs\libxavs\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libxavs\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libxavs\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libxavs\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libxavs PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;_LIB;WIN64;__XAVS__;ARCH_X86;HAVE_STDINT_H;HAVE_PTHREAD;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libxavs PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libxavs PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libxavs PRIVATE
    ""
  )
endif ()
