QT       += core gui svg widgets

TEMPLATE = app
CONFIG += no_docs_target
CONFIG += c++11

TARGET = QtHelloWorld
TARGET_NAME=QtHelloWorld

include(../config_detect_os.pri)

!android:{
    OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
}

DESTDIR=$${DEST_EXE_DIR}

SOURCES += \
        $$PWD/QtHelloWorld/main.cpp \
        $$PWD/QtHelloWorld/mainwindow.cpp

HEADERS += \
        $$PWD/QtHelloWorld/mainwindow.h

FORMS += \
        $$PWD/QtHelloWorld/mainwindow.ui

android:{
    CONFIG += mobility
    MOBILITY =

    # NOTE: cannont substitute QtHellowWorldWithConfig with $${TARGET_NAME} becouse it causes "Cannot find android sources" error
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android_qt_manifest/QtHellowWorld/android

    LIBS +=  -ldl -lm -landroid -lc -lstdc++ -llog
}

message(qt bin directory $$[QT_INSTALL_BINS])


