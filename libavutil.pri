
INCLUDEPATH += $$PWD/DependLibs/ffmpeg/libavutil
INCLUDEPATH += $$PWD/DependLibs/ffmpeg
INCLUDEPATH += $$PWD/DependLibs/libcurl/include
INCLUDEPATH += $$PWD/DependLibs/libcurl/lib


HEADERS += 	$$PWD/AppCompilerConfig.h \
    $$PWD/AppCpuArchDefines.h \
    $$PWD/AppDependLibrariesConfig.h \
    $$PWD/AppTargetOsConfig.h \
    $$PWD/DependLibs/ffmpeg/atomics/dummy/stdatomic.h \
    $$PWD/DependLibs/ffmpeg/atomics/gcc/stdatomic.h \
    $$PWD/DependLibs/ffmpeg/atomics/pthread/stdatomic.h \
    $$PWD/DependLibs/ffmpeg/atomics/suncc/stdatomic.h \
    $$PWD/DependLibs/ffmpeg/atomics/win32/stdatomic.h \
    $$PWD/DependLibs/ffmpeg/config_ffmpeg.h \
    $$PWD/DependLibs/ffmpeg/stdatomic.h \
    $$PWD/DependLibs/ffmpeg/libavutil/adler32.h \
    $$PWD/DependLibs/ffmpeg/libavutil/aes.h \
    $$PWD/DependLibs/ffmpeg/libavutil/aes_ctr.h \
    $$PWD/DependLibs/ffmpeg/libavutil/aes_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/atomic.h \
    $$PWD/DependLibs/ffmpeg/libavutil/atomic_gcc.h \
    $$PWD/DependLibs/ffmpeg/libavutil/atomic_suncc.h \
    $$PWD/DependLibs/ffmpeg/libavutil/atomic_win32.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_attributes.h \
    $$PWD/DependLibs/ffmpeg/libavutil/audio_fifo.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avassert.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avstring.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_attributes.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_common.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_intmath.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_time.h \
    $$PWD/DependLibs/ffmpeg/libavutil/base64.h \
    $$PWD/DependLibs/ffmpeg/libavutil/blowfish.h \
    $$PWD/DependLibs/ffmpeg/libavutil/bprint.h \
    $$PWD/DependLibs/ffmpeg/libavutil/bswap.h \
    $$PWD/DependLibs/ffmpeg/libavutil/buffer.h \
    $$PWD/DependLibs/ffmpeg/libavutil/buffer_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/camellia.h \
    $$PWD/DependLibs/ffmpeg/libavutil/cast5.h \
    $$PWD/DependLibs/ffmpeg/libavutil/channel_layout.h \
    $$PWD/DependLibs/ffmpeg/libavutil/color_utils.h \
    $$PWD/DependLibs/ffmpeg/libavutil/colorspace.h \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_common.h \
    $$PWD/DependLibs/ffmpeg/libavutil/cpu.h \
    $$PWD/DependLibs/ffmpeg/libavutil/cpu_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/crc.h \
    $$PWD/DependLibs/ffmpeg/libavutil/des.h \
    $$PWD/DependLibs/ffmpeg/libavutil/dict.h \
    $$PWD/DependLibs/ffmpeg/libavutil/display.h \
    $$PWD/DependLibs/ffmpeg/libavutil/downmix_info.h \
    $$PWD/DependLibs/ffmpeg/libavutil/dynarray.h \
    $$PWD/DependLibs/ffmpeg/libavutil/error.h \
    $$PWD/DependLibs/ffmpeg/libavutil/eval.h \
    $$PWD/DependLibs/ffmpeg/libavutil/ffmath.h \
    $$PWD/DependLibs/ffmpeg/libavutil/ffversion.h \
    $$PWD/DependLibs/ffmpeg/libavutil/fifo.h \
    $$PWD/DependLibs/ffmpeg/libavutil/file.h \
    $$PWD/DependLibs/ffmpeg/libavutil/fixed_dsp.h \
    $$PWD/DependLibs/ffmpeg/libavutil/float_dsp.h \
    $$PWD/DependLibs/ffmpeg/libavutil/frame.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hash.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hmac.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_cuda.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_cuda_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_d3d11va.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_drm.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_dxva2.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_qsv.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_vaapi.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_vdpau.h \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_videotoolbox.h \
    $$PWD/DependLibs/ffmpeg/libavutil/imgutils.h \
    $$PWD/DependLibs/ffmpeg/libavutil/imgutils_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/integer.h \
    $$PWD/DependLibs/ffmpeg/libavutil/intfloat.h \
    $$PWD/DependLibs/ffmpeg/libavutil/intreadwrite.h \
    $$PWD/DependLibs/ffmpeg/libavutil/lfg.h \
    $$PWD/DependLibs/ffmpeg/libavutil/libm.h \
    $$PWD/DependLibs/ffmpeg/libavutil/lls.h \
    $$PWD/DependLibs/ffmpeg/libavutil/log.h \
    $$PWD/DependLibs/ffmpeg/libavutil/lzo.h \
    $$PWD/DependLibs/ffmpeg/libavutil/macros.h \
    $$PWD/DependLibs/ffmpeg/libavutil/mastering_display_metadata.h \
    $$PWD/DependLibs/ffmpeg/libavutil/mathematics.h \
    $$PWD/DependLibs/ffmpeg/libavutil/md5.h \
    $$PWD/DependLibs/ffmpeg/libavutil/mem.h \
    $$PWD/DependLibs/ffmpeg/libavutil/mem_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/motion_vector.h \
    $$PWD/DependLibs/ffmpeg/libavutil/murmur3.h \
    $$PWD/DependLibs/ffmpeg/libavutil/opencl.h \
    $$PWD/DependLibs/ffmpeg/libavutil/opencl_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/opt.h \
    $$PWD/DependLibs/ffmpeg/libavutil/parseutils.h \
    $$PWD/DependLibs/ffmpeg/libavutil/pca.h \
    $$PWD/DependLibs/ffmpeg/libavutil/pixdesc.h \
    $$PWD/DependLibs/ffmpeg/libavutil/pixelutils.h \
    $$PWD/DependLibs/ffmpeg/libavutil/pixfmt.h \
    $$PWD/DependLibs/ffmpeg/libavutil/qsort.h \
    $$PWD/DependLibs/ffmpeg/libavutil/random_seed.h \
    $$PWD/DependLibs/ffmpeg/libavutil/rational.h \
    $$PWD/DependLibs/ffmpeg/libavutil/rc4.h \
    $$PWD/DependLibs/ffmpeg/libavutil/replaygain.h \
    $$PWD/DependLibs/ffmpeg/libavutil/reverse.h \
    $$PWD/DependLibs/ffmpeg/libavutil/ripemd.h \
    $$PWD/DependLibs/ffmpeg/libavutil/samplefmt.h \
    $$PWD/DependLibs/ffmpeg/libavutil/sha.h \
    $$PWD/DependLibs/ffmpeg/libavutil/sha512.h \
    $$PWD/DependLibs/ffmpeg/libavutil/slicethread.h \
    $$PWD/DependLibs/ffmpeg/libavutil/softfloat.h \
    $$PWD/DependLibs/ffmpeg/libavutil/softfloat_ieee754.h \
    $$PWD/DependLibs/ffmpeg/libavutil/softfloat_tables.h \
    $$PWD/DependLibs/ffmpeg/libavutil/spherical.h \
    $$PWD/DependLibs/ffmpeg/libavutil/stereo3d.h \
    $$PWD/DependLibs/ffmpeg/libavutil/tablegen.h \
    $$PWD/DependLibs/ffmpeg/libavutil/tea.h \
    $$PWD/DependLibs/ffmpeg/libavutil/thread.h \
    $$PWD/DependLibs/ffmpeg/libavutil/threadmessage.h \
    $$PWD/DependLibs/ffmpeg/libavutil/time_internal.h \
    $$PWD/DependLibs/ffmpeg/libavutil/timecode.h \
    $$PWD/DependLibs/ffmpeg/libavutil/timer.h \
    $$PWD/DependLibs/ffmpeg/libavutil/timestamp.h \
    $$PWD/DependLibs/ffmpeg/libavutil/tree.h \
    $$PWD/DependLibs/ffmpeg/libavutil/twofish.h \
    $$PWD/DependLibs/ffmpeg/libavutil/version.h \
    $$PWD/DependLibs/ffmpeg/libavutil/wchar_filename.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/asm.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/bswap.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/cpu_x86.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/emms.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/intmath.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/intreadwrite.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/pixelutils.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/timer.h \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/w64xmmtest.h \
    $$PWD/DependLibs/ffmpeg/libavutil/xga_font_data.h \
    $$PWD/DependLibs/ffmpeg/libavutil/xtea.h \
    $$PWD/DependLibs/ffmpeg/libavutil/../config_ffmpeg.h

