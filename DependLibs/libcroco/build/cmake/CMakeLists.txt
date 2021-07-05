cmake_minimum_required(VERSION 3.12.2)

project(libcroco)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libcroco STATIC
  ../../cr-additional-sel.c
  ../../cr-additional-sel.h
  ../../cr-attr-sel.c
  ../../cr-attr-sel.h
  ../../cr-cascade.c
  ../../cr-cascade.h
  ../../cr-declaration.c
  ../../cr-declaration.h
  ../../cr-doc-handler.c
  ../../cr-doc-handler.h
  ../../cr-enc-handler.c
  ../../cr-enc-handler.h
  ../../cr-fonts.c
  ../../cr-fonts.h
  ../../cr-input.c
  ../../cr-input.h
  ../../cr-num.c
  ../../cr-num.h
  ../../cr-om-parser.c
  ../../cr-om-parser.h
  ../../cr-parser.c
  ../../cr-parser.h
  ../../cr-parsing-location.c
  ../../cr-parsing-location.h
  ../../cr-prop-list.c
  ../../cr-prop-list.h
  ../../cr-pseudo.c
  ../../cr-pseudo.h
  ../../cr-rgb.c
  ../../cr-rgb.h
  ../../cr-selector.c
  ../../cr-selector.h
  ../../cr-sel-eng.c
  ../../cr-sel-eng.h
  ../../cr-simple-sel.c
  ../../cr-simple-sel.h
  ../../cr-statement.c
  ../../cr-statement.h
  ../../cr-string.c
  ../../cr-string.h
  ../../cr-style.c
  ../../cr-style.h
  ../../cr-stylesheet.c
  ../../cr-stylesheet.h
  ../../cr-term.c
  ../../cr-term.h
  ../../cr-tknzr.c
  ../../cr-tknzr.h
  ../../cr-token.c
  ../../cr-token.h
  ../../cr-utils.c
  ../../cr-utils.h
  ../../guid.h
  ../../libcroco.h
  ../../libcroco-config.h
)
# Output file name
set_target_properties(libcroco
  PROPERTIES
  OUTPUT_NAME_DEBUG libcrocoD
)

# Additional include directories
set_property(TARGET libcroco
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libcroco\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libcroco\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libcroco\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libcroco PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_MBCS;TARGET_OS_WINDOWS;_HAS_ITERATOR_DEBUGGING=0>
)

# SDL check
target_compile_options(libcroco PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libcroco PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libcroco PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libcroco PRIVATE
    ""
  )
endif ()
