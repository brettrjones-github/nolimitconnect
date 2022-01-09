

TEMPLATE = app
CONFIG += no_docs_target
CONFIG += c++11
android:{
    CONFIG += mobility
    MOBILITY =
}

TARGET = QtSimpleTestApp
PROJECT_NAME = QtSimpleTestApp
TARGET_NAME=QtSimpleTestApp

# Minumum required QT += gui core widgets svg

QT += gui core concurrent widgets network opengl xml svg quickwidgets
# the multimedia libraries cause crashes for K99 with Qt 6.2.0 Beta 3
QT += multimedia
QT += multimediawidgets

android:{
    versionAtMost(QT_VERSION, 5.15.2){
        QT += androidextras
    }
}

# these defines not needed .. just added for test
DEFINES += QT_SVG_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_GUI_LIB QT_CORE_LIB QT_MULTIMEDIA_LIB
DEFINES += LIB_STATIC _LIB

# Resource files
!android:{
    QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 5
}

#required
include(../config_detect_os.pri)

#not required .. just for test
# include(../config_version.pri)
include(../config_compiler.pri)
include(../config_opensslp_include.pri)

!android:{
#    OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
}

DESTDIR=$$PWD/bin-Android

INCLUDEPATH += $$PWD/../QtSimpleTestStaticLib
INCLUDEPATH += $$PWD/../QtSimpleTestSharedLib

SOURCES += \
        $$PWD/main.cpp \
        $$PWD/mainwindow.cpp \
        $$PWD/VxDebug.cpp \
        $$PWD/LogWidget.cpp

HEADERS += \
        $$PWD/mainwindow.h \
        $$PWD/VxDebug.h \
        $$PWD/LogWidget.h

FORMS += \
        $$PWD/mainwindow.ui

android:{

    # NOTE: cannont substitute QtHellowWorldWithConfig with $${TARGET_NAME} becouse it causes "Cannot find android sources" error
    #ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../android_qt_manifest/QtSimpleTestApp/android

    # these libs are required
    #LIBS +=  -ldl -lm -landroid -lc -lstdc++ -llog
    # these libs are not needed but added as test
    LIBS +=  -ldl -lm -lEGL -lGLESv2  -lc -lstdc++ -llog -ljnigraphics -landroid

}

message(qt bin directory $$[QT_INSTALL_BINS] target arch $${TARGET_ARCH_NAME} root of source code $${SRC_CODE_ROOT_DIR})

#static libs
message(static libs directory $${BUILD_STATIC_LIBS_DIR})
message(static lib Name qtsimpleteststaticlib$${STATIC_LIB_EXTENTION})


LIBS += -L$${BUILD_STATIC_LIBS_DIR} -lqtsimpleteststaticlib$${STATIC_LIB_EXTENTION}

#shared libs

android:{
    message(shared lib name QtSimpleTestSharedLib_$${TARGET_ARCH_NAME})
    BIN_FINAL_LIB_DIR=$${SRC_CODE_ROOT_DIR}/bin-Android/$${TARGET_ARCH_NAME}
    message(loading shared $${BIN_FINAL_LIB_DIR} libQtSimpleTestSharedLib_$${TARGET_ARCH_NAME}.so)

    LOAD_LIB_FILE=$${SRC_CODE_ROOT_DIR}/bin-Android/$${TARGET_ARCH_NAME}/QtSimpleTestSharedLib_$${TARGET_ARCH_NAME}.so
     message(loading file name $${LOAD_LIB_FILE})
    DEPENDPATH += $${BIN_FINAL_LIB_DIR}/
    LIBS += -L$${LOAD_LIB_FILE}
}

!android:{
    DEPENDPATH += $${DEST_EXE_DIR}/
    message(shared lib directory $${DEST_EXE_DIR})
    LIBS += -L$${DEST_EXE_DIR} -lQtSimpleTestSharedLib
}



