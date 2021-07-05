cmake_minimum_required(VERSION 3.12.2)

project(libexpat)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libexpat STATIC
  ../../lib/ascii.h
  ../../lib/asciitab.h
  ../../lib/expat.h
  ../../lib/expat_external.h
  ../../lib/iasciitab.h
  ../../lib/internal.h
  ../../lib/latin1tab.h
  ../../lib/nametab.h
  ../../lib/utf8tab.h
  ../../lib/xmlparse.c
  ../../lib/xmlrole.c
  ../../lib/xmlrole.h
  ../../lib/xmltok.c
  ../../lib/xmltok.h
  ../../lib/xmltok_impl.c
  ../../lib/xmltok_impl.h
  ../../lib/xmltok_ns.c
)
# Output file name
set_target_properties(libexpat
  PROPERTIES
  OUTPUT_NAME_DEBUG libexpatD
)

# Additional include directories
set_property(TARGET libexpat
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libexpat\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libexpat\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libexpat\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libexpat PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;WIN32;_DEBUG;_WINDOWS;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libexpat PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libexpat PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libexpat PRIVATE
    ""
  )
endif ()
