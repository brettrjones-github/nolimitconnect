# Global
TEMPLATE = app
CONFIG += qt thread silent

QT += widgets
versionAtMost(QT_VERSION, 5.15.2){
    QT += multimedia
    QT += multimediawidgets
}

# keep it all lowercase to match program naming convention on *nix systems
PROJECT_NAME = gotvcoreapp

win32:TARGET_OS_DIR = Win32
linux:!android: TARGET_OS_DIR = Linux
android: TARGET_OS__DIR = Android
macx: TARGET_OS__DIR = Apple

DESTDIR = ./../../../bin/$$TARGET_OS__DIR/

TARGET_LIB_APPEND = .exe
CONFIG(release, debug|release){
 TARGET_LIB_APPEND = .exe
}

CONFIG(debug, debug|release){
 TARGET_LIB_APPEND = D.exe
}

include(../os_compile_config.pri)

#include(../../../NlcCore/build/Qt/libNlcCore.pri)
include($$PWD/../../../NlcCore/build/Qt/gotvcoresources.pri)

DISTFILES += \
    ../os_compile_config.pri \
    android-libAndroid.exe.so-deployment-settings.json \
    .qmake.stash

HEADERS += \
    ../../../AppCompilerConfig.h \
    ../../../AppCompilerConfig.h \
    ../../../NlcConfig.h \
    ../../../AppCpuArchDefines.h \
    ../../../AppDependLibrariesConfig.h \
    ../../../NlcFeaturesConfig.h \
    ../../../AppTargetOsConfig.h

SUBDIRS += \
    ../dependlibs/gotvdependlibs.pro \
    gotvcoreapp.pro

SOURCES += \
    ../build-gotvcoreapp-Android_for_armeabi_v7a_GCC_4_9_Qt_5_9_1_for_Android_armv7-Debug/.qrc/qrc_commonresources.cpp \
    ../generated/Android/.qrc/qrc_commonresources.cpp

