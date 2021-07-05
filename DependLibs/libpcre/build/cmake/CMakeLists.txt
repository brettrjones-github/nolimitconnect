cmake_minimum_required(VERSION 3.12.2)

project(libpcre)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libpcre STATIC
  ../../config_pcre.c
  ../../config_pcre.h
  ../../dftables_pcre.c
  ../../pcre.h
  ../../pcre_chartables.c
  ../../pcre_compile.c
  ../../pcre_config_android.h
  ../../pcre_config_linux.h
  ../../pcre_config_windows.h
  ../../pcre_dfa_exec.c
  ../../pcre_exec.c
  ../../pcre_fullinfo.c
  ../../pcre_get.c
  ../../pcre_globals.c
  ../../pcre_info.c
  ../../pcre_internal.h
  ../../pcre_maketables.c
  ../../pcre_newline.c
  ../../pcre_ord2utf8.c
  ../../pcre_refcount.c
  ../../pcre_scanner.cc
  ../../pcre_scanner.h
  ../../pcre_stringpiece.cc
  ../../pcre_stringpiece.h
  ../../pcre_study.c
  ../../pcre_tables.c
  ../../pcre_try_flipped.c
  ../../pcre_ucp_searchfuncs.c
  ../../pcre_valid_utf8.c
  ../../pcre_version.c
  ../../pcre_xclass.c
  ../../pcrecpp.cc
  ../../pcrecpp.h
  ../../pcrecpparg.h
  ../../pcreposix.c
  ../../pcreposix.h
  ../../ucp.h
  ../../ucpinternal.h
  ../../ucptable_pcre.c
)
# Output file name
set_target_properties(libpcre
  PROPERTIES
  OUTPUT_NAME_DEBUG libpcreD
)

# Additional include directories
set_property(TARGET libpcre
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libpcre\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libpcre\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libpcre\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libpcre\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libpcre PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libpcre PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libpcre PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libpcre PRIVATE
    ""
  )
endif ()
