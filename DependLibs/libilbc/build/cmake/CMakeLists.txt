cmake_minimum_required(VERSION 3.12.2)

project(libilbc)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libilbc STATIC
  ../../../../GoTvCompilerConfig.h
  ../../../../GoTvCompilerIntTypes.h
  ../../../../GoTvConfig.h
  ../../../../GoTvConfigAndroid.h
  ../../../../GoTvConfigLinux.h
  ../../../../GoTvConfigWindows.h
  ../../../../GoTvCpuArchDefines.h
  ../../../../GoTvDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../ilbc/abs_quant.h
  ../../ilbc/abs_quant_ilbc.c
  ../../ilbc/abs_quant_loop.h
  ../../ilbc/abs_quant_loop_ilbc.c
  ../../ilbc/augmented_cb_corr.h
  ../../ilbc/augmented_cb_corr_ilbc.c
  ../../ilbc/bw_expand.h
  ../../ilbc/bw_expand_ilbc.c
  ../../ilbc/cb_construct.h
  ../../ilbc/cb_construct_ilbc.c
  ../../ilbc/cb_mem_energy.h
  ../../ilbc/cb_mem_energy_augmentation.h
  ../../ilbc/cb_mem_energy_augmentation_ilbc.c
  ../../ilbc/cb_mem_energy_calc.h
  ../../ilbc/cb_mem_energy_calc_ilbc.c
  ../../ilbc/cb_mem_energy_ilbc.c
  ../../ilbc/cb_search.h
  ../../ilbc/cb_search_core.h
  ../../ilbc/cb_search_core_ilbc.c
  ../../ilbc/cb_search_ilbc.c
  ../../ilbc/cb_update_best_index.h
  ../../ilbc/cb_update_best_index_ilbc.c
  ../../ilbc/chebyshev.h
  ../../ilbc/chebyshev_ilbc.c
  ../../ilbc/comp_corr.h
  ../../ilbc/comp_corr_ilbc.c
  ../../ilbc/constants.h
  ../../ilbc/constants_ilbc.c
  ../../ilbc/create_augmented_vec.h
  ../../ilbc/create_augmented_vec_ilbc.c
  ../../ilbc/decode.h
  ../../ilbc/decode_ilbc.c
  ../../ilbc/decode_residual.h
  ../../ilbc/decode_residual_ilbc.c
  ../../ilbc/decoder_interpolate_lsf.h
  ../../ilbc/decoder_interpolate_lsf_ilbc.c
  ../../ilbc/defines.h
  ../../ilbc/do_plc.h
  ../../ilbc/do_plc_ilbc.c
  ../../ilbc/encode.h
  ../../ilbc/encode_ilbc.c
  ../../ilbc/energy_inverse.h
  ../../ilbc/energy_inverse_ilbc.c
  ../../ilbc/enh_upsample.h
  ../../ilbc/enh_upsample_ilbc.c
  ../../ilbc/enhancer.h
  ../../ilbc/enhancer_ilbc.c
  ../../ilbc/enhancer_interface.h
  ../../ilbc/enhancer_interface_ilbc.c
  ../../ilbc/filtered_cb_vecs.h
  ../../ilbc/filtered_cb_vecs_ilbc.c
  ../../ilbc/frame_classify.h
  ../../ilbc/frame_classify_ilbc.c
  ../../ilbc/gain_dequant.h
  ../../ilbc/gain_dequant_ilbc.c
  ../../ilbc/gain_quant.h
  ../../ilbc/gain_quant_ilbc.c
  ../../ilbc/get_cd_vec.h
  ../../ilbc/get_cd_vec_ilbc.c
  ../../ilbc/get_lsp_poly.h
  ../../ilbc/get_lsp_poly_ilbc.c
  ../../ilbc/get_sync_seq.h
  ../../ilbc/get_sync_seq_ilbc.c
  ../../ilbc/hp_input.h
  ../../ilbc/hp_input_ilbc.c
  ../../ilbc/hp_output.h
  ../../ilbc/hp_output_ilbc.c
  ../../ilbc/ilbc_ilbc.c
  ../../ilbc/index_conv_dec.h
  ../../ilbc/index_conv_dec_ilbc.c
  ../../ilbc/index_conv_enc.h
  ../../ilbc/index_conv_enc_ilbc.c
  ../../ilbc/init_decode.h
  ../../ilbc/init_decode_ilbc.c
  ../../ilbc/init_encode.h
  ../../ilbc/init_encode_ilbc.c
  ../../ilbc/interface/ilbc.h
  ../../ilbc/interpolate.h
  ../../ilbc/interpolate_ilbc.c
  ../../ilbc/interpolate_samples.h
  ../../ilbc/interpolate_samples_ilbc.c
  ../../ilbc/lpc_encode.h
  ../../ilbc/lpc_encode_ilbc.c
  ../../ilbc/lsf_check.h
  ../../ilbc/lsf_check_ilbc.c
  ../../ilbc/lsf_interpolate_to_poly_dec.h
  ../../ilbc/lsf_interpolate_to_poly_dec_ilbc.c
  ../../ilbc/lsf_interpolate_to_poly_enc.h
  ../../ilbc/lsf_interpolate_to_poly_enc_ilbc.c
  ../../ilbc/lsf_to_lsp.h
  ../../ilbc/lsf_to_lsp_ilbc.c
  ../../ilbc/lsf_to_poly.h
  ../../ilbc/lsf_to_poly_ilbc.c
  ../../ilbc/lsp_to_lsf.h
  ../../ilbc/lsp_to_lsf_ilbc.c
  ../../ilbc/my_corr.h
  ../../ilbc/my_corr_ilbc.c
  ../../ilbc/nearest_neighbor.h
  ../../ilbc/nearest_neighbor_ilbc.c
  ../../ilbc/pack_bits.h
  ../../ilbc/pack_bits_ilbc.c
  ../../ilbc/poly_to_lsf.h
  ../../ilbc/poly_to_lsf_ilbc.c
  ../../ilbc/poly_to_lsp.h
  ../../ilbc/poly_to_lsp_ilbc.c
  ../../ilbc/refiner.h
  ../../ilbc/refiner_ilbc.c
  ../../ilbc/simple_interpolate_lsf.h
  ../../ilbc/simple_interpolate_lsf_ilbc.c
  ../../ilbc/simple_lpc_analysis.h
  ../../ilbc/simple_lpc_analysis_ilbc.c
  ../../ilbc/simple_lsf_dequant.h
  ../../ilbc/simple_lsf_dequant_ilbc.c
  ../../ilbc/simple_lsf_quant.h
  ../../ilbc/simple_lsf_quant_ilbc.c
  ../../ilbc/smooth.h
  ../../ilbc/smooth_ilbc.c
  ../../ilbc/smooth_out_data.h
  ../../ilbc/smooth_out_data_ilbc.c
  ../../ilbc/sort_sq.h
  ../../ilbc/sort_sq_ilbc.c
  ../../ilbc/split_vq.h
  ../../ilbc/split_vq_ilbc.c
  ../../ilbc/state_construct.h
  ../../ilbc/state_construct_ilbc.c
  ../../ilbc/state_search.h
  ../../ilbc/state_search_ilbc.c
  ../../ilbc/swap_bytes.h
  ../../ilbc/swap_bytes_ilbc.c
  ../../ilbc/unpack_bits.h
  ../../ilbc/unpack_bits_ilbc.c
  ../../ilbc/vq3.h
  ../../ilbc/vq3_ilbc.c
  ../../ilbc/vq4.h
  ../../ilbc/vq4_ilbc.c
  ../../ilbc/window32_w32.h
  ../../ilbc/window32_w32_ilbc.c
  ../../ilbc/xcorr_coef.h
  ../../ilbc/xcorr_coef_ilbc.c
)
# Output file name
set_target_properties(libilbc
  PROPERTIES
  OUTPUT_NAME_DEBUG libilbcD
)

# Additional include directories
set_property(TARGET libilbc
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libilbc\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libilbc\build\VS2017\..\..\include;
    J:\NlcJunk\DependLibs\libilbc\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libilbc\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libilbc\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libilbc PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;WIN32;_DEBUG;_LIB>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libilbc PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libilbc PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libilbc PRIVATE
    ""
  )
endif ()
