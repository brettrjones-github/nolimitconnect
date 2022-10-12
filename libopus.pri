
DEFINES += OPUS_BUILD


QMAKE_CFLAGS += -msse3
QMAKE_CFLAGS += -msse4.1
QMAKE_CXXFLAGS += -msse3
QMAKE_CXXFLAGS += -msse4.1


INCLUDEPATH += $$PWD/DependLibs/libopus
INCLUDEPATH += $$PWD/DependLibs/libopus/silk
INCLUDEPATH += $$PWD/DependLibs/libopus/celt
INCLUDEPATH += $$PWD/DependLibs/libopus/opus
INCLUDEPATH += $$PWD/DependLibs/libopus/include

HEADERS += 	$$PWD/NlcCompilerConfig.h \
    $$PWD/NlcCpuArchDefines.h \
    $$PWD/NlcDependLibrariesConfig.h \
    $$PWD/DependLibs/libopus/celt/_kiss_fft_guts.h \
    $$PWD/DependLibs/libopus/celt/arch.h \
    $$PWD/DependLibs/libopus/celt/arm/armcpu.h \
    $$PWD/DependLibs/libopus/celt/arm/fft_arm.h \
    $$PWD/DependLibs/libopus/celt/arm/fixed_arm64.h \
    $$PWD/DependLibs/libopus/celt/arm/fixed_armv4.h \
    $$PWD/DependLibs/libopus/celt/arm/fixed_armv5e.h \
    $$PWD/DependLibs/libopus/celt/arm/kiss_fft_armv4.h \
    $$PWD/DependLibs/libopus/celt/arm/kiss_fft_armv5e.h \
    $$PWD/DependLibs/libopus/celt/arm/mdct_arm.h \
    $$PWD/DependLibs/libopus/celt/arm/pitch_arm.h \
    $$PWD/DependLibs/libopus/celt/bands.h \
    $$PWD/DependLibs/libopus/celt/celt.h \
    $$PWD/DependLibs/libopus/celt/celt_lpc.h \
    $$PWD/DependLibs/libopus/celt/cpu_support.h \
    $$PWD/DependLibs/libopus/celt/cwrs.h \
    $$PWD/DependLibs/libopus/celt/ecintrin.h \
    $$PWD/DependLibs/libopus/celt/entcode.h \
    $$PWD/DependLibs/libopus/celt/entdec.h \
    $$PWD/DependLibs/libopus/celt/entenc.h \
    $$PWD/DependLibs/libopus/celt/fixed_debug.h \
    $$PWD/DependLibs/libopus/celt/fixed_generic.h \
    $$PWD/DependLibs/libopus/celt/float_cast.h \
    $$PWD/DependLibs/libopus/celt/kiss_fft.h \
    $$PWD/DependLibs/libopus/celt/laplace.h \
    $$PWD/DependLibs/libopus/celt/mathops.h \
    $$PWD/DependLibs/libopus/celt/mdct.h \
    $$PWD/DependLibs/libopus/celt/mfrngcod.h \
    $$PWD/DependLibs/libopus/celt/mips/celt_mipsr1.h \
    $$PWD/DependLibs/libopus/celt/mips/fixed_generic_mipsr1.h \
    $$PWD/DependLibs/libopus/celt/mips/kiss_fft_mipsr1.h \
    $$PWD/DependLibs/libopus/celt/mips/mdct_mipsr1.h \
    $$PWD/DependLibs/libopus/celt/mips/pitch_mipsr1.h \
    $$PWD/DependLibs/libopus/celt/mips/vq_mipsr1.h \
    $$PWD/DependLibs/libopus/celt/modes.h \
    $$PWD/DependLibs/libopus/celt/os_support.h \
    $$PWD/DependLibs/libopus/celt/pitch.h \
    $$PWD/DependLibs/libopus/celt/quant_bands.h \
    $$PWD/DependLibs/libopus/celt/rate.h \
    $$PWD/DependLibs/libopus/celt/stack_alloc.h \
    $$PWD/DependLibs/libopus/celt/static_modes_fixed.h \
    $$PWD/DependLibs/libopus/celt/static_modes_fixed_arm_ne10.h \
    $$PWD/DependLibs/libopus/celt/static_modes_float.h \
    $$PWD/DependLibs/libopus/celt/static_modes_float_arm_ne10.h \
    $$PWD/DependLibs/libopus/celt/vq.h \
    $$PWD/DependLibs/libopus/config_libopus.h \
    $$PWD/DependLibs/libopus/opus/analysis.h \
    $$PWD/DependLibs/libopus/opus/mlp.h \
    $$PWD/DependLibs/libopus/opus/opus_private.h \
    $$PWD/DependLibs/libopus/opus/opus_version.h \
    $$PWD/DependLibs/libopus/opus/tansig_table.h \
    $$PWD/DependLibs/libopus/opus/unicode_support.h \
    $$PWD/DependLibs/libopus/silk/arm/NSQ_neon.h \
    $$PWD/DependLibs/libopus/silk/arm/SigProc_FIX_armv4.h \
    $$PWD/DependLibs/libopus/silk/arm/SigProc_FIX_armv5e.h \
    $$PWD/DependLibs/libopus/silk/arm/macros_arm64.h \
    $$PWD/DependLibs/libopus/silk/arm/macros_armv4.h \
    $$PWD/DependLibs/libopus/silk/arm/macros_armv5e.h \
    $$PWD/DependLibs/libopus/silk/float/SigProc_FLP.h \
    $$PWD/DependLibs/libopus/silk/float/main_FLP.h \
    $$PWD/DependLibs/libopus/silk/float/structs_FLP.h \
    $$PWD/DependLibs/libopus/silk/mips/NSQ_del_dec_mipsr1.h \
    $$PWD/DependLibs/libopus/silk/mips/macros_mipsr1.h \
    $$PWD/DependLibs/libopus/silk/mips/sigproc_fix_mipsr1.h \
    $$PWD/DependLibs/libopus/silk/x86/SigProc_FIX_sse.h \
    $$PWD/DependLibs/libopus/silk/x86/main_sse.h \
    $$PWD/DependLibs/libopus/celt/x86/celt_lpc_sse.h \
    $$PWD/DependLibs/libopus/celt/x86/pitch_sse.h \
    $$PWD/DependLibs/libopus/celt/x86/x86cpu.h \
    $$PWD/DependLibs/libopus/include/opus.h \
    $$PWD/DependLibs/libopus/include/opus_custom.h \
    $$PWD/DependLibs/libopus/include/opus_defines.h \
    $$PWD/DependLibs/libopus/include/opus_multistream.h \
    $$PWD/DependLibs/libopus/include/opus_types.h

