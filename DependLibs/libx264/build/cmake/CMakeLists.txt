cmake_minimum_required(VERSION 3.12.2)

project(libx264)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libx264 STATIC
  ../../../../arpa/inet.h
  ../../../../asm/types.h
  ../../../../byteswap.h
  ../../../../dirent.h
  ../../../../dlfcn.h
  ../../../../GoTvCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../GoTvCpuArchDefines.h
  ../../../../GoTvDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../GoTvTargetOsConfig.h
  ../../../../langinfo.h
  ../../../../linux/ioctl.h
  ../../../../linux/param.h
  ../../../../netdb.h
  ../../../../netinet/in.h
  ../../../../netinet/tcp.h
  ../../../../strings.h
  ../../../../sys/ioctl.h
  ../../../../sys/mman.h
  ../../../../sys/param.h
  ../../../../sys/select.h
  ../../../../sys/signal.h
  ../../../../sys/socket.h
  ../../../../sys/syslog.h
  ../../../../sys/time.h
  ../../../../sys/times.h
  ../../../../sys/un.h
  ../../../../sys/utsname.h
  ../../../../syslog.h
  ../../../../unistd.h
  ../../common/bitstream.h
  ../../common/bitstream_x264.c
  ../../common/cabac.h
  ../../common/cabac_x264.c
  ../../common/common.h
  ../../common/common_x264.c
  ../../common/cpu.h
  ../../common/cpu_x264.c
  ../../common/dct.h
  ../../common/dct_x264.c
  ../../common/deblock_x264.c
  ../../common/frame.h
  ../../common/frame_x264.c
  ../../common/macroblock.h
  ../../common/macroblock_x264.c
  ../../common/mc.h
  ../../common/mc_x264.c
  ../../common/mvpred_x264.c
  ../../common/opencl.h
  ../../common/opencl_x264.c
  ../../common/osdep.h
  ../../common/osdep_x264.c
  ../../common/pixel.h
  ../../common/pixel_x264.c
  ../../common/predict.h
  ../../common/predict_x264.c
  ../../common/quant.h
  ../../common/quant_x264.c
  ../../common/rectangle.h
  ../../common/rectangle_x264.c
  ../../common/set.h
  ../../common/set_x264.c
  ../../common/threadpool.h
  ../../common/threadpool_x264.c
  ../../common/vlc_x264.c
  ../../common/win32thread.h
  ../../common/win32thread_x264.c
  ../../config_libx264.h
  ../../x264.h
  ../../x264cli.h
)
# Output file name
set_target_properties(libx264
  PROPERTIES
  OUTPUT_NAME_DEBUG libx264D
)

# Additional include directories
set_property(TARGET libx264
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libx264\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libx264\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libx264\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libx264\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libx264 PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libx264 PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libx264 PRIVATE
    ""
  )
endif ()
