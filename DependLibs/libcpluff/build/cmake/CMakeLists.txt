cmake_minimum_required(VERSION 3.12.2)

project(libcpluff)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libcpluff STATIC
  ../../config_libcpluff.h
  ../../context.c
  ../../cpluff.c
  ../../cpluff.h
  ../../cpluffdef_linux.h
  ../../cpluffdef_win32.h
  ../../defines.h
  ../../internal.h
  ../../kazlib/hash.c
  ../../kazlib/hash.h
  ../../kazlib/list.h
  ../../kazlib/list_cp.c
  ../../logging_cpluff.c
  ../../pcontrol.c
  ../../pinfo.c
  ../../ploader.c
  ../../pscan.c
  ../../psymbol.c
  ../../serial.c
  ../../thread.h
  ../../thread_posix.c
  ../../thread_windows.c
  ../../util_cpluff.c
  ../../util_cpluff.h
)
# Output file name
set_target_properties(libcpluff
  PROPERTIES
  OUTPUT_NAME_DEBUG libcpluffD
)

# Additional include directories
set_property(TARGET libcpluff
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\..\libfreetype\include;
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\..\libfribid\lib;
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\..\libfribid\charset;
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libcpluff\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libcpluff PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libcpluff PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libcpluff PRIVATE
    ""
  )
endif ()
