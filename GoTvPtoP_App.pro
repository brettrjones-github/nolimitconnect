# Global
TARGET = nolimitconnect
TEMPLATE = app

# keep it all lowercase to match program naming convention on *nix systems
PROJECT_NAME = nolimitconnect

TARGET_NAME = nolimitconnect

QT += gui core concurrent widgets network multimedia opengl xml svg quickwidgets multimediawidgets
android:{
    QT += androidextras
}

DEFINES += QT_SVG_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_GUI_LIB QT_CORE_LIB QT_MULTIMEDIA_LIB
DEFINES += LIB_STATIC _LIB


#CONFIG += qt thread silent
# C++11 support
CONFIG += c++11

CONFIG += mobility
MOBILITY =

#win32: DEFINES += NOMINMAX

#strace_win {
#    DEFINES += STACKTRACE_WIN
#    DEFINES += STACKTRACE_WIN_PROJECT_PATH=$$PWD
#    DEFINES += STACKTRACE_WIN_MAKEFILE_PATH=$$OUT_PWD
#}

# Resource files
!android:{
QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 5
}

RESOURCES += $$PWD/GoTvApps/GoTvCommon/gotvcommon.qrc


# Translations
# TRANSLATIONS += $$files(lang/gotvptop_*.ts)


include(config_version.pri)
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_opensslp_include.pri)

PRE_TARGETDEPS += $$PWD/libptopengine.pro
PRE_TARGETDEPS += $$PWD/libnetlib.pro
PRE_TARGETDEPS += $$PWD/libpktlib.pro
PRE_TARGETDEPS += $$PWD/libcorelib.pro
PRE_TARGETDEPS += $$PWD/libcrossguid.pro

nolimitconnect.depends += $$PWD/libptopengine.pro
nolimitconnect.depends += $$PWD/libnetlib.pro
nolimitconnect.depends += $$PWD/libpktlib.pro
nolimitconnect.depends += $$PWD/libcorelib.pro
nolimitconnect.depends += $$PWD/libcrossguid.pro


#DESTDIR = $$PWD/bin/
#gotvptop.depends += $$PWD/GoTvPtoPAppLib.pro
#gotvptop.depends += $$PWD/GoTvCoreLibs.pro
#gotvptop.depends += $$PWD/GoTvPythonLib.pro
#gotvptop.depends += $$PWD/GoTvDependLibs.pro

#gotvptop.depends += $$PWD/libcurl.pro
#gotvptop.depends += $$PWD/python_pythoncore.pro
#gotvptop.depends += $$PWD/python_bz2.pro
#gotvptop.depends += $$PWD/python_ctypes.pro
#gotvptop.depends += $$PWD/python_ctypes_test.pro
#gotvptop.depends += $$PWD/python_elementtree.pro
#gotvptop.depends += $$PWD/python_hashlib.pro
#gotvptop.depends += $$PWD/python_sqlite3.pro
#gotvptop.depends += $$PWD/python_socket.pro
#gotvptop.depends += $$PWD/python_ssl.pro
#gotvptop.depends += $$PWD/python_testcapi.pro
#gotvptop.depends += $$PWD/python_pyexpat.pro
#gotvptop.depends += $$PWD/python_select.pro
#gotvptop.depends += $$PWD/python_unicodedata.pro

#gotvptop.depends += $$PWD/GoTvOpenSslLib.pro

#gotvptop.depends += $$PWD/libgnu.pro
#gotvptop.depends += $$PWD/libnetlib.pro
#gotvptop.depends += $$PWD/libpktlib.pro
#gotvptop.depends += $$PWD/libcorelib.pro
#gotvptop.depends += $$PWD/libcrossguid.pro


OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
MOC_DIR =.moc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
RCC_DIR =.qrc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
UI_DIR =.ui/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}

# windows build of android for large projects
# fails because command line limit in windoz is 32,768.. so
# use short obj path so does not overflow windows command line limit "make (e=87): The parameter is incorrect"
android{
    CONFIG(debug, debug|release){
        OBJECTS_DIR=.ad
        MOC_DIR =.mocd
        RCC_DIR =.qrcd
    }

    CONFIG(release, debug|release){
        OBJECTS_DIR=.ar
        MOC_DIR =.mocr
        RCC_DIR =.qrcr
    }
}

