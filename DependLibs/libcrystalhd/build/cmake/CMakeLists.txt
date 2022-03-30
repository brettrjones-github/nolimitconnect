cmake_minimum_required(VERSION 3.12.2)

project(libcrystalhd)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libcrystalhd STATIC
  ../../../../base64.h
  ../../../../byteswap.h
  ../../../../dirent.h
  ../../../../dlfcn.h
  ../../../../AppCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../AppCpuArchDefines.h
  ../../../../AppDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../AppTargetOsConfig.h
  ../../../../langinfo.h
  ../../../../linux/ioctl.h
  ../../../../linux/param.h
  ../../../../netdb.h
  ../../../../pwd.h
  ../../../../strings.h
  ../../../../sysheaders/dirent.h
  ../../../../sysheaders/netdb.h
  ../../../../sysheaders/pwd.h
  ../../../../sysheaders/strings.h
  ../../../../sysheaders/sys/cdefs.h
  ../../../../sysheaders/sys/file.h
  ../../../../sysheaders/sys/fse_open.c
  ../../../../sysheaders/sys/fse_zero.c
  ../../../../sysheaders/sys/fsext.c
  ../../../../sysheaders/sys/ioctl.h
  ../../../../sysheaders/sys/ipc.h
  ../../../../sysheaders/sys/mman.h
  ../../../../sysheaders/sys/param.h
  ../../../../sysheaders/sys/select.h
  ../../../../sysheaders/sys/shm.h
  ../../../../sysheaders/sys/signal.h
  ../../../../sysheaders/sys/socket.h
  ../../../../sysheaders/sys/syslog.h
  ../../../../sysheaders/sys/time.h
  ../../../../sysheaders/sys/times.h
  ../../../../sysheaders/sys/un.h
  ../../../../sysheaders/sys/utsname.h
  ../../../../sysheaders/sys/wait.h
  ../../../../sysheaders/syslog.h
  ../../../../sysheaders/termios.h
  ../../../../sysheaders/unistd.h
  ../../../../sysheaders/utime.h
  ../../../../syslog.h
  ../../../../unistd.h
  ../../include/7411d.h
  ../../include/bc_decoder_regs.h
  ../../include/bc_dts_defs.h
  ../../include/bc_dts_glob_lnx.h
  ../../include/bc_dts_types.h
  ../../include/config_libcrystalhd.h
  ../../include/libcrystalhd_version.h
  ../../include/link/bc_defines.h
  ../../include/link/bcm_70012_regs.h
  ../../src/libcrystalhd_fwcmds.c
  ../../src/libcrystalhd_fwcmds.h
  ../../src/libcrystalhd_fwdiag_if.c
  ../../src/libcrystalhd_fwdiag_if.h
  ../../src/libcrystalhd_fwload_if.c
  ../../src/libcrystalhd_fwload_if.h
  ../../src/libcrystalhd_if.c
  ../../src/libcrystalhd_if.h
  ../../src/libcrystalhd_int_if.c
  ../../src/libcrystalhd_int_if.h
  ../../src/libcrystalhd_parser.c
  ../../src/libcrystalhd_parser.h
  ../../src/libcrystalhd_priv.c
  ../../src/libcrystalhd_priv.h
)
# Output file name
set_target_properties(libcrystalhd
  PROPERTIES
  OUTPUT_NAME_DEBUG libcrystalhdD
)

# Additional include directories
set_property(TARGET libcrystalhd
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libcrystalhd\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libcrystalhd\build\VS2017\..\..\src;
    J:\NlcJunk\DependLibs\libcrystalhd\build\VS2017\..\..\include;
    J:\NlcJunk\DependLibs\libcrystalhd\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libcrystalhd\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libcrystalhd\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libcrystalhd PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libcrystalhd PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libcrystalhd PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libcrystalhd PRIVATE
    ""
  )
endif ()
