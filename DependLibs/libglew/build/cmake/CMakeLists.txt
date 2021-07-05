cmake_minimum_required(VERSION 3.12.2)

project(libglew)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libglew STATIC
  ../../include/GL/glew.h
  ../../include/GL/wglew.h
  ../../src/glew.c
  ../glew.rc
)
# Output file name
set_target_properties(libglew
  PROPERTIES
  OUTPUT_NAME_DEBUG glew64sd
)

# Preprocessor definitions
target_compile_definitions(libglew PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;_DEBUG;_LIB;WIN32_LEAN_AND_MEAN;GLEW_STATIC>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libglew PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libglew PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libglew PRIVATE
    ""
  )
endif ()
