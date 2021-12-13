

DEFINES += OUTSIDE_SPEEX __LITTLE_ENDIAN

INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/OpusTools/include
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/include
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/opus/win32
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/opus/celt
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/opus/silk
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/opus/silk/float
INCLUDEPATH += $$PWD/GuiInterface
INCLUDEPATH += $$PWD/GoTvCore/NoLimitConnect
INCLUDEPATH += $$PWD/kodi_src/xbmc/xbmc



win32{
INCLUDEPATH += $$PWD/DependLibs/MediaToolsLib/opus/win32
}

HEADERS += 	$$PWD/GoTvCompilerConfig.h \
    $$PWD/GoTvCpuArchDefines.h \
    $$PWD/GoTvDependLibrariesConfig.h \
    $$PWD/GoTvTargetOsConfig.h \
    $$PWD/DependLibs/MediaToolsLib/AviDefs.h \
    $$PWD/DependLibs/MediaToolsLib/MJPEGReader.h \
    $$PWD/DependLibs/MediaToolsLib/MJPEGWriter.h \
    $$PWD/DependLibs/MediaToolsLib/MediaTools.h \
    $$PWD/DependLibs/MediaToolsLib/MyOpusHeader.h \
    $$PWD/DependLibs/MediaToolsLib/OggBuffer.h \
    $$PWD/DependLibs/MediaToolsLib/OggStream.h \
    $$PWD/DependLibs/MediaToolsLib/OpusAudioDecoder.h \
    $$PWD/DependLibs/MediaToolsLib/OpusAudioEncoder.h \
    $$PWD/DependLibs/MediaToolsLib/OpusCallbackInterface.h \
    $$PWD/DependLibs/MediaToolsLib/OpusFileDecoder.h \
    $$PWD/DependLibs/MediaToolsLib/OpusFileEncoder.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/arch.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/cpusupport.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/diag_range.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/flac.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/include/getopt.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/lpc.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/opus_header.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/opusenc.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/os_support.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/picture.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/resample_sse.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/speex_resampler.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/stack_alloc.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/wav_io.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/wave_out.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/win32/config.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/win32/unicode_support.h \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/win32/opus_version.h \
    $$PWD/DependLibs/MediaToolsLib/SndDefs.h \
    $$PWD/DependLibs/MediaToolsLib/SndReader.h \
    $$PWD/DependLibs/MediaToolsLib/SndWriter.h \
    $$PWD/DependLibs/MediaToolsLib/include/ogg/ogg.h \
    $$PWD/DependLibs/MediaToolsLib/include/ogg/os_types.h \
    $$PWD/DependLibs/MediaToolsLib/include/opus.h \
    $$PWD/DependLibs/MediaToolsLib/include/opus_custom.h \
    $$PWD/DependLibs/MediaToolsLib/include/opus_defines.h \
    $$PWD/DependLibs/MediaToolsLib/include/opus_multistream.h \
    $$PWD/DependLibs/MediaToolsLib/include/opus_types.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/_kiss_fft_guts.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arch_celt.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/armcpu.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/fixed_armv4.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/fixed_armv5e.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/kiss_fft_armv4.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/kiss_fft_armv5e.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/pitch_arm.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/bands.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/celt.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/celt_lpc.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/cpu_support.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/cwrs.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/ecintrin.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/entcode.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/entdec.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/entenc.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/fixed_debug.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/fixed_generic.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/float_cast.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/kiss_fft.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/laplace.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/mathops.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/mdct.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/mfrngcod.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/modes.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/os_support.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/pitch.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/quant_bands.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/rate.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/stack_alloc.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/static_modes_fixed.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/static_modes_float.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/vq.h \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/x86/pitch_sse.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/API.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/Inlines.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/MacroCount.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/MacroDebug.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/PLC.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/SigProc_FIX.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/control.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/debug.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/define.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/errors.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/main_FIX.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/structs_FIX.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/SigProc_FLP.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/main_FLP.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/structs_FLP.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/macros.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/main.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/pitch_est_defines.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_private.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_rom.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_structs.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/structs.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tuning_parameters.h \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/typedef.h \
    $$PWD/DependLibs/MediaToolsLib/opus/src/analysis.h \
    $$PWD/DependLibs/MediaToolsLib/opus/src/mlp.h \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_private.h \
    $$PWD/DependLibs/MediaToolsLib/opus/src/tansig_table.h \
    $$PWD/DependLibs/MediaToolsLib/opus/win32/config.h


