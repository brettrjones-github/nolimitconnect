
QT       += core gui svg widgets

TEMPLATE = app
CONFIG += no_docs_target
CONFIG += c++11

TARGET = QtSimpleTestApp
TARGET_NAME=QtSimpleTestApp

include(../config_detect_os.pri)

!android:{
#    OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
}

DESTDIR=$${DEST_EXE_DIR}

INCLUDEPATH += $$PWD/../QtSimpleTestStaticLib
INCLUDEPATH += $$PWD/../QtSimpleTestSharedLib

SOURCES += \
        $$PWD/main.cpp \
        $$PWD/mainwindow.cpp

HEADERS += \
        $$PWD/mainwindow.h

FORMS += \
        $$PWD/mainwindow.ui

android:{
    CONFIG += mobility
    MOBILITY =

    # NOTE: cannont substitute QtHellowWorldWithConfig with $${TARGET_NAME} becouse it causes "Cannot find android sources" error
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../android_qt_manifest/QtSimpleTestApp/android

    LIBS +=  -ldl -lm -landroid -lc -lstdc++ -llog
}

message(qt bin directory $$[QT_INSTALL_BINS])

#static libs
message(static libs directory $${BUILD_STATIC_LIBS_DIR})
message(static lib Name qtsimpleteststaticlib$${STATIC_LIB_EXTENTION})

DEPENDPATH += $${BUILD_STATIC_LIBS_DIR}
LIBS += -L$${BUILD_STATIC_LIBS_DIR} -lqtsimpleteststaticlib$${STATIC_LIB_EXTENTION}


#shared libs
android:{
message(OUT_PWD directory $${OUT_PWD})
    message(shared lib directory $$OUT_PWD/../QtSimpleTestSharedLib)
    DEPENDPATH += $${DEST_SHARED_LIBS_DIR}
    #DEPENDPATH += $${OUT_PWD}/../QtSimpleTestSharedLib
    LIBS += -lQtSharedLibTest$${TARGET_ARCH_APPEND}
}

!android:{
    DEPENDPATH += $${DEST_EXE_DIR}/
    message(shared lib directory $${DEST_EXE_DIR})
    LIBS += -L$${DEST_EXE_DIR} -lQtSimpleTestSharedLib
}



