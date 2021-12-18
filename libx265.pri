



INCLUDEPATH += $$PWD/DependLibs/libx265/

HEADERS += 	$$PWD/AppCompilerConfig.h \
    $$PWD/AppCpuArchDefines.h \
    $$PWD/AppDependLibrariesConfig.h \
    $$PWD/AppTargetOsConfig.h \
    $$PWD/DependLibs/libx265/common/bitstream.h \
    $$PWD/DependLibs/libx265/common/common.h \
    $$PWD/DependLibs/libx265/common/constants.h \
    $$PWD/DependLibs/libx265/common/contexts.h \
    $$PWD/DependLibs/libx265/common/cpu.h \
    $$PWD/DependLibs/libx265/common/cudata.h \
    $$PWD/DependLibs/libx265/common/deblock.h \
    $$PWD/DependLibs/libx265/common/frame.h \
    $$PWD/DependLibs/libx265/common/framedata.h \
    $$PWD/DependLibs/libx265/common/lowres.h \
    $$PWD/DependLibs/libx265/common/md5.h \
    $$PWD/DependLibs/libx265/common/mv.h \
    $$PWD/DependLibs/libx265/common/param.h \
    $$PWD/DependLibs/libx265/common/piclist.h \
    $$PWD/DependLibs/libx265/common/picyuv.h \
    $$PWD/DependLibs/libx265/common/predict.h \
    $$PWD/DependLibs/libx265/common/primitives.h \
    $$PWD/DependLibs/libx265/common/quant.h \
    $$PWD/DependLibs/libx265/common/scalinglist.h \
    $$PWD/DependLibs/libx265/common/shortyuv.h \
    $$PWD/DependLibs/libx265/common/slice.h \
    $$PWD/DependLibs/libx265/common/threading.h \
    $$PWD/DependLibs/libx265/common/threadpool.h \
    $$PWD/DependLibs/libx265/common/wavefront.h \
    $$PWD/DependLibs/libx265/common/winxp.h \
    $$PWD/DependLibs/libx265/common/yuv.h \
    $$PWD/DependLibs/libx265/config_libx265.h \
    $$PWD/DependLibs/libx265/encoder/analysis.h \
    $$PWD/DependLibs/libx265/encoder/bitcost.h \
    $$PWD/DependLibs/libx265/encoder/dpb.h \
    $$PWD/DependLibs/libx265/encoder/encoder.h \
    $$PWD/DependLibs/libx265/encoder/entropy.h \
    $$PWD/DependLibs/libx265/encoder/frameencoder.h \
    $$PWD/DependLibs/libx265/encoder/framefilter.h \
    $$PWD/DependLibs/libx265/encoder/level.h \
    $$PWD/DependLibs/libx265/encoder/motion.h \
    $$PWD/DependLibs/libx265/encoder/nal.h \
    $$PWD/DependLibs/libx265/encoder/ratecontrol.h \
    $$PWD/DependLibs/libx265/encoder/rdcost.h \
    $$PWD/DependLibs/libx265/encoder/reference.h \
    $$PWD/DependLibs/libx265/encoder/sao.h \
    $$PWD/DependLibs/libx265/encoder/search.h \
    $$PWD/DependLibs/libx265/encoder/sei.h \
    $$PWD/DependLibs/libx265/encoder/slicetype.h \
    $$PWD/DependLibs/libx265/x265.h \
    $$PWD/DependLibs/libx265/x265cli.h

SOURCES += 	\
#    $$PWD/DependLibs/libx265/x265.cpp \
    $$PWD/DependLibs/libx265/common/bitstream_x265.cpp \
    $$PWD/DependLibs/libx265/common/common_x265.cpp \
    $$PWD/DependLibs/libx265/common/cpu_x265.cpp \
    $$PWD/DependLibs/libx265/common/version_x265.cpp \
    $$PWD/DependLibs/libx265/common/md5_x265.cpp \
    $$PWD/DependLibs/libx265/common/dct_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/api_x265.cpp \
    $$PWD/DependLibs/libx265/common/constants_x265.cpp \
    $$PWD/DependLibs/libx265/common/cudata_x265.cpp \
    $$PWD/DependLibs/libx265/common/deblock_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/encoder_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/entropy_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/level_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/motion_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/nal_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/bitcost_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/dpb_x265.cpp \
    $$PWD/DependLibs/libx265/common/yuv_x265.cpp \
    $$PWD/DependLibs/libx265/common/slice_x265.cpp \
    $$PWD/DependLibs/libx265/common/pixel_x265.cpp \
    $$PWD/DependLibs/libx265/common/predict_x265.cpp \
    $$PWD/DependLibs/libx265/common/primitives_x265.cpp \
    $$PWD/DependLibs/libx265/common/quant_x265.cpp \
    $$PWD/DependLibs/libx265/common/frame_x265.cpp \
    $$PWD/DependLibs/libx265/common/framedata_x265.cpp \
    $$PWD/DependLibs/libx265/common/intrapred_x265.cpp \
    $$PWD/DependLibs/libx265/common/ipfilter_x265.cpp \
    $$PWD/DependLibs/libx265/common/loopfilter_x265.cpp \
    $$PWD/DependLibs/libx265/common/lowres_x265.cpp \
    $$PWD/DependLibs/libx265/common/param_x265.cpp \
    $$PWD/DependLibs/libx265/common/piclist_x265.cpp \
    $$PWD/DependLibs/libx265/common/picyuv_x265.cpp \
    $$PWD/DependLibs/libx265/common/scalinglist_x265.cpp \
    $$PWD/DependLibs/libx265/common/shortyuv_x265.cpp \
    $$PWD/DependLibs/libx265/common/threading_x265.cpp \
    $$PWD/DependLibs/libx265/common/threadpool_x265.cpp \
    $$PWD/DependLibs/libx265/common/wavefront_x265.cpp \
    $$PWD/DependLibs/libx265/common/winxp_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/analysis_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/frameencoder_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/framefilter_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/ratecontrol_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/reference_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/sao_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/search_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/sei_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/slicetype_x265.cpp \
    $$PWD/DependLibs/libx265/encoder/weightPrediction_x265.cpp

