

TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered
# python must be build seperately and before this project
#SUBDIRS += $$PWD/NlcPythonLib.pro

SUBDIRS += $$PWD/libechocancel.pro

SUBDIRS += $$PWD/libavdevice.pro
SUBDIRS += $$PWD/libpostproc.pro
SUBDIRS += $$PWD/libavformat.pro
SUBDIRS += $$PWD/libavfilter.pro
SUBDIRS += $$PWD/libavcodec.pro

#SUBDIRS += $$PWD/libavresample.pro
SUBDIRS += $$PWD/libswscale.pro
SUBDIRS += $$PWD/libswresample.pro

SUBDIRS += $$PWD/libavutil.pro

SUBDIRS += $$PWD/libnfs.pro
SUBDIRS += $$PWD/libpcre.pro
SUBDIRS += $$PWD/libcdio.pro

#SUBDIRS += $$PWD/libnetlib.pro

SUBDIRS += $$PWD/libarmrwbenc.pro
SUBDIRS += $$PWD/libfdk-aac.pro
SUBDIRS += $$PWD/libopencore-amr.pro
SUBDIRS += $$PWD/libopenmpt-full.pro
SUBDIRS += $$PWD/libvpx.pro
SUBDIRS += $$PWD/libx264.pro
SUBDIRS += $$PWD/libx265.pro

SUBDIRS += $$PWD/libmediatools.pro
SUBDIRS += $$PWD/libopus.pro
SUBDIRS += $$PWD/libspeex.pro
SUBDIRS += $$PWD/liblame.pro

SUBDIRS += $$PWD/libasprintf.pro


SUBDIRS += $$PWD/libvorbis.pro

SUBDIRS += $$PWD/libmicrohttpd.pro
SUBDIRS += $$PWD/libogg.pro
SUBDIRS += $$PWD/libssh.pro
SUBDIRS += $$PWD/libfribidi.pro
SUBDIRS += $$PWD/libfreetype.pro
SUBDIRS += $$PWD/libpng.pro
SUBDIRS += $$PWD/libtinyxml.pro
SUBDIRS += $$PWD/libxml2.pro
SUBDIRS += $$PWD/libcompress.pro
SUBDIRS += $$PWD/libzlib.pro

# these are built in Build1_PythonDepends.pro but do again here because Build1 is release only and we my be doing a debug build
SUBDIRS += $$PWD/libpktlib.pro
SUBDIRS += $$PWD/libdepends.pro
SUBDIRS += $$PWD/libgnu.pro
SUBDIRS += $$PWD/libbz2.pro
SUBDIRS += $$PWD/libcurl.pro
SUBDIRS += $$PWD/libcorelib.pro
SUBDIRS += $$PWD/libcrossguid.pro

#SUBDIRS += $$PWD/libssl.pro

