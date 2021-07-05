cmake_minimum_required(VERSION 3.12.2)

project(libcelt)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libcelt STATIC
  ../../arch_libcelt.h
  ../../bands_libcelt.c
  ../../bands_libcelt.h
  ../../celt_header_libcelt.h
  ../../celt_libcelt.c
  ../../celt_libcelt.h
  ../../celt_types_libcelt.h
  ../../config_libcelt.h
  ../../cwrs_libcelt.c
  ../../cwrs_libcelt.h
  ../../ecintrin_libcelt.h
  ../../entcode_libcelt.c
  ../../entcode_libcelt.h
  ../../entdec_libcelt.c
  ../../entdec_libcelt.h
  ../../entenc_libcelt.c
  ../../entenc_libcelt.h
  ../../fixed_debug_libcelt.h
  ../../fixed_generic_libcelt.h
  ../../float_cast_libcelt.h
  ../../header_libcelt.c
  ../../kiss_fft_libcelt.c
  ../../kiss_fft_libcelt.h
  ../../laplace_libcelt.c
  ../../laplace_libcelt.h
  ../../mathops_libcelt.c
  ../../mathops_libcelt.h
  ../../mdct_libcelt.c
  ../../mdct_libcelt.h
  ../../modes_libcelt.c
  ../../modes_libcelt.h
  ../../os_support_libcelt.h
  ../../pitch_libcelt.c
  ../../pitch_libcelt.h
  ../../plc_libcelt.c
  ../../plc_libcelt.h
  ../../quant_bands_libcelt.c
  ../../quant_bands_libcelt.h
  ../../rate_libcelt.c
  ../../rate_libcelt.h
  ../../stack_alloc_libcelt.h
  ../../vq_libcelt.c
  ../../vq_libcelt.h
)
# Output file name
set_target_properties(libcelt
  PROPERTIES
  OUTPUT_NAME_DEBUG libceltD
)

# Additional include directories
set_property(TARGET libcelt
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libcelt\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libcelt\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libcelt\build\VS2017\..\..\..\..>
)

# Preprocessor definitions
target_compile_definitions(libcelt PRIVATE
  $<$<CONFIG:Debug>:_MBCS;HAVE_CONFIG_H;WIN64;_DEBUG;_LIB>
)

# SDL check
target_compile_options(libcelt PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libcelt PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libcelt PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libcelt PRIVATE
    ""
  )
endif ()
