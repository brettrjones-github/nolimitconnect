cmake_minimum_required(VERSION 3.12.2)

project(libxvid)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libxvid STATIC
  ../../src/bitstream/bitstream.c
  ../../src/bitstream/bitstream.h
  ../../src/bitstream/cbp.c
  ../../src/bitstream/cbp.h
  ../../src/bitstream/mbcoding.c
  ../../src/bitstream/mbcoding.h
  ../../src/bitstream/vlc_codes.h
  ../../src/bitstream/zigzag.h
  ../../src/dct/fdct.c
  ../../src/dct/fdct.h
  ../../src/dct/idct.c
  ../../src/dct/idct.h
  ../../src/dct/simple_idct.c
  ../../src/decoder.c
  ../../src/decoder.h
  ../../src/encoder.c
  ../../src/encoder.h
  ../../src/global.h
  ../../src/image/colorspace.c
  ../../src/image/colorspace.h
  ../../src/image/font.c
  ../../src/image/font.h
  ../../src/image/image.c
  ../../src/image/image.h
  ../../src/image/interpolate8x8.c
  ../../src/image/interpolate8x8.h
  ../../src/image/postprocessing.c
  ../../src/image/postprocessing.h
  ../../src/image/qpel.c
  ../../src/image/qpel.h
  ../../src/image/reduced.c
  ../../src/image/reduced.h
  ../../src/motion/estimation.h
  ../../src/motion/estimation_bvop.c
  ../../src/motion/estimation_common.c
  ../../src/motion/estimation_gmc.c
  ../../src/motion/estimation_pvop.c
  ../../src/motion/estimation_rd_based.c
  ../../src/motion/estimation_rd_based_bvop.c
  ../../src/motion/gmc.c
  ../../src/motion/gmc.h
  ../../src/motion/motion.h
  ../../src/motion/motion_comp.c
  ../../src/motion/motion_inlines.h
  ../../src/motion/sad.c
  ../../src/motion/sad.h
  ../../src/motion/vop_type_decision.c
  ../../src/plugins/plugin_2pass1.c
  ../../src/plugins/plugin_2pass2.c
  ../../src/plugins/plugin_dump.c
  ../../src/plugins/plugin_lumimasking.c
  ../../src/plugins/plugin_psnr.c
  ../../src/plugins/plugin_psnrhvsm.c
  ../../src/plugins/plugin_single.c
  ../../src/plugins/plugin_ssim.c
  ../../src/portab.h
  ../../src/prediction/mbprediction.c
  ../../src/prediction/mbprediction.h
  ../../src/quant/quant.h
  ../../src/quant/quant_h263.c
  ../../src/quant/quant_matrix.c
  ../../src/quant/quant_matrix.h
  ../../src/quant/quant_mpeg.c
  ../../src/utils/emms.c
  ../../src/utils/emms.h
  ../../src/utils/mbfunctions.h
  ../../src/utils/mbtransquant.c
  ../../src/utils/mem_align.c
  ../../src/utils/mem_align.h
  ../../src/utils/mem_transfer.c
  ../../src/utils/mem_transfer.h
  ../../src/utils/ratecontrol.h
  ../../src/utils/timer.c
  ../../src/utils/timer.h
  ../../src/xvid.c
  ../../src/xvid.h
)
# Output file name
set_target_properties(libxvid
  PROPERTIES
  OUTPUT_NAME_DEBUG libxvidD
)

# Additional include directories
set_property(TARGET libxvid
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libxvid\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libxvid\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libxvid\build\VS2017\..\..\..\..\>
)

# Preprocessor definitions
target_compile_definitions(libxvid PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_DEBUG;HAVE_PTHREAD;WIN64;_WINDOWS;ARCH_IS_64BIT;ARCH_IS_X86_64;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libxvid PRIVATE
    "$<$<CONFIG:Debug>:/Gm>"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libxvid PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libxvid PRIVATE
    ""
  )
endif ()
