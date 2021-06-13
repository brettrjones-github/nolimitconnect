QT       += core gui svg widgets

TEMPLATE = lib
CONFIG += staticlib
CONFIG -= sharedlib
CONFIG += no_docs_target
CONFIG += c++11

TARGET = qtsimpleteststaticlib
TARGET_NAME=qtsimpleteststaticlib

include(../config_detect_os.pri)

!android:{
    OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
}

DESTDIR = $${BUILD_STATIC_LIBS_DIR}

SOURCES += \
        $$PWD/qtteststaticlib3.cpp

HEADERS += \
        $$PWD/qtteststaticlib3.h




message(qtsimpleteststaticlib OUT_PWD $$OUT_PWD)
message(qtsimpleteststaticlib DESTDIR $$DESTDIR)
