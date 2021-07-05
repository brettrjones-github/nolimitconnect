cmake_minimum_required(VERSION 3.12.2)

project(libwebp)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libwebp STATIC
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
  ../../config_libwebp.h
  ../../src/dec/alpha_dec_webp.c
  ../../src/dec/alphai_dec.h
  ../../src/dec/buffer_dec_webp.c
  ../../src/dec/common_dec.h
  ../../src/dec/frame_dec_webp.c
  ../../src/dec/idec_dec_webp.c
  ../../src/dec/io_dec_webp.c
  ../../src/dec/quant_dec_webp.c
  ../../src/dec/tree_dec_webp.c
  ../../src/dec/vp8_dec.h
  ../../src/dec/vp8_dec_webp.c
  ../../src/dec/vp8i_dec.h
  ../../src/dec/vp8l_dec_webp.c
  ../../src/dec/vp8li_dec.h
  ../../src/dec/webp_dec_webp.c
  ../../src/dec/webpi_dec.h
  ../../src/demux/anim_decode_webp.c
  ../../src/demux/demux_webp.c
  ../../src/dsp/alpha_processing_mips_dsp_r2_webp.c
  ../../src/dsp/alpha_processing_neon_webp.c
  ../../src/dsp/alpha_processing_sse2_webp.c
  ../../src/dsp/alpha_processing_sse41_webp.c
  ../../src/dsp/alpha_processing_webp.c
  ../../src/dsp/argb_sse2_webp.c
  ../../src/dsp/argb_webp.c
  ../../src/dsp/cost_mips_dsp_r2_webp.c
  ../../src/dsp/cost_mips32_webp.c
  ../../src/dsp/cost_sse2_webp.c
  ../../src/dsp/cost_webp.c
  ../../src/dsp/cpu_webp.c
  ../../src/dsp/dec_clip_tables.c
  ../../src/dsp/dec_msa_webp.c
  ../../src/dsp/dec_neon_webp.c
  ../../src/dsp/dec_sse2_webp.c
  ../../src/dsp/dec_sse41_webp.c
  ../../src/dsp/dec_webp.c
  ../../src/dsp/dsp.h
  ../../src/dsp/enc_avx2_webp.c
  ../../src/dsp/enc_mips_dsp_r2_webp.c
  ../../src/dsp/enc_mips32_webp.c
  ../../src/dsp/enc_msa_webp.c
  ../../src/dsp/enc_neon_webp.c
  ../../src/dsp/enc_sse2_webp.c
  ../../src/dsp/enc_sse41_webp.c
  ../../src/dsp/enc_webp.c
  ../../src/dsp/filters_mips_dsp_r2_webp.c
  ../../src/dsp/filters_msa_webp.c
  ../../src/dsp/filters_neon_webp.c
  ../../src/dsp/filters_sse2_webp.c
  ../../src/dsp/filters_webp.c
  ../../src/dsp/lossless_enc.c
  ../../src/dsp/lossless_enc_msa_webp.c
  ../../src/dsp/lossless_enc_neon_webp.c
  ../../src/dsp/lossless_enc_sse2_webp.c
  ../../src/dsp/lossless_enc_sse41_webp.c
  ../../src/dsp/lossless_mips_dsp_r2_webp.c
  ../../src/dsp/lossless_msa_webp.c
  ../../src/dsp/lossless_neon_webp.c
  ../../src/dsp/lossless_sse2_webp.c
  ../../src/dsp/lossless_webp.c
  ../../src/dsp/msa_macro.h
  ../../src/dsp/rescaler_mips_dsp_r2_webp.c
  ../../src/dsp/rescaler_mips32_webp.c
  ../../src/dsp/rescaler_msa_webp.c
  ../../src/dsp/rescaler_neon_webp.c
  ../../src/dsp/rescaler_sse2_webp.c
  ../../src/dsp/rescaler_webp.c
  ../../src/dsp/ssim_sse2_webp.c
  ../../src/dsp/ssim_webp.c
  ../../src/dsp/upsampling_mips_dsp_r2_webp.c
  ../../src/dsp/upsampling_msa_webp.c
  ../../src/dsp/upsampling_neon_webp.c
  ../../src/dsp/upsampling_sse2_webp.c
  ../../src/dsp/upsampling_webp.c
  ../../src/dsp/yuv_mips_dsp_r2_webp.c
  ../../src/dsp/yuv_mips32_webp.c
  ../../src/dsp/yuv_neon_webp.c
  ../../src/dsp/yuv_sse2_webp.c
  ../../src/dsp/yuv_webp.c
  ../../src/enc/alpha_enc_webp.c
  ../../src/enc/analysis_enc_webp.c
  ../../src/enc/backward_references_cost_enc_webp.c
  ../../src/enc/backward_references_enc.h
  ../../src/enc/backward_references_enc_webp.c
  ../../src/enc/config_enc_webp.c
  ../../src/enc/cost_enc.h
  ../../src/enc/cost_enc_webp.c
  ../../src/enc/delta_palettization_enc.h
  ../../src/enc/delta_palettization_enc_webp.c
  ../../src/enc/filter_enc_webp.c
  ../../src/enc/frame_enc_webp.c
  ../../src/enc/histogram_enc.h
  ../../src/enc/histogram_enc_webp.c
  ../../src/enc/near_lossless_enc_webp.c
  ../../src/enc/picture_csp_enc_webp.c
  ../../src/enc/picture_enc_webp.c
  ../../src/enc/picture_psnr_enc_webp.c
  ../../src/enc/picture_rescale_enc_webp.c
  ../../src/enc/picture_tools_enc_webp.c
  ../../src/enc/predictor_enc_webp.c
  ../../src/enc/quant_enc_webp.c
  ../../src/enc/syntax_enc_webp.c
  ../../src/enc/token_enc_webp.c
  ../../src/enc/tree_enc_webp.c
  ../../src/enc/vp8i_enc.h
  ../../src/enc/vp8l_enc_webp.c
  ../../src/enc/vp8li_enc.h
  ../../src/enc/webp_enc_webp.c
  ../../src/mux/anim_encode_webp.c
  ../../src/mux/animi.h
  ../../src/mux/muxedit_webp.c
  ../../src/mux/muxi.h
  ../../src/mux/muxinternal_webp.c
  ../../src/mux/muxread_webp.c
  ../../src/utils/bit_reader_inl_utils.h
  ../../src/utils/bit_reader_utils.h
  ../../src/utils/bit_reader_utils_webp.c
  ../../src/utils/bit_writer_utils.h
  ../../src/utils/bit_writer_utils_webp.c
  ../../src/utils/color_cache_utils.h
  ../../src/utils/color_cache_utils_webp.c
  ../../src/utils/endian_inl_utils.h
  ../../src/utils/filters_utils.h
  ../../src/utils/filters_utils_webp.c
  ../../src/utils/huffman_encode_utils.h
  ../../src/utils/huffman_encode_utils_webp.c
  ../../src/utils/huffman_utils.h
  ../../src/utils/huffman_utils_webp.c
  ../../src/utils/quant_levels_dec_utils.h
  ../../src/utils/quant_levels_dec_utils_webp.c
  ../../src/utils/quant_levels_utils.h
  ../../src/utils/quant_levels_utils_webp.c
  ../../src/utils/random_utils.h
  ../../src/utils/random_utils_webp.c
  ../../src/utils/rescaler_utils.h
  ../../src/utils/rescaler_utils_webp.c
  ../../src/utils/thread_utils.h
  ../../src/utils/thread_utils_webp.c
  ../../src/utils/utils.h
  ../../src/utils/utils_webp.c
  ../../src/webp/decode.h
  ../../src/webp/demux.h
  ../../src/webp/encode.h
  ../../src/webp/format_constants.h
  ../../src/webp/mux.h
  ../../src/webp/mux_types.h
  ../../src/webp/types.h
)
# Output file name
set_target_properties(libwebp
  PROPERTIES
  OUTPUT_NAME_DEBUG libwebpD
)

# Additional include directories
set_property(TARGET libwebp
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\..\;
    ..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libwebp PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libwebp PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libwebp PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libwebp PRIVATE
    ""
  )
endif ()
