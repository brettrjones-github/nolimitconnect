# Global
TEMPLATE = lib
QT       -= gui
CONFIG += staticlib
CONFIG -= sharedlib

TARGET_NAME = depends

INCLUDEPATH += $$PWD/DependLibs
INCLUDEPATH += $$PWD/DependLibs/libcurl/include
INCLUDEPATH += $$PWD/DependLibs/libcurl/lib
INCLUDEPATH += $$PWD/DependLibs/ffmpeg
INCLUDEPATH += $$PWD/kodi_src/xbmc/xbmc

include(config_static_dependlib.pri)

PRE_TARGETDEPS += $$PWD/libgnu.pro

#short obj path so does not overflow windows 32K command line limit "make (e=87): The parameter is incorrect"
CONFIG(debug, debug|release){
    OBJECTS_DIR=.dependd
}

CONFIG(release, debug|release){
    OBJECTS_DIR=.dependr
}

android{
INCLUDEPATH += $$PWD/DependLibs/androidNdk/android/cpufeatures
HEADERS += 	$$PWD/DependLibs/androidNdk/android/cpufeatures/cpu-features.h
SOURCES += 	$$PWD/DependLibs/androidNdk/android/cpufeatures/cpu-features_ndk.c
}


### build seperately #include(../../GoTvCore/build/Qt/libptopengine.pri)
### build seperately #include(../../GoTvCore/build/Qt/libxbmc.pri)

### build seperately include(../../DependLibs/CoreLib/build/Qt/CoreLib.pri)

win32{
#include(../../DependLibs/EasyHookLib/build/Qt/EasyHook.pri)
#include(../../DependLibs/Effects11/build/Qt/Effects11.pri)
}

### build in libffmpeg
#include(../../DependLibs/ffmpeg/libavcodec/build/Qt/libavcodec.pri)
#include(../../DependLibs/ffmpeg/libavdevice/build/Qt/libavdevice.pri)
#include(../../DependLibs/ffmpeg/libavfilter/build/Qt/libavfilter.pri)
#include(../../DependLibs/ffmpeg/libavformat/build/Qt/libavformat.pri)
#include(../../DependLibs/ffmpeg/libavresample/build/Qt/libavresample.pri)
#include(../../DependLibs/ffmpeg/libavutil/build/Qt/libavutil.pri)
#include(../../DependLibs/ffmpeg/libpostproc/build/Qt/libpostproc.pri)
#include(../../DependLibs/ffmpeg/libswresample/build/Qt/libswresample.pri)
#include(../../DependLibs/ffmpeg/libswscale/build/Qt/libswscale.pri)

### build seperately include(../../DependLibs/libarmrwbenc/build/Qt/libarmrwbenc.pri)
include(libass.pri)

include(libcpluff.pri)
include(libcroco.pri)
### build seperately include(../../DependLibs/libcrossguid/build/Qt/libcrossguid.pri)
###include(../../DependLibs/libcrystalhd/build/Qt/libcrystalhd.pri)
### build seperately include(../../DependLibs/libcurl/build/Qt/libcurl.pri)

include(libexif.pri)
include(libexpat.pri)

###include(../../DependLibs/libfaac/build/Qt/libfaac.pri)
### build seperately include(../../DependLibs/libfdk-aac/build/Qt/libfdk-aac.pri)
include(libflac.pri)
### build seperately include(../../DependLibs/libfreetype/build/Qt/libfreetype.pri)
include(libfribidi.pri)

include(libglew.pri)
### build seperately include(../../DependLibs/libgnu/build/Qt/libgnu.pri)
### build seperately include(libgsm.pri)

include(libiconv.pri)
include(libilbc.pri)
include(libjpg.pri)

###include(../../DependLibs/libirrlicht/build/Qt/libirrlicht.pri)
###include(../../DependLibs/libkvazaar/build/Qt/libkvazaar.pri)

### build seperately include(../../DependLibs/liblame/build/Qt/liblame.pri)

include(libmdnssd.pri)
### build seperately include(../../DependLibs/libmicrohttpd/build/Qt/libmicrohttpd.pri)

### build seperately include(../../DependLibs/libogg/build/Qt/libogg.pri)
### build seperately include(../../DependLibs/libopencore-amr/build/Qt/libopencore-amr.pri)
###include(../../DependLibs/libopenh264/build/Qt/libopenh264.pri)

include(vxvideolib.pri)

include(libopenjpeg.pri)
### build seperately include(../../DependLibs/libopus/build/Qt/libopus.pri)

### build seperately include(../../DependLibs/libpcre/build/Qt/libpcre.pri)
### build seperately include(../../DependLibs/libpng/build/Qt/libpng.pri)
### build seperately include(../../DependLibs/libpthread/build/Qt/libpthread.pri)

###include(../../DependLibs/librsvg/build/Qt/librsvg.pri)
###include(../../DependLibs/libsdl/build/Qt/libsdl.pri)
include(libshine.pri)
include(libsnappy.pri)
include(libspeex.pri)
### build seperately include(../../DependLibs/libssh/build/Qt/libssh.pri)
### build seperately include(../../DependLibs/openssl-1.0.2o/build/Qt/libssl.pri)

include(libtag.pri)
include(libtheora.pri)
include(libtwolame.pri)
include(libUPnP.pri)

### not built include(libvmaf.pri)
### build seperately include(../../DependLibs/libvorbis/build/Qt/libvorbis.pri)
### build seperately include(../../DependLibs/libvpx/build/Qt/libvpx.pri)

include(libwavepack.pri)
include(libwebp.pri)

### build seperately include(../../DependLibs/libx264/build/Qt/libx264.pri)
### build seperately include(../../DependLibs/libx265/build/Qt/libx265.pri)
### build seperately include(../../DependLibs/libxavs/build/Qt/libxavs.pri)

###include(../../DependLibs/libxpm/build/Qt/libxpm.pri)
include(libxslt.pri)
###include(../../DependLibs/libxvid/build/Qt/libxvid.pri)

include(libyajl.pri)
include(libzvbi.pri)

### build seperately include(../../DependLibs/MediaToolsLib/build/Qt/libmediatools.pri)


### build seperately because changes often include(libnetlib.pri)
### build seperately because changes often include(libpktlib.pri)
include(libupnp2lib.pri)

### include(../../DependLibs/libasprintf/build/Qt/libasprintf.pri)
### build seperately include(../../DependLibs/libcdio/build/Qt/libcdio.pri)

HEADERS += \
    GoTvAppConfig.h \
    AppCompilerConfig.h \
    AppCpuArchDefines.h \
    AppDependLibrariesConfig.h






