cmake_minimum_required(VERSION 3.12.2)

project(libdnssd)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libdnssd STATIC
  ../../config_libmdnsresponder.h
  ../../mDNSShared/CommonServices.h
  ../../mDNSShared/DebugServices.c
  ../../mDNSShared/DebugServices.h
  ../../mDNSShared/dns_sd.h
  ../../mDNSShared/dnssd_clientlib.c
  ../../mDNSShared/dnssd_clientstub.c
  ../../mDNSShared/dnssd_ipc.c
  ../../mDNSShared/dnssd_ipc.h
  ../../mDNSShared/GenLinkedList.c
  ../../mDNSShared/GenLinkedList.h
  dllmain.c
  resource.h
)
# Output file name
set_target_properties(libdnssd
  PROPERTIES
  OUTPUT_NAME_DEBUG libdnssdD
)

# Additional include directories
set_property(TARGET libdnssd
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ../../mDNSCore;
    ../../mDNSShared;
    ../../mDNSWindows;
    J:\NlcJunk\DependLibs\libmdnsresponder\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libmdnsresponder\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libmdnsresponder\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libmdnsresponder\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libdnssd PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN64;_DEBUG;_WINDOWS;_LIB;DEBUG=1;NOT_HAVE_SA_LEN;MDNS_DEBUGMSGS=0;WIN32_LEAN_AND_MEAN;USE_TCP_LOOPBACK;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# SDL check
target_compile_options(libdnssd PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libdnssd PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libdnssd PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libdnssd PRIVATE
    ""
  )
endif ()

# Link libraries
set_property(TARGET libdnssd
  APPEND PROPERTY LINK_LIBRARIES
  "$<$<CONFIG:Debug>:ws2_32.lib;iphlpapi.lib>"
)
