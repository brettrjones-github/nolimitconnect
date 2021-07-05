cmake_minimum_required(VERSION 3.12.2)

project(libtinyxml)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libtinyxml STATIC
  ../../tinystr.cpp
  ../../tinystr.h
  ../../tinyxml.cpp
  ../../tinyxml.h
  ../../tinyxml2.cpp
  ../../tinyxml2.h
  ../../tinyxmlerror.cpp
  ../../tinyxmlparser.cpp
)
# Output file name
set_target_properties(libtinyxml
  PROPERTIES
  OUTPUT_NAME_DEBUG libtinyxmlD
)

# Additional include directories
set_property(TARGET libtinyxml
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libtinyxml\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libtinyxml\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libtinyxml\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libtinyxml\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libtinyxml PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;TARGET_WINDOWS;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libtinyxml PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libtinyxml PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libtinyxml PRIVATE
    ""
  )
endif ()
