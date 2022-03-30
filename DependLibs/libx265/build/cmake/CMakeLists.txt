cmake_minimum_required(VERSION 3.12.2)

project(libx265)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libx265 STATIC
  ../../../../arpa/inet.h
  ../../../../asm/types.h
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
  ../../common/bitstream_x265.cpp
  ../../common/common.h
  ../../common/common_x265.cpp
  ../../common/constants.h
  ../../common/constants_x265.cpp
  ../../common/contexts.h
  ../../common/cpu.h
  ../../common/cpu_x265.cpp
  ../../common/cudata.h
  ../../common/cudata_x265.cpp
  ../../common/dct_x265.cpp
  ../../common/deblock.h
  ../../common/deblock_x265.cpp
  ../../common/frame.h
  ../../common/frame_x265.cpp
  ../../common/framedata.h
  ../../common/framedata_x265.cpp
  ../../common/intrapred_x265.cpp
  ../../common/ipfilter_x265.cpp
  ../../common/loopfilter_x265.cpp
  ../../common/lowres.h
  ../../common/lowres_x265.cpp
  ../../common/md5.h
  ../../common/md5_x265.cpp
  ../../common/mv.h
  ../../common/param.h
  ../../common/param_x265.cpp
  ../../common/piclist.h
  ../../common/piclist_x265.cpp
  ../../common/picyuv.h
  ../../common/picyuv_x265.cpp
  ../../common/pixel_x265.cpp
  ../../common/predict.h
  ../../common/predict_x265.cpp
  ../../common/primitives.h
  ../../common/primitives_x265.cpp
  ../../common/quant.h
  ../../common/quant_x265.cpp
  ../../common/scalinglist.h
  ../../common/scalinglist_x265.cpp
  ../../common/shortyuv.h
  ../../common/shortyuv_x265.cpp
  ../../common/slice.h
  ../../common/slice_x265.cpp
  ../../common/threading.h
  ../../common/threading_x265.cpp
  ../../common/threadpool.h
  ../../common/threadpool_x265.cpp
  ../../common/version_x265.cpp
  ../../common/wavefront.h
  ../../common/wavefront_x265.cpp
  ../../common/winxp.h
  ../../common/winxp_x265.cpp
  ../../common/yuv.h
  ../../common/yuv_x265.cpp
  ../../config_lib265.h
  ../../encoder/analysis.h
  ../../encoder/analysis_x265.cpp
  ../../encoder/api_x265.cpp
  ../../encoder/bitcost.h
  ../../encoder/bitcost_x265.cpp
  ../../encoder/dpb.h
  ../../encoder/dpb_x265.cpp
  ../../encoder/encoder.h
  ../../encoder/encoder_x265.cpp
  ../../encoder/entropy.h
  ../../encoder/entropy_x265.cpp
  ../../encoder/frameencoder.h
  ../../encoder/frameencoder_x265.cpp
  ../../encoder/framefilter.h
  ../../encoder/framefilter_x265.cpp
  ../../encoder/level.h
  ../../encoder/level_x265.cpp
  ../../encoder/motion.h
  ../../encoder/motion_x265.cpp
  ../../encoder/nal.h
  ../../encoder/nal_x265.cpp
  ../../encoder/ratecontrol.h
  ../../encoder/ratecontrol_x265.cpp
  ../../encoder/rdcost.h
  ../../encoder/reference.h
  ../../encoder/reference_x265.cpp
  ../../encoder/sao.h
  ../../encoder/sao_x265.cpp
  ../../encoder/search.h
  ../../encoder/search_x265.cpp
  ../../encoder/sei.h
  ../../encoder/sei_x265.cpp
  ../../encoder/slicetype.h
  ../../encoder/slicetype_x265.cpp
  ../../encoder/weightPrediction_x265.cpp
  ../../x265.h
  ../../x265cli.h
)
# Output file name
set_target_properties(libx265
  PROPERTIES
  OUTPUT_NAME_DEBUG libx265D
)

# Additional include directories
set_property(TARGET libx265
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libx265\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libx265\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libx265\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libx265\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libx265 PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libx265 PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libx265 PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libx265 PRIVATE
    ""
  )
endif ()
