# Global
TEMPLATE = lib
QT       -= gui
CONFIG += staticlib
CONFIG -= sharedlib

TARGET_NAME = curl

include(config_static_dependlib.pri)
include(config_opensslp_include.pri)

include(libcurl.pri)

PRE_TARGETDEPS += $$PWD/libssl.pro
