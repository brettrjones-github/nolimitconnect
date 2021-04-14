
TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

include(config_os_detect.pri)

SUBDIRS += $$PWD/libpktlib.pro
SUBDIRS += $$PWD/libdepends.pro
SUBDIRS += $$PWD/libgnu.pro
SUBDIRS += $$PWD/libbz2.pro
SUBDIRS += $$PWD/libcurl.pro
SUBDIRS += $$PWD/libcorelib.pro
SUBDIRS += $$PWD/libcrossguid.pro

SUBDIRS += $$PWD/libssl.pro