# linux needs a qualified path
unix:!android{
    CONFIG(debug, debug|release){
        OBJECTS_DIR=$$PWD//objs/obs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
        MOC_DIR =$$PWD/objs/moc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
        RCC_DIR =$$PWD/objs/qrc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
        UI_DIR =$$PWD/objs/ui/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
    }

    CONFIG(release, debug|release){
        OBJECTS_DIR=$$PWD/objs/obs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
        MOC_DIR =$$PWD/objs/moc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
        RCC_DIR =$$PWD/objs/qrc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
        UI_DIR =$$PWD/objs/ui/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
    }

    message( linux dirs obj $${OBJECTS_DIR} moc $${MOC_DIR} qrc $${RCC_DIR} ui $${UI_DIR} )
}

include(GoTvPtoPAppLib.pri)

# look in same directory as executable for shared libraries
unix:{
QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

#QMAKE_CFLAGS_YACC   = -Wno-unused -Wno-parentheses
#QMAKE_CXXFLAGS_RTTI_OFF = -fno-rtti
#QMAKE_CXXFLAGS_EXCEPTIONS_OFF = -fno-exceptions
#QMAKE_CXXFLAGS += -Wno-unused -Wno-parentheses -Wno-attributes  -Wno-ignored-qualifiers

#### for static linked qt libs only
#### QMAKE_LFLAGS += -static



#link dependent librarys
include(config_link.pri)

#shared libs
#PRE_TARGETDEPS +=  $${SHARED_LIB_PREFIX}pythoncore$${SHARED_PYTHON_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${SHARED_LIB_PREFIX}ssl$${SHARED_PYTHON_LIB_SUFFIX}

#static libs
##PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}gotvptoplib$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}kodi$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegavdevice$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegavformat$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegavfilter$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegavcodec$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegpostproc$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegswresample$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegswscale$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ffmpegavutil$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}nfs$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}cdio$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}pcre$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}armrwbenc$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}fdk-aac$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}opencore-amr$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}openmpt-full$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}vpx$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}x264$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}x265$${STATIC_LIB_SUFFIX}

#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ptopengine$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}mediatools$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}opus$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}speex$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}lame$${STATIC_LIB_SUFFIX}

#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}vorbis2$${STATIC_LIB_SUFFIX}

#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}microhttpd$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}gnu$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}depends$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ogg$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}curl$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ssh$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}netlib$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}pktlib$${STATIC_LIB_SUFFIX}
##PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}ssl$${STATIC_LIB_SUFFIX} // shared lib
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}fribidi$${STATIC_LIB_SUFFIX}
##PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}iconv$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}freetype$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}png$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}tinyxml$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}xml2$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}compress$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}corelib$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}
#PRE_TARGETDEPS +=  $${STATIC_LIB_PREFIX}zlib$${STATIC_LIB_SUFFIX}

#message(Static Lib prefix($${STATIC_LIB_PREFIX})  suffix($${STATIC_LIB_SUFFIX})  )
    #static libs

#NOTE: link order is important.. otherwise you will get link errors like libvorbisenc.so.2: error adding symbols: DSO missing from command line
    #static libs
    LIBS +=  $${STATIC_LIB_PREFIX}ptopengine$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}kodi$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegavdevice$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegavformat$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegavfilter$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegavcodec$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegpostproc$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegswresample$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegswscale$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ffmpegavutil$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}nfs$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}cdio$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}pcre$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}armrwbenc$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}fdk-aac$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}opencore-amr$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}openmpt-full$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}vpx$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}x264$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}x265$${STATIC_LIB_SUFFIX}

    LIBS +=  $${STATIC_LIB_PREFIX}ptopengine$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}mediatools$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}opus$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}speex$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}lame$${STATIC_LIB_SUFFIX}

    LIBS +=  $${STATIC_LIB_PREFIX}vorbis2$${STATIC_LIB_SUFFIX}

    LIBS +=  $${STATIC_LIB_PREFIX}microhttpd$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}gnu$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}depends$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ogg$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ssh$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}netlib$${STATIC_LIB_SUFFIX}

    LIBS +=  $${STATIC_LIB_PREFIX}pktlib$${STATIC_LIB_SUFFIX}

#    LIBS +=  $${STATIC_LIB_PREFIX}ssl$${STATIC_LIB_SUFFIX} // shared lib
    LIBS +=  $${STATIC_LIB_PREFIX}fribidi$${STATIC_LIB_SUFFIX}
