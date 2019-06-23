
# a subset of GoTvAll.pro to avoid rebuilding rarely changed libs

TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

#SUBDIRS += $$PWD/libcrossguid.pro
SUBDIRS += $$PWD/libcorelib.pro

SUBDIRS += $$PWD/GoTvCoreLibs.pro

SUBDIRS += $$PWD/GoTvPtoP_App.pro
