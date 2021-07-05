cmake_minimum_required(VERSION 3.12.2)

project(libshine)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libshine STATIC
  ../../bitstream.c
  ../../bitstream.h
  ../../config_libshine.h
  ../../huffman.c
  ../../huffman.h
  ../../l3bitstream.c
  ../../l3bitstream.h
  ../../l3loop.c
  ../../l3loop.h
  ../../l3mdct.c
  ../../l3mdct.h
  ../../l3subband.c
  ../../l3subband.h
  ../../layer3.c
  ../../layer3.h
  ../../mult_mips_gcc.h
  ../../mult_noarch_gcc.h
  ../../mult_sarm_gcc.h
  ../../reservoir.c
  ../../reservoir.h
  ../../tables.c
  ../../tables.h
  ../../types_libshine.h
)
# Output file name
set_target_properties(libshine
  PROPERTIES
  OUTPUT_NAME_DEBUG libshineD
)

# Additional include directories
set_property(TARGET libshine
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\codec\decoder\core\inc;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\codec\common\inc;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\codec\api\svc;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\codec\;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\codec\..\;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libshine\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libshine PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;_LIB;X86_ASM>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libshine PRIVATE
    "$<$<CONFIG:Debug>:/Gm>"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libshine PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libshine PRIVATE
    ""
  )
endif ()