#    LIBS +=  $${STATIC_LIB_PREFIX}iconv$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}freetype$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}png$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}tinyxml$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}xml2$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}compress$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}corelib$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}zlib$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}curl$${STATIC_LIB_SUFFIX}


unix:!android{
    message( Unix load dir $${DEST_SHARED_LIBS_DIR} )
        #give linux the path of where to load our shared libraries from for debugger
        LIBS += -L$${DEST_SHARED_LIBS_DIR}
        #shared libs
    message( Unix )
    #linux seems to be very particular and the absolute path does not work
         #LIBS +=  $${SHARED_LIB_PREFIX}pythoncore$${SHARED_PYTHON_LIB_SUFFIX}
         #LIBS +=  $${SHARED_LIB_PREFIX}ssl$${SHARED_PYTHON_LIB_SUFFIX}
    CONFIG(debug, debug|release){
        LIBS +=  -lpythoncore_d
        LIBS +=  -lssl_d

    }

    CONFIG(release, debug|release){
        LIBS +=  -lpythoncore
        LIBS +=  -lssl
    }
}
    LIBS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}

unix:!android:{
    LIBS +=  -lpthread -ldl -lGLU -lGL -lm -luuid -lrt
}

!android{
#copy shared libraries to out directory
#message(Static Lib prefix($${STATIC_LIB_PREFIX})  suffix($${STATIC_LIB_SUFFIX})  )
#message( Exe dest dir ($${DEST_EXE_DIR})  )
# message( Share Lib dest dir ($${DEST_SHARED_LIBS_DIR})  )

    DESTDIR = $${DEST_EXE_DIR}
}

unix:!android{
#copy shared libs to local output directory so can easily be linked to
    copydata.commands = $(COPY_DIR) $$shell_path($$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/* $$shell_path($${DEST_SHARED_LIBS_DIR}))

    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

win32:{
    #shared libs
    LIBS +=  $${SHARED_LIB_PREFIX}pythoncore$${SHARED_PYTHON_LIB_SUFFIX}
    LIBS +=  $${SHARED_LIB_PREFIX}ssl$${SHARED_PYTHON_LIB_SUFFIX}

    LIBS +=  opengl32.lib
	LIBS +=  glu32.lib
    LIBS +=  ole32.lib
    LIBS +=  winmm.lib
    LIBS +=  Rpcrt4.lib
}

android:{
    CONFIG += mobility
    MOBILITY =

    #shared libs
#    SHARED_LIB_PREFIX = $$PWD/bin-Android/lib/armeabi-v7a/
#    ANDROID_EXTRA_LIBS = \
#            $${SHARED_LIB_PREFIX}libpythoncore$${SHARED_PYTHON_LIB_SUFFIX}
#            $${SHARED_LIB_PREFIX}libssl$${SHARED_PYTHON_LIB_SUFFIX}
    ANDROID_LIBS = $$PWD/bin-Android/libs/armeabi-v7a
#    ANDROID_EXTRA_LIBS += \
#        $${ANDROID_LIBS}/libpythoncore$${SHARED_PYTHON_LIB_SUFFIX} \
 #       $${ANDROID_LIBS}/libssl$${SHARED_PYTHON_LIB_SUFFIX}

    LIBS += -l$${ANDROID_LIBS}/libssl$${SHARED_PYTHON_LIB_SUFFIX}
    LIBS += -l$${ANDROID_LIBS}/libpythoncore$${SHARED_PYTHON_LIB_SUFFIX}

#    LIBS +=  -ldl -lm -landroid -lEGL -lGLESv2  -lc -lstdc++ -llog -ljnigraphics
    LIBS +=  -ldl -lm -lEGL -lGLESv2  -lc -lstdc++ -llog -ljnigraphics -landroid

    # Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/bin-Android

DISTFILES += \
    bin-Android/AndroidManifest.xml \
    bin-Android/gradle/wrapper/gradle-wrapper.jar \
    bin-Android/gradlew \
    bin-Android/res/values/libs.xml \
    bin-Android/build.gradle \
    bin-Android/gradle/wrapper/gradle-wrapper.properties \
    bin-Android/gradlew.bat \
    bin-Android/res/values/strings.xml
}

