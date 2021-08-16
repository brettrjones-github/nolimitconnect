#-------------------------------------------------
#
# Project created by QtCreator 2019-01-20T11:11:14
#
#-------------------------------------------------

TARGET = QtSoundTest
TEMPLATE = app

QT += gui core concurrent widgets network multimedia

android:{
    versionAtMost(QT_VERSION, 5.15.2) {
        QT += androidextras
    }
#    QT += gui-private
    DEFINES += TARGET_OS_ANDROID
}

unix:!android:{
    DEFINES += TARGET_OS_LINUX
}

win32:{
    DEFINES += TARGET_OS_WINDOWS
}

CONFIG += c++11

INCLUDEPATH += $$PWD/QtSoundTest
INCLUDEPATH += $$PWD/Forms

SOURCES += \
        $$PWD/QtSoundTest/main.cpp \
        $$PWD/QtSoundTest/mainwindow.cpp \
        $$PWD/QtSoundTest/AudioIoMgr.cpp \
        $$PWD/QtSoundTest/AudioMixer.cpp \
        $$PWD/QtSoundTest/AudioInIo.cpp \
        $$PWD/QtSoundTest/AudioInThread.cpp \
        $$PWD/QtSoundTest/AudioOutIo.cpp \
        $$PWD/QtSoundTest/AudioOutThread.cpp \
        $$PWD/QtSoundTest/AudioUtils.cpp \
        $$PWD/QtSoundTest/AudioTestGenerator.cpp \
        $$PWD/QtSoundTest/LogWidget.cpp \
        $$PWD/QtSoundTest/SoundTestLogic.cpp \
        $$PWD/QtSoundTest/SoundTestThread.cpp \
        $$PWD/QtSoundTest/VxDebug.cpp \
        $$PWD/QtSoundTest/VxFileUtil.cpp \
        $$PWD/QtSoundTest/VxParse.cpp \
        $$PWD/QtSoundTest/VxTime.cpp \
        $$PWD/QtSoundTest/VxTimer.cpp \
        $$PWD/QtSoundTest/VxTimeUtil.cpp \
        $$PWD/QtSoundTest/WaveForm.cpp

HEADERS += \
        $$PWD/QtSoundTest/mainwindow.h \
        $$PWD/QtSoundTest/AudioDefs.h \
        $$PWD/QtSoundTest/AudioIoMgr.h \
        $$PWD/QtSoundTest/AudioMixer.h \
        $$PWD/QtSoundTest/AudioInIo.h \
        $$PWD/QtSoundTest/AudioInThread.h \
        $$PWD/QtSoundTest/AudioOutIo.h \
        $$PWD/QtSoundTest/AudioOutThread.h \
        $$PWD/QtSoundTest/AudioUtils.h \
        $$PWD/QtSoundTest/AudioTestGenerator.h \
        $$PWD/QtSoundTest/IAudioInterface.h \
        $$PWD/QtSoundTest/IGoTvDefs.h \
        $$PWD/QtSoundTest/LogWidget.h \
        $$PWD/QtSoundTest/SoundTestLogic.h \
        $$PWD/QtSoundTest/SoundTestThread.h \
        $$PWD/QtSoundTest/VxDebug.h \
        $$PWD/QtSoundTest/VxFileUtil.h \
        $$PWD/QtSoundTest/VxParse.h \
        $$PWD/QtSoundTest/VxTime.h \
        $$PWD/QtSoundTest/VxTimer.h \
        $$PWD/QtSoundTest/VxTimeUtil.h \
        $$PWD/QtSoundTest/VxDefs.h \
        $$PWD/QtSoundTest/WaveForm.h

FORMS += \
        $$PWD/Forms/mainwindow.ui

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
        $$PWD/androidsoundtest

DISTFILES += \
    androidsoundtest/AndroidManifest.xml \
    androidsoundtest/build.gradle \
    androidsoundtest/gradle.properties \
    androidsoundtest/gradlew \
    androidsoundtest/gradlew.bat \
    androidsoundtest/local.properties \
    androidsoundtest/res/drawable/icon.png \
    androidsoundtest/res/values/strings.xml
