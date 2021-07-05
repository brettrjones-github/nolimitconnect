cmake_minimum_required(VERSION 3.12.2)

project(libshairplay)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libshairplay STATIC
  ../../alac/alac.c
  ../../alac/alac.h
  ../../alac/stdint_win.h
  ../../base64.c
  ../../base64.h
  ../../compat.h
  ../../config_libshairplay.h
  ../../crypto/aes.c
  ../../crypto/bigint.c
  ../../crypto/bigint.h
  ../../crypto/bigint_impl.h
  ../../crypto/crypto.h
  ../../crypto/hmac.c
  ../../crypto/md5.c
  ../../crypto/os_port.h
  ../../crypto/rc4.c
  ../../crypto/sha1.c
  ../../digest.c
  ../../digest.h
  ../../dnssd.c
  ../../dnssd.h
  ../../dnssdint.h
  ../../global.h
  ../../http_parser.c
  ../../http_parser.h
  ../../http_request.c
  ../../http_request.h
  ../../http_response.c
  ../../http_response.h
  ../../httpd.c
  ../../httpd.h
  ../../logger.c
  ../../logger.h
  ../../memalign.h
  ../../netutils.c
  ../../netutils.h
  ../../raop.c
  ../../raop.h
  ../../raop_buffer.c
  ../../raop_buffer.h
  ../../raop_rtp.c
  ../../raop_rtp.h
  ../../rsakey.c
  ../../rsakey.h
  ../../rsapem.c
  ../../rsapem.h
  ../../sdp.c
  ../../sdp.h
  ../../shairplay.c
  ../../sockets.h
  ../../threads.h
  ../../utils.c
  ../../utils.h
)
# Output file name
set_target_properties(libshairplay
  PROPERTIES
  OUTPUT_NAME_DEBUG libshairplayD
)

# Additional include directories
set_property(TARGET libshairplay
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libshairplay\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libshairplay\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libshairplay\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libshairplay\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libshairplay PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libshairplay PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libshairplay PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libshairplay PRIVATE
    ""
  )
endif ()
