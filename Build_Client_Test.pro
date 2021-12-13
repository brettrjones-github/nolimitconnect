
# Network client test

TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

SUBDIRS += $$PWD/libnetlib.pro
SUBDIRS += $$PWD/libpktlib.pro
SUBDIRS += $$PWD/libcorelib.pro
SUBDIRS += $$PWD/libcrossguid.pro

SUBDIRS += $$PWD/AppsSrc/NetClientTest/NetClientTest.pro
