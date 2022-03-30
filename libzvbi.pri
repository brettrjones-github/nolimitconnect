

INCLUDEPATH += $$PWD/DependLibs/libzvbi/src
INCLUDEPATH += $$PWD/DependLibs/libzvbi

HEADERS += 	$$PWD/AppCompilerConfig.h \
    $$PWD/AppCpuArchDefines.h \
    $$PWD/AppDependLibrariesConfig.h \
    $$PWD/AppTargetOsConfig.h \
    $$PWD/DependLibs/libzvbi/config_libzvbi.h \
    $$PWD/DependLibs/libzvbi/contrib/dvbsubs.h \
    $$PWD/DependLibs/libzvbi/site_def.h \
    $$PWD/DependLibs/libzvbi/src/_videodev.h \
    $$PWD/DependLibs/libzvbi/src/_videodev2.h \
    $$PWD/DependLibs/libzvbi/src/_videodev2k.h \
    $$PWD/DependLibs/libzvbi/src/bcd.h \
    $$PWD/DependLibs/libzvbi/src/bit_slicer.h \
    $$PWD/DependLibs/libzvbi/src/cache-priv.h \
    $$PWD/DependLibs/libzvbi/src/cache.h \
    $$PWD/DependLibs/libzvbi/src/cc.h \
    $$PWD/DependLibs/libzvbi/src/cc608_decoder.h \
    $$PWD/DependLibs/libzvbi/src/conv.h \
    $$PWD/DependLibs/libzvbi/src/decoder.h \
    $$PWD/DependLibs/libzvbi/src/dlist.h \
    $$PWD/DependLibs/libzvbi/src/dvb.h \
    $$PWD/DependLibs/libzvbi/src/dvb_demux.h \
    $$PWD/DependLibs/libzvbi/src/dvb_mux.h \
    $$PWD/DependLibs/libzvbi/src/event-priv.h \
    $$PWD/DependLibs/libzvbi/src/event.h \
    $$PWD/DependLibs/libzvbi/src/exp-gfx.h \
    $$PWD/DependLibs/libzvbi/src/exp-txt.h \
    $$PWD/DependLibs/libzvbi/src/export_zvbi.h \
    $$PWD/DependLibs/libzvbi/src/format.h \
    $$PWD/DependLibs/libzvbi/src/hamm-tables.h \
    $$PWD/DependLibs/libzvbi/src/hamm.h \
    $$PWD/DependLibs/libzvbi/src/idl_demux.h \
    $$PWD/DependLibs/libzvbi/src/intl-priv.h \
    $$PWD/DependLibs/libzvbi/src/io-sim.h \
    $$PWD/DependLibs/libzvbi/src/io_zvbi_capture.h \
    $$PWD/DependLibs/libzvbi/src/lang.h \
    $$PWD/DependLibs/libzvbi/src/libzvbi.h \
    $$PWD/DependLibs/libzvbi/src/macros.h \
    $$PWD/DependLibs/libzvbi/src/misc.h \
    $$PWD/DependLibs/libzvbi/src/network-table.h \
    $$PWD/DependLibs/libzvbi/src/network.h \
    $$PWD/DependLibs/libzvbi/src/packet-830.h \
    $$PWD/DependLibs/libzvbi/src/page_table.h \
    $$PWD/DependLibs/libzvbi/src/pdc.h \
    $$PWD/DependLibs/libzvbi/src/pfc_demux.h \
    $$PWD/DependLibs/libzvbi/src/proxy-client.h \
    $$PWD/DependLibs/libzvbi/src/proxy-msg.h \
    $$PWD/DependLibs/libzvbi/src/raw_decoder.h \
    $$PWD/DependLibs/libzvbi/src/sampling_par.h \
    $$PWD/DependLibs/libzvbi/src/sliced.h \
    $$PWD/DependLibs/libzvbi/src/sliced_filter.h \
    $$PWD/DependLibs/libzvbi/src/tables.h \
    $$PWD/DependLibs/libzvbi/src/teletext_decoder.h \
    $$PWD/DependLibs/libzvbi/src/trigger.h \
    $$PWD/DependLibs/libzvbi/src/ure.h \
    $$PWD/DependLibs/libzvbi/src/vbi.h \
    $$PWD/DependLibs/libzvbi/src/version.h \
    $$PWD/DependLibs/libzvbi/src/videodev.h \
    $$PWD/DependLibs/libzvbi/src/videodev2.h \
    $$PWD/DependLibs/libzvbi/src/videodev2k.h \
    $$PWD/DependLibs/libzvbi/src/vps.h \
    $$PWD/DependLibs/libzvbi/src/vt.h \
    $$PWD/DependLibs/libzvbi/src/wss.h \
    $$PWD/DependLibs/libzvbi/src/xds_demux.h \
    $$PWD/DependLibs/libzvbi/src/search_zvbi.h

SOURCES += 	\
    $$PWD/DependLibs/libzvbi/src/export_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/search_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/bit_slicer_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/cache_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/caption_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/cc608_decoder_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/chains_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/conv_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/decoder_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/dvb_demux_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/dvb_mux_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/event_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/exp-gfx_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/exp-html_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/exp-templ_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/exp-txt_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/exp-vtx_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/hamm_zvbi.c \
#    $$PWD/DependLibs/libzvbi/src/hammgen_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/idl_demux_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io-bktr_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io-dvb_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io-sim_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io-v4l_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io-v4l2k_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io-v4l2_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/io_zvbi_capture_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/lang_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/misc_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/packet-830_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/packet_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/page_table_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/pdc_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/pfc_demux_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/proxy-client_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/proxy-msg_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/raw_decoder_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/sampling_par_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/sliced_filter_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/tables_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/teletext_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/trigger_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/ure_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/vbi_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/vps_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/wss_zvbi.c \
    $$PWD/DependLibs/libzvbi/src/xds_demux_zvbi.c \
    $$PWD/DependLibs/libzvbi/contrib/atsc-cc_zvbi.c \
    $$PWD/DependLibs/libzvbi/contrib/dvbsubs_zvbi.c \
    $$PWD/DependLibs/libzvbi/contrib/ntsc-cc_zvbi.c \
    $$PWD/DependLibs/libzvbi/contrib/x11font_zvbi.c

