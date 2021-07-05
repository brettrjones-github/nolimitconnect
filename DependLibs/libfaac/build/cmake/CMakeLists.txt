cmake_minimum_required(VERSION 3.12.2)

project(libfaac)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libfaac STATIC
  ../../aacquant.c
  ../../aacquant.h
  ../../backpred.c
  ../../backpred.h
  ../../bitstream.c
  ../../bitstream.h
  ../../channels.c
  ../../channels.h
  ../../coder.h
  ../../config_libfaac.h
  ../../fft.c
  ../../fft.h
  ../../filtbank.c
  ../../filtbank.h
  ../../frame.c
  ../../frame.h
  ../../huffman.c
  ../../huffman.h
  ../../hufftab.h
  ../../include/faac.h
  ../../include/faaccfg.h
  ../../joint.h
  ../../ltp.c
  ../../ltp.h
  ../../midside.c
  ../../psych.h
  ../../psychkni.c
  ../../tns.c
  ../../tns.h
  ../../util.c
  ../../util.h
)
# Output file name
set_target_properties(libfaac
  PROPERTIES
  OUTPUT_NAME_DEBUG libfaacD
)

# Additional include directories
set_property(TARGET libfaac
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libfaac\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libfaac\build\VS2017\..\..\include;
    J:\NlcJunk\DependLibs\libfaac\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libfaac\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libfaac\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libfaac PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN64;_DEBUG;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libfaac PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libfaac PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libfaac PRIVATE
    ""
  )
endif ()
