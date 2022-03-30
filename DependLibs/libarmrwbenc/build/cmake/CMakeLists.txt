cmake_minimum_required(VERSION 3.12.2)

project(libarmrwbenc)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libarmrwbenc STATIC
  ../../../../arpa/inet.h
  ../../../../asm/types.h
  ../../../../byteswap.h
  ../../../../dirent.h
  ../../../../dlfcn.h
  ../../../../AppCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../AppCpuArchDefines.h
  ../../../../AppDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../AppTargetOsConfig.h
  ../../../../langinfo.h
  ../../../../linux/ioctl.h
  ../../../../linux/param.h
  ../../../../netdb.h
  ../../../../netinet/in.h
  ../../../../netinet/tcp.h
  ../../../../strings.h
  ../../../../sys/ioctl.h
  ../../../../sys/mman.h
  ../../../../sys/param.h
  ../../../../sys/select.h
  ../../../../sys/signal.h
  ../../../../sys/socket.h
  ../../../../sys/syslog.h
  ../../../../sys/time.h
  ../../../../sys/times.h
  ../../../../sys/un.h
  ../../../../sys/utsname.h
  ../../../../syslog.h
  ../../../../unistd.h
  ../../common/cmnMemory.c
  ../../common/include/cmnMemory.h
  ../../common/include/voAAC.h
  ../../common/include/voAMRWB.h
  ../../common/include/voAudio.h
  ../../common/include/voIndex.h
  ../../common/include/voMem.h
  ../../common/include/voType.h
  ../../config_libarmrwbenc.h
  ../../enc_if.h
  ../../inc/acelp.h
  ../../inc/basic_op.h
  ../../inc/bits.h
  ../../inc/cnst.h
  ../../inc/cod_main.h
  ../../inc/dtx.h
  ../../inc/log2.h
  ../../inc/log2_tab.h
  ../../inc/main.h
  ../../inc/math_op.h
  ../../inc/mem_align.h
  ../../inc/oper_32b.h
  ../../inc/p_med_o.h
  ../../inc/q_pulse.h
  ../../inc/stream.h
  ../../inc/typedef_libarmrwbenc.h
  ../../inc/typedefs.h
  ../../inc/wb_vad.h
  ../../inc/wb_vad_c.h
  ../../SoftAMRWBEncoder.cpp
  ../../SoftAMRWBEncoder.h
  ../../src/autocorr.c
  ../../src/az_isp.c
  ../../src/bits.c
  ../../src/c2t64fx.c
  ../../src/c4t64fx.c
  ../../src/convolve.c
  ../../src/cor_h_x.c
  ../../src/decim54.c
  ../../src/deemph.c
  ../../src/dtx.c
  ../../src/g_pitch.c
  ../../src/gpclip.c
  ../../src/homing.c
  ../../src/hp_wsp.c
  ../../src/hp400.c
  ../../src/hp50.c
  ../../src/hp6k.c
  ../../src/int_lpc.c
  ../../src/isp_az.c
  ../../src/isp_isf.c
  ../../src/lag_wind.c
  ../../src/levinson.c
  ../../src/log2.c
  ../../src/lp_dec2.c
  ../../src/math_op.c
  ../../src/mem_align.c
  ../../src/oper_32b.c
  ../../src/p_med_ol.c
  ../../src/pit_shrp.c
  ../../src/pitch_f4.c
  ../../src/pred_lt4.c
  ../../src/preemph.c
  ../../src/q_gain2.c
  ../../src/q_pulse.c
  ../../src/qisf_ns.c
  ../../src/qpisf_2s.c
  ../../src/random.c
  ../../src/residu.c
  ../../src/scale.c
  ../../src/stream.c
  ../../src/syn_filt.c
  ../../src/updt_tar.c
  ../../src/util.c
  ../../src/voAMRWBEnc.c
  ../../src/voicefac.c
  ../../src/wb_vad.c
  ../../src/weight_a.c
  ../../wavreader.c
  ../../wavreader.h
  ../../wrapper.c
)
# Output file name
set_target_properties(libarmrwbenc
  PROPERTIES
  OUTPUT_NAME_DEBUG libarmrwbencD
)

# Additional include directories
set_property(TARGET libarmrwbenc
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\inc;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\src;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\common\include;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\..\openssl-1.0.2p\include;
    J:\NlcJunk\DependLibs\libarmrwbenc\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libarmrwbenc PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libarmrwbenc PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libarmrwbenc PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libarmrwbenc PRIVATE
    ""
  )
endif ()