SOURCES += 	\
    $$PWD/DependLibs/MediaToolsLib/opus/celt/mdct_mediatools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NSQ_del_dec.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/bwexpander_32.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_down2_3.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/resample_mediatools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/debug_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/sort_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/analysis_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/mlp_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/mlp_data_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_encoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_multistream_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_multistream_encoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_multistream_decoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/repacketizer_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/ogg/bitwise_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/ogg/framing_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/arm_celt_map_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/arm/armcpu_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/bands_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/celt_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/celt_encoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/celt_lpc_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/cwrs_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/entcode_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/entdec_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/entenc_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/kiss_fft_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/laplace_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/mathops_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/modes_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/pitch_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/quant_bands_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/rate_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/vq_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/celt/celt_decoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/apply_sine_window_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/autocorr_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/burg_modified_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/corrMatrix_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/encode_frame_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/find_LPC_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/find_LTP_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/find_pitch_lags_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/find_pred_coefs_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/k2a_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/k2a_Q16_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/LTP_analysis_filter_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/LTP_scale_ctrl_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/noise_shape_analysis_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/pitch_analysis_core_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/prefilter_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/process_gains_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/regularize_correlations_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/residual_energy16_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/residual_energy_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/schur64_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/schur_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/solve_LS_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/vector_ops_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/fixed/warped_autocorrelation_FIX_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/apply_sine_window_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/autocorrelation_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/burg_modified_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/bwexpander_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/corrMatrix_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/encode_frame_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/energy_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/find_LPC_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/find_LTP_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/find_pitch_lags_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/find_pred_coefs_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/inner_product_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/k2a_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/levinsondurbin_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/LPC_analysis_filter_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/LPC_inv_pred_gain_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/LTP_analysis_filter_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/LTP_scale_ctrl_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/noise_shape_analysis_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/pitch_analysis_core_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/prefilter_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/process_gains_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/regularize_correlations_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/residual_energy_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/scale_copy_vector_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/scale_vector_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/schur_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/solve_LS_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/sort_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/warped_autocorrelation_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/float/wrappers_FLP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/A2NLSF_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/ana_filt_bank_1_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/biquad_alt_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/bwexpander_mtools_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/check_control_input_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/CNG_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/code_signs_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/control_audio_bandwidth_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/control_codec_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/control_SNR_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/dec_API_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decode_core_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decode_frame_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decode_indices_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decode_parameters_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decode_pitch_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decode_pulses_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/decoder_set_fs_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/enc_API_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/encode_indices_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/encode_pulses_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/gain_quant_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/HP_variable_cutoff_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/init_decoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/init_encoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/inner_prod_aligned_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/interpolate_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/lin2log_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/log2lin_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/LP_variable_cutoff_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/LPC_analysis_filter_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/LPC_inv_pred_gain_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF2A_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_decode_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_del_dec_quant_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_encode_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_stabilize_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_unpack_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_VQ_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NLSF_VQ_weights_laroia_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/pitch_est_tables_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/NSQ_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/PLC_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/process_NLSFs_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/quant_LTP_gains_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_down2_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_private_AR2_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_private_down_FIR_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_private_IIR_FIR_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_private_up2_HQ_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/resampler_rom_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/shell_coder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/sigm_Q15_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/stereo_decode_pred_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/stereo_encode_pred_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/stereo_find_predictor_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/stereo_LR_to_MS_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/stereo_MS_to_LR_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/stereo_quant_pred_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/sum_sqr_shift_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/table_LSF_cos_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_gain_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_LTP_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_NLSF_CB_NB_MB_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_NLSF_CB_WB_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_other_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_pitch_lag_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/tables_pulses_per_block_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/VAD_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/silk/VQ_WMat_EC_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/opus/src/opus_decoder_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/win32/unicode_support_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/audio-in_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/diag_range_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/flac_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/lpc_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/opus_header_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/opusdec_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/opusenc_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/picture_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/wav_io_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/OpusTools/wave_out_mtools.c \
    $$PWD/DependLibs/MediaToolsLib/AviDefs_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/MediaTools_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/MJPEGReader_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/MJPEGWriter_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/MyOpusHeader_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/OggBuffer_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/OggStream_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/OpusAudioDecoder_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/OpusAudioEncoder_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/OpusFileDecoder_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/OpusFileEncoder_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/SndDefs_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/SndReader_mtools.cpp \
    $$PWD/DependLibs/MediaToolsLib/SndWriter_mtools.cpp

