cmake_minimum_required(VERSION 3.12.2)

project(libxpm)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libxpm STATIC
  ../../include/X11/xpm.h
  ../../src/amigax.h
  ../../src/Attrib.c
  ../../src/CrBufFrI.c
  ../../src/CrDatFrI.c
  ../../src/create.c
  ../../src/CrIFrBuf.c
  ../../src/CrIFrDat.c
  ../../src/data.c
  ../../src/hashtab.c
  ../../src/Image.c
  ../../src/Info.c
  ../../src/misc.c
  ../../src/parse.c
  ../../src/RdFToBuf.c
  ../../src/RdFToDat.c
  ../../src/RdFToI.c
  ../../src/rgb.c
  ../../src/rgbtab.h
  ../../src/scan.c
  ../../src/simx.c
  ../../src/simx.h
  ../../src/WrFFrBuf.c
  ../../src/WrFFrDat.c
  ../../src/WrFFrI.c
  ../../src/XpmI.h
)
# Output file name
set_target_properties(libxpm
  PROPERTIES
  OUTPUT_NAME_DEBUG libxpmD
)

# Additional include directories
set_property(TARGET libxpm
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libxpm\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libxpm\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libxpm\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libxpm\build\VS2017\..\..\include\X11>
)

# Preprocessor definitions
target_compile_definitions(libxpm PRIVATE
  $<$<CONFIG:Debug>:WIN64;_DEBUG;_WINDOWS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libxpm PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libxpm PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libxpm PRIVATE
    ""
  )
endif ()
