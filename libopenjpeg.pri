
INCLUDEPATH += $$PWD/DependLibs/libopenjpeg/src/lib
INCLUDEPATH += $$PWD/DependLibs/libopenjpeg/src/lib/openjp2
INCLUDEPATH += $$PWD/DependLibs/libopenjpeg/


HEADERS += 	$$PWD/DependLibs/libopenjpeg/../../AppCompilerConfig.h \
    $$PWD/DependLibs/libopenjpeg/../../AppCpuArchDefines.h \
    $$PWD/DependLibs/libopenjpeg/../../AppDependLibrariesConfig.h \
    $$PWD/DependLibs/libopenjpeg/../../AppTargetOsConfig.h \
    $$PWD/DependLibs/libopenjpeg/config_libopenjpeg.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/bio.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/cidx_manager.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/cio.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/dwt.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/event.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/function_list.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/image.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/indexbox_manager.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/invert.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/j2k.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/jp2.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/mct.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/mqc.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/mqc_inl.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/openjpeg.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_clock.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_codec.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_common.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_includes.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_intmath.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_inttypes.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_malloc.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_stdint.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/pi.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/t1.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/t1_luts.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/t2.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/tcd.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/tgt.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/thread.h \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/tls_keys.h \
    $$PWD/DependLibs/libopenjpeg/thirdparty/include/zconf.h \
    $$PWD/DependLibs/libopenjpeg/thirdparty/include/zlib.h \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/include/lcms2.h \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/include/lcms2_plugin.h \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/lcms2_internal.h

SOURCES += 	$$PWD/DependLibs/libopenjpeg/src/lib/openjp2/bench_dwt.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/bio.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/cidx_manager.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/cio.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/dwt.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/event.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/function_list.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/image.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/invert.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/j2k.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/jp2.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/mct.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/mqc.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/openjpeg.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_clock.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/opj_malloc.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/phix_manager.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/pi.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/ppix_manager.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/t1.c \
#    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/t1_generate_luts.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/t2.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/tcd.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/tgt.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/thix_manager.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/thread.c \
    $$PWD/DependLibs/libopenjpeg/src/lib/openjp2/tpix_manager.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsalpha.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmscam02.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmscgats.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmscnvrt.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmserr.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsgamma.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsgmt.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmshalf.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsintrp.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsio0.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsio1.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmslut.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsmd5.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsmtrx.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsnamed.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsopt.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmspack.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmspcs.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsplugin.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsps2.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmssamp.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmssm.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmstypes.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsvirt.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmswtpnt.c \
    $$PWD/DependLibs/libopenjpeg/thirdparty/liblcms2/src/cmsxform.c

