# Global
TARGET = nolimitconnect
TEMPLATE = app
CONFIG += no_docs_target
CONFIG += c++11

# keep it all lowercase to match program naming convention on *nix systems

PROJECT_NAME = nolimitconnect
TARGET_NAME = nolimitconnect

QT += gui core concurrent widgets network opengl xml svg quickwidgets
QT += multimedia
QT += multimediawidgets

android:{
    versionAtMost(QT_VERSION, 5.15.2){
        QT += androidextras
    }
}

DEFINES += QT_SVG_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_GUI_LIB QT_CORE_LIB QT_MULTIMEDIA_LIB
DEFINES += LIB_STATIC _LIB

# Resource files
!android:{
    QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 5
}

RESOURCES += $$PWD/GoTvApps/GoTvCommon/NoLimitConnect.qrc

# Translations
# TRANSLATIONS += $$files(lang/nolimitconnect_*.ts)

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

    LIBS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}

    LIBS +=  -lpthread -ldl -lGLU -lGL -lm -luuid -lrt
}

!android{
#copy shared libraries to out directory
#message(Static Lib prefix($${STATIC_LIB_PREFIX})  suffix($${STATIC_LIB_SUFFIX})  )
#message( Exe dest dir ($${DEST_EXE_DIR})  )
# message( Share Lib dest dir ($${DEST_SHARED_LIBS_DIR})  )

    DESTDIR = $${DEST_EXE_DIR}
}

#the linux shared libs are now moved to bin directory when built
#so copy from shared folder to bin is no longer needed
#unix:!android{
##copy shared libs to local output directory so can easily be linked to
#    copydata.commands = $(COPY_DIR) $$shell_path($$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/* $$shell_path($${DEST_SHARED_LIBS_DIR}))

#    first.depends = $(first) copydata
#    export(first.depends)
#    export(copydata.commands)
#    QMAKE_EXTRA_TARGETS += first copydata
#}

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

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/bin-Android
    #shared libs   
#    ANDROID_LIBS = $$PWD/bin-Android/libs/armeabi-v7a

#    LIBS += -l$${ANDROID_LIBS}/libssl_$${ANDROID_TARGET_ARCH}$${SHARED_PYTHON_LIB_SUFFIX}#
#    LIBS += -l$${ANDROID_LIBS}/libpythoncore_$${ANDROID_TARGET_ARCH}$${SHARED_PYTHON_LIB_SUFFIX}
    ANDROID_LIBS_DIR = $${SRC_CODE_ROOT_DIR}/bin-Android/libs/$${ANDROID_TARGET_ARCH}
    message(android libs $${ANDROID_LIBS_DIR})


    message(loading python lib  $${ANDROID_LIBS_DIR}/libpythoncore_$${ANDROID_TARGET_ARCH}$${SHARED_PYTHON_LIB_SUFFIX})

    DEPENDPATH += $${ANDROID_LIBS_DIR}/
    LOAD_PYTHON_LIB_FILE=$${ANDROID_LIBS_DIR}/libssl_$${ANDROID_TARGET_ARCH}$${SHARED_PYTHON_LIB_SUFFIX}
    message(loading python lib  $${LOAD_PYTHON_LIB_FILE})

    ANDROID_EXTRA_LIBS  += -L$${LOAD_PYTHON_LIB_FILE}
    LIBS += -L$${LOAD_PYTHON_LIB_FILE}

    LOAD_SSL_LIB_FILE=$${ANDROID_LIBS_DIR}/libssl_$${ANDROID_TARGET_ARCH}$${SHARED_PYTHON_LIB_SUFFIX}
    message(loading ssl lib  $${LOAD_SSL_LIB_FILE})

    ANDROID_EXTRA_LIBS  += -L$${LOAD_SSL_LIB_FILE}
    LIBS += -L$${LOAD_SSL_LIB_FILE}

    LIBS +=  -ldl -lm -lEGL -lGLESv2  -lc -lstdc++ -llog -ljnigraphics -landroid

    DESTDIR = $${DEST_EXE_DIR}

#    versionAtMost(QT_VERSION, 5.15.2){
#        ANDROID_PACKAGE_SOURCE_DIR = \
#            $$PWD/bin-Android

#        DISTFILES += \
#        bin-Android/AndroidManifest.xml \
#        bin-Android/gradle/wrapper/gradle-wrapper.jar \
#        bin-Android/gradlew \
#        bin-Android/res/values/libs.xml \
#        bin-Android/build.gradle \
#        bin-Android/gradle/wrapper/gradle-wrapper.properties \
#        bin-Android/gradlew.bat \
#        bin-Android/res/values/strings.xml
#    }
}


