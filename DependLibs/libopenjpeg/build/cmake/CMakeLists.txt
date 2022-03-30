cmake_minimum_required(VERSION 3.12.2)

project(libopenjpeg)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libopenjpeg STATIC
  ../../../../AppCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../AppCpuArchDefines.h
  ../../../../AppDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../AppTargetOsConfig.h
  ../../config_libopenjpeg.h
  ../../src/lib/openjp2/bench_dwt.c
  ../../src/lib/openjp2/bio.c
  ../../src/lib/openjp2/bio.h
  ../../src/lib/openjp2/cidx_manager.c
  ../../src/lib/openjp2/cidx_manager.h
  ../../src/lib/openjp2/cio.c
  ../../src/lib/openjp2/cio.h
  ../../src/lib/openjp2/dwt.c
  ../../src/lib/openjp2/dwt.h
  ../../src/lib/openjp2/event.c
  ../../src/lib/openjp2/event.h
  ../../src/lib/openjp2/function_list.c
  ../../src/lib/openjp2/function_list.h
  ../../src/lib/openjp2/image.c
  ../../src/lib/openjp2/image.h
  ../../src/lib/openjp2/indexbox_manager.h
  ../../src/lib/openjp2/invert.c
  ../../src/lib/openjp2/invert.h
  ../../src/lib/openjp2/j2k.c
  ../../src/lib/openjp2/j2k.h
  ../../src/lib/openjp2/jp2.c
  ../../src/lib/openjp2/jp2.h
  ../../src/lib/openjp2/mct.c
  ../../src/lib/openjp2/mct.h
  ../../src/lib/openjp2/mqc.c
  ../../src/lib/openjp2/mqc.h
  ../../src/lib/openjp2/mqc_inl.h
  ../../src/lib/openjp2/openjpeg.c
  ../../src/lib/openjp2/openjpeg.h
  ../../src/lib/openjp2/opj_clock.c
  ../../src/lib/openjp2/opj_clock.h
  ../../src/lib/openjp2/opj_codec.h
  ../../src/lib/openjp2/opj_common.h
  ../../src/lib/openjp2/opj_includes.h
  ../../src/lib/openjp2/opj_intmath.h
  ../../src/lib/openjp2/opj_inttypes.h
  ../../src/lib/openjp2/opj_malloc.c
  ../../src/lib/openjp2/opj_malloc.h
  ../../src/lib/openjp2/opj_stdint.h
  ../../src/lib/openjp2/phix_manager.c
  ../../src/lib/openjp2/pi.c
  ../../src/lib/openjp2/pi.h
  ../../src/lib/openjp2/ppix_manager.c
  ../../src/lib/openjp2/t1.c
  ../../src/lib/openjp2/t1.h
  ../../src/lib/openjp2/t1_generate_luts.c
  ../../src/lib/openjp2/t1_luts.h
  ../../src/lib/openjp2/t2.c
  ../../src/lib/openjp2/t2.h
  ../../src/lib/openjp2/tcd.c
  ../../src/lib/openjp2/tcd.h
  ../../src/lib/openjp2/tgt.c
  ../../src/lib/openjp2/tgt.h
  ../../src/lib/openjp2/thix_manager.c
  ../../src/lib/openjp2/thread.c
  ../../src/lib/openjp2/thread.h
  ../../src/lib/openjp2/tls_keys.h
  ../../src/lib/openjp2/tpix_manager.c
  ../../thirdparty/include/zconf.h
  ../../thirdparty/include/zlib.h
  ../../thirdparty/liblcms2/include/lcms2.h
  ../../thirdparty/liblcms2/include/lcms2_plugin.h
  ../../thirdparty/liblcms2/src/cmsalpha.c
  ../../thirdparty/liblcms2/src/cmscam02.c
  ../../thirdparty/liblcms2/src/cmscgats.c
  ../../thirdparty/liblcms2/src/cmscnvrt.c
  ../../thirdparty/liblcms2/src/cmserr.c
  ../../thirdparty/liblcms2/src/cmsgamma.c
  ../../thirdparty/liblcms2/src/cmsgmt.c
  ../../thirdparty/liblcms2/src/cmshalf.c
  ../../thirdparty/liblcms2/src/cmsintrp.c
  ../../thirdparty/liblcms2/src/cmsio0.c
  ../../thirdparty/liblcms2/src/cmsio1.c
  ../../thirdparty/liblcms2/src/cmslut.c
  ../../thirdparty/liblcms2/src/cmsmd5.c
  ../../thirdparty/liblcms2/src/cmsmtrx.c
  ../../thirdparty/liblcms2/src/cmsnamed.c
  ../../thirdparty/liblcms2/src/cmsopt.c
  ../../thirdparty/liblcms2/src/cmspack.c
  ../../thirdparty/liblcms2/src/cmspcs.c
  ../../thirdparty/liblcms2/src/cmsplugin.c
  ../../thirdparty/liblcms2/src/cmsps2.c
  ../../thirdparty/liblcms2/src/cmssamp.c
  ../../thirdparty/liblcms2/src/cmssm.c
  ../../thirdparty/liblcms2/src/cmstypes.c
  ../../thirdparty/liblcms2/src/cmsvirt.c
  ../../thirdparty/liblcms2/src/cmswtpnt.c
  ../../thirdparty/liblcms2/src/cmsxform.c
  ../../thirdparty/liblcms2/src/lcms2_internal.h
)
# Output file name
set_target_properties(libopenjpeg
  PROPERTIES
  OUTPUT_NAME_DEBUG libopenjpegD
)

# Additional include directories
set_property(TARGET libopenjpeg
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libopenjpeg\build\VS2017\..\..\openjp2;
    J:\NlcJunk\DependLibs\libopenjpeg\build\VS2017\..\..\src\lib;
    J:\NlcJunk\DependLibs\libopenjpeg\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libopenjpeg\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libopenjpeg\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libopenjpeg\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libopenjpeg PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libopenjpeg PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libopenjpeg PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libopenjpeg PRIVATE
    ""
  )
endif ()
