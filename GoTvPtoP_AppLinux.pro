# Global
TARGET = gotvptopl
TEMPLATE = app

#TARGET_NAME = gotvptop

QT += gui core concurrent widgets network multimedia opengl xml svg quickwidgets

DEFINES += QT_SVG_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_GUI_LIB QT_CORE_LIB QT_MULTIMEDIA_LIB


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
#QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 5
#RESOURCES += $$PWD/GoTvApps/GoTvCommon/gotvcommon.qrc


# Translations
# TRANSLATIONS += $$files(lang/gotvptop_*.ts)


include(config_version.pri)
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_opensslp_include.pri)

DESTDIR = DEST_EXE_DIR



CONFIG(debug, debug|release){
    OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug
    MOC_DIR =.moc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug
    RCC_DIR =.qrc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug
    UI_DIR =.ui/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug
}

CONFIG(release, debug|release){
    OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release
    MOC_DIR =.moc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release
    RCC_DIR =.qrc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release
    UI_DIR =.ui/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release
}

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

#message(Static Lib prefix($${STATIC_LIB_PREFIX})  suffix($${STATIC_LIB_SUFFIX})  )

#NOTE: link order is important.. otherwise you will get link errors like libvorbisenc.so.2: error adding symbols: DSO missing from command line
CONFIG(debug, debug|release){
    #static libs
    LIBS +=  $${STATIC_LIB_PREFIX}gotvptoplib$${STATIC_LIB_SUFFIX}
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
    LIBS +=  $${STATIC_LIB_PREFIX}curl$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}ssh$${STATIC_LIB_SUFFIX}
#    LIBS +=  $${STATIC_LIB_PREFIX}ssl$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}pktlib$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}fribidi$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}iconv$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}freetype$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}png$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}tinyxml$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}xml2$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}compress$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}corelib$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}
    LIBS +=  $${STATIC_LIB_PREFIX}zlib$${STATIC_LIB_SUFFIX}


    #shared libs
    LIBS +=  $${SHARED_LIB_PREFIX}pythoncore$${SHARED_PYTHON_LIB_SUFFIX}
    LIBS +=  $${SHARED_LIB_PREFIX}ssl$${SHARED_PYTHON_LIB_SUFFIX}
}

unix:!android:{
    LIBS +=  -lpthread -ldl -lGLU -lGL -lm -luuid -lrt -lvorbis
}

android:{
#    LIBS +=  $${SHARED_LIB_PREFIX}pythoncore$${SHARED_LIB_SUFFIX}
    LIBS +=  -ldl -lm -landroid -lEGL -lGLESv2  -lc -lstdc++ -llog -ljnigraphics
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../android/ndk-windows-x86-64/android-ndk-r19/platforms/android-21/arch-arm/usr/lib/libGLESv2.so \
        $${SHARED_LIB_PREFIX}pythoncore$${SHARED_PYTHON_LIB_SUFFIX}


    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

android:{
DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/res/values/strings.xml
}

#copy shared libraries to out directory
#message(Static Lib prefix($${STATIC_LIB_PREFIX})  suffix($${STATIC_LIB_SUFFIX})  )



#copy shared libs to local output directory so can easily be linked to
 CONFIG(debug, debug|release){
    copydata.commands = $(COPY_DIR) $$shell_path($$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug/* $$shell_path($$OUT_PWD/))
 }

 CONFIG(release, debug|release){
    copydata.commands = $(COPY_DIR) $$shell_path($$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release/* $$shell_path($$OUT_PWD/) )
 }

 first.depends = $(first) copydata
 export(first.depends)
 export(copydata.commands)
 QMAKE_EXTRA_TARGETS += first copydata

unix:{
    #give linux the path of where to load our shared libraries from for debugger
    LIBS += -L$$OUT_PWD/
}
