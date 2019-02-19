# Global
TEMPLATE = app
TARGET = gotvptop
TARGET_NAME = gotvptop

QT += gui core concurrent widgets network multimedia opengl xml svg quickwidgets


CONFIG += qt thread silent
# C++11 support
CONFIG += c++11

#DEFINES += BOOST_NO_CXX11_RVALUE_REFERENCES
#DEFINES += QT_NO_CAST_TO_ASCII
# Fast concatenation (Qt >= 4.6)
#DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

win32: DEFINES += NOMINMAX

strace_win {
    DEFINES += STACKTRACE_WIN
    DEFINES += STACKTRACE_WIN_PROJECT_PATH=$$PWD
    DEFINES += STACKTRACE_WIN_MAKEFILE_PATH=$$OUT_PWD
}


# Resource files
QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 5
RESOURCES += \
    ./../../GoTvApps/GoTvCommon/gotvcommon.qrc


# Translations
TRANSLATIONS += $$files(lang/gotvptop_*.ts)

DESTDIR = ./../../bin/
LIBS += -L./../../lib

OBJECTS_DIR = ./.obj

MOC_DIR = ./.moc
RCC_DIR = ./.qrc
UI_DIR = ./.ui


QMAKE_CFLAGS_YACC   = -Wno-unused -Wno-parentheses
QMAKE_CXXFLAGS_RTTI_OFF = -fno-rtti
QMAKE_CXXFLAGS_EXCEPTIONS_OFF = -fno-exceptions
QMAKE_CXXFLAGS += -Wno-unused -Wno-parentheses -Wno-attributes  -Wno-ignored-qualifiers

include(version.pri)
include(os_detect.pri)
include(compile_config.pri)

#so use our static linked version of freetype
INCLUDEPATH += ./../../DependLibs/libfreetype/include

INCLUDEPATH += ./../../
INCLUDEPATH += ./../../GoTvApps/GoTvCommon
INCLUDEPATH += ./../../GoTvApps/GoTvCommon/QtSource
INCLUDEPATH += ./../../DependLibs
INCLUDEPATH += ./../../DependLibs/libcurl/include
INCLUDEPATH += ./../../DependLibs/libcurl/lib
INCLUDEPATH += ./../../DependLibs/ffmpeg
INCLUDEPATH += ./../../GoTvCore/xbmc/xbmc


include(../../GoTvApps/GoTvPtoP/build/Qt/GoTvPtoP.pri)

#### for static linked qt libs only
#### QMAKE_LFLAGS += -static

#link dependent library

#QMAKE_LFLAGS += -static
unix{
#NOTE: link order is important.. otherwise you will get link errors like libvorbisenc.so.2: error adding symbols: DSO missing from command line

    CONFIG(debug, debug|release){
        LIBS += -lkodiLinuxD
        LIBS += -lffmpegavdeviceLinuxD
        LIBS += -lffmpegavfilterLinuxD
        LIBS += -lffmpegavformatLinuxD
        LIBS += -lffmpegavcodecLinuxD
        LIBS += -lffmpegpostprocLinuxD
        LIBS += -lffmpegswresampleLinuxD
        LIBS += -lffmpegswscaleLinuxD
        LIBS += -lffmpegavutilLinuxD
        LIBS += -lnfsLinuxD
        LIBS += -lcdioLinuxD
        LIBS += -lpcreLinuxD
        LIBS += -larmrwbencLinuxD
        LIBS += -lfdk-aacLinuxD
        LIBS += -lopencore-amrLinuxD
        LIBS += -lopenmpt-fullLinuxD
        LIBS += -lvpxLinuxD
        LIBS += -lx264LinuxD
        LIBS += -lx265LinuxD
        LIBS += -lvorbisLinuxD
        LIBS += -loggLinuxD
        LIBS += -lopusLinuxD
        LIBS += -lspeexLinuxD
        LIBS += -lptopengineLinuxD
        LIBS += -lmediatoolsLinuxD
        LIBS += -llameLinuxD
        LIBS += -lmicrohttpdLinuxD
        LIBS += -lgnuLinuxD
        LIBS += -ldependsLinuxD
        LIBS += -lcurlLinuxD
        LIBS += -lsshLinuxD
        LIBS += -lsslLinuxD
        LIBS += -lpktlibLinuxD
        LIBS += -lfribidiLinuxD
        LIBS += -liconvLinuxD
        LIBS += -lfreetypeLinuxD
        LIBS += -lpngLinuxD
        LIBS += -ltinyxmlLinuxD
        LIBS += -lxml2LinuxD
        LIBS += -lcompressLinuxD
        LIBS += -lcorelibLinuxD
        LIBS += -lcrossguidLinuxD
        LIBS += -lbz2LinuxD
        LIBS += -lzlibLinuxD
    }

    CONFIG(release, debug|release){
#        LIBS += -L./../lib/gnuLinux.a
    }

    LIBS += -ldl -lGLU -lGL -lm -luuid -lrt -lpthread -lpython2.7


}
