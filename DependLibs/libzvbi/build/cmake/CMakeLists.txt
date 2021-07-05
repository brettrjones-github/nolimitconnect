cmake_minimum_required(VERSION 3.12.2)

project(libzvbi)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libzvbi STATIC
  ../../../../dlfcn.h
  ../../../../GoTvCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../GoTvCpuArchDefines.h
  ../../../../GoTvDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../GoTvTargetOsConfig.h
  ../../../../langinfo.h
  ../../../../netdb.h
  ../../../../strings.h
  ../../../../sys/ioctl.h
  ../../../../sys/mman.h
  ../../../../sys/param.h
  ../../../../sys/signal.h
  ../../../../sys/socket.h
  ../../../../sys/syslog.h
  ../../../../sys/time.h
  ../../../../sys/un.h
  ../../../../sys/utsname.h
  ../../../../unistd.h
  ../../config_libzvbi.h
  ../../contrib/atsc-cc_zvbi.c
  ../../contrib/dvbsubs.h
  ../../contrib/dvbsubs_zvbi.c
  ../../contrib/ntsc-cc_zvbi.c
  ../../contrib/x11font_zvbi.c
  ../../site_def.h
  ../../src/_videodev.h
  ../../src/_videodev2.h
  ../../src/_videodev2k.h
  ../../src/bcd.h
  ../../src/bit_slicer.h
  ../../src/bit_slicer_zvbi.c
  ../../src/cache.h
  ../../src/cache_zvbi.c
  ../../src/cache-priv.h
  ../../src/caption_zvbi.c
  ../../src/cc.h
  ../../src/cc608_decoder.h
  ../../src/cc608_decoder_zvbi.c
  ../../src/chains_zvbi.c
  ../../src/conv.h
  ../../src/conv_zvbi.c
  ../../src/decoder.h
  ../../src/decoder_zvbi.c
  ../../src/dlist.h
  ../../src/dvb.h
  ../../src/dvb_demux.h
  ../../src/dvb_demux_zvbi.c
  ../../src/dvb_mux.h
  ../../src/dvb_mux_zvbi.c
  ../../src/event.h
  ../../src/event_zvbi.c
  ../../src/event-priv.h
  ../../src/exp-gfx.h
  ../../src/exp-gfx_zvbi.c
  ../../src/exp-html_zvbi.c
  ../../src/export_zvbi.c
  ../../src/export_zvbi.h
  ../../src/exp-templ_zvbi.c
  ../../src/exp-txt.h
  ../../src/exp-txt_zvbi.c
  ../../src/exp-vtx_zvbi.c
  ../../src/format.h
  ../../src/hamm.h
  ../../src/hamm_zvbi.c
  ../../src/hamm-tables.h
  ../../src/idl_demux.h
  ../../src/idl_demux_zvbi.c
  ../../src/intl-priv.h
  ../../src/io_zvbi_capture.h
  ../../src/io_zvbi_capture_zvbi.c
  ../../src/io-bktr_zvbi.c
  ../../src/io-dvb_zvbi.c
  ../../src/io-sim.h
  ../../src/io-sim_zvbi.c
  ../../src/io-v4l_zvbi.c
  ../../src/io-v4l2_zvbi.c
  ../../src/io-v4l2k_zvbi.c
  ../../src/lang.h
  ../../src/lang_zvbi.c
  ../../src/libzvbi.h
  ../../src/macros.h
  ../../src/misc.h
  ../../src/misc_zvbi.c
  ../../src/network.h
  ../../src/network-table.h
  ../../src/packet_zvbi.c
  ../../src/packet-830.h
  ../../src/packet-830_zvbi.c
  ../../src/page_table.h
  ../../src/page_table_zvbi.c
  ../../src/pdc.h
  ../../src/pdc_zvbi.c
  ../../src/pfc_demux.h
  ../../src/pfc_demux_zvbi.c
  ../../src/proxy-client.h
  ../../src/proxy-client_zvbi.c
  ../../src/proxy-msg.h
  ../../src/proxy-msg_zvbi.c
  ../../src/raw_decoder.h
  ../../src/raw_decoder_zvbi.c
  ../../src/sampling_par.h
  ../../src/sampling_par_zvbi.c
  ../../src/search.h
  ../../src/search_zvbi.c
  ../../src/sliced.h
  ../../src/sliced_filter.h
  ../../src/sliced_filter_zvbi.c
  ../../src/tables.h
  ../../src/tables_zvbi.c
  ../../src/teletext_decoder.h
  ../../src/teletext_zvbi.c
  ../../src/trigger.h
  ../../src/trigger_zvbi.c
  ../../src/ure.h
  ../../src/ure_zvbi.c
  ../../src/vbi.h
  ../../src/vbi_zvbi.c
  ../../src/version.h
  ../../src/videodev.h
  ../../src/videodev2.h
  ../../src/videodev2k.h
  ../../src/vps.h
  ../../src/vps_zvbi.c
  ../../src/vt.h
  ../../src/wss.h
  ../../src/wss_zvbi.c
  ../../src/xds_demux.h
  ../../src/xds_demux_zvbi.c
)
# Output file name
set_target_properties(libzvbi
  PROPERTIES
  OUTPUT_NAME_DEBUG libzvbiD
)

# Additional include directories
set_property(TARGET libzvbi
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\src;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\codec\decoder\core\inc;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\codec\common\inc;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\codec\api\svc;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\codec\;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\codec\..\;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libzvbi\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libzvbi PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;STATIC_LIB;_DEBUG;_LIB;X86_ASM>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libzvbi PRIVATE
    "$<$<CONFIG:Debug>:/Gm>"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libzvbi PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libzvbi PRIVATE
    ""
  )
endif ()
