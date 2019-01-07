
DEFINES += OPUS_BUILD


QMAKE_CFLAGS += -msse3
QMAKE_CFLAGS += -msse4.1
QMAKE_CXXFLAGS += -msse3
QMAKE_CXXFLAGS += -msse4.1


INCLUDEPATH += $$PWD/../../
INCLUDEPATH += $$PWD/../../silk
INCLUDEPATH += $$PWD/../../celt
INCLUDEPATH += $$PWD/../../opus
INCLUDEPATH += $$PWD/../../include

HEADERS += 	$$PWD/../../../../GoTvCompilerConfig.h \
	$$PWD/../../../../GoTvCpuArchDefines.h \
	$$PWD/../../../../GoTvDependLibrariesConfig.h \
	$$PWD/../../celt/_kiss_fft_guts.h \
	$$PWD/../../celt/arch.h \
	$$PWD/../../celt/arm/armcpu.h \
	$$PWD/../../celt/arm/fft_arm.h \
	$$PWD/../../celt/arm/fixed_arm64.h \
	$$PWD/../../celt/arm/fixed_armv4.h \
	$$PWD/../../celt/arm/fixed_armv5e.h \
	$$PWD/../../celt/arm/kiss_fft_armv4.h \
	$$PWD/../../celt/arm/kiss_fft_armv5e.h \
	$$PWD/../../celt/arm/mdct_arm.h \
	$$PWD/../../celt/arm/pitch_arm.h \
	$$PWD/../../celt/bands.h \
	$$PWD/../../celt/celt.h \
	$$PWD/../../celt/celt_lpc.h \
	$$PWD/../../celt/cpu_support.h \
	$$PWD/../../celt/cwrs.h \
	$$PWD/../../celt/ecintrin.h \
	$$PWD/../../celt/entcode.h \
	$$PWD/../../celt/entdec.h \
	$$PWD/../../celt/entenc.h \
	$$PWD/../../celt/fixed_debug.h \
	$$PWD/../../celt/fixed_generic.h \
	$$PWD/../../celt/float_cast.h \
	$$PWD/../../celt/kiss_fft.h \
	$$PWD/../../celt/laplace.h \
	$$PWD/../../celt/mathops.h \
	$$PWD/../../celt/mdct.h \
	$$PWD/../../celt/mfrngcod.h \
	$$PWD/../../celt/mips/celt_mipsr1.h \
	$$PWD/../../celt/mips/fixed_generic_mipsr1.h \
	$$PWD/../../celt/mips/kiss_fft_mipsr1.h \
	$$PWD/../../celt/mips/mdct_mipsr1.h \
	$$PWD/../../celt/mips/pitch_mipsr1.h \
	$$PWD/../../celt/mips/vq_mipsr1.h \
	$$PWD/../../celt/modes.h \
	$$PWD/../../celt/os_support.h \
	$$PWD/../../celt/pitch.h \
	$$PWD/../../celt/quant_bands.h \
	$$PWD/../../celt/rate.h \
	$$PWD/../../celt/stack_alloc.h \
	$$PWD/../../celt/static_modes_fixed.h \
	$$PWD/../../celt/static_modes_fixed_arm_ne10.h \
	$$PWD/../../celt/static_modes_float.h \
	$$PWD/../../celt/static_modes_float_arm_ne10.h \
	$$PWD/../../celt/vq.h \
	$$PWD/../../config_libopus.h \
	$$PWD/../../opus/analysis.h \
	$$PWD/../../opus/mlp.h \
	$$PWD/../../opus/opus_config.h \
	$$PWD/../../opus/opus_private.h \
	$$PWD/../../opus/opus_version.h \
	$$PWD/../../opus/tansig_table.h \
	$$PWD/../../opus/unicode_support.h \
	$$PWD/../../silk/arm/NSQ_neon.h \
	$$PWD/../../silk/arm/SigProc_FIX_armv4.h \
	$$PWD/../../silk/arm/SigProc_FIX_armv5e.h \
	$$PWD/../../silk/arm/macros_arm64.h \
	$$PWD/../../silk/arm/macros_armv4.h \
	$$PWD/../../silk/arm/macros_armv5e.h \
	$$PWD/../../silk/float/SigProc_FLP.h \
	$$PWD/../../silk/float/main_FLP.h \
	$$PWD/../../silk/float/structs_FLP.h \
	$$PWD/../../silk/mips/NSQ_del_dec_mipsr1.h \
	$$PWD/../../silk/mips/macros_mipsr1.h \
	$$PWD/../../silk/mips/sigproc_fix_mipsr1.h \
	$$PWD/../../silk/x86/SigProc_FIX_sse.h \
	$$PWD/../../silk/x86/main_sse.h \
    $$PWD/../../celt/x86/celt_lpc_sse.h \
    $$PWD/../../celt/x86/pitch_sse.h \
    $$PWD/../../celt/x86/x86cpu.c.autosave \
    $$PWD/../../celt/x86/x86cpu.h

