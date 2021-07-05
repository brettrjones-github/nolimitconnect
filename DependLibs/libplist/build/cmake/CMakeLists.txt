cmake_minimum_required(VERSION 3.12.2)

project(libplist)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libplist STATIC
  ../../Array.cpp
  ../../base64.c
  ../../Boolean.cpp
  ../../bplist.c
  ../../bytearray.c
  ../../config_libplist.h
  ../../Data.cpp
  ../../Date.cpp
  ../../Dictionary.cpp
  ../../hashtable.c
  ../../Integer.cpp
  ../../Key.cpp
  ../../Key.h
  ../../libcnary/cnary.c
  ../../libcnary/include/iterator.h
  ../../libcnary/include/list.h
  ../../libcnary/include/node.h
  ../../libcnary/include/node_iterator.h
  ../../libcnary/include/node_list.h
  ../../libcnary/include/object.h
  ../../libcnary/iterator.c
  ../../libcnary/list.c
  ../../libcnary/node.c
  ../../libcnary/node_iterator.c
  ../../libcnary/node_list.c
  ../../Node.cpp
  ../../plist.c
  ../../plist/Array.h
  ../../plist/Boolean.h
  ../../plist/Data.h
  ../../plist/Date.h
  ../../plist/Dictionary.h
  ../../plist/Integer.h
  ../../plist/Key.h
  ../../plist/Node.h
  ../../plist/plist.h
  ../../plist/plist++.h
  ../../plist/Real.h
  ../../plist/String.h
  ../../plist/Structure.h
  ../../plist/Uid.h
  ../../ptrarray.c
  ../../Real.cpp
  ../../String.cpp
  ../../Structure.cpp
  ../../time64.c
  ../../time64_limits.h
  ../../Uid.cpp
  ../../xplist.c
)
# Output file name
set_target_properties(libplist
  PROPERTIES
  OUTPUT_NAME_DEBUG libplistD
)

# Additional include directories
set_property(TARGET libplist
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libplist\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libplist\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libplist\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libplist\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libplist PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN64;_DEBUG;_WINDOWS;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libplist PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libplist PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libplist PRIVATE
    ""
  )
endif ()