SOURCES += 	\
    $$PWD/DependLibs/libopus/celt/x86/pitch_sse2.c \
    $$PWD/DependLibs/libopus/celt/arm/arm_celt_map_opus.c \
    $$PWD/DependLibs/libopus/celt/arm/armcpu_opus.c \
    $$PWD/DependLibs/libopus/celt/arm/celt_ne10_fft_opus.c \
    $$PWD/DependLibs/libopus/celt/arm/celt_ne10_mdct_opus.c \
    $$PWD/DependLibs/libopus/celt/arm/celt_neon_intr_opus.c \
    $$PWD/DependLibs/libopus/celt/x86/celt_lpc_sse_opus.c \
    $$PWD/DependLibs/libopus/celt/x86/pitch_sse_opus.c \
    $$PWD/DependLibs/libopus/celt/x86/pitch_sse4_1_opus.c \
    $$PWD/DependLibs/libopus/celt/x86/x86_celt_map_opus.c \
    $$PWD/DependLibs/libopus/celt/x86/x86cpu_opus.c \
    $$PWD/DependLibs/libopus/celt/bands_opus.c \
    $$PWD/DependLibs/libopus/celt/celt_opus.c \
    $$PWD/DependLibs/libopus/celt/celt_encoder_opus.c \
    $$PWD/DependLibs/libopus/celt/celt_decoder_opus.c \
    $$PWD/DependLibs/libopus/celt/celt_lpc_opus.c \
    $$PWD/DependLibs/libopus/celt/cwrs_opus.c \
    $$PWD/DependLibs/libopus/celt/entcode_opus.c \
    $$PWD/DependLibs/libopus/celt/entdec_opus.c \
    $$PWD/DependLibs/libopus/celt/entenc_opus.c \
    $$PWD/DependLibs/libopus/celt/kiss_fft_opus.c \
    $$PWD/DependLibs/libopus/celt/laplace_opus.c \
    $$PWD/DependLibs/libopus/celt/mathops_opus.c \
    $$PWD/DependLibs/libopus/celt/mdct_opus.c \
    $$PWD/DependLibs/libopus/celt/modes_opus.c \
