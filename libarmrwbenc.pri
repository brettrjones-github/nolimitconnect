
INCLUDEPATH += $$PWD/DependLibs/libarmrwbenc
INCLUDEPATH += $$PWD/DependLibs/libarmrwbenc/inc
INCLUDEPATH += $$PWD/DependLibs/libarmrwbenc/common/include

HEADERS += 	$$PWD/NlcCompilerConfig.h \
    $$PWD/NlcCpuArchDefines.h \
    $$PWD/NlcDependLibrariesConfig.h \
    $$PWD/NlcTargetOsConfig.h \
    $$PWD/DependLibs/libarmrwbenc/SoftAMRWBEncoder.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/cmnMemory.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/voAAC.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/voAMRWB.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/voAudio.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/voIndex.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/voMem.h \
    $$PWD/DependLibs/libarmrwbenc/common/include/voType.h \
    $$PWD/DependLibs/libarmrwbenc/config_libarmrwbenc.h \
    $$PWD/DependLibs/libarmrwbenc/enc_if.h \
    $$PWD/DependLibs/libarmrwbenc/inc/acelp.h \
    $$PWD/DependLibs/libarmrwbenc/inc/basic_op.h \
    $$PWD/DependLibs/libarmrwbenc/inc/bits.h \
    $$PWD/DependLibs/libarmrwbenc/inc/cnst.h \
    $$PWD/DependLibs/libarmrwbenc/inc/cod_main.h \
    $$PWD/DependLibs/libarmrwbenc/inc/dtx.h \
    $$PWD/DependLibs/libarmrwbenc/inc/log2.h \
    $$PWD/DependLibs/libarmrwbenc/inc/log2_tab.h \
    $$PWD/DependLibs/libarmrwbenc/inc/main.h \
    $$PWD/DependLibs/libarmrwbenc/inc/math_op.h \
    $$PWD/DependLibs/libarmrwbenc/inc/mem_align.h \
    $$PWD/DependLibs/libarmrwbenc/inc/oper_32b.h \
    $$PWD/DependLibs/libarmrwbenc/inc/p_med_o.h \
    $$PWD/DependLibs/libarmrwbenc/inc/q_pulse.h \
    $$PWD/DependLibs/libarmrwbenc/inc/stream.h \
    $$PWD/DependLibs/libarmrwbenc/inc/typedef_libarmrwbenc.h \
    $$PWD/DependLibs/libarmrwbenc/inc/typedefs.h \
    $$PWD/DependLibs/libarmrwbenc/inc/wb_vad.h \
    $$PWD/DependLibs/libarmrwbenc/inc/wb_vad_c.h \
    $$PWD/DependLibs/libarmrwbenc/wavreader.h

SOURCES += 	$$PWD/DependLibs/libarmrwbenc/SoftAMRWBEncoder.cpp \
#    $$PWD/DependLibs/libarmrwbenc/amrwb-enc.c \
    $$PWD/DependLibs/libarmrwbenc/common/cmnMemory.c \
    $$PWD/DependLibs/libarmrwbenc/src/autocorr.c \
    $$PWD/DependLibs/libarmrwbenc/src/az_isp.c \
    $$PWD/DependLibs/libarmrwbenc/src/bits.c \
    $$PWD/DependLibs/libarmrwbenc/src/c2t64fx.c \
    $$PWD/DependLibs/libarmrwbenc/src/c4t64fx.c \
    $$PWD/DependLibs/libarmrwbenc/src/convolve.c \
    $$PWD/DependLibs/libarmrwbenc/src/cor_h_x.c \
    $$PWD/DependLibs/libarmrwbenc/src/decim54.c \
    $$PWD/DependLibs/libarmrwbenc/src/deemph.c \
    $$PWD/DependLibs/libarmrwbenc/src/dtx.c \
    $$PWD/DependLibs/libarmrwbenc/src/g_pitch.c \
    $$PWD/DependLibs/libarmrwbenc/src/gpclip.c \
    $$PWD/DependLibs/libarmrwbenc/src/homing.c \
    $$PWD/DependLibs/libarmrwbenc/src/hp400.c \
    $$PWD/DependLibs/libarmrwbenc/src/hp50.c \
    $$PWD/DependLibs/libarmrwbenc/src/hp6k.c \
    $$PWD/DependLibs/libarmrwbenc/src/hp_wsp.c \
    $$PWD/DependLibs/libarmrwbenc/src/int_lpc.c \
    $$PWD/DependLibs/libarmrwbenc/src/isp_az.c \
    $$PWD/DependLibs/libarmrwbenc/src/isp_isf.c \
    $$PWD/DependLibs/libarmrwbenc/src/lag_wind.c \
    $$PWD/DependLibs/libarmrwbenc/src/levinson.c \
    $$PWD/DependLibs/libarmrwbenc/src/log2.c \
    $$PWD/DependLibs/libarmrwbenc/src/lp_dec2.c \
    $$PWD/DependLibs/libarmrwbenc/src/math_op.c \
    $$PWD/DependLibs/libarmrwbenc/src/mem_align.c \
    $$PWD/DependLibs/libarmrwbenc/src/oper_32b.c \
    $$PWD/DependLibs/libarmrwbenc/src/p_med_ol.c \
    $$PWD/DependLibs/libarmrwbenc/src/pit_shrp.c \
    $$PWD/DependLibs/libarmrwbenc/src/pitch_f4.c \
    $$PWD/DependLibs/libarmrwbenc/src/pred_lt4.c \
    $$PWD/DependLibs/libarmrwbenc/src/preemph.c \
    $$PWD/DependLibs/libarmrwbenc/src/q_gain2.c \
    $$PWD/DependLibs/libarmrwbenc/src/q_pulse.c \
    $$PWD/DependLibs/libarmrwbenc/src/qisf_ns.c \
    $$PWD/DependLibs/libarmrwbenc/src/qpisf_2s.c \
    $$PWD/DependLibs/libarmrwbenc/src/random.c \
    $$PWD/DependLibs/libarmrwbenc/src/residu.c \
    $$PWD/DependLibs/libarmrwbenc/src/scale.c \
    $$PWD/DependLibs/libarmrwbenc/src/stream.c \
    $$PWD/DependLibs/libarmrwbenc/src/syn_filt.c \
    $$PWD/DependLibs/libarmrwbenc/src/updt_tar.c \
    $$PWD/DependLibs/libarmrwbenc/src/util.c \
    $$PWD/DependLibs/libarmrwbenc/src/voAMRWBEnc.c \
    $$PWD/DependLibs/libarmrwbenc/src/voicefac.c \
    $$PWD/DependLibs/libarmrwbenc/src/wb_vad.c \
    $$PWD/DependLibs/libarmrwbenc/src/weight_a.c \
    $$PWD/DependLibs/libarmrwbenc/wavreader.c \
    $$PWD/DependLibs/libarmrwbenc/wrapper.c

