cmake_minimum_required(VERSION 3.12.2)

project(Upnp2Lib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(Upnp2Lib STATIC
  ../../AltXmlParser.h
  ../../UpnpAction.h
  ../../UpnpAllowedValue.h
  ../../UpnpAltXmlParser.cpp
  ../../UpnpClientInterface.h
  ../../UpnpDevice.h
  ../../UpnpParser.cpp
  ../../UpnpParser.h
  ../../UpnpPortForward.cpp
  ../../UpnpPortForward.h
  ../../UpnpRootDevice.cpp
  ../../UpnpRootDevice.h
  ../../UpnpService.cpp
  ../../UpnpService.h
  ../../UpnpStateVariable.h
  ../../UpnpTcpSkt.cpp
  ../../UpnpTcpSkt.h
)
# Output file name
set_target_properties(Upnp2Lib
  PROPERTIES
  OUTPUT_NAME_DEBUG Upnp2LibD
)

# Additional include directories
set_property(TARGET Upnp2Lib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\Upnp2Lib\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\Upnp2Lib\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\Upnp2Lib\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\Upnp2Lib\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(Upnp2Lib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;HAVE_CONFIG_H;WIN64;_DEBUG;_LIB;TARGET_OS_WINDOWS;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(Upnp2Lib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(Upnp2Lib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(Upnp2Lib PRIVATE
    ""
  )
endif ()
