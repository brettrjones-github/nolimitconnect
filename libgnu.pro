# Global
TEMPLATE = lib
QT       -= gui
CONFIG += staticlib
CONFIG -= sharedlib

TARGET_NAME = gnu

include(config_static_dependlib.pri)
include(config_opensslp_include.pri)

#short obj path so does not overflow windows 32K command line limit "make (e=87): The parameter is incorrect"
CONFIG(debug, debug|release){
    OBJECTS_DIR=.gnud
}

CONFIG(release, debug|release){
    OBJECTS_DIR=.gnur
}

include(libgnu.pri)

PRE_TARGETDEPS += $$PWD/libssl.pro

HEADERS += \
    AppCompilerConfig.h \
    AppCpuArchDefines.h \
    AppDependLibrariesConfig.h
