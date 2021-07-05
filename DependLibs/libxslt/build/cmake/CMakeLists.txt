cmake_minimum_required(VERSION 3.12.2)

project(libxslt)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libxslt STATIC
  ../../attributes.h
  ../../attributes_xslt.c
  ../../attrvt_xslt.c
  ../../documents.h
  ../../documents_xslt.c
  ../../extensions.h
  ../../extensions_xslt.c
  ../../extra.h
  ../../extra_xslt.c
  ../../functions.h
  ../../functions_xslt.c
  ../../imports.h
  ../../imports_xslt.c
  ../../keys.h
  ../../keys_xslt.c
  ../../libxslt.h
  ../../libxslt_config.h
  ../../libxslt_config_android.h
  ../../libxslt_config_linux.h
  ../../libxslt_config_windows.h
  ../../namespaces.h
  ../../namespaces_xslt.c
  ../../numbers_xslt.c
  ../../numbersInternals.h
  ../../pattern.h
  ../../pattern_xslt.c
  ../../preproc.h
  ../../preproc_xslt.c
  ../../security.h
  ../../security_xslt.c
  ../../templates.h
  ../../templates_xslt.c
  ../../transform.h
  ../../transform_xslt.c
  ../../trio.h
  ../../triodef.h
  ../../variables.h
  ../../variables_xslt.c
  ../../xslt.h
  ../../xslt_xslt.c
  ../../xsltexports.h
  ../../xsltInternals.h
  ../../xsltlocale.h
  ../../xsltlocale_xslt.c
  ../../xsltutils.h
  ../../xsltutils_xslt.c
)
# Output file name
set_target_properties(libxslt
  PROPERTIES
  OUTPUT_NAME_DEBUG libxsltD
)

# Additional include directories
set_property(TARGET libxslt
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libxslt\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libxslt\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libxslt\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libxslt\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libxslt PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN64;_DEBUG;_WINDOWS;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libxslt PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libxslt PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libxslt PRIVATE
    ""
  )
endif ()

# Link libraries
set_property(TARGET libxslt
  APPEND PROPERTY LINK_LIBRARIES
  "$<$<CONFIG:Debug>:J:/NlcJunk/build-staticlibs/Windows/x64/Xml2LibD.lib>"
)
