cmake_minimum_required(VERSION 3.12.2)

project(libnfs)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libnfs STATIC
  ../../config_libnfs.h
  ../../init.c
  ../../libnfs.c
  ../../libnfs.h
  ../../libnfs-private.h
  ../../libnfs-raw.h
  ../../libnfs-raw-mount.c
  ../../libnfs-raw-mount.h
  ../../libnfs-raw-nfs.c
  ../../libnfs-raw-nfs.h
  ../../libnfs-raw-nlm.c
  ../../libnfs-raw-nlm.h
  ../../libnfs-raw-nsm.c
  ../../libnfs-raw-nsm.h
  ../../libnfs-raw-portmap.c
  ../../libnfs-raw-portmap.h
  ../../libnfs-raw-rquota.c
  ../../libnfs-raw-rquota.h
  ../../libnfs-sync.c
  ../../libnfs-zdr.c
  ../../libnfs-zdr.h
  ../../mount.c
  ../../nfs.c
  ../../nfsacl.c
  ../../nlm.c
  ../../nsm.c
  ../../pdu.c
  ../../portmap.c
  ../../rquota.c
  ../../slist.h
  ../../socket.c
  ../../win32_compat.c
  ../../win32_compat.h
  ../../win32_errnowrapper.h
)
# Output file name
set_target_properties(libnfs
  PROPERTIES
  OUTPUT_NAME_DEBUG libnfsD
)

# Additional include directories
set_property(TARGET libnfs
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\;
    ..\..\nfsutil;
    J:\NlcJunk\DependLibs\libnfs\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libnfs\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libnfs\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libnfs PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;NDEBUG;_WINDOWS;_LIB;_U_=;__STDC_CONSTANT_MACROS>
)

# SDL check
target_compile_options(libnfs PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libnfs PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libnfs PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libnfs PRIVATE
    ""
  )
endif ()

# Link libraries
set_property(TARGET libnfs
  APPEND PROPERTY LINK_LIBRARIES
  "$<$<CONFIG:Debug>:oncrpc.lib;ws2_32.lib>"
)
