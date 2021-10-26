# Global
TEMPLATE = lib
QT       -= gui
CONFIG += staticlib
CONFIG -= sharedlib

DEFINES += LIB_STATIC_LIB
DEFINES -= LIB_SHARED_LIB

TARGET_NAME = ssl

include(config_static_dependlib.pri)

include(config_opensslp_include.pri)

include(libssl.pri)
