cmake_minimum_required(VERSION 3.12.2)

project(VxVideoLib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(VxVideoLib STATIC
  ../../GdvBufferUtil.cpp
  ../../GdvBufferUtil.h
  ../../IVxVidCap.h
  ../../VxRescaleRgb.cpp
  ../../VxRescaleRgb.h
  ../../VxVidCap.cpp
  ../../VxVidCap.h
  ../../VxVidCapImp.cpp
  ../../VxVidCapImp.h
  ../../VxVideoConvert.cpp
  ../../VxVideoConvert.h
  ../../VxVideoLib.h
)
# Output file name
set_target_properties(VxVideoLib
  PROPERTIES
  OUTPUT_NAME_DEBUG VxVideoLibD
)

# Additional include directories
set_property(TARGET VxVideoLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\VxVideoLib\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\VxVideoLib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\VxVideoLib\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\VxVideoLib\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(VxVideoLib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;_DEBUG;_LIB;TARGET_OS_WINDOWS;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(VxVideoLib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(VxVideoLib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(VxVideoLib PRIVATE
    ""
  )
endif ()
