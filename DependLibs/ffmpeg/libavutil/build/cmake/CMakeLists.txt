cmake_minimum_required(VERSION 3.12.2)

project(FfmpegAvUtilLib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(FfmpegAvUtilLib STATIC
  ../../../../../GoTvCompilerConfig.h
  ../../../../../GoTvCompilerIntTypes.h
  ../../../../../GoTvConfig.h
  ../../../../../GoTvConfigAndroid.h
  ../../../../../GoTvConfigLinux.h
  ../../../../../GoTvConfigWindows.h
  ../../../../../GoTvCpuArchDefines.h
  ../../../../../GoTvDependLibrariesConfig.h
  ../../../../../GoTvFeaturesConfig.h
  ../../../../../GoTvTargetOsConfig.h
  ../../../atomics/dummy/stdatomic.h
  ../../../atomics/gcc/stdatomic.h
  ../../../atomics/pthread/stdatomic.h
  ../../../atomics/pthread/stdatomic_avutil.c
  ../../../atomics/suncc/stdatomic.h
  ../../../atomics/win32/stdatomic.h
  ../../../ffmpeg_config.h
  ../../../ffmpeg_config_android.h
  ../../../ffmpeg_config_linux.h
  ../../../ffmpeg_config_windows.h
  ../../../stdatomic.h
  ../../adler32.h
  ../../adler32_avutil.c
  ../../aes.h
  ../../aes_avutil.c
  ../../aes_ctr.h
  ../../aes_ctr_avutil.c
  ../../aes_internal.h
  ../../atomic.h
  ../../atomic_avutil.c
  ../../atomic_gcc.h
  ../../atomic_suncc.h
  ../../atomic_win32.h
  ../../attributes.h
  ../../audio_fifo.h
  ../../audio_fifo_avutil.c
  ../../avassert.h
  ../../avconfig.h
  ../../avstring.h
  ../../avstring_avutil.c
  ../../avutil.h
  ../../avutil_attributes.h
  ../../avutil_common.h
  ../../avutil_internal.h
  ../../avutil_intmath.c
  ../../avutil_intmath.h
  ../../avutil_time.c
  ../../avutil_time.h
  ../../base64.h
  ../../base64_avutil.c
  ../../blowfish.h
  ../../blowfish_avutil.c
  ../../bprint.h
  ../../bprint_avutil.c
  ../../bswap.h
  ../../buffer.h
  ../../buffer_avutil.c
  ../../buffer_internal.h
  ../../camellia.h
  ../../camellia_avutil.c
  ../../cast5.h
  ../../cast5_avutil.c
  ../../channel_layout.h
  ../../channel_layout_avutil.c
  ../../color_utils.h
  ../../color_utils_avutil.c
  ../../colorspace.h
  ../../common.h
  ../../cpu.h
  ../../cpu_avutil.c
  ../../cpu_internal.h
  ../../crc.h
  ../../crc_avutil.c
  ../../des.h
  ../../des_avutil.c
  ../../dict.h
  ../../dict_avutil.c
  ../../display.h
  ../../display_avutil.c
  ../../downmix_info.h
  ../../downmix_info_avutil.c
  ../../dynarray.h
  ../../error.h
  ../../error_avutil.c
  ../../eval.h
  ../../eval_avutil.c
  ../../ffmath.h
  ../../ffversion.h
  ../../fifo.h
  ../../fifo_avutil.c
  ../../file.h
  ../../file_avutil.c
  ../../file_open_avutil.c
  ../../fixed_dsp.h
  ../../fixed_dsp_avutil.c
  ../../float_dsp.h
  ../../float_dsp_avutil.c
  ../../frame.h
  ../../frame_avutil.c
  ../../hash.h
  ../../hash_avutil.c
  ../../hmac.h
  ../../hmac_avutil.c
  ../../hwcontext.h
  ../../hwcontext_avutil.c
  ../../hwcontext_cuda.h
  ../../hwcontext_cuda_avutil.c
  ../../hwcontext_cuda_internal.h
  ../../hwcontext_d3d11va.h
  ../../hwcontext_d3d11va_avutil.c
  ../../hwcontext_drm.h
  ../../hwcontext_drm_avutil.c
  ../../hwcontext_dxva2.h
  ../../hwcontext_dxva2_avutil.c
  ../../hwcontext_internal.h
  ../../hwcontext_qsv.h
  ../../hwcontext_qsv_avutil.c
  ../../hwcontext_vaapi.h
  ../../hwcontext_vaapi_avutil.c
  ../../hwcontext_vdpau.h
  ../../hwcontext_vdpau_avutil.c
  ../../hwcontext_videotoolbox.h
  ../../hwcontext_videotoolbox_avutil.c
  ../../imgutils.h
  ../../imgutils_avutil.c
  ../../imgutils_internal.h
  ../../integer.h
  ../../integer_avutil.c
  ../../intfloat.h
  ../../intreadwrite.h
  ../../lfg.h
  ../../lfg_avutil.c
  ../../libm.h
  ../../lls.h
  ../../lls_avutil.c
  ../../log.h
  ../../log_avutil.c
  ../../log2_tab_avutil.c
  ../../lzo.h
  ../../lzo_avutil.c
  ../../macros.h
  ../../mastering_display_metadata.h
  ../../mastering_display_metadata_avutil.c
  ../../mathematics.h
  ../../mathematics_avutil.c
  ../../md5.h
  ../../md5_avutil.c
  ../../mem.h
  ../../mem_avutil.c
  ../../mem_internal.h
  ../../motion_vector.h
  ../../murmur3.h
  ../../murmur3_avutil.c
  ../../opencl.h
  ../../opencl_avutil.c
  ../../opencl_internal.h
  ../../opencl_internal_avutil.c
  ../../opt.h
  ../../opt_avutil.c
  ../../parseutils.h
  ../../parseutils_avutil.c
  ../../pca.h
  ../../pca_avutil.c
  ../../pixdesc.h
  ../../pixdesc_avutil.c
  ../../pixelutils.h
  ../../pixelutils_avutil.c
  ../../pixfmt.h
  ../../qsort.h
  ../../random_seed.h
  ../../random_seed_avutil.c
  ../../rational.h
  ../../rational_avutil.c
  ../../rc4.h
  ../../rc4_avutil.c
  ../../replaygain.h
  ../../reverse.h
  ../../reverse_avutil.c
  ../../ripemd.h
  ../../ripemd_avutil.c
  ../../samplefmt.h
  ../../samplefmt_avutil.c
  ../../sha.h
  ../../sha_avutil.c
  ../../sha512.h
  ../../sha512_avutil.c
  ../../slicethread.h
  ../../slicethread_avutil.c
  ../../softfloat.h
  ../../softfloat_ieee754.h
  ../../softfloat_tables.h
  ../../spherical.h
  ../../spherical_avutil.c
  ../../stereo3d.h
  ../../stereo3d_avutil.c
  ../../tablegen.h
  ../../tea.h
  ../../tea_avutil.c
  ../../thread.h
  ../../threadmessage.h
  ../../threadmessage_avutil.c
  ../../time_internal.h
  ../../timecode.h
  ../../timecode_avutil.c
  ../../timer.h
  ../../timestamp.h
  ../../tree.h
  ../../tree_avutil.c
  ../../twofish.h
  ../../twofish_avutil.c
  ../../utils_avutil.c
  ../../version.h
  ../../wchar_filename.h
  ../../x86/asm.h
  ../../x86/bswap.h
  ../../x86/cpu_x86.h
  ../../x86/cpu_x86_avutil.c
  ../../x86/emms.h
  ../../x86/fixed_dsp_init_avutil.c
  ../../x86/float_dsp_init_avutil.c
  ../../x86/imgutils_init_avutil.c
  ../../x86/intmath.h
  ../../x86/intreadwrite.h
  ../../x86/lls_init_avutil.c
  ../../x86/pixelutils.h
  ../../x86/pixelutils_init_avutil.c
  ../../x86/timer.h
  ../../x86/w64xmmtest.h
  ../../xga_font_data.h
  ../../xga_font_data_avutil.c
  ../../xtea.h
  ../../xtea_avutil.c
)
# Output file name
set_target_properties(FfmpegAvUtilLib
  PROPERTIES
  OUTPUT_NAME_DEBUG FfmpegAvUtilLibD
)

# Additional include directories
set_property(TARGET FfmpegAvUtilLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\..\..\..\sysheaders\;
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\..\..\..\kodi_src\xbmc\xbmc;
    J:\NlcJunk\DependLibs\ffmpeg\libavutil\build\VS2017\..\..\..\..\openssl-1.0.2p\include\>
)

# Preprocessor definitions
target_compile_definitions(FfmpegAvUtilLib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(FfmpegAvUtilLib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(FfmpegAvUtilLib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(FfmpegAvUtilLib PRIVATE
    ""
  )
endif ()
