cmake_minimum_required(VERSION 3.12.2)

project(libpng)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libpng STATIC
  ../../png.c
  ../../png.h
  ../../pngconf.h
  ../../pngerror.c
  ../../pngget.c
  ../../pngmem.c
  ../../pngpread.c
  ../../pngpriv.h
  ../../pngread.c
  ../../pngrio.c
  ../../pngrtran.c
  ../../pngrutil.c
  ../../pngset.c
  ../../pngtrans.c
  ../../pngwio.c
  ../../pngwrite.c
  ../../pngwtran.c
  ../../pngwutil.c
  ../../scripts/pngwin.rc
)
# Output file name
set_target_properties(libpng
  PROPERTIES
  OUTPUT_NAME_DEBUG libpngD
)

# Additional include directories
set_property(TARGET libpng
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..;
    ..\..\..\libzlib;
    ..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libpng PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN32;_DEBUG;DEBUG;PNG_DEBUG=1;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libpng PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libpng PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  set_property(SOURCE ../../pngerror.c
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yc"pngpriv.h">")
  set_property(SOURCE ../../pngget.c ../../pngmem.c ../../pngpread.c ../../pngread.c ../../pngrio.c ../../pngrtran.c ../../pngrutil.c ../../pngset.c ../../pngtrans.c ../../pngwio.c ../../pngwrite.c ../../pngwtran.c ../../pngwutil.c
    APPEND_STRING PROPERTY COMPILE_FLAGS
    "$<$<CONFIG:Debug>:/Yu"pngpriv.h">")
endif ()