SOURCES += 	$$PWD/../../celt/arm/arm_celt_map.c \
	$$PWD/../../celt/arm/armcpu.c \
	$$PWD/../../celt/arm/celt_ne10_fft.c \
	$$PWD/../../celt/arm/celt_ne10_mdct.c \
	$$PWD/../../celt/arm/celt_neon_intr.c \
	$$PWD/../../celt/bands.c \
	$$PWD/../../celt/celt.c \
	$$PWD/../../celt/celt_decoder.c \
	$$PWD/../../celt/celt_encoder.c \
	$$PWD/../../celt/celt_lpc.c \
	$$PWD/../../celt/cwrs.c \
	$$PWD/../../celt/entcode.c \
	$$PWD/../../celt/entdec.c \
	$$PWD/../../celt/entenc.c \
	$$PWD/../../celt/kiss_fft.c \
	$$PWD/../../celt/laplace.c \
	$$PWD/../../celt/mathops.c \
	$$PWD/../../celt/mdct.c \
	$$PWD/../../celt/modes.c \
	$$PWD/../../celt/opus_custom_demo.c \
	$$PWD/../../celt/pitch.c \
	$$PWD/../../celt/quant_bands.c \
	$$PWD/../../celt/rate.c \
	$$PWD/../../celt/vq.c \
	$$PWD/../../opus/analysis.c \
	$$PWD/../../opus/mlp.c \
	$$PWD/../../opus/mlp_data.c \
	$$PWD/../../opus/opus.c \
	$$PWD/../../opus/opus_compare.c \
	$$PWD/../../opus/opus_decoder.c \
	$$PWD/../../opus/opus_encoder.c \
	$$PWD/../../opus/opus_multistream.c \
	$$PWD/../../opus/opus_multistream_decoder.c \
	$$PWD/../../opus/opus_multistream_encoder.c \
	$$PWD/../../opus/repacketizer.c \
	$$PWD/../../opus/unicode_support.c \
	$$PWD/../../silk/arm/NSQ_neon.c \
	$$PWD/../../silk/arm/arm_silk_map.c \
	$$PWD/../../silk/float/LPC_analysis_filter_FLP.c \
	$$PWD/../../silk/float/LPC_inv_pred_gain_FLP.c \
	$$PWD/../../silk/float/LTP_analysis_filter_FLP.c \
	$$PWD/../../silk/float/LTP_scale_ctrl_FLP.c \
	$$PWD/../../silk/float/apply_sine_window_FLP.c \
	$$PWD/../../silk/float/autocorrelation_FLP.c \
	$$PWD/../../silk/float/burg_modified_FLP.c \
	$$PWD/../../silk/float/bwexpander_FLP.c \
	$$PWD/../../silk/float/corrMatrix_FLP.c \
	$$PWD/../../silk/float/encode_frame_FLP.c \
	$$PWD/../../silk/float/energy_FLP.c \
	$$PWD/../../silk/float/find_LPC_FLP.c \
	$$PWD/../../silk/float/find_LTP_FLP.c \
	$$PWD/../../silk/float/find_pitch_lags_FLP.c \
	$$PWD/../../silk/float/find_pred_coefs_FLP.c \
	$$PWD/../../silk/float/inner_product_FLP.c \
	$$PWD/../../silk/float/k2a_FLP.c \
	$$PWD/../../silk/float/levinsondurbin_FLP.c \
	$$PWD/../../silk/float/noise_shape_analysis_FLP.c \
	$$PWD/../../silk/float/pitch_analysis_core_FLP.c \
	$$PWD/../../silk/float/prefilter_FLP.c \
	$$PWD/../../silk/float/process_gains_FLP.c \
	$$PWD/../../silk/float/regularize_correlations_FLP.c \
	$$PWD/../../silk/float/residual_energy_FLP.c \
	$$PWD/../../silk/float/scale_copy_vector_FLP.c \
	$$PWD/../../silk/float/scale_vector_FLP.c \
	$$PWD/../../silk/float/schur_FLP.c \
	$$PWD/../../silk/float/solve_LS_FLP.c \
	$$PWD/../../silk/float/sort_FLP.c \
	$$PWD/../../silk/float/warped_autocorrelation_FLP.c \
	$$PWD/../../silk/float/wrappers_FLP.c \
	$$PWD/../../silk/x86/NSQ_del_dec_sse.c \
	$$PWD/../../silk/x86/NSQ_sse.c \
	$$PWD/../../silk/x86/VAD_sse.c \
	$$PWD/../../silk/x86/VQ_WMat_EC_sse.c \
	$$PWD/../../silk/x86/x86_silk_map.c \
    $$PWD/../../celt/x86/celt_lpc_sse.c \
    $$PWD/../../celt/x86/pitch_sse.c \
    $$PWD/../../celt/x86/pitch_sse2.c \
    $$PWD/../../celt/x86/pitch_sse4_1.c \
    $$PWD/../../celt/x86/x86_celt_map.c \
    $$PWD/../../celt/x86/x86cpu.c

DISTFILES += \
    $$PWD/../../celt/arm/arm2gnu.pl \
    $$PWD/../../celt/arm/armopts.s.in \
    $$PWD/../../celt/arm/celt_pitch_xcorr_arm.s \
    $$PWD/../../celt/arm/celt_pitch_xcorr_arm-gnu.S