SOURCES += 	$$PWD/DependLibs/ffmpeg/atomics/pthread/stdatomic_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_intmath.c \
    $$PWD/DependLibs/ffmpeg/libavutil/avutil_time.c \
    $$PWD/DependLibs/ffmpeg/libavutil/cpu_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/utils_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/fifo_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/file_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/cpu_x86_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/fixed_dsp_init_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/float_dsp_init_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/imgutils_init_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/lls_init_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/x86/pixelutils_init_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/adler32_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/aes_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/aes_ctr_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/atomic_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/audio_fifo_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/avstring_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/base64_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/blowfish_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/bprint_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/buffer_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/camellia_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/cast5_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/channel_layout_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/color_utils_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/crc_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/des_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/dict_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/display_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/downmix_info_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/error_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/eval_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/file_open_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/fixed_dsp_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/float_dsp_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/frame_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hash_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hmac_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_cuda_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_d3d11va_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_drm_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_dxva2_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_qsv_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_vaapi_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_vdpau_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/hwcontext_videotoolbox_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/imgutils_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/integer_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/lfg_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/lls_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/log_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/log2_tab_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/lzo_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/mastering_display_metadata_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/mathematics_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/md5_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/mem_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/murmur3_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/opencl_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/opencl_internal_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/opt_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/parseutils_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/pca_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/pixdesc_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/pixelutils_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/random_seed_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/rational_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/rc4_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/reverse_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/ripemd_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/samplefmt_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/sha_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/sha512_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/slicethread_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/spherical_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/stereo3d_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/tea_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/threadmessage_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/timecode_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/tree_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/twofish_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/xga_font_data_avutil.c \
    $$PWD/DependLibs/ffmpeg/libavutil/xtea_avutil.c

