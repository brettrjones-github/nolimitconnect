cmake_minimum_required(VERSION 3.12.2)

project(libdca)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libdca STATIC
  ../../ExifParse.h
  ../../IptcParse.h
  ../../JpegParse.h
  ../../libdca/bitstream.c
  ../../libdca/bitstream.h
  ../../libdca/dca_internal.h
  ../../libdca/downmix.c
  ../../libdca/parse.c
  ../../libdca/tables.h
  ../../libdca/tables_adpcm.h
  ../../libdca/tables_fir.h
  ../../libdca/tables_huffman.h
  ../../libdca/tables_quantization.h
  ../../libdca/tables_vq.h
)
# Output file name
set_target_properties(libdca
  PROPERTIES
  OUTPUT_NAME_DEBUG libdcaD
)

# Additional include directories
set_property(TARGET libdca
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libdca\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libdca\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libdca\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libdca PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libdca PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libdca PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libdca PRIVATE
    ""
  )
endif ()
