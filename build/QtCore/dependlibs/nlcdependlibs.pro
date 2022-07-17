# Global
TEMPLATE = lib
CONFIG += qt thread silent

# keep it all lowercase to match program naming convention on *nix systems
PROJECT_NAME = gotvdependlibs

win32:TARGET_OS_DIR = Win32
linux:!android: TARGET_OS_DIR = Linux
android: TARGET_OS__DIR = Android
macx: TARGET_OS__DIR = Apple

DESTDIR = $$PWD/../../../libs/$$TARGET_OS__DIR/

TARGET_LIB_APPEND = .lib
CONFIG(release, debug|release){
 TARGET_LIB_APPEND = .lib
}

CONFIG(debug, debug|release){
 TARGET_LIB_APPEND = D.lib
}


include($$PWD/../../../DependLibs/bzlib/build/Qt/libbz2.pri)
#include(../../../NlcCore/build/Qt/libNlcCore.pri)


#include(../../DependLibs/CoreLib/build/Qt/CoreLib.pri)
#include(../../DependLibs/db-4.7.25.0/buildQt/db_static.pri)

win32{
#include(../../DependLibs/EasyHookLib/build/Qt/EasyHook.pri)
#include(../../DependLibs/Effects11/build/Qt/Effects11.pri)
}

#include(../../DependLibs/ffmpeg/libavcodec/build/Qt/FfmpegAvCodec.pri)
#include(../../DependLibs/ffmpeg/libavdevice/build/Qt/FfmpegAvDevice.pri)
#include(../../DependLibs/ffmpeg/libavfilter/build/Qt/FfmpegAvFilterLib.pri)
#include(../../DependLibs/ffmpeg/libavformat/build/Qt/FfmpegAvFormatLib.pri)
#include(../../DependLibs/ffmpeg/libavresample/build/Qt/FfmpegAvResampleLib.pri)
#include(../../DependLibs/ffmpeg/libavutil/build/Qt/FfmpegAvUtilLib.pri)
#include(../../DependLibs/ffmpeg/libpostproc/build/Qt/FfmpegPostProcLib.pri)
#include(../../DependLibs/ffmpeg/libswresample/build/Qt/FfmpegSwResampleLib.pri)
#include(../../DependLibs/ffmpeg/libswscale/build/Qt/FfmpegSwScaleLib.pri)
#include(../../DependLibs/ffmpeg/build/Qt/libffmpeg.pri)

#include(../../DependLibs/libasprintf/buildQt/libasprintf.pri)

#include(../../DependLibs/libass/build/Qt/libass.pri)

#include(../../DependLibs/libbzip2/build/Qt/libbzip2.pri)

#include(../../DependLibs/libcpluff/build/Qt/libcpluff.pri)
#include(../../DependLibs/libcroco/build/Qt/libcroco.pri)
#include(../../DependLibs/libcrossguid/build/Qt/libcrossguid.pri)
#include(../../DependLibs/libcrystalhd/build/Qt/libcrystalhd.pri)
#include(../../DependLibs/libcurl/build/Qt/libcurl.pri)

#include(../../DependLibs/libexif/build/Qt/libexif.pri)
#include(../../DependLibs/libexpat/build/Qt/libexpat.pri)

#include(../../DependLibs/libfaac/build/Qt/libfaac.pri)
#include(../../DependLibs/libfdk-aac/build/Qt/libfdk-aac.pri)
#include(../../DependLibs/libflac/build/Qt/libflac.pri)
#include(../../DependLibs/libfreetype/build/Qt/libfreetype.pri)
#include(../../DependLibs/libfribid/build/Qt/libfribid.pri)

#include(../../DependLibs/libglew/build/Qt/libglew.pri)
#include(../../DependLibs/libgnu/build/Qt/libgnu.pri)
#include(../../DependLibs/libgsm/build/Qt/libgsm.pri)

#include(../../DependLibs/libiconv/build/Qt/libiconv.pri)
#include(../../DependLibs/libilbc/build/Qt/libilbc.pri)
##include(../../DependLibs/libirrlicht/build/Qt/libirrlicht.pri)
#include(../../DependLibs/libkvazaar/build/Qt/libkvazaar.pri)

#include(../../DependLibs/liblame/build/Qt/liblame.pri)
##include(../../DependLibs/liblinuxsim/build/Qt/liblinuxsim.pri)
#include(../../DependLibs/liblzo/build/Qt/liblzo.pri)
#include(../../DependLibs/libmdnsresponder/build/Qt/libmdnsresponder.pri)
#include(../../DependLibs/libmicrohttpd/build/Qt/libmicrohttpd.pri)

#include(../../DependLibs/libogg/build/Qt/libogg.pri)
#include(../../DependLibs/libopencore-amrwb/build/Qt/libopencore-amrwb.pri)
#include(../../DependLibs/libopenh264/build/Qt/libopenh264.pri)
#include(../../DependLibs/libopenjpeg/build/Qt/libopenjpeg.pri)
#include(../../DependLibs/libopus/build/Qt/libopus.pri)

#include(../../DependLibs/libpcre/build/Qt/libpcre.pri)
#include(../../DependLibs/libpng/build/Qt/libpng.pri)
#include(../../DependLibs/libpthread/build/Qt/libpthread.pri)

#include(../../DependLibs/librsvg/build/Qt/librsvg.pri)
#include(../../DependLibs/libsdl/build/Qt/libsdl.pri)
#include(../../DependLibs/libshine/build/Qt/libshine.pri)
#include(../../DependLibs/libsnappy/build/Qt/libsnappy.pri)
#include(../../DependLibs/libspeex/build/Qt/libspeex.pri)
#include(../../DependLibs/libssh/build/Qt/libssh.pri)

#include(../../DependLibs/libtheora/build/Qt/libtheora.pri)
#include(../../DependLibs/libtwolame/build/Qt/libtwolame.pri)
#include(../../DependLibs/libUPnP/build/Qt/libUPnP.pri)

#include(../../DependLibs/libvo-amrwbenc/build/Qt/libvo-amrwbenc.pri)
#include(../../DependLibs/libvorbis/build/Qt/libvorbis.pri)
#include(../../DependLibs/libvpx/build/Qt/libvpx.pri)

#include(../../DependLibs/libwavepack/build/Qt/libwavepack.pri)
#include(../../DependLibs/libwebp/build/Qt/libwebp.pri)
#include(../../DependLibs/libvpx/build/Qt/libvpx.pri)

#include(../../DependLibs/libx264/build/Qt/libx264.pri)
#include(../../DependLibs/libx265/build/Qt/libx265.pri)
#include(../../DependLibs/libxavs/build/Qt/libxavs.pri)
#include(../../DependLibs/libxml/build/Qt/libxml.pri)
#include(../../DependLibs/libxml2/build/Qt/libxml2.pri)
#include(../../DependLibs/libxslt/build/Qt/libxslt.pri)
#include(../../DependLibs/libxvid/build/Qt/libxvid.pri)

#include(../../DependLibs/libzvbi/build/Qt/libzvbi.pri)
#include(../../DependLibs/MediaToolsLib/build/Qt/libmediatools.pri)

#include(../../DependLibs/libzvbi/build/Qt/libzvbi.pri)
#include(../../DependLibs/libzvbi/build/Qt/libzvbi.pri)
#include(../../DependLibs/libzvbi/build/Qt/libzvbi.pri)
#include(../../DependLibs/libzvbi/build/Qt/libzvbi.pri)
#include(../../DependLibs/libzvbi/build/Qt/libzvbi.pri)

#SOURCES +=




