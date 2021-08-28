#-------------------------------------------------
#
# Project created by QtCreator 2019-01-20T11:11:14
#
#-------------------------------------------------

QT       += core gui svg widgets
android:{
    versionAtMost(QT_VERSION, 5.15.2) {
        QT += androidextras
    }
#    QT += gui-private
    DEFINES += TARGET_OS_ANDROID
}

TARGET = QtAndroidJni
TEMPLATE = app

versionAtMost(QT_VERSION, 5.15.2){
    CONFIG += c++11
}

versionAtLeast(QT_VERSION, 6.0.0){
    CONFIG += c++17
}

SOURCES += \
        $$PWD/QtAndroidJni/main.cpp \
        $$PWD/QtAndroidJni/mainwindow.cpp \
        $$PWD/QtAndroidJni/ApplicationInfo.cpp \
        $$PWD/QtAndroidJni/Build.cpp \
        $$PWD/QtAndroidJni/Context.cpp \
        $$PWD/QtAndroidJni/guid.cpp \
        $$PWD/QtAndroidJni/JNIBase.cpp \
        $$PWD/QtAndroidJni/JNIFile.cpp \
        $$PWD/QtAndroidJni/JNIThreading.cpp \
        $$PWD/QtAndroidJni/jutils.cpp \
        $$PWD/QtAndroidJni/PackageItemInfo.cpp \
        $$PWD/QtAndroidJni/PackageManager.cpp \
        $$PWD/QtAndroidJni/VxFileUtil.cpp \
        $$PWD/QtAndroidJni/VxJava.cpp \
        $$PWD/QtAndroidJni/VxDebug.cpp

HEADERS += \
        $$PWD/QtAndroidJni/mainwindow.h \
        $$PWD/QtAndroidJni/ApplicationInfo.h \
        $$PWD/QtAndroidJni/Build.h \
        $$PWD/QtAndroidJni/Context.h \
        $$PWD/QtAndroidJni/guid.h \
        $$PWD/QtAndroidJni/JNIBase.h \
        $$PWD/QtAndroidJni/JNIFile.h \
        $$PWD/QtAndroidJni/JNIThreading.h \
        $$PWD/QtAndroidJni/jni.inc \
        $$PWD/QtAndroidJni/jutils.hpp \
        $$PWD/QtAndroidJni/jutils-details.hpp \
        $$PWD/QtAndroidJni/PackageItemInfo.h \
        $$PWD/QtAndroidJni/PackageManager.h \
        $$PWD/QtAndroidJni/VxFileUtil.h \
        $$PWD/QtAndroidJni/VxJava.h \
        $$PWD/QtAndroidJni/VxDebug.h

FORMS += \
        $$PWD/QtAndroidJni/mainwindow.ui

android:{
    CONFIG += mobility
    MOBILITY =

    CONFIG(debug, debug|release){
        QMAKE_CXXFLAGS += -O0
        QMAKE_CFLAGS += -O0
        DEFINES += DEBUG
    }

#os libs
    LIBS +=  -ldl -lm -landroid -lc -lstdc++ -llog

# Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
}

message($$[QT_INSTALL_BINS])
ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/androidjnitest

DISTFILES += \
    androidjnitest/AndroidManifest.xml \
    androidjnitest/build.gradle \
    androidjnitest/gradle.properties \
    androidjnitest/gradlew \
    androidjnitest/gradlew.bat \
    androidjnitest/local.properties \
    androidjnitest/res/drawable/icon.png \
    androidjnitest/res/values/strings.xml
