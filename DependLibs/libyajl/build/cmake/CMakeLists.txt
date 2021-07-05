cmake_minimum_required(VERSION 3.12.2)

project(libyajl)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libyajl STATIC
  ../../yajl.c
  ../../yajl_alloc.c
  ../../yajl_alloc.h
  ../../yajl_buf.c
  ../../yajl_buf.h
  ../../yajl_bytestack.h
  ../../yajl_encode.c
  ../../yajl_encode.h
  ../../yajl_gen.c
  ../../yajl_lex.c
  ../../yajl_lex.h
  ../../yajl_parser.c
  ../../yajl_parser.h
  ../../yajl_tree.c
  ../../yajl_version.c
)
# Output file name
set_target_properties(libyajl
  PROPERTIES
  OUTPUT_NAME_DEBUG libyajlD
)

# Additional include directories
set_property(TARGET libyajl
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libyajl\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libyajl\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libyajl\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libyajl\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libyajl PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libyajl PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libyajl PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libyajl PRIVATE
    ""
  )
endif ()