#    $$PWD/DependLibs/libopus/celt/opus_custom_demo_opus.c \
    $$PWD/DependLibs/libopus/celt/pitch_opus.c \
    $$PWD/DependLibs/libopus/celt/quant_bands_opus.c \
    $$PWD/DependLibs/libopus/celt/rate_opus.c \
    $$PWD/DependLibs/libopus/celt/vq_opus.c \
    $$PWD/DependLibs/libopus/opus/analysis_opus.c \
    $$PWD/DependLibs/libopus/opus/mlp_opus.c \
    $$PWD/DependLibs/libopus/opus/mlp_data_opus.c \
    $$PWD/DependLibs/libopus/opus/opus_opus.c \
    $$PWD/DependLibs/libopus/opus/opus_decoder_opus.c \
#    $$PWD/DependLibs/libopus/opus/opus_compare_opus.c \
    $$PWD/DependLibs/libopus/opus/opus_encoder_opus.c \
    $$PWD/DependLibs/libopus/opus/opus_multistream_opus.c \
    $$PWD/DependLibs/libopus/opus/opus_multistream_encoder_opus.c \
    $$PWD/DependLibs/libopus/opus/opus_multistream_decoder_opus.c \
    $$PWD/DependLibs/libopus/opus/repacketizer_opus.c \
    $$PWD/DependLibs/libopus/opus/unicode_support_opus.c \
    $$PWD/DependLibs/libopus/silk/arm/arm_silk_map_opus.c \
    $$PWD/DependLibs/libopus/silk/arm/NSQ_neon_opus.c \
    $$PWD/DependLibs/libopus/silk/float/apply_sine_window_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/autocorrelation_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/burg_modified_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/bwexpander_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/corrMatrix_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/encode_frame_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/energy_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/find_LPC_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/find_LTP_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/find_pitch_lags_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/find_pred_coefs_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/inner_product_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/k2a_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/levinsondurbin_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/LPC_analysis_filter_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/LPC_inv_pred_gain_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/LTP_analysis_filter_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/LTP_scale_ctrl_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/noise_shape_analysis_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/pitch_analysis_core_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/prefilter_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/process_gains_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/regularize_correlations_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/residual_energy_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/scale_copy_vector_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/scale_vector_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/schur_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/solve_LS_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/sort_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/warped_autocorrelation_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/float/wrappers_FLP_opus.c \
    $$PWD/DependLibs/libopus/silk/x86/NSQ_del_dec_sse_opus.c \
    $$PWD/DependLibs/libopus/silk/x86/NSQ_sse_opus.c \
    $$PWD/DependLibs/libopus/silk/x86/VAD_sse_opus.c \
    $$PWD/DependLibs/libopus/silk/x86/VQ_WMat_EC_sse_opus.c \
    $$PWD/DependLibs/libopus/silk/x86/x86_silk_map_opus.c

DISTFILES += \
    $$PWD/DependLibs/libopus/celt/arm/arm2gnu.pl \
    $$PWD/DependLibs/libopus/celt/arm/armopts.s.in \
    $$PWD/DependLibs/libopus/celt/arm/celt_pitch_xcorr_arm.s \
    $$PWD/DependLibs/libopus/celt/arm/celt_pitch_xcorr_arm-gnu.S

