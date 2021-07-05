cmake_minimum_required(VERSION 3.12.2)

project(libopus)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libopus STATIC
  ../../../../GoTvCompilerConfig.h
  ../../../../GoTvCompilerEnviromentDefines.h
  ../../../../GoTvCompilerIntTypes.h
  ../../../../GoTvConfig.h
  ../../../../GoTvConfigAndroid.h
  ../../../../GoTvConfigLinux.h
  ../../../../GoTvConfigWindows.h
  ../../../../GoTvCpuArchDefines.h
  ../../../../GoTvCpuFeatureDefines.h
  ../../../../GoTvDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../celt/_kiss_fft_guts.h
  ../../celt/arch.h
  ../../celt/arm/arm_celt_map_opus.c
  ../../celt/arm/armcpu.h
  ../../celt/arm/armcpu_opus.c
  ../../celt/arm/celt_ne10_fft_opus.c
  ../../celt/arm/celt_ne10_mdct_opus.c
  ../../celt/arm/celt_neon_intr_opus.c
  ../../celt/arm/fft_arm.h
  ../../celt/arm/fixed_arm64.h
  ../../celt/arm/fixed_armv4.h
  ../../celt/arm/fixed_armv5e.h
  ../../celt/arm/kiss_fft_armv4.h
  ../../celt/arm/kiss_fft_armv5e.h
  ../../celt/arm/mdct_arm.h
  ../../celt/arm/pitch_arm.h
  ../../celt/bands.h
  ../../celt/bands_opus.c
  ../../celt/celt.h
  ../../celt/celt_decoder_opus.c
  ../../celt/celt_encoder_opus.c
  ../../celt/celt_lpc.h
  ../../celt/celt_lpc_opus.c
  ../../celt/celt_opus.c
  ../../celt/cpu_support.h
  ../../celt/cwrs.h
  ../../celt/cwrs_opus.c
  ../../celt/ecintrin.h
  ../../celt/entcode.h
  ../../celt/entcode_opus.c
  ../../celt/entdec.h
  ../../celt/entdec_opus.c
  ../../celt/entenc.h
  ../../celt/entenc_opus.c
  ../../celt/fixed_debug.h
  ../../celt/fixed_generic.h
  ../../celt/float_cast.h
  ../../celt/kiss_fft.h
  ../../celt/kiss_fft_opus.c
  ../../celt/laplace.h
  ../../celt/laplace_opus.c
  ../../celt/mathops.h
  ../../celt/mathops_opus.c
  ../../celt/mdct.h
  ../../celt/mdct_opus.c
  ../../celt/mfrngcod.h
  ../../celt/mips/celt_mipsr1.h
  ../../celt/mips/fixed_generic_mipsr1.h
  ../../celt/mips/kiss_fft_mipsr1.h
  ../../celt/mips/mdct_mipsr1.h
  ../../celt/mips/pitch_mipsr1.h
  ../../celt/mips/vq_mipsr1.h
  ../../celt/modes.h
  ../../celt/modes_opus.c
  ../../celt/opus_custom_demo_opus.c
  ../../celt/os_support.h
  ../../celt/pitch.h
  ../../celt/pitch_opus.c
  ../../celt/quant_bands.h
  ../../celt/quant_bands_opus.c
  ../../celt/rate.h
  ../../celt/rate_opus.c
  ../../celt/stack_alloc.h
  ../../celt/static_modes_fixed.h
  ../../celt/static_modes_fixed_arm_ne10.h
  ../../celt/static_modes_float.h
  ../../celt/static_modes_float_arm_ne10.h
  ../../celt/vq.h
  ../../celt/vq_opus.c
  ../../celt/x86/celt_lpc_sse.h
  ../../celt/x86/celt_lpc_sse_opus.c
  ../../celt/x86/pitch_sse.h
  ../../celt/x86/pitch_sse_opus.c
  ../../celt/x86/pitch_sse4_1_opus.c
  ../../celt/x86/x86_celt_map_opus.c
  ../../celt/x86/x86cpu.h
  ../../celt/x86/x86cpu_opus.c
  ../../config_libopus.h
  ../../opus/analysis.h
  ../../opus/analysis_opus.c
  ../../opus/mlp.h
  ../../opus/mlp_data_opus.c
  ../../opus/mlp_opus.c
  ../../opus/opus_compare_opus.c
  ../../opus/opus_config.h
  ../../opus/opus_decoder_opus.c
  ../../opus/opus_encoder_opus.c
  ../../opus/opus_multistream_decoder_opus.c
  ../../opus/opus_multistream_encoder_opus.c
  ../../opus/opus_multistream_opus.c
  ../../opus/opus_opus.c
  ../../opus/opus_private.h
  ../../opus/opus_version.h
  ../../opus/repacketizer_opus.c
  ../../opus/tansig_table.h
  ../../opus/unicode_support.h
  ../../opus/unicode_support_opus.c
  ../../silk/arm/arm_silk_map_opus.c
  ../../silk/arm/macros_arm64.h
  ../../silk/arm/macros_armv4.h
  ../../silk/arm/macros_armv5e.h
  ../../silk/arm/NSQ_neon.h
  ../../silk/arm/NSQ_neon_opus.c
  ../../silk/arm/SigProc_FIX_armv4.h
  ../../silk/arm/SigProc_FIX_armv5e.h
  ../../silk/float/apply_sine_window_FLP_opus.c
  ../../silk/float/autocorrelation_FLP_opus.c
  ../../silk/float/burg_modified_FLP_opus.c
  ../../silk/float/bwexpander_FLP_opus.c
  ../../silk/float/corrMatrix_FLP_opus.c
  ../../silk/float/encode_frame_FLP_opus.c
  ../../silk/float/energy_FLP_opus.c
  ../../silk/float/find_LPC_FLP_opus.c
  ../../silk/float/find_LTP_FLP_opus.c
  ../../silk/float/find_pitch_lags_FLP_opus.c
  ../../silk/float/find_pred_coefs_FLP_opus.c
  ../../silk/float/inner_product_FLP_opus.c
  ../../silk/float/k2a_FLP_opus.c
  ../../silk/float/levinsondurbin_FLP_opus.c
  ../../silk/float/LPC_analysis_filter_FLP_opus.c
  ../../silk/float/LPC_inv_pred_gain_FLP_opus.c
  ../../silk/float/LTP_analysis_filter_FLP_opus.c
  ../../silk/float/LTP_scale_ctrl_FLP_opus.c
  ../../silk/float/main_FLP.h
  ../../silk/float/noise_shape_analysis_FLP_opus.c
  ../../silk/float/pitch_analysis_core_FLP_opus.c
  ../../silk/float/prefilter_FLP_opus.c
  ../../silk/float/process_gains_FLP_opus.c
  ../../silk/float/regularize_correlations_FLP_opus.c
  ../../silk/float/residual_energy_FLP_opus.c
  ../../silk/float/scale_copy_vector_FLP_opus.c
  ../../silk/float/scale_vector_FLP_opus.c
  ../../silk/float/schur_FLP_opus.c
  ../../silk/float/SigProc_FLP.h
  ../../silk/float/solve_LS_FLP_opus.c
  ../../silk/float/sort_FLP_opus.c
  ../../silk/float/structs_FLP.h
  ../../silk/float/warped_autocorrelation_FLP_opus.c
  ../../silk/float/wrappers_FLP_opus.c
  ../../silk/mips/macros_mipsr1.h
  ../../silk/mips/NSQ_del_dec_mipsr1.h
  ../../silk/mips/sigproc_fix_mipsr1.h
  ../../silk/x86/main_sse.h
  ../../silk/x86/NSQ_del_dec_sse_opus.c
  ../../silk/x86/NSQ_sse_opus.c
  ../../silk/x86/SigProc_FIX_sse.h
  ../../silk/x86/VAD_sse_opus.c
  ../../silk/x86/VQ_WMat_EC_sse_opus.c
  ../../silk/x86/x86_silk_map_opus.c
)
# Output file name
set_target_properties(libopus
  PROPERTIES
  OUTPUT_NAME_DEBUG libopusD
)

# Additional include directories
set_property(TARGET libopus
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\silk;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\celt;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\opus;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\include;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libopus\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libopus PRIVATE
  $<$<CONFIG:Debug>:OPUS_BUILD;TARGET_OS_WINDOWS;HAVE_CONFIG_H;WIN64;_DEBUG;_CONSOLE>
)

# SDL check
target_compile_options(libopus PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libopus PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libopus PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libopus PRIVATE
    ""
  )
endif ()

# Additional library directories
if (MSVC)
  target_link_options(libopus PRIVATE
    $<$<CONFIG:Debug>:
      /LIBPATH:${CMAKE_SOURCE_DIR}/Debug64>
  )
else ()
  target_link_options(libopus PRIVATE
    $<$<CONFIG:Debug>:
      -L${CMAKE_SOURCE_DIR}/Debug64>
  )
endif ()

# Link libraries
set_property(TARGET libopus
  APPEND PROPERTY LINK_LIBRARIES
  "$<$<CONFIG:Debug>:celt.lib;silk_common.lib;silk_fixed.lib;silk_float.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib>"
)
