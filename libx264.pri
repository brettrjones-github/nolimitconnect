INCLUDEPATH += $$PWD/DependLibs/libx264/

HEADERS += 	$$PWD/NlcCompilerConfig.h \
    $$PWD/NlcCpuArchDefines.h \
    $$PWD/NlcDependLibrariesConfig.h \
    $$PWD/NlcTargetOsConfig.h \
    $$PWD/DependLibs/libx264/common/bitstream.h \
    $$PWD/DependLibs/libx264/common/cabac.h \
    $$PWD/DependLibs/libx264/common/common.h \
    $$PWD/DependLibs/libx264/common/cpu.h \
    $$PWD/DependLibs/libx264/common/dct.h \
    $$PWD/DependLibs/libx264/common/frame.h \
    $$PWD/DependLibs/libx264/common/macroblock.h \
    $$PWD/DependLibs/libx264/common/mc.h \
    $$PWD/DependLibs/libx264/common/opencl.h \
    $$PWD/DependLibs/libx264/common/osdep.h \
    $$PWD/DependLibs/libx264/common/pixel.h \
    $$PWD/DependLibs/libx264/common/predict.h \
    $$PWD/DependLibs/libx264/common/quant.h \
    $$PWD/DependLibs/libx264/common/rectangle.h \
    $$PWD/DependLibs/libx264/common/set.h \
    $$PWD/DependLibs/libx264/common/threadpool.h \
    $$PWD/DependLibs/libx264/common/win32thread.h \
    $$PWD/DependLibs/libx264/config_libx264.h \
    $$PWD/DependLibs/libx264/x264.h \
    $$PWD/DependLibs/libx264/x264cli.h

SOURCES += 	\
#    $$PWD/DependLibs/libx264/x264.c \
    $$PWD/DependLibs/libx264/common/cpu_x264.c \
    $$PWD/DependLibs/libx264/common/common_x264.c \
    $$PWD/DependLibs/libx264/common/bitstream_x264.c \
    $$PWD/DependLibs/libx264/common/cabac_x264.c \
    $$PWD/DependLibs/libx264/common/dct_x264.c \
    $$PWD/DependLibs/libx264/common/deblock_x264.c \
    $$PWD/DependLibs/libx264/common/frame_x264.c \
    $$PWD/DependLibs/libx264/common/macroblock_x264.c \
    $$PWD/DependLibs/libx264/common/mc_x264.c \
    $$PWD/DependLibs/libx264/common/mvpred_x264.c \
    $$PWD/DependLibs/libx264/common/opencl_x264.c \
    $$PWD/DependLibs/libx264/common/osdep_x264.c \
    $$PWD/DependLibs/libx264/common/pixel_x264.c \
    $$PWD/DependLibs/libx264/common/predict_x264.c \
    $$PWD/DependLibs/libx264/common/quant_x264.c \
    $$PWD/DependLibs/libx264/common/rectangle_x264.c \
    $$PWD/DependLibs/libx264/common/set_x264.c \
    $$PWD/DependLibs/libx264/common/threadpool_x264.c \
    $$PWD/DependLibs/libx264/common/vlc_x264.c \
    $$PWD/DependLibs/libx264/common/win32thread_x264.c

