# Global

TEMPLATE = lib
TARGET_NAME = gotvptoplib

QT += gui core concurrent widgets network multimedia opengl xml svg quickwidgets

DEFINES += QT_SVG_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_GUI_LIB QT_CORE_LIB QT_MULTIMEDIA_LIB


#CONFIG += qt thread silent
win32{
    # C++11 support
    versionAtMost(QT_VERSION, 5.15.2){
        CONFIG += c++11
    }

    versionAtLeast(QT_VERSION, 6.0.0)){
        CONFIG += c++17
    }
}

android{
    # C++11 support
    versionAtMost(QT_VERSION, 5.15.2){
        CONFIG += c++11
    }

    versionAtLeast(QT_VERSION, 6.0.0)){
        CONFIG += c++17
    }
}

unix:!android{
    # C++11 support
    versionAtMost(QT_VERSION, 5.15.2){
        CONFIG += c++11
    }

    versionAtLeast(QT_VERSION, 6.0.0)){
        CONFIG += c++17
    }
}
}

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
#RESOURCES += $$PWD/AppsSrc/CommonSrc/NoLimitConnect.qrc


# Translations
# TRANSLATIONS += $$files(lang/gotvptop_*.ts)

# include(config_version.pri) # not needed unless building kodi
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_opensslp_include.pri)


CONFIG += staticlib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug
}

CONFIG(release, debug|release){
    DESTDIR = $$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release
}


DEFINES += LIB_STATIC _LIB


TARGET_LIB_APPEND = .lib

CONFIG(debug, debug|release) {
win32 {
    TARGET_LIB_APPEND =  D.lib
}

unix: {
    TARGET_LIB_APPEND =  D
}

}

CONFIG(release, debug|release) {
win32 {
    TARGET_LIB_APPEND =  .lib
}

unix: {
    TARGET_LIB_APPEND =
}

}


OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
MOC_DIR =.moc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
RCC_DIR =.qrc/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
UI_DIR =.ui/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}


TARGET=$${TARGET_NAME}$${TARGET_OS_NAME}$${TARGET_LIB_APPEND}


DESTDIR = $$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}

include(NoLimitAppLib.pri)

#DESTDIR = $$PWD/bin/


#QMAKE_CFLAGS_YACC   = -Wno-unused -Wno-parentheses
#QMAKE_CXXFLAGS_RTTI_OFF = -fno-rtti
#QMAKE_CXXFLAGS_EXCEPTIONS_OFF = -fno-exceptions
#QMAKE_CXXFLAGS += -Wno-unused -Wno-parentheses -Wno-attributes  -Wno-ignored-qualifiers

#### for static linked qt libs only
#### QMAKE_LFLAGS += -static

HEADERS += \
    AppsSrc/CommonSrc/QtSource/LogoRenderer.h \
    AppsSrc/CommonSrc/QtSource/RenderLogoShaders.h

SOURCES += \
    AppsSrc/CommonSrc/QtSource/LogoRenderer.cpp \
    AppsSrc/CommonSrc/QtSource/RenderLogoShaders.cpp

