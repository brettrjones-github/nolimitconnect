cmake_minimum_required(VERSION 3.12.2)

project(liblzma)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(liblzma STATIC
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
  ../../api/lzma.h
  ../../api/lzma/base.h
  ../../api/lzma/bcj.h
  ../../api/lzma/block.h
  ../../api/lzma/check.h
  ../../api/lzma/container.h
  ../../api/lzma/delta.h
  ../../api/lzma/filter.h
  ../../api/lzma/hardware.h
  ../../api/lzma/index.h
  ../../api/lzma/index_hash.h
  ../../api/lzma/lzma.h
  ../../api/lzma/stream_flags.h
  ../../api/lzma/version.h
  ../../api/lzma/vli.h
  ../../check/check.c
  ../../check/check.h
  ../../check/crc_macros.h
  ../../check/crc32_fast.c
  ../../check/crc32_small.c
  ../../check/crc32_table.c
  ../../check/crc32_table_be.h
  ../../check/crc32_table_le.h
  ../../check/crc32_tablegen.c
  ../../check/crc64_fast.c
  ../../check/crc64_small.c
  ../../check/crc64_table.c
  ../../check/crc64_table_be.h
  ../../check/crc64_table_le.h
  ../../check/crc64_tablegen.c
  ../../check/sha256.c
  ../../common/alone_decoder.c
  ../../common/alone_decoder.h
  ../../common/alone_encoder.c
  ../../common/auto_decoder.c
  ../../common/block_buffer_decoder.c
  ../../common/block_buffer_encoder.c
  ../../common/block_decoder.c
  ../../common/block_decoder.h
  ../../common/block_encoder.c
  ../../common/block_encoder.h
  ../../common/block_header_decoder.c
  ../../common/block_header_encoder.c
  ../../common/block_util.c
  ../../common/common_lzma.c
  ../../common/common_lzma.h
  ../../common/easy_buffer_encoder.c
  ../../common/easy_decoder_memusage.c
  ../../common/easy_encoder.c
  ../../common/easy_encoder_memusage.c
  ../../common/easy_preset.c
  ../../common/easy_preset.h
  ../../common/filter_buffer_decoder.c
  ../../common/filter_buffer_encoder.c
  ../../common/filter_common.c
  ../../common/filter_common.h
  ../../common/filter_decoder.c
  ../../common/filter_decoder.h
  ../../common/filter_encoder.c
  ../../common/filter_encoder.h
  ../../common/filter_flags_decoder.c
  ../../common/filter_flags_encoder.c
  ../../common/hardware_physmem.c
  ../../common/index.c
  ../../common/index.h
  ../../common/index_decoder.c
  ../../common/index_encoder.c
  ../../common/index_encoder.h
  ../../common/index_hash.c
  ../../common/stream_buffer_decoder.c
  ../../common/stream_buffer_encoder.c
  ../../common/stream_decoder.c
  ../../common/stream_decoder.h
  ../../common/stream_encoder.c
  ../../common/stream_encoder.h
  ../../common/stream_flags_common.c
  ../../common/stream_flags_common.h
  ../../common/stream_flags_decoder.c
  ../../common/stream_flags_encoder.c
  ../../common/sysdefs_lzma.h
  ../../common/tuklib_integer.h
  ../../common/vli_decoder.c
  ../../common/vli_encoder.c
  ../../common/vli_size.c
  ../../config_liblzma.h
  ../../delta/delta_common.c
  ../../delta/delta_common.h
  ../../delta/delta_decoder.c
  ../../delta/delta_decoder.h
  ../../delta/delta_encoder.c
  ../../delta/delta_encoder.h
  ../../delta/delta_private.h
  ../../lz/lz_decoder.c
  ../../lz/lz_decoder.h
  ../../lz/lz_encoder.c
  ../../lz/lz_encoder.h
  ../../lz/lz_encoder_hash.h
  ../../lz/lz_encoder_hash_table.h
  ../../lz/lz_encoder_mf.c
  ../../lzma/fastpos.h
  ../../lzma/fastpos_table.c
  ../../lzma/fastpos_tablegen.c
  ../../lzma/lzma_common.h
  ../../lzma/lzma_decoder.c
  ../../lzma/lzma_decoder.h
  ../../lzma/lzma_encoder.c
  ../../lzma/lzma_encoder.h
  ../../lzma/lzma_encoder_optimum_fast.c
  ../../lzma/lzma_encoder_optimum_normal.c
  ../../lzma/lzma_encoder_presets.c
  ../../lzma/lzma_encoder_private.h
  ../../lzma/lzma2_decoder.c
  ../../lzma/lzma2_decoder.h
  ../../lzma/lzma2_encoder.c
  ../../lzma/lzma2_encoder.h
  ../../rangecoder/price.h
  ../../rangecoder/price_table.c
  ../../rangecoder/price_tablegen.c
  ../../rangecoder/range_common.h
  ../../rangecoder/range_decoder.h
  ../../rangecoder/range_encoder.h
  ../../simple/arm.c
  ../../simple/armthumb.c
  ../../simple/ia64.c
  ../../simple/powerpc.c
  ../../simple/simple_coder.c
  ../../simple/simple_coder.h
  ../../simple/simple_decoder.c
  ../../simple/simple_decoder.h
  ../../simple/simple_encoder.c
  ../../simple/simple_encoder.h
  ../../simple/simple_private.h
  ../../simple/sparc.c
  ../../simple/x86.c
)
# Output file name
set_target_properties(liblzma
  PROPERTIES
  OUTPUT_NAME_DEBUG liblzmaD
)

# Additional include directories
set_property(TARGET liblzma
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\..\;
    J:\NlcJunk\DependLibs\liblzma\build\VS2017\../../../../sysheaders;
    ..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(liblzma PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(liblzma PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(liblzma PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(liblzma PRIVATE
    ""
  )
endif ()
