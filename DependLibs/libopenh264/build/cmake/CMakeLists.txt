cmake_minimum_required(VERSION 3.12.2)

project(libwelsdec)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libwelsdec STATIC
  ../../codec/common/inc/copy_mb.h
  ../../codec/common/inc/cpu.h
  ../../codec/common/inc/cpu_core.h
  ../../codec/common/inc/deblocking_common.h
  ../../codec/common/inc/expand_pic.h
  ../../codec/common/inc/ls_defines.h
  ../../codec/common/inc/macros.h
  ../../codec/common/inc/mc.h
  ../../codec/common/inc/measure_time.h
  ../../codec/common/inc/memory_align.h
  ../../codec/common/inc/typedefs.h
  ../../codec/common/inc/utils.h
  ../../codec/common/inc/wels_common_defs.h
  ../../codec/common/inc/wels_const_common.h
  ../../codec/common/src/common_tables.cpp
  ../../codec/common/src/copy_mb.cpp
  ../../codec/common/src/cpu.cpp
  ../../codec/common/src/crt_util_safe_x.cpp
  ../../codec/common/src/deblocking_common.cpp
  ../../codec/common/src/expand_pic.cpp
  ../../codec/common/src/mc.cpp
  ../../codec/common/src/memory_align.cpp
  ../../codec/common/src/utils.cpp
  ../../codec/decoder/core/inc/au_parser.h
  ../../codec/decoder/core/inc/bit_stream.h
  ../../codec/decoder/core/inc/cabac_decoder.h
  ../../codec/decoder/core/inc/deblocking.h
  ../../codec/decoder/core/inc/dec_frame.h
  ../../codec/decoder/core/inc/dec_golomb.h
  ../../codec/decoder/core/inc/decode_mb_aux.h
  ../../codec/decoder/core/inc/decode_slice.h
  ../../codec/decoder/core/inc/decoder.h
  ../../codec/decoder/core/inc/decoder_context.h
  ../../codec/decoder/core/inc/decoder_core.h
  ../../codec/decoder/core/inc/error_code.h
  ../../codec/decoder/core/inc/error_concealment.h
  ../../codec/decoder/core/inc/fmo.h
  ../../codec/decoder/core/inc/get_intra_predictor.h
  ../../codec/decoder/core/inc/manage_dec_ref.h
  ../../codec/decoder/core/inc/mb_cache.h
  ../../codec/decoder/core/inc/memmgr_nal_unit.h
  ../../codec/decoder/core/inc/mv_pred.h
  ../../codec/decoder/core/inc/nal_prefix.h
  ../../codec/decoder/core/inc/nalu.h
  ../../codec/decoder/core/inc/parameter_sets.h
  ../../codec/decoder/core/inc/parse_mb_syn_cabac.h
  ../../codec/decoder/core/inc/parse_mb_syn_cavlc.h
  ../../codec/decoder/core/inc/pic_queue.h
  ../../codec/decoder/core/inc/picture.h
  ../../codec/decoder/core/inc/rec_mb.h
  ../../codec/decoder/core/inc/slice.h
  ../../codec/decoder/core/inc/vlc_decoder.h
  ../../codec/decoder/core/inc/wels_common_basis.h
  ../../codec/decoder/core/inc/wels_const.h
  ../../codec/decoder/core/src/au_parser.cpp
  ../../codec/decoder/core/src/bit_stream.cpp
  ../../codec/decoder/core/src/cabac_decoder.cpp
  ../../codec/decoder/core/src/deblocking.cpp
  ../../codec/decoder/core/src/decode_mb_aux.cpp
  ../../codec/decoder/core/src/decode_slice.cpp
  ../../codec/decoder/core/src/decoder.cpp
  ../../codec/decoder/core/src/decoder_core.cpp
  ../../codec/decoder/core/src/decoder_data_tables.cpp
  ../../codec/decoder/core/src/error_concealment.cpp
  ../../codec/decoder/core/src/fmo.cpp
  ../../codec/decoder/core/src/get_intra_predictor.cpp
  ../../codec/decoder/core/src/manage_dec_ref.cpp
  ../../codec/decoder/core/src/memmgr_nal_unit.cpp
  ../../codec/decoder/core/src/mv_pred.cpp
  ../../codec/decoder/core/src/parse_mb_syn_cabac.cpp
  ../../codec/decoder/core/src/parse_mb_syn_cavlc.cpp
  ../../codec/decoder/core/src/pic_queue.cpp
  ../../codec/decoder/core/src/rec_mb.cpp
)
# Output file name
set_target_properties(libwelsdec
  PROPERTIES
  OUTPUT_NAME_DEBUG libwelsdecD
)

# Additional include directories
set_property(TARGET libwelsdec
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\codec\decoder\core\inc;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\codec\common\inc;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\codec\api\svc;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\codec\;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\codec\..\;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libopenh264\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libwelsdec PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;_DEBUG;_LIB;X86_ASM>
)

# SDL check
target_compile_options(libwelsdec PRIVATE
  "$<$<CONFIG:Debug>:/sdl->"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libwelsdec PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libwelsdec PRIVATE
    ""
  )
endif ()
