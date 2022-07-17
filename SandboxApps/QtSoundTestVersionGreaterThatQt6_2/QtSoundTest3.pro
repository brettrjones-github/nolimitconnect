#-------------------------------------------------
#
# Project created by QtCreator 2019-01-20T11:11:14
#
#-------------------------------------------------

TARGET = QtSoundTest3
TEMPLATE = app
versionAtLeast(QT_VERSION, 6.0.0){
    CONFIG += c++17
}
versionAtMost(QT_VERSION, 5.15.2){
    CONFIG += c++11
}

QT += gui core concurrent widgets network multimedia

android:{
    DEFINES += TARGET_OS_ANDROID
}

unix:!android:{
    DEFINES += TARGET_OS_LINUX
}

win32:{
    DEFINES += TARGET_OS_WINDOWS
}

include(../../config_os_detect.pri)

INCLUDEPATH += $$PWD/QtSoundTest
INCLUDEPATH += $$PWD/Forms

SOURCES += \
        $$PWD/QtSoundTest/main.cpp \
        $$PWD/QtSoundTest/mainwindow.cpp \
        $$PWD/QtSoundTest/AudioIoMgr.cpp \
        $$PWD/QtSoundTest/AudioMixer.cpp \
        $$PWD/QtSoundTest/AudioMixerFrame.cpp \
        $$PWD/QtSoundTest/AudioMixerThread.cpp \
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
        $$PWD/QtSoundTest/AudioMixerFrame.h \
        $$PWD/QtSoundTest/AudioMixerThread.h \
        $$PWD/QtSoundTest/AudioInIo.h \
        $$PWD/QtSoundTest/AudioInThread.h \
        $$PWD/QtSoundTest/AudioOutIo.h \
        $$PWD/QtSoundTest/AudioOutThread.h \
        $$PWD/QtSoundTest/AudioUtils.h \
        $$PWD/QtSoundTest/AudioTestGenerator.h \
        $$PWD/QtSoundTest/IAudioInterface.h \
        $$PWD/QtSoundTest/INlcDefs.h \
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

    # NOTE: cannont substitute QtHellowWorldWithConfig with $${TARGET_NAME} becouse it causes "Cannot find android sources" error
    #ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../android_qt_manifest/QtSimpleTestApp/android

    LIBS +=  -ldl -lm -landroid -lc -lstdc++ -llog
}

message(QT_INSTALL_BINS $$[QT_INSTALL_BINS])
message(QT_INSTALL_EXAMPLES DIR $$[QT_INSTALL_EXAMPLES])
SND_TEST3_INSTALL_PATH = $${SRC_CODE_ROOT_DIR}/SandboxApps/QtSoundTestQt6_2_0
message(SRC_CODE_ROOT_DIR3 is $${SRC_CODE_ROOT_DIR})
message(SND_TEST3_INSTALL_PATH is $${SND_TEST3_INSTALL_PATH})

#target.path = $$[SND_TEST3_INSTALL_PATH]
#INSTALLS += target

